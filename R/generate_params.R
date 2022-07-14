####generate parameters file fo Xpalm####



# Load packages -----------------------------------------------------------

packs <- c("lubridate", "stringr", "ggplot2",'dplyr','viridis','tidyr','data.table','plotly')
InstIfNec<-function (pack) {
  if (!do.call(require,as.list(pack))) {
    do.call(install.packages,as.list(pack))  }
  do.call(require,as.list(pack)) }
lapply(packs, InstIfNec)


#' Generate_params
#'
#' @param LATITUDE (rad)
#' @param ALTITUDE (m)
#' @param DENS (plnat.ha-1)
#' @param T_BASE (°C)
#' @param T_OPT1 (°C)
#' @param T_OPT2 (°C)
#' @param T_LIM (°C)
#' @param T_EFF_INI (dd)
#' @param Z1 (cm)
#' @param Z2 (cm)
#' @param H_CR 
#' @param H_PF 
#' @param H_0 
#' @param H_PF_Z1 
#' @param VITESSE_RACINES (mm.dd)
#' @param FTSW_RACINES 
#' @param SEUIL_EVAP 
#' @param SEUIL_TRANSPI 
#' @param Z_INIT (cm)
#' @param SEED 
#' @param AGE_INI (years)
#' @param AGE_ADULT (years)
#' @param AGE_PLANTING (years)
#' @param AGE_START_PROD (years)
#' @param PRODUCTION_SPEED_INITIAL (phytomer.dd-1)
#' @param PRODUCTION_SPEED_ADULT (phytomer.dd-1)
#' @param EFFICIENCE_BIOLOGIQUE (gCH2O.MJ-1)
#' @param K 
#' @param KC 
#' @param INACTIVE_PHYTOMER_NUMBER 
#' @param RANG_D_ABLATION 
#' @param AF_FRUITS 
#' @param STEM_APPARENT_DENSITY (g.cm-3)
#' @param STEM_RAYON (cm)
#' @param EN_LENGTH_INI (cm)
#' @param EN_LENGTH_ADULTE (cm)
#' @param DEBUT_CROISSANCE_EN (years)
#' @param FIN_CROISSANCE_EN (years)
#' @param PLASTICITY_INTERNODE_IC 
#' @param COUT_RESPI_MAINTENANCE_STIPE (gCH2O.gDM)
#' @param COUT_RESPI_INTERNODE (gCH2O.gDM)
#' @param TT_FLOWERING_ADULT (dd)
#' @param TT_FLOWERING_INITIAL (dd)
#' @param TT_HARVEST_ADULT (dd)
#' @param TT_HARVEST_INITIAL (dd)
#' @param PERIOD_ABORTION (dd)
#' @param PERIOD_SEX_DETERMINATION (dd)
#' @param FRACTION_PERIOD_OLEOSYNTHESIS
#' @param FRACTION_PERIOD_DEV_PEDUNCLE 
#' @param REPRO_CONSTRUCTION_COST (gCH2O.gDM)
#' @param COUT_RESPI_MAINTENANCE_REPRO (gCH2O.gDM)
#' @param SENSITIVITY_SEX 
#' @param SENSITIVITY_ABORTION 
#' @param INI_SEX_RATIO 
#' @param SEX_RATIO_REF 
#' @param ABORTION_RATE_REF 
#' @param ABORTION_RATE_MAX 
#' @param SEX_RATIO_MIN 
#' @param MASSE_INFLO_MALE_ADULTE (g)
#' @param PERIOD_MALE_INFLO (dd)
#' @param PERIOD_DEV_SPIKELET (dd)
#' @param SENSIVITY_IC_SPIKELET 
#' @param MASSE_MEAN_PEDUNCULE_ADULTE (g)
#' @param COUT_OIL (gCH2O.gDM)
#' @param OIL_CONTENT 
#' @param IND_FRUIT_WEIGHT (g)
#' @param MEAN_FRUIT_NUMBER_ADULTE 
#' @param PERIOD_FRUIT_SET (dd)
#' @param SENSIVITY_IC_SETTING 
#' @param POURCENT_NSC_ST_MAX 
#' @param POURCENT_NSC_ST_MIN 
#' @param POURCENT_NSC_ST_INI 
#' @param COUT_RESERVE (gCH2O.gDM)
#' @param REALL_COST (gCH2O.gDM)
#' @param MAXIMAL_SFIND (m2)
#' @param INITIAL_SFIND (m2)
#' @param INFLEXION (dd)
#' @param COURBURE (m2.dd)
#' @param COUT_RESPI_MAINTENANCE_LEAF (gCH2O.gDM)
#' @param COUT_RESPI_FEUILLE (gCH2O.gDM)
#' @param SLW_max (g.cm-2)
#' @param SLW_min (g.cm-2)
#' @param SLW_ini (g.cm-2)
#' @param POURC_FOLIOLE 
#' @param TRESH_SLOW_PHYLO 
#'
#' @return
#' @export
#'
#' @examples
generate_param=function(dir='',
                        filename='parameters',
                        LATITUDE = 0.97,
                        ALTITUDE = 50,
                        DENS = 143,
                        T_BASE = 15,
                        T_OPT1 = 25,
                        T_OPT2 = 30,
                        T_LIM = 40,
                        T_EFF_INI = 9,
                        Z1 = 200,
                        Z2 = 2000,
                        H_CR = 0.23,
                        H_PF = 0.10,
                        H_0 = 0.15,
                        H_PF_Z1 = 0.05,
                        VITESSE_RACINES = 0.3,
                        FTSW_RACINES = 0.2,     
                        SEUIL_EVAP = 0.5,
                        SEUIL_TRANSPI = 0.5,
                        Z_INIT = 2000 ,
                        SEED = 3,
                        AGE_INI = 3,
                        AGE_ADULT = 8,
                        AGE_PLANTING = 3,
                        AGE_START_PROD = 0,
                        PRODUCTION_SPEED_INITIAL = 0.01111111,
                        PRODUCTION_SPEED_ADULT = 0.007407407,
                        EFFICIENCE_BIOLOGIQUE = 4.8,
                        K = 0.5,
                        KC = 1,
                        INACTIVE_PHYTOMER_NUMBER = 60,
                        RANG_D_ABLATION = 50,
                        AF_FRUITS = 1.5,
                        STEM_APPARENT_DENSITY = 0.15,
                        STEM_RAYON = 30,
                        EN_LENGTH_INI = 1,
                        EN_LENGTH_ADULTE = 3,
                        DEBUT_CROISSANCE_EN = 4,
                        FIN_CROISSANCE_EN = 8,
                        PLASTICITY_INTERNODE_IC = 0.5,
                        COUT_RESPI_MAINTENANCE_STIPE = 0.001,
                        COUT_RESPI_INTERNODE = 1.44,
                        TT_FLOWERING_ADULT = 9430,
                        TT_FLOWERING_INITIAL = 6300,
                        TT_HARVEST_ADULT = 12150,
                        TT_HARVEST_INITIAL = 8100,
                        PERIOD_ABORTION = 1350,
                        PERIOD_SEX_DETERMINATION = 1350,
                        FRACTION_PERIOD_OLEOSYNTHESIS = 0.8,
                        FRACTION_PERIOD_DEV_PEDUNCLE = 0.2,
                        REPRO_CONSTRUCTION_COST = 1.44,
                        COUT_RESPI_MAINTENANCE_REPRO =  0.0044,
                        SENSITIVITY_SEX = 2,
                        SENSITIVITY_ABORTION = 2,
                        INI_SEX_RATIO = 0.8,
                        SEX_RATIO_REF = 0.6,
                        ABORTION_RATE_REF = 0.2,
                        ABORTION_RATE_MAX = 0.8,
                        SEX_RATIO_MIN = 0.2,
                        MASSE_INFLO_MALE_ADULTE = 1200,
                        PERIOD_MALE_INFLO = 1800,
                        PERIOD_DEV_SPIKELET = 675,
                        SENSIVITY_IC_SPIKELET =  0.30,
                        MASSE_MEAN_PEDUNCULE_ADULTE = 2100,
                        COUT_OIL = 3.2,
                        OIL_CONTENT = 0.73, 
                        IND_FRUIT_WEIGHT = 6.5,
                        MEAN_FRUIT_NUMBER_ADULTE = 1600,
                        PERIOD_FRUIT_SET = 405,
                        SENSIVITY_IC_SETTING = 0.30,
                        POURCENT_NSC_ST_MAX = 0.30,
                        POURCENT_NSC_ST_MIN = 0.05,
                        POURCENT_NSC_ST_INI = 0.07,
                        COUT_RESERVE = 0.75,
                        REALL_COST = 0.60,
                        MAXIMAL_SFIND = 12,   
                        INITIAL_SFIND = 2.5,    
                        INFLEXION = 6750,
                        COURBURE = 100,
                        COUT_RESPI_MAINTENANCE_LEAF = 0.0044,
                        COUT_RESPI_FEUILLE = 1.44,
                        SLW_max = 0.0166,
                        SLW_min = 0.008,
                        SLW_ini = 0.010 ,
                        POURC_FOLIOLE = 0.35,
                        TRESH_SLOW_PHYLO = 0.3){
  
  file=c(paste('LATITUDE =',LATITUDE),
         paste('ALTITUDE =', ALTITUDE),
         paste('DENS =', DENS),
         paste('T_BASE =', T_BASE),
         paste('T_OPT1 =', T_OPT1),
         paste('T_OPT2 =' ,T_OPT2),
         paste('T_LIM =' ,T_LIM),
         paste('T_EFF_INI =' ,T_EFF_INI),
         paste('Z1 =' ,Z1),
         paste('Z2 =' ,Z2),
         paste('H_CR =' ,H_CR),
         paste('H_PF =' ,H_PF),
         paste('H_0 =' ,H_0),
         paste('H_PF_Z1 =' ,H_PF_Z1),
         paste('VITESSE_RACINES =', VITESSE_RACINES),
         paste('FTSW_RACINES =', FTSW_RACINES),     
         paste('SEUIL_EVAP =', SEUIL_EVAP),
         paste('SEUIL_TRANSPI =' ,SEUIL_TRANSPI),
         paste('Z_INIT =', Z_INIT) ,
         paste('SEED =' ,SEED),
         paste('AGE_INI =', AGE_INI),
         paste('AGE_ADULT =' ,AGE_ADULT),
         paste('AGE_PLANTING =', AGE_PLANTING),
         paste('AGE_START_PROD =' ,AGE_START_PROD),
         paste('PRODUCTION_SPEED_INITIAL =' ,PRODUCTION_SPEED_INITIAL),
         paste('PRODUCTION_SPEED_ADULT =' ,PRODUCTION_SPEED_ADULT),
         paste('EFFICIENCE_BIOLOGIQUE =', EFFICIENCE_BIOLOGIQUE),
         paste('K =', K),
         paste('KC =' ,KC),
         paste('INACTIVE_PHYTOMER_NUMBER =', INACTIVE_PHYTOMER_NUMBER),
         paste('RANG_D_ABLATION =', RANG_D_ABLATION),
         paste('AF_FRUITS =' ,AF_FRUITS),
         paste('STEM_APPARENT_DENSITY =' ,STEM_APPARENT_DENSITY),
         paste('STEM_RAYON =' ,STEM_RAYON),
         paste('EN_LENGTH_INI =', EN_LENGTH_INI),
         paste('EN_LENGTH_ADULTE =', EN_LENGTH_ADULTE),
         paste('DEBUT_CROISSANCE_EN =' ,DEBUT_CROISSANCE_EN),
         paste('FIN_CROISSANCE_EN =' ,FIN_CROISSANCE_EN),
         paste('PLASTICITY_INTERNODE_IC =' ,PLASTICITY_INTERNODE_IC),
         paste('COUT_RESPI_MAINTENANCE_STIPE =' ,COUT_RESPI_MAINTENANCE_STIPE),
         paste('COUT_RESPI_INTERNODE =' ,COUT_RESPI_INTERNODE),
         paste('TT_FLOWERING_ADULT =', TT_FLOWERING_ADULT),
         paste('TT_FLOWERING_INITIAL =' ,TT_FLOWERING_INITIAL),
         paste('TT_HARVEST_ADULT =' ,TT_HARVEST_ADULT),
         paste('TT_HARVEST_INITIAL =' ,TT_HARVEST_INITIAL),
         paste('PERIOD_ABORTION =' ,PERIOD_ABORTION),
         paste('PERIOD_SEX_DETERMINATION =' ,PERIOD_SEX_DETERMINATION),
         paste('FRACTION_PERIOD_OLEOSYNTHESIS =', FRACTION_PERIOD_OLEOSYNTHESIS),
         paste('FRACTION_PERIOD_DEV_PEDUNCLE =', FRACTION_PERIOD_DEV_PEDUNCLE),
         paste('REPRO_CONSTRUCTION_COST =', REPRO_CONSTRUCTION_COST),
         paste('COUT_RESPI_MAINTENANCE_REPRO =' , COUT_RESPI_MAINTENANCE_REPRO),
         paste('SENSITIVITY_SEX =', SENSITIVITY_SEX),
         paste('SENSITIVITY_ABORTION =' ,SENSITIVITY_ABORTION),
         paste('INI_SEX_RATIO =' ,INI_SEX_RATIO),
         paste('SEX_RATIO_REF =', SEX_RATIO_REF),
         paste('ABORTION_RATE_REF =' ,ABORTION_RATE_REF),
         paste('ABORTION_RATE_MAX =', ABORTION_RATE_MAX),
         paste('SEX_RATIO_MIN =' ,SEX_RATIO_MIN),
         paste('MASSE_INFLO_MALE_ADULTE =' ,MASSE_INFLO_MALE_ADULTE),
         paste('PERIOD_MALE_INFLO =', PERIOD_MALE_INFLO),
         paste('PERIOD_DEV_SPIKELET =' ,PERIOD_DEV_SPIKELET),
         paste('SENSIVITY_IC_SPIKELET =',  SENSIVITY_IC_SPIKELET),
         paste('MASSE_MEAN_PEDUNCULE_ADULTE =', MASSE_MEAN_PEDUNCULE_ADULTE),
         paste('COUT_OIL =', COUT_OIL),
         paste('OIL_CONTENT =', OIL_CONTENT), 
         paste('IND_FRUIT_WEIGHT =', IND_FRUIT_WEIGHT),
         paste('MEAN_FRUIT_NUMBER_ADULTE =', MEAN_FRUIT_NUMBER_ADULTE),
         paste('PERIOD_FRUIT_SET =', PERIOD_FRUIT_SET),
         paste('SENSIVITY_IC_SETTING =', SENSIVITY_IC_SETTING),
         paste('POURCENT_NSC_ST_MAX =', POURCENT_NSC_ST_MAX),
         paste('POURCENT_NSC_ST_MIN =', POURCENT_NSC_ST_MIN),
         paste('POURCENT_NSC_ST_INI =', POURCENT_NSC_ST_INI),
         paste('COUT_RESERVE =', COUT_RESERVE),
         paste('REALL_COST =', REALL_COST),
         paste('MAXIMAL_SFIND =', MAXIMAL_SFIND),   
         paste('INITIAL_SFIND =', INITIAL_SFIND),    
         paste('INFLEXION =', INFLEXION),
         paste('COURBURE =', COURBURE),
         paste('COUT_RESPI_MAINTENANCE_LEAF =', COUT_RESPI_MAINTENANCE_LEAF),
         paste('COUT_RESPI_FEUILLE =', COUT_RESPI_FEUILLE),
         paste('SLW_max =', SLW_max),
         paste('SLW_min =', SLW_min),
         paste('SLW_ini =', SLW_ini) ,
         paste('POURC_FOLIOLE =', POURC_FOLIOLE),
         paste('TRESH_SLOW_PHYLO =', TRESH_SLOW_PHYLO))
         
  print(paste(length(file),'parameters'))
  
  write.table(x = file,file = paste0(dir,filename,'.txt'),row.names = F,quote=F,col.names = F,sep='\t')
  
}

# generate_param(dir='../parameters/',filename = 'XPalm_parameters')
