#!/usr/bin/env python


class GlobalVariables(object):
    
    DRIVE = ''
    DIRECTORYNAME = '/Pasdenom-multiarbres-avec_nb_fruits_masse_fruits/'
    FILENAME = 'meteo.txt'
    OUTPUTDIRECTORYNAME = '/Outputs/'
    NUMBER_OF_TREES = 2
    NUMBER_OF_SIMULATION_STEP = 12 # 6847
    
    TYPE = 'JEUNE'
    
    
    #### mode interception localisee
    
    LOCAL_LIGHT_INTERCEPTION = "NO"  ## YES pour mettre le mode actif
    a_loi_beta_interception = 1.4   ### plus a est petit plus le haut intercepte beaucoup
    b_loi_beta_interception = 7
    rang_chute_pn_age = 20   ### rang du debut de chute de l activite photosynthetique
    rang_pn_50 = 35 ## rang ou l activite photosynthetique atteint 50% du max
    
    REMANENCE_STRESS = "NO"  ## YES POUR LE METTRE ACTIF
    a_sigmoide_declin_pn = 13 ### sigmoide pour le declin de pn en fonction du stress
    b_sigmoide_declin_pn = 6  ### depend du nombre de jours au dessus d un certain seuil de FTSW
    seuil_FTSW_pour_red_a_long_termes_de_Pn = 0.4
    SENS_REMANESCENCE = 2.5 ### exposant pour rendre compte de la sensibilite au stress
    RANG_DEBUT_SENS_PN_LONG_TERMES = -3
    RANG_FIN_SENS_PN_LONG_TERMES = 3
    
    ### deux processus sont pris en compte pour la remanescence le premier concerne la duree et l autre l intensite
    
    
    ### dates de sorties des simulations
    SORTIE1 =  1000
    SORTIE2 =  2000  
    SORTIE3 =  3000
    SORTIE4 = 6600
    SORTIE5 = ""
    
    # ablation_feuille
    DEBUT_DEFOLIATON = 2500
    FIN_DEFOLIATON =   3500 
    RANG_DEFOLIATON = 15
    POURC_DEFOLIATON = 0  # en unite de 0,0.1,0.2,0.3 1
    
    # ablation_regime 
    DEBUT_ABLATION_REGIME = 2500
    FIN_ABLATION_REGIME =   3500
    POURC_ABLATION_REGIME = 0
    RANG_D_ABLATION_REGIME = 25
    
    #  irrigation
    IRRIGATION_DAY =       [1] #[25, 45, 55, 65, 85, 95, 115, 135, 145, 157, 175, 195, 205, 225, 235, 255, 265, 285, 295, 310, 325, 335, 355] #[1]
    IRRIGATION_QUANTITY =  [0] # [35, 35, 35, 35, 35, 35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,  35] # [0]
    
    ## site
    LATITUDE = 0.97
    ALTITUDE = 50 # en metres
    
    ## sol
    Z1 = 200 # profondeur du compartiment de surface ou il y a peut y avoir de l'evap ou alors egal a la profondeur d enracinement de la plante de couverture
    Z2 = 2000 # profondeur du compartiment de profodeur Z1+Z2 = profondeur du sol
    H_CR = 0.23 # humidite a la capacite de retention
    H_PF = 0.10 # humidite au point de fletrissement
    H_0 = 0.15 # humidite du sol a l initialisation
    H_PF_Z1 = 0.05 # 0.5*HPF_EVAP si pas de plante de surface 0.10 si plante de surface
    
    ##caracteristique plantation en debut de simulation
    DENS = 143
    
    if TYPE == 'ADULTE' :    
        AGE = 8   ### mettre 8 pour l adulte !!! 
    else :
        AGE = 3
        
    INI_SEX_RATIO = 0.8
    INI_TAUX_D_AVORTEMENT = 0.2
    
    EFFICIENCE_BIOLOGIQUE = 4.8
    SEUIL_PHOTO = 0.6
    K = 0.5
    KC = 1
    INACTIVE_PHYTOMER_NUMBER = 60
    
    ### temperatures cardinales
    T_BASE = 15
    T_OPT1 = 25
    T_OPT2 =  30
    T_LIM =  40
    
    ### parametre genotypique_regimes
       ### 3.5
    
    AGE_ADULTE = 8  
    FACTEUR_AGE_INI = float(AGE) / float(AGE_ADULTE)  #### 1 il est adulte
    
    INCREASE_TAILLE_REGIMES =  (1-0) * 1 / (float(AGE_ADULTE) * 365)   
    
    MASSE_MEAN_PEDUNCULE_ADULTE = 2.1 
    MEAN_FRUIT_NUMBER_ADULTE = 1600
    IND_FRUIT_WEIGHT = float(6.5) 

   # if TYPE == 'ADULTE' :
   #     MASSE_REGIME_INI = MASSE_REGIMES_ADULTE
   # else :    
   #     MASSE_REGIME_INI = 3.5    
    
    MASSE_INFLO_MALE_ADULTE = float(1.2)
   # INCREASE_OF_BUNCH_MASS = 0.00259
   # POURC_FRUIT_REGIME = 0.85
   # MASSE_REGIMES_ADULTE = (MEAN_FRUIT_NUMBER_ADULTE * IND_FRUIT_WEIGHT * 1/POURC_FRUIT_REGIME ) / 1000
    OIL_CONTENT = 0.73 
    
    COUT_STRUCTURE_REGIME = 1.44
    COUT_OIL = 3.2
    
    TT_FLOWERING_ADULTE = 2640          # 2950
    TT_HARVEST_ADULTE = 2640 + 1455       # 2700 1600
    DUREE_OLEO = (28-24)/ 0.0063  # recolte au rang 28 a peu pres et debut au rang 24 
    RANG_DEBUT_CROISSANCE_PEDUNCULE = 8  # nombre de rangs avant floraison # 11.5
    COUT_RESPI_MAINTENANCE_BUNCH  =  0.0044
    
    RATIO_DUREE_JEUNES_FLOWERING = 1
    RATIO_DUREE_JEUNES_HARVEST = 1
    RATIO_DUREE_JEUNES_OLEO = 1
    
    ### parametre_genotypique_feuilles
      #### 0.0141
    MINIMAL_PRODUCTION_SPEED = 0.0063  # l appeler plutot adulte
    if TYPE == 'ADULTE' :
        INITIAL_PRODUCTION_SPEED = MINIMAL_PRODUCTION_SPEED
    else :    
        INITIAL_PRODUCTION_SPEED = 0.0141      
    
    DECREASE_OF_PRODUCTION_SPEED = 0.00000329 
    RANG_D_ABLATION = 50   #l appeler rang de mort
    
    MAXIMAL_SFIND = 12    
    if TYPE == 'ADULTE' :
        INITIAL_SFIND = MAXIMAL_SFIND
    else :    
        INITIAL_SFIND = 2.5    
    
       ### 2.5
       # l appeler adulte
    INCREASE_OF_LEAF_AREA =  0.00419  # (cm2.?Cd-1)
    
    INFLEXION = 200     # deux parametres rendant compte de la vitesse d expansion des feuilles
    COURBURE = 50
    COUT_RESPI_MAINTENANCE_LEAF = 0.0044
    COUT_RESPI_FEUILLE = 1.44
    SLW = 0.0125
    SLW_max = 0.0166
    SLW_min = 0.008
    SLW_ini = 0.012 
    POURC_FOLIOLE = 0.35
    
    PLASTICITY_LEAF_IC = 0.7 # plasticite leaf au trophique entre 0 et 1 0 : tout plastique, 1 : pas plastique
    SEUIL_EXPAN = 0.2
    SEUIL_DUREE = 0.6 # ralentissement de la duree d expansion en stress (affecte aussi potential leaf development)
    SEUIL_ORGANO = 0.1
    VITESSE_SENSITIVITY = 0.15 #plasticite de la vitesse de prod a l'IC
    
    
    ### stipe
    if TYPE == 'ADULTE' :
        INITIAL_HEIGHT = 4
    else :    
        INITIAL_HEIGHT = 0.2     
    
    
    STEM_APPARENT_DENSITY = 0.15
    STEM_RAYON = 0.30
    EN_LENGHT_INI = 0.1  # (cm)
    LENGHT_ADULTE =  3 #(cm)
    DEBUT_CROISSANCE_EN = 4 # (annee)
    FIN_CROISSANCE_EN = 8 # (annee)
    PLASTICITY_INTERNODE_IC = 0.5 # plasticite internode au trophique entre 0 et 1 0 : tout plastique, 1 : pas plastique
    
    #b_EN = 12
    #c_EN = 25
    #k_EN = 0.82   # ces 3 parametres permettent de caculer la hauteur des EN en fct de l age
    COUT_RESPI_MAINTENANCE_STIPE = 0.001
    COUT_RESPI_INTERNODE = 1.44
    
    
    # Racines
    VITESSE_RACINES = 0.3  # mm par degre jour
    FTSW_RACINES = 0.2     
    SEUIL_EVAP = 0.5
    SEUIL_TRANSPI = 0.5
    if TYPE == 'ADULTE' :
        Z_INIT = 2000    
    else : 
        Z_INIT = 500 #en mm
    
    # Reserves
    POURCENT_NSC_ST_MAX = 0.30 # pourcentage de reserve max dans le stipe a verifier c est beaucoup !!! 0.7
    POURCENT_NSC_ST_MIN = 0.05 # pourcentage de reserve min dans le stipe
    POURCENT_NSC_ST_INI = 0.30  ### 0.7
    POURCENT_NSC_FE_MAX =   0.10 # pourcentage de reserve max feuille
    POURCENT_NSC_FE_MIN =   0.02 # pourcentage de reserve min feuille
    POURCENT_NSC_FE_INI = 0.10
    COUT_RESERVE = 0.75 # cout de la mise en reserve % du CH20 qui peut etre mis en reserve
    MOB_RATE_MAX = 0.025 # % max de la biomasse dans les reserves qui peut etre destockee 
    REALL_COST = 0.60
    
    ## Variables relatives a la sensibilite a IC
    ICsex_RANG_DEBUT = -28
    ICsex_RANG_FIN = -20
    SENSITIVITY_SEX = 2
    Seuil_IC_sex = 1
    
    ICabort_RANG_DEBUT = 8          #8
    ICabort_RANG_FIN = 12           # 12
    SENSITIVITY_ABORTION = 2
    Seuil_IC_abort = 1
    
    IC_spikelet_RANG_DEBUT = -10
    IC_spikelet_RANG_FIN = -5
    SENSIVITY_IC_SPIKELET =  0.30 #  0.05 0.5 ?
    
    
    DEBUT_RANG_SENSITIVITY_NOUAISON = 3 # nombre de rang avant floraison pour le debut de determination du nombre de grains   
    FIN_RANG_SENSITIVITY_NOUAISON = 1  # nombre de rang avant floraison pour le debut de determination du nombre de grains
    SENSIVITY_IC_SETTING = 0.30   #0.05 0.5 ?
    
    PLASTICITY_BUNCH_IC_AVANT_FLORAISON  = 0.6  # plasticite bunch au trophique entre 0 et 1 0 : tout plastique, 1 : pas plastique
    PLASTICITY_BUNCH_IC_APRES_FLORAISON  = 0.2
    
    SEUIL_MEDIAN_FTSW = 0.75
    SENS_FTSW = 0
    
    AF_FRUITS = 1.5 # coefficient d affinite pour les fruits
    
    
    
    
    #INITIAL_PHYTOMER_NUMBER = RANG_D_ABLATION
    
    
    
    
    
    INITIAL_BIOMASS = 500   # non utilise
    
    
    
    
   
   
    
    
    
    RANG_DEBUT_OLEO = 24  # n est plus fonctionnel
    
    
    
    
    
   
    
    
    
    # leaf
    
    
    
    
    
    
    
    
   
   
    