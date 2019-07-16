#!/usr/bin/env python


class GlobalVariables(object):
    
    DRIVE = 'D:'
    DIRECTORYNAME = '/Benoit/2011/Simulateur_python/Pasdenom-multiarbres-avec_SLA_et_retard/'
    FILENAME = 'LIBO_meteo.txt'
    OUTPUTDIRECTORYNAME = '/Benoit/2011/Simulateur_python/Pasdenom-multiarbres-avec_SLA_et_retard/Sorties/'
    NUMBER_OF_TREES = 1
    NUMBER_OF_SIMULATION_STEP = 6209 # 6209
    
    ### dates de sorties des simulations
    SORTIE1 =  600
    SORTIE2 =  2000    
    SORTIE3 =  4000
    SORTIE4 = 6000
    SORTIE5 = ""
    
    # ablation_feuille
    DEBUT_DEFOLIATON = 2000
    FIN_DEFOLIATON =   4500 
    RANG_DEFOLIATON = 15
    POURC_DEFOLIATON = 0  # en unite de 0,0.1,0.2,0.3 1
    
    # ablation_regime 
    DEBUT_ABLATION_REGIME = 2000
    FIN_ABLATION_REGIME =   4500
    POURC_ABLATION_REGIME = 0
    RANG_D_ABLATION_REGIME = 12
    
    #  irrigation
    IRRIGATION_DAY =       [1] #[25, 45, 55, 65, 85, 95, 115, 135, 145, 157, 175, 195, 205, 225, 235, 255, 265, 285, 295, 310, 325, 335, 355] #[1]
    IRRIGATION_QUANTITY =  [0] # [35, 35, 35, 35, 35, 35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35] # [0]
    
    ## site
    LATITUDE = 0.97
    ALTITUDE = 50 # en metres
    INITIAL_PRODUCTION_SPEED = 0.0141
    DECREASE_OF_PRODUCTION_SPEED = 0.00000329 
    MINIMAL_PRODUCTION_SPEED = 0.0063
    TT_FLOWERING_ADULTE = 2640          # 2950
    TT_HARVEST_ADULTE = 2640 + 1455       # 2700 1600
    INFLEXION = 200
    COURBURE = 50
    RANG_D_ABLATION = 50
    INITIAL_SFIND = 2.5
    MAXIMAL_SFIND = 12
    INCREASE_OF_LEAF_AREA =  0.00419  # (cm2.?Cd-1)
    b_EN = 12
    c_EN = 25
    k_EN = 0.82
    AGE = 3
    INITIAL_HEIGHT = 0.2
    DENS = 143
    EFFICIENCE_BIOLOGIQUE = 4.2
    K = 0.5
    INACTIVE_PHYTOMER_NUMBER = 60
    
    #INITIAL_PHYTOMER_NUMBER = RANG_D_ABLATION
    POURC_FOLIOLE = 0.35
    COUT_RESPI_FEUILLE = 1.44
    SLW = 0.0125
    SLW_max = 0.0166
    SLW_min = 0.008
    SLW_ini = 0.012 
    COUT_RESPI_INTERNODE = 1.44
    STEM_APPARENT_DENSITY = 0.15
    STEM_RAYON = 0.30
    INITIAL_BIOMASS = 500
    INI_SEX_RATIO = 0.8
    INI_TAUX_D_AVORTEMENT = 0.2
    COUT_RESPI_MAINTENANCE_STIPE = 0.001
    COUT_RESPI_MAINTENANCE_LEAF = 0.0044
    
    ## Variables relatives a la croissance des regimes ##
    RANG_DEBUT_CROISSANCE_PEDUNCULE = 8  # nombre de rangs avant floraison
    MASSE_REGIMES_ADULTE = 11.5          # 11.5
    MASSE_REGIME_INI = 2.5
    POURC_FRUIT_REGIME = 0.85
    INCREASE_OF_BUNCH_MASS = 0.00259
    COUT_STRUCTURE_REGIME = 1.44
    OIL_CONTENT = 0.78
    RANG_DEBUT_OLEO = 24  # n est plus fonctionnel
    COUT_OIL = 3.2
    SEUIL_MEDIAN_FTSW = 0.75
    SENS_FTSW = 0
    PLASTICITY_BUNCH_IC_AVANT_FLORAISON  = 0.6
    PLASTICITY_BUNCH_IC_APRES_FLORAISON  = 0.2
    DUREE_OLEO = (28-24)/ 0.0063  # recolte au rang 28 a peu pres et debut au rang 24 
    
    ## Variables relatives a la sensibilite a IC
    ICsex_RANG_DEBUT = -28
    ICsex_RANG_FIN = -20
    Seuil_IC_abort = 1
    Seuil_IC_sex = 1
    SENSITIVITY_SEX = 2 
    ICabort_RANG_DEBUT = 8
    ICabort_RANG_FIN = 12
    IC_spikelet_RANG_DEBUT = -10
    IC_spikelet_RANG_FIN = -5
    RANG_SENSITIVITY_NOUAISON = 3 # nombre de rang avant floraison pour la determination du nombre de grains
    SENSIVITY_IC_SPIKELET =  0.05 #  0.05 0.5 ?
    SENSITIVITY_ABORTION = 2
    SENSIVITY_IC_SETTING = 0.05   #0.05 0.5 ?
    RAPPORT_POIDS_MALE_FEMELLE = 0.1
    COUT_RESPI_MAINTENANCE_BUNCH  =  0.0044
    
    
    # leaf
    PLASTICITY_LEAF_IC = 0.5 # plasticite leaf au trophique entre 0 et 1 0 : tout plastique, 1 : pas plastique
    SEUIL_EXPAN = 0.2
    SEUIL_DUREE = 0.6 # ralentissement de la duree d expansion en stress (affecte aussi potential leaf development)
    SEUIL_ORGANO = 0.1
    VITESSE_SENSITIVITY = 0.30
    
    # Reserves
    POURCENT_NSC_ST_MAX = 0.70 # pourcentage de reserve max dans le stipe
    POURCENT_NSC_ST_MIN = 0.05 # pourcentage de reserve min dans le stipe
    POURCENT_NSC_ST_INI = 0.70
    POURCENT_NSC_FE_MAX =   0.10 # pourcentage de reserve max feuille
    POURCENT_NSC_FE_MIN =   0.02 # pourcentage de reserve min feuille
    POURCENT_NSC_FE_INI = 0.10
    COUT_RESERVE = 0.75 # cout de la mise en reserve % du CH20 qui peut etre mis en reserve
    MOB_RATE_MAX = 0.025 # % max de la biomasse dans les reserves qui peut etre destockee 
    REALL_COST = 0.60
    AF_FRUITS = 1.5 # coefficient d affinite pour les fruits
    
    
    # Racines
    Z_INIT = 500 #en mm
    Z1 = 200 # profondeur du compartiment de surface
    Z2 = 2000 # profondeur du compartiment de profodeur Z1+Z2 = profondeur du sol
    H_CR = 0.23 # humidite a la capacite de retention
    H_PF = 0.10 # humidite au point de fletrissement
    H_0 = 0.15 # humidite du sol a l initialisation
    VITESSE_RACINES = 0.3  # mm par degre jour
    FTSW_RACINES = 0.2 
    
    # BH
    KC = 1
    SEUIL_EVAP = 0.5
    SEUIL_TRANSPI = 0.5
    SEUIL_PHOTO = 0.6
    