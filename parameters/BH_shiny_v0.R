######SCRIPT TO ESTIMATE WATER BALANCE#####
# adapted from Pallas B. &  Rouan L.

# rm(list=ls(all=TRUE))

# Packages  ---------------------------------------------------------------

packs <- c("lubridate",'dplyr','data.table')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)


# Functions ---------------------------------------------------------------

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

ksTransp = function(NivRempC,pTransp)
{
  ks=ifelse(test = NivRempC >= pTransp,yes = 1,no= 1/(pTransp) * NivRempC)

  return(ks)
}


#' Coefficient of stress for transpiration
#'
#' @param NivRempC fill level of the root compartment
#' @param pRacine treshold
#'
#' @return
#' @export
#'
#' @examples
ksRacine = function(NivRempC,pRacine)
{
  ks=ifelse(test = NivRempC >= pRacine, yes = 1,no = 1/(pRacine) * NivRempC)
  return(ks)
}


#' Estimation of water balance and derived variables
#'
#' @param data : meteo data 
#' @param Lat Latitude in degree (+ for North, - for South)
#' @param Alt Altitude (m)
#' @param Hcr Soil humidity at field capacity (%)
#' @param Hpf Soil humidity at wilting point (%)
#' @param Z1 Depth of the evaporative layer
#' @param Z2 Depth of the roots (Z1 +Z2 = soil depth)
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
waterBalance_V0=function(data,Lat=-2.93416,Alt= 15.50000,Hcr=0.25000,Hpf= 0.12000,Z1 = 200.00000,Z2 = 2500.00000,LAImax=4,Tresh_Inf_Rainfall=2,Tresh_Sup_Rainfall=50,H0 = 0.16,Kc = 1,k = 0.5,debut = 2,pEvap = 0.5,pTransp = 0.5 ){

  
  # fixed parameters ---------------------------------------------------------
  
  # H0 = 0.16   #initial  soil humidity(mm.mm-1)
  # Kc = 1      # cultural coefficient
  # k = 0.5      # Beer Lambert extinction coefficient
  # debut = 2    # days of simulation start
  # pEvap = 0.5   # filling treshold of the evaporative compartment reducing EvMax
  # pTransp = 0.5 # filling treshold of the transpiration compartment reducing Tr
  # 
  ##default values
  # data=read.table('Data/Meteo_extract_BDD_PR.txt', header=TRUE,  sep="\t")
  # Lat <- 2.93416
  # Alt <- 15.50000
  # Hcr <- 0.25000
  # Hpf <- 0.12000
  # Z1 <- 200.00000
  # Z2 <- 2500.00000
  # LAImax <- 3.00000
  # Tresh_Inf_Rainfall <- 2.00000
  # Tresh_Sup_Rainfall <- 50.00000
  
  # id = unique(data$Site)
  
  
  data=data%>%
    mutate(Date = ymd(ObservationDate),
           Day=lubridate::day(Date),
           Month=lubridate::month(Date),
           Year=lubridate::year(Date),
           Lat_rad=Lat*pi/180,
           Tmoy = (TMin+TMax)/2,
           Hmoy =(HRMax+HRMin)/2)
  
  
  # Estimation of ETO (adapted from JC Combres) -----------------------------
  
  ###Estimate Rg when data is missing
  a = lm(data$Rg ~ data$Tmoy)
  coef.lm = coef(a)
  data$Rg[is.na(data$Rg)] = coef.lm[1] + coef.lm[2] * data$Tmoy[is.na(data$Rg)]
  
  # Tmoy=average daily temperature
  # Hmoy=average daily relative humidity
  # Doy=day of hte year
  # Decli=sun declination (radians)
  # SunPos= sun position (degree)
  # Sundist=distance to sun 
  # Ray_extra= extraterestrial radiation
  # esat= saturing water vapor pressure (kPa)
  # ea=partial water vapor pressure (kPa)
  # VPD=vapor pressure deficit (kPa)
  # Erad= radiative term
  # Eaero= advective term
  
  data=data%>%
    mutate(Doy=yday(Date),
           Decli=0.409*sin(0.0172*Doy-1.39),
           SunPos=acos(-tan(Lat_rad)*tan(Decli)),
           Sundist=1+0.033*cos(2*(pi/365)*Doy),
           Ray_extra=24*60*0.0820/pi*Sundist*(SunPos*sin(Decli)*sin(Lat_rad)+cos(Decli)*cos(Lat_rad)*sin(SunPos)),
           Rg_max=(0.75+0.00002*Alt)*Ray_extra,
           Day_Lenght=7.64*SunPos,
           PAR=0.48*Rg,
           esat=0.3054*(exp(17.27*TMax/(TMax+237.3))+exp(17.27*TMin/(TMin+237.3))),
           ea=0.3054*(exp(17.27*TMax/(TMax+237.3))*HRMin/100+exp(17.27*TMin/(TMin+237.3))*HRMax/100),
           VPD=esat-ea,
           VPDmax=0.6108*exp(17.27*TMax/(TMax+237.3))*(1-HRMin/100),
           ratioRg=ifelse(Rg>Rg_max,1,Rg/Rg_max),
           Rn=0.77*Rg-(1.35*ratioRg-0.35)*(0.34-0.14*(ea)**(0.5))*((TMax+273.16)**4+(TMin+273.16)**4)*2.45015*10**(-9),
           Tlat=2.501-2.361*10**(-3)*Tmoy,
           pent_vap_sat=4098*(0.6108*exp(17.27*Tmoy/(Tmoy+237.3)))/((Tmoy+237.3)**2),
           Kpsy= 0.00163*101.3*(1-(0.0065*Alt/293))**5.26,
           Erad= 0.408*Rn*pent_vap_sat/(pent_vap_sat+Kpsy*(1+0.34*WindSpeed)),
           Eaero=((900/(Tmoy+273.16))*(( esat - ea)* WindSpeed)*Kpsy)/(pent_vap_sat+Kpsy*(1+0.34*WindSpeed)),
           ET0=Erad+Eaero)
  
  
  # initialisation of water balance calculation -----------------------------------------------
  NbDays=nrow(data)
  All_NA = rep(NA,NbDays)
  All_zero = rep(0,NbDays)
  
  ###EvapMax = maximal evaporation
  # TranspMax= maximal transpiration
  data=data%>%
    mutate(LAI=LAImax,
           epsi =  1 - exp(-k*LAI),
           EvapMax = (1 - epsi) * ET0 * Kc,
           TranspMax = epsi * ET0 * Kc)
  
  
  Rainfall = data$Rainfall
  EvapMax=data$EvapMax
  TranspMax=data$TranspMax
  LAI = data$LAI
  
  
  ###initialisation of compartment size
  
  Z = All_zero
  TailleC1Racine = All_zero
  TailleVapRacine = All_zero
  TailleC2Racine = All_zero
  TailleC1moinsVapRacine = All_zero
  TailleCRacine = All_zero
  TT = All_zero
  Evap = All_NA
  Transp = All_NA
  QuantH2OC1 = All_NA
  QuantH2OC1moinsVap = All_NA
  QuantH2OVap = All_NA
  QuantH2OC2 = All_NA
  QuantH2OC = All_NA
  
  QuantH2OC1Racine = All_NA
  QuantH2OC1moinsVapRacine = All_NA
  QuantH2OVapRacine = All_NA
  QuantH2OC2Racine = All_NA
  QuantH2OCRacine = All_NA
  
  EvapC1moinsVap = All_NA
  EvapVap = All_NA
  
  TranspC1moinsVap = All_NA
  TranspC2 = All_NA
  
  toto =  All_NA
  
  QuantH2OC1RacineApresEvap = All_NA
  QuantH2OC2RacineApresEvap = All_NA
  QuantH2OCRacineApresEvap = All_NA
  QuantH2OVapRacineApresEvap = All_NA
  QuantH2OC1moinsVapRacineApresEvap = All_NA
  QuantH2OC1ApresEvap = All_NA
  QuantH2OC2ApresEvap = All_NA
  QuantH2OCApresEvap = All_NA
  QuantH2OVapApresEvap = All_NA
  QuantH2OC1moinsVapApresEvap = All_NA
  Z1Racine = All_NA
  Z2Racine = All_NA
  EfficientRain = All_NA ##rain that goes to the compartments
  
  TailleC1 = (Hcr - 0.5 * Hpf) * Z1
  TailleVap = 0.5 * Hpf * Z1
  TailleC1moinsVap = TailleC1 - TailleVap
  TailleC2 = (Hcr - Hpf) * Z2
  TailleC = TailleC2 + TailleC1 - TailleVap
  TailleC1Racine[debut - 1 ] = TailleC1
  TailleVapRacine[debut - 1] = TailleVap
  TailleC1moinsVapRacine[debut - 1]=TailleC1moinsVap
  
  ##initial water content in the compartment
  
  QuantH2OInitC1 = min(TailleC1,(H0 - 0.5 * Hpf) * Z1)
  
  QuantH2OInitVap = min(TailleVap, (H0 - 0.5 * Hpf) * Z1)
  
  QuantH2OInitC2 = min(TailleC2,(H0 - Hpf) * Z2)
  
  QuantH2OInitC = QuantH2OInitC1 + QuantH2OInitC2 - QuantH2OInitVap
  
  QuantH2OInitC1moinsVap = QuantH2OInitC1 - QuantH2OInitVap
  
  
  QuantH2OC1[debut-1] = max(0,QuantH2OInitC1)
  QuantH2OVap[debut-1] = max(0,QuantH2OInitVap)
  QuantH2OC1moinsVap[debut-1] =  max(0,QuantH2OInitC1 - QuantH2OInitVap)
  
  QuantH2OC2[debut-1] = max(0,QuantH2OInitC2)
  QuantH2OC[debut-1] = max(0,QuantH2OInitC)
  
  
  FractionC1 = QuantH2OC1 / TailleC1
  FractionC2 = QuantH2OC2 / TailleC2
  FractionC = QuantH2OC / TailleC
  
  
  missing_data = All_NA
  Recalcul_ET0 = All_NA 
  
  
  
  
  for (j in debut:NbDays){
    # j=2
    if (is.na(EvapMax[j]))  { EvapMax[j] = mean(EvapMax[!is.na(EvapMax)])
    Recalcul_ET0[j] = "average estimation of ET " }
    
    if (is.na(TranspMax[j]))  { TranspMax[j] = mean(TranspMax[!is.na(TranspMax)])
    Recalcul_ET0[j] = "average estimation of ET " }
    
    ifelse(is.na(Rainfall[j]),                                
           {   
             QuantH2OC2[j] = QuantH2OC2[j-1] 
             QuantH2OC[j] = QuantH2OC[j-1] 
             QuantH2OC1moinsVap[j] = QuantH2OC1moinsVap[j-1]
             QuantH2OVap[j] = QuantH2OVap[j-1]
             QuantH2OC1[j] = QuantH2OC1[j-1]
             
             FractionC1[j] =  QuantH2OC1[j] / TailleC1
             FractionC2[j] =  QuantH2OC2[j] / TailleC2
             FractionC[j] =  QuantH2OC[j] / TailleC
             missing_data[j] = "STOP_CALCULATION_WB"
             
           },
           {                                                  
             
             
             
             
             # Distribution of rainfall in the compartments ----------------------------
             
             
             
             ifelse(Rainfall[j] < Tresh_Inf_Rainfall,
                    {
                      EfficientRain[j] = 0
                    },
                    {
                      ifelse(Rainfall[j] > Tresh_Sup_Rainfall,
                             {
                               EfficientRain[j] = Tresh_Sup_Rainfall - Tresh_Inf_Rainfall
                             },
                             {
                               EfficientRain[j] = Rainfall[j] - Tresh_Inf_Rainfall
                             }
                      )
                    }
             )
             
             ifelse((QuantH2OVap[j-1] + EfficientRain[j]) >= TailleVap,
                    {
                      
                      QuantH2OVap[j] = TailleVap
                      ifelse((QuantH2OC1moinsVap[j-1] + ( EfficientRain[j] - TailleVap + QuantH2OVap[j-1] )) >= TailleC1moinsVap,
                             {
                               QuantH2OC1moinsVap[j] = TailleC1moinsVap
                               QuantH2OC1[j] = QuantH2OC1moinsVap[j] + QuantH2OVap[j]
                               ifelse((QuantH2OC2[j-1] + QuantH2OC1[j-1] + EfficientRain[j] - TailleC1) >= TailleC2,
                                      {
                                        QuantH2OC2[j] = TailleC2
                                      },
                                      {QuantH2OC2[j] = QuantH2OC2[j-1] + QuantH2OC1[j-1] + EfficientRain[j] - TailleC1
                                      }
                               )
                             },
                             {QuantH2OC1moinsVap[j] = QuantH2OC1moinsVap[j-1] + ( EfficientRain[j] - TailleVap + QuantH2OVap[j-1] )
                             QuantH2OC1[j] = QuantH2OC1moinsVap[j] + QuantH2OVap[j]
                             QuantH2OC2[j] =QuantH2OC2[j-1]
                             }
                      )
                    },
                    {QuantH2OVap[j] = QuantH2OVap[j-1] + EfficientRain[j]
                    QuantH2OC1moinsVap[j] = QuantH2OC1moinsVap[j-1]
                    QuantH2OC1[j] = QuantH2OC1moinsVap[j] + QuantH2OVap[j]
                    QuantH2OC2[j] = QuantH2OC2[j-1]
                    }
             )
             
             QuantH2OC[j] = QuantH2OC1[j] - QuantH2OVap[j] + QuantH2OC2[j]
             FractionC1[j] =  QuantH2OC1[j] / TailleC1
             FractionC2[j] =  QuantH2OC2[j] / TailleC2
             FractionC[j] =  QuantH2OC[j] / TailleC
             
             
             
             
             # Daily evaporation and ditribution among compartments --------------------
             
             Evap[j] = EvapMax[j] * ksEvap(FractionC1[j],pEvap)
             
             ifelse(QuantH2OC1moinsVap[j] - Evap[j] >=0,
                    {
                      QuantH2OC1moinsVap[j] = QuantH2OC1moinsVap[j] - Evap[j]
                      EvapC1moinsVap[j] = Evap[j]
                      EvapVap[j] = 0
                    },
                    {
                      EvapC1moinsVap[j] = QuantH2OC1moinsVap[j]
                      QuantH2OC1moinsVap[j] = 0
                      EvapVap[j] = Evap[j] - EvapC1moinsVap[j]
                      QuantH2OVap[j] = QuantH2OVap[j] - EvapVap[j]
                    }
             )
             
             QuantH2OC1[j] = QuantH2OC1moinsVap[j] + QuantH2OVap[j]
             QuantH2OC[j] = QuantH2OC1[j] + QuantH2OC2[j] - QuantH2OVap[j]
             
             
             FractionC1[j] =  QuantH2OC1[j] / TailleC1
             FractionC2[j] =  QuantH2OC2[j] / TailleC2
             FractionC[j] =  QuantH2OC[j] / TailleC
             
             
             
             
             # Daily Transpiration -----------------------------------------------------
             Transp[j] = TranspMax[j] * ksTransp(FractionC[j],pTransp)
             ifelse((QuantH2OC2[j]>0),
                    {TranspC2[j]= min(Transp[j] * (QuantH2OC2[j] / (QuantH2OC2[j] + QuantH2OC1moinsVap[j])),QuantH2OC2[j])},
                    {TranspC2[j]=0}
             )
             
             ifelse((QuantH2OC1moinsVap[j]>0),
                    {TranspC1moinsVap[j]= min(Transp[j] * (QuantH2OC1moinsVap[j] / (QuantH2OC2[j] + QuantH2OC1moinsVap[j])),QuantH2OC1moinsVap[j])},
                    {TranspC1moinsVap[j]=0}
             )
             # Attention il y a des chiffres n?gatif dans FracRacine d'o? le min max du coup j'obtiens des 0 dans les quantit?s QuantH2OC1moinsVap
             # et je divise du coup par 0 par la suite
             # attention pb sur apresEvap apres transpi
             
             
             QuantH2OC2[j] = QuantH2OC2[j] - TranspC2[j]
             QuantH2OC[j] = QuantH2OC1moinsVap[j] + QuantH2OC2[j]
             QuantH2OC1[j] = QuantH2OC1moinsVap[j] + QuantH2OVap[j]
             
           })
  }
  
  cases_a_enlever =   missing_data == "STOP_CALCULATION_WB"
  QuantH2OC[cases_a_enlever] = 'NA'
  QuantH2OC = as.numeric(QuantH2OC)
  
  
  
  # outputs -----------------------------------------------------------------

  data=data%>%
    mutate(Evap=Evap,
           Transp=Transp,
           ETR = Transp + Evap,
           ETM = EvapMax + TranspMax,
           ATSW = QuantH2OC,
           FTSW=FractionC,
           Calcul = Recalcul_ET0,
           missing_data=ifelse(is.na(data$Rainfall),1,0)
    )
  
  return(data)
  
}




# test --------------------------------------------------------------------

met=data.table::fread(file = 'meteo.txt')
colnames(met)=c('day','month','year','TMax','TMin','HRMax','HRMin','WindSpeed','Rg','Rainfall')
met=met%>%
  mutate(ObservationDate=paste(year,month,day,sep='-'))
  
met$Date=ymd(met$ObservationDate)
met=waterBalance_V0(data =met,Lat = 0.97,Alt = 50,Hcr = 0.23,Hpf = 0.1,Z1 = 200,Z2 = 2000,LAImax = 4,Tresh_Inf_Rainfall = 2,Tresh_Sup_Rainfall = 50)

summary(met)

