######SCRIPT TO ESTIMATE WATER BALANCE#####
# Raphaël Perez, June 2019
# adapted from Pallas B. &  Rouan L.

# rm(list=ls(all=TRUE))

# Packages  ---------------------------------------------------------------

packs <- c("lubridate",'dplyr','stringr')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)


# Functions ---------------------------------------------------------------


#' sigmoid function
#'
#' @param X x axis value
#' @param max treshhold
#' @param slope slope at inflexion point
#' @param infl x value at inflexion point
#'
#' @return
#' @export
#'
#' @examples
sigmoid=function(X,max,slope,infl){
  max/(1+exp(4* slope*(infl-X)))
}


#' Fit sigmoid curve on observed LAI
#'
#' @param dataLAI data with LAI value over MAP
#' @param LAI_max LAI max when the canopy is closed
#' @return coefficient of the sigmaoid:
#' @export
#'
#' @examples
mod_LAI_evo= function(dataLAI,LAI_max=6){
  
  # coeff=dataLAI%>%
  #   filter(!is.na(LAI) & !is.na(MAP))%>%
  #   dplyr::do(mod=
  #               nls(data = .,
  #                   formula =
  #                     LAI~sigmoid(X= MAP,max= LAI_max,
  #                                 slope= LAI_slope,infl= LAI_infl),
  #                   start= c(LAI_slope= 0.01, LAI_infl= 60)))%>%
  #   dplyr::mutate(LAI_max=LAI_max,
  #                 LAI_slope= stats::coef(.data$mod)['LAI_slope'],
  #                 LAI_infl= stats::coef(.data$mod)['LAI_infl'],
  #                 sigma= summary(.data$mod)$sigma)
  
  
  coeff=coef(nls(data = dataLAI%>%
                   filter(!is.na(LAI) & !is.na(MAP)),
                 formula =
                   LAI~sigmoid(X= MAP,max= LAI_max,
                               slope= LAI_slope,infl= LAI_infl),
                 start= c(LAI_slope= 0.01, LAI_infl= 60)))
  
  return(coeff)
}


#' Coefficient of stress for evaporation
#'
#' @param NivRempC fill level of the evaporative compartment
#' @param pEvap treshold
#'
#' @return
#' @export
#'
#' @examples
#' 
ksEvap = function(NivRempC,pEvap)
{
  ks=ifelse(test = NivRempC >= pEvap ,yes =  1,no=1/(pEvap) * NivRempC)
  return(ks)
}

#' Coefficient of stress for transpiration
#'
#' @param NivRempC fill level of the root compartment
#' @param pTransp treshold
#'
#' @return
#' @export
#'
#' @examples
#' 
ksTransp = function(NivRempC,pTransp)
{
  ks=ifelse(test = NivRempC >= pTransp,yes = 1,no= 1/(pTransp) * NivRempC)
  
  return(ks)
}






#' Estimation of water balance and derived variables
#'
#' @param data : meteo data 
#' @param dataLAI :data with fitted value of LAI over time generate from mod_LAI_evo
#' @param Lat Latitude in degree (+ for North, - for South)
#' @param Alt Altitude (m)
#' @param Hcr Soil humidity at field capacity (%)
#' @param Hpf Soil humidity at wilting point (%)
#' @param Z1 Depth of the evaporative layer(mm)
#' @param root_depth_max maximum depth of the roots (Z1 +root_depth_max = soil depth)(mm)
#' @param root_depth_ini depth of the roots at planting (mm)
#' @param root_slope speed of root growth in depth (mm. MAP-1)
#' @param root_infl MAP when the root growth in depth is maximal (MAP)
#' @param LAImax Leaf area index maximal (m2.m-2)
#' @param Tresh_Inf_Rainfall Rainfall treshold below which rainfall is totally evaporated (mm)
#' @param Tresh_Sup_Rainfall Rainfall treshold above which rainfall runs off (mm)
#' @param H0 initial  soil humidity(mm.mm-1)
#' @param Kc cultural coefficient
#' @param k Beer Lambert extinction coefficient
#' @param debut days of simulation start 
#' @param pEvap filling treshold of the evaporative compartment reducing EvMax
#' @param pTransp filling treshold of the transpiration compartment reducing Tr
#' 
#' @return
#' @export
#' @examples
waterBalance=function(data,dataLAI,Lat=-2.93416,Alt= 15.50000,Hcr=0.25000,Hpf= 0.12000,Z1 = 200.00000,root_depth_max = 40000,root_depth_ini=500, root_slope=0.03,root_infl=18,LAImax=6,Tresh_Inf_Rainfall=2,Tresh_Sup_Rainfall=50,H0 = 0.16,Kc = 1,k = 0.5,debut = 2,pEvap = 0.5,pTransp = 0.5 ){
  
  
  print('running water balance model')
  
  # fixed parameters ---------------------------------------------------------
  
  # ##default values
  # H0 = 0.16   #initial  soil humidity(mm.mm-1)
  # Kc = 1      # cultural coefficient
  # k = 0.5      # Beer Lambert extinction coefficient
  # debut = 2    # days of simulation start
  # pEvap = 0.5   # filling treshold of the evaporative compartment reducing EvMax
  # pTransp = 0.5 # filling treshold of the transpiration compartment reducing Tr
  
  #### debug
  
  # LAImax<- 6
  # Lat <- 2.93416
  # Alt <- 15.50000
  # Hcr <- 0.25000
  # Hpf <- 0.12000
  # Z1 <- 200.00000
  # root_depth_max = 4000
  # root_depth_ini=500
  # root_slope=0.03
  # root_infl=18
  # Tresh_Inf_Rainfall <- 2.00000
  # Tresh_Sup_Rainfall <- 50.00000
  
  
  # data=read.table('Data/Meteo_extract_BDD_PR.txt', header=TRUE,  sep="\t")%>%
  #   mutate(ObservationDate=ymd(ObservationDate),
  #          PlantingDate=ymd('2010-05-01'),
  #          MAP=mondf(d2 = ObservationDate,d1 = PlantingDate))
  # 
  # growthPR=read.csv(file ='Data/Full_Data_Plot_Growth_PR_G5-1.txt',header=T,sep='\t')
  # growthPR$PlantingDate=ymd('2010-05-01')
  # growthPR$Density=143
  # growthPR=growthPR%>%
  #   mutate(ObservationDate=ymd(ObservationDate),
  #          HarvestDate=ymd(HarvestDate),
  #          HarvestMonth=ymd(paste0(str_sub(string =HarvestDate,start = 0,end = 7),'-01')),
  #          MAP=mondf(d2 = ObservationDate,d1 = PlantingDate),
  #          HarvestMAP=mondf(d2 = HarvestDate,d1 = PlantingDate),
  #          TreeId=as.factor(paste(Site,Plot,BlockNumber,IdGenotype,LineNumber,TreeNumber,sep='_')))
  # 
  ###compute LAI####
  
  # coefArrea=0.7
  # NbLeaves=32
  # 
  # dataLAI=growthPR%>%
  #   filter(!is.na(MAP))%>%
  #   mutate(AverageleafletAreaBase=AverageLeafletSampleLengthBase*AverageLeafletSampleWidthBase*coefArrea,
  #          AverageleafletAreaMidd=AverageLeafletSampleLengthMidd*AverageLeafletSampleWidthMidd*coefArrea,
  #          AverageleafletAreaTop=AverageLeafletSampleLengthTop*AverageLeafletSampleWidthTop*coefArrea
  #   )%>%
  #   mutate(LeafArea=NumberOfLeaflets*(AverageleafletAreaBase+AverageleafletAreaMidd+AverageleafletAreaTop)/(3*10000),
  #          LAI=NbLeaves*LeafArea*Density/10000)%>%
  #   dplyr::select(Site,IdGenotype,TreeId,Plot,BlockNumber,LineNumber,TreeNumber,PhytomerNumber,MAP,LAI)%>%
  #   group_by(Site,MAP)%>%
  #   summarize(LAI=mean(LAI,na.rm=T))
  
  dataLAI=dataLAI%>%
    dplyr::select(Site,IdGenotype,TreeId,Plot,BlockNumber,LineNumber,TreeNumber,PhytomerNumber,MAP,LAI)%>%
    group_by(Site,MAP)%>%
    summarize(LAI=mean(LAI,na.rm=T))
  
  
  coeff_LAI=mod_LAI_evo(dataLAI,LAI_max = LAImax)
  
  data=merge(data,dataLAI,all.x=T)%>%
    mutate(LAI_estim=sigmoid(X = MAP,max =LAImax,slope = coeff_LAI['LAI_slope'],infl = coeff_LAI['LAI_infl']))
  
  
  ###compute root depth
  data=data%>%
    mutate(root_depth=root_depth_ini+sigmoid(X = MAP,max=root_depth_max,slope=root_slope,infl=root_infl))
  
  
  ###change dates format
  data=data%>%
    mutate(Date = ymd(ObservationDate),
           Day=lubridate::day(Date),
           Month=lubridate::month(Date),
           Year=lubridate::year(Date),
           Lat_rad=Lat*pi/180)%>%
    arrange(Date)
  
  
  # Estimation of ETO (adapted from JC Combres) -----------------------------
  
  ###Estimate Rg when data is missing
  a = lm(data$Rg ~ data$TAverage)
  coef.lm = coef(a)
  data$Rg[is.na(data$Rg)] = coef.lm[1] + coef.lm[2] * data$TAverage[is.na(data$Rg)]
  
  
  data=data%>%
    mutate(Tmoy = (TMin+TMax)/2, # Tmoy=average daily temperature
           Hmoy =(HRMax+HRMin)/2, # Hmoy=average daily relative humidity
           Doy=yday(Date), # Doy=day of hte year
           Decli=0.409*sin(0.0172*Doy-1.39), # Decli=sun declination (radians)
           SunPos=acos(-tan(Lat_rad)*tan(Decli)), # SunPos= sun position (degree)
           Sundist=1+0.033*cos(2*(pi/365)*Doy), # Sundist=distance to sun 
           Ray_extra=24*60*0.0820/pi*Sundist*(SunPos*sin(Decli)*sin(Lat_rad)+cos(Decli)*cos(Lat_rad)*sin(SunPos)),# Ray_extra= extraterestrial radiation
           Rg_max=(0.75+0.00002*Alt)*Ray_extra,
           Day_Lenght=7.64*SunPos,
           PAR=0.48*Rg,
           esat=0.3054*(exp(17.27*TMax/(TMax+237.3))+exp(17.27*TMin/(TMin+237.3))),# esat= saturing water vapor pressure (kPa)
           ea=0.3054*(exp(17.27*TMax/(TMax+237.3))*HRMin/100+exp(17.27*TMin/(TMin+237.3))*HRMax/100),# ea=partial water vapor pressure (kPa)
           VPD=esat-ea,# VPD=vapor pressure deficit (kPa)
           VPDmax=0.6108*exp(17.27*TMax/(TMax+237.3))*(1-HRMin/100),
           ratioRg=ifelse(Rg>Rg_max,1,Rg/Rg_max),
           Rn=0.77*Rg-(1.35*ratioRg-0.35)*(0.34-0.14*(ea)**(0.5))*((TMax+273.16)**4+(TMin+273.16)**4)*2.45015*10**(-9),
           Tlat=2.501-2.361*10**(-3)*Tmoy,
           pent_vap_sat=4098*(0.6108*exp(17.27*Tmoy/(Tmoy+237.3)))/((Tmoy+237.3)**2),
           Kpsy= 0.00163*101.3*(1-(0.0065*Alt/293))**5.26,
           Erad= 0.408*Rn*pent_vap_sat/(pent_vap_sat+Kpsy*(1+0.34*WindSpeed)),# Erad= radiative term
           Eaero=((900/(Tmoy+273.16))*(( esat - ea)* WindSpeed)*Kpsy)/(pent_vap_sat+Kpsy*(1+0.34*WindSpeed)),# Eaero= advective term
           ET0=Erad+Eaero)
  
  
  # initialisation of water balance calculation -----------------------------------------------
  
  data=data%>%
    mutate(LAI_obs=LAI,
           LAI=LAI_estim,
           epsi =  1 - exp(-k*LAI),
           EvapMax = (1 - epsi) * ET0 * Kc,# EvapMax = maximal evaporation
           TranspMax = epsi * ET0 * Kc # TranspMax= maximal transpiration
    ) 
  
  
  
  ###initialisation of compartment size
  
  data=data%>%
    mutate(Z=0,
           Evap = NA,
           Transp = NA,
           QuantH2OC1 = NA,
           QuantH2OC1moinsVap = NA,
           QuantH2OVap = NA,
           QuantH2OC2 = NA,
           QuantH2OC = NA,
           
           QuantH2OC1Racine = NA,
           QuantH2OC1moinsVapRacine = NA,
           QuantH2OVapRacine = NA,
           QuantH2OC2Racine = NA,
           QuantH2OCRacine = NA,
           
           EvapC1moinsVap = NA,
           EvapVap = NA,
           
           TranspC1moinsVap = NA,
           TranspC2 = NA,
           QuantH2OC1RacineApresEvap = NA,
           QuantH2OC2RacineApresEvap = NA,
           QuantH2OCRacineApresEvap = NA,
           QuantH2OVapRacineApresEvap = NA,
           QuantH2OC1moinsVapRacineApresEvap = NA,
           QuantH2OC1ApresEvap = NA,
           QuantH2OC2ApresEvap = NA,
           QuantH2OCApresEvap = NA,
           QuantH2OVapApresEvap = NA,
           QuantH2OC1moinsVapApresEvap = NA,
           Z1Racine = NA,
           Z2Racine = NA,
           EfficientRain = NA, ##rain that goes to the compartments
           TailleC1 = (Hcr - 0.5 * Hpf) * Z1,
           TailleVap = 0.5 * Hpf * Z1,
           TailleC1moinsVap = TailleC1 - TailleVap,
           TailleC2 = (Hcr - Hpf) * root_depth,
           TailleC = TailleC2 + TailleC1 - TailleVap,
           
           ##initial water content in the compartment
           
           QuantH2OInitC1 = min(TailleC1,(H0 - 0.5 * Hpf) * Z1),
           QuantH2OInitVap = min(TailleVap, (H0 - 0.5 * Hpf) * Z1),
           QuantH2OInitC2 = min(TailleC2,(H0 - Hpf) * root_depth),
           QuantH2OInitC = QuantH2OInitC1 + QuantH2OInitC2 - QuantH2OInitVap,
           QuantH2OInitC1moinsVap = QuantH2OInitC1 - QuantH2OInitVap,
           Recalcul_ET0 = NA
    )
  
  ###initialisation of the root compartments size and water content
  data[debut - 1,]=data%>%
    filter(row_number()==debut - 1)%>%
    mutate(QuantH2OC1=max(0,QuantH2OInitC1),
           QuantH2OVap= max(0,QuantH2OInitVap),
           QuantH2OC1moinsVap=  max(0,QuantH2OInitC1 - QuantH2OInitVap),
           QuantH2OC2 = max(0,QuantH2OInitC2),
           QuantH2OC = max(0,QuantH2OInitC))
  
  
  ### fraction of water in compartment
  data=data%>%
    mutate(FractionC1 = QuantH2OC1 / TailleC1,
           FractionC2 = QuantH2OC2 / TailleC2,
           FractionC = QuantH2OC / TailleC)
  
  
  ### average estimation when missing data
  data=data%>%
    mutate(Recalcul_ET0 =ifelse(is.na(EvapMax), "average estimation of EvapMax ",Recalcul_ET0),
           Recalcul_ET0 =ifelse(is.na(TranspMax), "average estimation of TranspMax ",Recalcul_ET0),
           EvapMax=ifelse(is.na(EvapMax),mean(EvapMax,na.rm=T),EvapMax),
           TranspMax=ifelse(is.na(TranspMax),mean(TranspMax,na.rm=T),TranspMax))
  
  ### stop calculation when rainfall is missing
  
  for (j in debut:nrow(data)){
    
    ifelse(is.na(data$Rainfall[j]),                                
           {   
             data$QuantH2OC2[j] = data$QuantH2OC2[j-1] 
             data$QuantH2OC[j] = data$QuantH2OC[j-1] 
             data$QuantH2OC1moinsVap[j] = data$QuantH2OC1moinsVap[j-1]
             data$QuantH2OVap[j] = data$QuantH2OVap[j-1]
             data$QuantH2OC1[j] = data$QuantH2OC1[j-1]
             
             data$FractionC1[j] =  data$QuantH2OC1[j] / data$TailleC1[j]
             data$FractionC2[j] =  data$QuantH2OC2[j] / data$TailleC2[j]
             data$FractionC[j] =  data$QuantH2OC[j] / data$TailleC[j]
           },
           {                                                  
             
             
             # Distribution of rainfall in the compartments ----------------------------
             
             
             ifelse(data$Rainfall[j] < Tresh_Inf_Rainfall,
                    {
                      data$EfficientRain[j] = 0
                    },
                    {
                      ifelse(data$Rainfall[j] > Tresh_Sup_Rainfall,
                             {
                               data$EfficientRain[j] = Tresh_Sup_Rainfall - Tresh_Inf_Rainfall
                             },
                             {
                               data$EfficientRain[j] = data$Rainfall[j] - Tresh_Inf_Rainfall
                             }
                      )
                    }
             )
             
             ifelse((data$QuantH2OVap[j-1] + data$EfficientRain[j]) >= data$TailleVap[j],
                    {
                      
                      data$QuantH2OVap[j] = data$TailleVap[j]
                      ifelse((data$QuantH2OC1moinsVap[j-1] + ( data$EfficientRain[j] - data$TailleVap[j] + data$QuantH2OVap[j-1] )) >= data$TailleC1moinsVap[j],
                             {
                               data$QuantH2OC1moinsVap[j] = data$TailleC1moinsVap[j]
                               data$QuantH2OC1[j] = data$QuantH2OC1moinsVap[j] + data$QuantH2OVap[j]
                               ifelse((data$QuantH2OC2[j-1] + data$QuantH2OC1[j-1] + data$EfficientRain[j] - data$TailleC1[j]) >= data$TailleC2[j],
                                      {
                                        data$QuantH2OC2[j] = data$TailleC2[j]
                                      },
                                      {data$QuantH2OC2[j] = data$QuantH2OC2[j-1] + data$QuantH2OC1[j-1] + data$EfficientRain[j] - data$TailleC1[j]
                                      }
                               )
                             },
                             {data$QuantH2OC1moinsVap[j] = data$QuantH2OC1moinsVap[j-1] + ( data$EfficientRain[j] - data$TailleVap[j] + data$QuantH2OVap[j-1] )
                             data$QuantH2OC1[j] = data$QuantH2OC1moinsVap[j] + data$QuantH2OVap[j]
                             data$QuantH2OC2[j] =data$QuantH2OC2[j-1]
                             }
                      )
                    },
                    {data$QuantH2OVap[j] = data$QuantH2OVap[j-1] + data$EfficientRain[j]
                    data$QuantH2OC1moinsVap[j] = data$QuantH2OC1moinsVap[j-1]
                    data$QuantH2OC1[j] = data$QuantH2OC1moinsVap[j] + data$QuantH2OVap[j]
                    data$QuantH2OC2[j] = data$QuantH2OC2[j-1]
                    }
             )
             
             data$QuantH2OC[j] = data$QuantH2OC1[j] - data$QuantH2OVap[j] + data$QuantH2OC2[j]
             data$FractionC1[j] =  data$QuantH2OC1[j] / data$TailleC1[j]
             data$FractionC2[j] =  data$QuantH2OC2[j] / data$TailleC2[j]
             data$FractionC[j] =  data$QuantH2OC[j] / data$TailleC[j]
             
             
             # Daily evaporation and ditribution among compartments --------------------
             
             data$Evap[j] = data$EvapMax[j] * ksEvap(data$FractionC1[j],pEvap)
             
             ifelse(data$QuantH2OC1moinsVap[j] - data$Evap[j] >=0,
                    {
                      data$QuantH2OC1moinsVap[j] = data$QuantH2OC1moinsVap[j] - data$Evap[j]
                      data$EvapC1moinsVap[j] = data$Evap[j]
                      data$EvapVap[j] = 0
                    },
                    {
                      data$EvapC1moinsVap[j] = data$QuantH2OC1moinsVap[j]
                      data$QuantH2OC1moinsVap[j] = 0
                      data$EvapVap[j] = data$Evap[j] - data$EvapC1moinsVap[j]
                      data$QuantH2OVap[j] = data$QuantH2OVap[j] - data$EvapVap[j]
                    }
             )
             
             data$QuantH2OC1[j] = data$QuantH2OC1moinsVap[j] + data$QuantH2OVap[j]
             data$QuantH2OC[j] = data$QuantH2OC1[j] + data$QuantH2OC2[j] - data$QuantH2OVap[j]
             
             
             data$FractionC1[j] =  data$QuantH2OC1[j] / data$TailleC1[j]
             data$FractionC2[j] =  data$QuantH2OC2[j] / data$TailleC2[j]
             data$FractionC[j] =  data$QuantH2OC[j] / data$TailleC[j]
             
             
             
             
             # Daily Transpiration -----------------------------------------------------
             data$Transp[j] = data$TranspMax[j] * ksTransp(data$FractionC[j],pTransp)
             ifelse((data$QuantH2OC2[j]>0),
                    {data$TranspC2[j]= min(data$Transp[j] * (data$QuantH2OC2[j] / (data$QuantH2OC2[j] + data$QuantH2OC1moinsVap[j])),data$QuantH2OC2[j])},
                    {data$TranspC2[j]=0}
             )
             
             ifelse((data$QuantH2OC1moinsVap[j]>0),
                    {data$TranspC1moinsVap[j]= min(data$Transp[j] * (data$QuantH2OC1moinsVap[j] / (data$QuantH2OC2[j] + data$QuantH2OC1moinsVap[j])),data$QuantH2OC1moinsVap[j])},
                    {data$TranspC1moinsVap[j]=0}
             )
             
             data$QuantH2OC2[j] = data$QuantH2OC2[j] - data$TranspC2[j]
             data$QuantH2OC[j] = data$QuantH2OC1moinsVap[j] + data$QuantH2OC2[j]
             data$QuantH2OC1[j] = data$QuantH2OC1moinsVap[j] + data$QuantH2OVap[j]
             
           })
  }
  
  
  
  
  
  # outputs -----------------------------------------------------------------
  
  data=data%>%
    mutate(Evap=Evap,
           Transp=Transp,
           ETR = Transp + Evap,
           ETM = EvapMax + TranspMax,
           ATSW=ifelse(is.na(Rainfall),NA,QuantH2OC),
           FTSW=FractionC,
           Calcul = Recalcul_ET0
    )
  
  
  return(data)
  
}




