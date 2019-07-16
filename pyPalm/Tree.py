#!/usr/bin/env python

from Phytomer import Phytomer
from GlobalVariables import GlobalVariables
from numpy import *
from math import *
from Reserve import Reserve
from BH import BH
from Racines import Racines


class Tree(object):
    
    def __init__(self, simulationInstance, treeName):
        self.name = treeName
        self.phytomerNumber = GlobalVariables.INACTIVE_PHYTOMER_NUMBER + GlobalVariables.RANG_D_ABLATION
        
        self.inactivephytomerNumber = GlobalVariables.INACTIVE_PHYTOMER_NUMBER
        self.activephytomerNumber = GlobalVariables.RANG_D_ABLATION
        self.phytomers = {}
        self.date_plus_jeune_feuille = 0         
        self.NF = 0 
        self.newPhytomerEmergence = 0
        self.simulation = simulationInstance
        self.productionSpeed = GlobalVariables.INITIAL_PRODUCTION_SPEED
        self.tabmemorise = zeros((0,12))
        self.trunk_height = GlobalVariables.INITIAL_HEIGHT
        self.bunch_biomass = 0
        self.respirable_bunch_biomass = 0
        self.female_bunch_biomass = 0
        self.male_biomass = 0 
        self.compute_structure_init()
        self.totalLeafArea = 0
        self.ei = 0 
        self.compute_SF()
        self.compute_LAI(self.totalLeafArea)
        self.compute_ei(self.LAI,GlobalVariables.K)
        self.Assim= 0
        self.production_decrease = 0 
        self.IC = 1
        self.fr_fruits = 0 # part de la demande des fruits satisfaite
        self.fr_reste = 0 # part de la demande du reste statisfaite
        self.internode_demand = 0
        self.leaf_demand = 0
        self.bunch_demand = 0
        self.fruit_demand = 0
        self.male_demand = 0
        self.peduncule_demand = 0 
        self.offre_nette = 0  
        self.offre_pour_respi = 0  # offre pour la respi prioritaire
        self.delta_av_respi = 0 # c est la difference entre l assim et la demande pour la respi
        self.offre_pour_croissance = 0 # c est l offre pour la croissance une fois que la respi est enlevee du bilan
        self.growth_demand = 0 # c est la demande pour la croissance des organes
        self.delta_avant_croissance = 0 # c est la difference entre l offre apres la respi et la demande pour la croissance
        self.fr_avant_croissance = 0 # c est la meme chose que precedemment mais en fraction
        self.fraction_pour_croissance = 0 # c est la fraction qui reste pour la croissance une fois que le stockage de biomasse dans les reserves a ete effectuee
        self.offre_fruits = 0 # offre pour les fruits
        self.offre_reste = 0 # offre pour le reste
        self.trunk_biomass = 1000 * GlobalVariables.INITIAL_HEIGHT * GlobalVariables.STEM_APPARENT_DENSITY * pi * ( GlobalVariables.STEM_RAYON )**2
        self.leaf_structural_biomass = self.totalLeafArea  * GlobalVariables.SLW_min * 10 / GlobalVariables.POURC_FOLIOLE  
        self.leaf_non_structural_biomass= self.totalLeafArea * (GlobalVariables.SLW_ini - GlobalVariables.SLW_min) * 10 / GlobalVariables.POURC_FOLIOLE
        self.total_leaf_biomass = self.leaf_structural_biomass + self.leaf_non_structural_biomass
        
        self.reserve = Reserve(self)
        self.biomass = self.leaf_structural_biomass + self.leaf_non_structural_biomass + self.reserve.biomass + self.trunk_biomass + self.female_bunch_biomass + self.male_biomass       
        self.racines = Racines(self)
        self.BH = BH(self)
        self.record = zeros((1, 13))
        self.recordIndex = 1
        self.record_flowering = zeros((1, 6))
        self.record_floweringIndex = 1
        self.tabmemorise_meteo = zeros((1,10))
        self.SLW = (((self.leaf_non_structural_biomass + self.leaf_structural_biomass) * GlobalVariables.POURC_FOLIOLE) / self.totalLeafArea) / 10
        self.biomasse_prod = 0
        
        
        
        
        
    def compute_structure_init(self) :
        
        
        phytomer = Phytomer(GlobalVariables.RANG_D_ABLATION, 1, "ACTIVE", self,  0, 0, 0, 0, 0, 0, 0, 0)  
        
        phytomer.step_apparition = self.simulation.step - (GlobalVariables.INACTIVE_PHYTOMER_NUMBER + GlobalVariables.RANG_D_ABLATION - phytomer.name) * (1/ (10 * GlobalVariables.INITIAL_PRODUCTION_SPEED))
        self.date_plus_jeune_feuille =  phytomer.step_apparition   
        
        
        for key in range(GlobalVariables.INACTIVE_PHYTOMER_NUMBER + 1) :
            phytomer = Phytomer(key+GlobalVariables.RANG_D_ABLATION, - key + 1, "INACTIVE", self,  0, 0, 0, 0, 0, 0, 0, 0)
            self.phytomers[phytomer.name] = phytomer
            self.phytomers[phytomer.name].step_apparition = self.simulation.step - (GlobalVariables.INACTIVE_PHYTOMER_NUMBER + GlobalVariables.RANG_D_ABLATION - self.phytomers[phytomer.name].name) * (1/ (10 * GlobalVariables.INITIAL_PRODUCTION_SPEED)) 
            phytomer.compute_facteur_age()  
            if self.phytomers[phytomer.name].rank > GlobalVariables.ICsex_RANG_FIN :
                self.phytomers[phytomer.name].bunch.sexe = phytomer.bunch.sexe_decision(GlobalVariables.INI_SEX_RATIO)
            
        for key in range(1 , GlobalVariables.RANG_D_ABLATION + 1) :
            nb_jour_depuis_l_appari = ((GlobalVariables.RANG_D_ABLATION) - key)/( 10 * GlobalVariables.INITIAL_PRODUCTION_SPEED)                         # rang d'ablation = nbre ini
            TTfeuille = nb_jour_depuis_l_appari * 10  # 10 temps thermique moyen a calculer
            
            
            phytomer = Phytomer(key , GlobalVariables.RANG_D_ABLATION - key + 1  , "ACTIVE", self,  0, 0, 0, TTfeuille, 0, 0, 0, 0)
            
            
            self.phytomers[phytomer.name] = phytomer
            self.phytomers[phytomer.name].step_apparition = self.simulation.step - (GlobalVariables.INACTIVE_PHYTOMER_NUMBER + GlobalVariables.RANG_D_ABLATION - self.phytomers[phytomer.name].name) * (1/ (10 * GlobalVariables.INITIAL_PRODUCTION_SPEED))                 
            phytomer.compute_facteur_age()   
            
            self.phytomers[phytomer.name].bunch.thermalTimeSinceAppearance = TTfeuille
            self.phytomers[phytomer.name].bunch.TT_corrige = TTfeuille
            SF = min(GlobalVariables.INCREASE_OF_LEAF_AREA * ( - nb_jour_depuis_l_appari ) + GlobalVariables.INITIAL_SFIND, GlobalVariables.MAXIMAL_SFIND)
            self.phytomers[phytomer.name].bunch.sexe = phytomer.bunch.sexe_decision(GlobalVariables.INI_SEX_RATIO)
            if self.phytomers[phytomer.name].rank > GlobalVariables.ICabort_RANG_FIN :
                self.phytomers[phytomer.name].bunch.avort = phytomer.bunch.abortion_decision(GlobalVariables.INI_TAUX_D_AVORTEMENT)
            phytomer.bunch.compute_structure_ini()
            phytomer.bunch.compute_flowering_date(GlobalVariables.INITIAL_PRODUCTION_SPEED)
            phytomer.bunch.compute_harvest_date(GlobalVariables.INITIAL_PRODUCTION_SPEED)
            
            if self.phytomers[phytomer.name].rank< 0 :
                self.phytomers[phytomer.name].bunch.statut = "INITIE"
            else :
            
                if (self.phytomers[phytomer.name].bunch.thermalTimeSinceAppearance > self.phytomers[phytomer.name].bunch.estimated_flowering_date)  : # on calcule une moyenne entre la dtae ? l'apparition et la date au jour j
                    if (self.phytomers[phytomer.name].bunch.thermalTimeSinceAppearance > self.phytomers[phytomer.name].bunch.estimated_harvest_date)  :
                        self.phytomers[phytomer.name].bunch.statut = "RECOLTE"
                    else :
                        self.phytomers[phytomer.name].bunch.statut = "FLORAISON_RECOLTE"
                else :     
                    self.phytomers[phytomer.name].bunch.statut = "APPARITION_FLORAISON"   
            
            self.phytomers[phytomer.name].leaf.thermalTimeSinceAppearance = TTfeuille
            self.phytomers[phytomer.name].leaf.TT_corrige = TTfeuille
            self.phytomers[phytomer.name].internode.TT_corrige = TTfeuille
            SF = min(GlobalVariables.INCREASE_OF_LEAF_AREA * ( - nb_jour_depuis_l_appari ) + GlobalVariables.INITIAL_SFIND, GlobalVariables.MAXIMAL_SFIND)
            
            
            self.phytomers[phytomer.name].leaf.leafArea = phytomer.leaf.computeSF_ind_finale(SF , TTfeuille)
             
            self.phytomers[phytomer.name].leaf.structural_biomass = self.phytomers[phytomer.name].leaf.leafArea * GlobalVariables.SLW_min * 10 / GlobalVariables.POURC_FOLIOLE
            self.phytomers[phytomer.name].leaf.non_structural_biomass = self.phytomers[phytomer.name].leaf.leafArea * (GlobalVariables.SLW_ini - GlobalVariables.SLW_min) * 10 / GlobalVariables.POURC_FOLIOLE
            
            if self.phytomers[phytomer.name].bunch.statut == "RECOLTE" :
                if self.phytomers[phytomer.name].bunch.avort == "NON_AVORTE" :
                    if self.phytomers[phytomer.name].bunch.sexe == "FEMELLE" :
                        self.phytomers[phytomer.name].leaf.leafArea = 0
                        self.phytomers[phytomer.name].leaf.structural_biomass = 0
                        self.phytomers[phytomer.name].leaf.non_structural_biomass = 0
            self.phytomers[phytomer.name].leaf.potLeafArea = self.phytomers[phytomer.name].leaf.leafArea
            
            if self.phytomers[phytomer.name].leaf.leafArea != 0 :
                self.phytomers[phytomer.name].leaf.SLW = (self.phytomers[phytomer.name].leaf.structural_biomass + self.phytomers[phytomer.name].leaf.non_structural_biomass) * GlobalVariables.POURC_FOLIOLE / self.phytomers[phytomer.name].leaf.leafArea / 10 
            
            #print "STRUCTURE_INI", "sexe",self.phytomers[phytomer.name].bunch.sexe,  "avort", self.phytomers[phytomer.name].bunch.avort,  "statut", self.phytomers[phytomer.name].bunch.statut, "leafarea", self.phytomers[phytomer.name].leaf.leafArea, "structural_biomass", self.phytomers[phytomer.name].leaf.structural_biomass, "non_structural_biomass", self.phytomers[phytomer.name].leaf.non_structural_biomass, "SLW",self.phytomers[phytomer.name].leaf.SLW
            
            self.phytomers[phytomer.name].internode.duree_allongement = 1 / self.productionSpeed
            #self.phytomers[phytomer.name].internode.length = GlobalVariables.INITIAL_HEIGHT / GlobalVariables.RANG_D_ABLATION 
            date_d_appar =  (GlobalVariables.RANG_D_ABLATION - phytomer.name) * 1 /(10 * self.productionSpeed )
            if self.phytomers[phytomer.name].internode.TT_corrige > self.phytomers[phytomer.name].internode.duree_allongement : 
                self.phytomers[phytomer.name].internode.length = phytomer.internode.compute_final_length(- date_d_appar )  # 10Cj en moyenne
            else :
                self.phytomers[phytomer.name].internode.length = phytomer.internode.compute_final_length(- date_d_appar ) * ( self.phytomers[phytomer.name].internode.TT_corrige) / self.phytomers[phytomer.name].internode.duree_allongement
                
            self.phytomers[phytomer.name].internode.biomass = 1000 * self.phytomers[phytomer.name].internode.length * GlobalVariables.STEM_APPARENT_DENSITY * pi * ( GlobalVariables.STEM_RAYON )**2 
            
          
        
    
        
     
    
    def getPhytomerNumber(self):
        return self.phytomerNumber
    
    def getPhytomers(self):
        return self.phytomers
    
    def getproductionSpeed(self) :
        return self.productionSpeed
    
    def getLAI(self) :
        return self.LAI
    
    def get_ei(self) :
        return self.ei
    
    def get_totalLeafArea(self) :
        return self.totalLeafArea
    
    def display(self):
        for key in sorted(self.phytomers):
            print ('phytomer=%s, appearance day=%i, appearance month=%i, appearance year=%i' %(key, self.phytomers[key].getAppearanceDay(), self.phytomers[key].getAppearanceMonth(), self.phytomers[key].getAppearanceYear()) )
        
    def newPhytomer(self, TT, day, month, year):
        phytomer = Phytomer(self.phytomerNumber + 1,  - GlobalVariables.INACTIVE_PHYTOMER_NUMBER, "INACTIVE", self, 0, 0, 0, 0,  day, month, year, TT)
        self.phytomers[phytomer.name] = phytomer
        self.phytomerNumber += 1
        return phytomer.name
        
    def computeProductionSpeed(self, nbOfDaySinceBeginning):
        temp = (-GlobalVariables.DECREASE_OF_PRODUCTION_SPEED * nbOfDaySinceBeginning) + GlobalVariables.INITIAL_PRODUCTION_SPEED
        self.productionSpeed = max(GlobalVariables.MINIMAL_PRODUCTION_SPEED, temp)
    
    def compute_biomasse_prod(self, SF , eb, Rg, FTSW) :
        self.compute_LAI(SF)
        self.compute_ei(self.LAI , GlobalVariables.K)
        total_beta_interception_leaf = 0
        for key in sorted(self.phytomers):    
            if self.phytomers[key].state == "ACTIVE" : 
                self.phytomers[key].leaf.compute_relative_position(self.phytomers[key].rank)    
                
        
        if GlobalVariables.LOCAL_LIGHT_INTERCEPTION == "YES" :
            for key in sorted(self.phytomers):    
                self.phytomers[key].leaf.compute_niveau_d_eclairement(self.phytomers[key].leaf.crown_position)
                total_beta_interception_leaf +=  self.phytomers[key].leaf.niveau_d_eclairement_loi_beta
            
            #decrease_FTSW_jour = self.compute_decrease_FTSW(FTSW, GlobalVariables.SEUIL_PHOTO)
            self.biomasse_prod  = 0
            
            for key in sorted(self.phytomers):     
                leaf = self.phytomers[key].leaf 
                leaf.light_pourc_interception = leaf.niveau_d_eclairement_loi_beta / total_beta_interception_leaf
                if GlobalVariables.REMANENCE_STRESS == "YES" : 
                    leaf.compute_declin_photo()
                else :
                    leaf.assim_max =  eb
                leaf.compute_biomasse_prod(leaf.light_pourc_interception, leaf.assim_max, self.ei, GlobalVariables.DENS, Rg)   ### on cacule la biomasse produite par la feuille en absence de contrainte
                self.biomasse_prod += self.compute_decrease_FTSW(FTSW, GlobalVariables.SEUIL_PHOTO) * leaf.biomasse_prod  
                
           # self.biomasse_prod = self.compute_decrease_FTSW(FTSW, GlobalVariables.SEUIL_PHOTO) * 10 * self.ei * eb * 0.48 * Rg / GlobalVariables.DENS  # on multiplie par 10 pour passer en kg
        
        else :
            self.biomasse_prod = self.compute_decrease_FTSW(FTSW, GlobalVariables.SEUIL_PHOTO) * 10 * self.ei * eb * 0.48 * Rg / GlobalVariables.DENS  # on multiplie par 10 pour passer en kg
   
    def compute_decrease_FTSW(self, FTSW, seuil) :
        
        if FTSW > seuil:
            y = 1
        else :   
            y = FTSW / seuil
        
        return(y)
    
    def compute_LAI(self,SF) :
        self.LAI = SF * GlobalVariables.DENS / 10000
        
    def compute_ei(self,LAI,K) :    
        self.ei= (1 - exp(- K * LAI))
        
    def compute_SF(self):
        self.totalLeafArea = 0
        for key in sorted(self.phytomers):
            if  (self.phytomers[key].state != 'DEAD') :  ### rajout 21/02/2012
                self.totalLeafArea += self.phytomers[key].leaf.leafArea
    
    def compute_height(self) :
        self.trunk_height = GlobalVariables.INITIAL_HEIGHT
        for key in sorted(self.phytomers):
            self.trunk_height += self.phytomers[key].internode.length 
       
    def nouvelle_structure(self, TT , day, month, year) :
        
        phytomer_a_detruire = self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)]
        phytomer_a_detruire.state = "DEAD"
        self.totalLeafArea += -  phytomer_a_detruire.leaf.leafArea
        self.leaf_structural_biomass +=   - phytomer_a_detruire.leaf.structural_biomass
        self.leaf_non_structural_biomass += - phytomer_a_detruire.leaf.non_structural_biomass
        
        ### rajout 21/02/2012
        
        self.totalLeafArea += - self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].leaf.leafArea
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].leaf.leafArea = 0 
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].leaf.structural_biomass = 0
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].leaf.non_structural_biomass = 0
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].leaf.total_biomass  = 0
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].leaf.SLW = 0
        
         
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER )].deathday = day
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].deathmonth = month
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].deathyear = year
        self.phytomers[(self.phytomerNumber - GlobalVariables.RANG_D_ABLATION - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].deathTT = TT
        self.phytomers[(self.phytomerNumber - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].state = "ACTIVE"
        self.phytomers[(self.phytomerNumber - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].appearanceDay = day
        self.phytomers[(self.phytomerNumber - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].appearanceMonth = month
        self.phytomers[(self.phytomerNumber - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].appearanceYear = year
        self.phytomers[(self.phytomerNumber - GlobalVariables.INACTIVE_PHYTOMER_NUMBER)].appearanceTT = TT
        for key in sorted(self.phytomers):
            self.phytomers[key].rank +=1
            if self.phytomers[key].rank == GlobalVariables.ICabort_RANG_FIN :
                self.phytomers[key].bunch.avort = self.phytomers[key].bunch.abortion_decision(1 - min(1,(self.phytomers[key].bunch.ICabort/ GlobalVariables.Seuil_IC_abort)**GlobalVariables.SENSITIVITY_ABORTION))
            if self.phytomers[key].rank == GlobalVariables.ICsex_RANG_FIN :
                self.phytomers[key].bunch.sexe = self.phytomers[key].bunch.sexe_decision(min(1,(self.phytomers[key].bunch.ICsex/GlobalVariables.Seuil_IC_sex)**GlobalVariables.SENSITIVITY_SEX))
            
            ### Simulation de l'ablation des feuilles 
            if self.simulation.step > GlobalVariables.DEBUT_DEFOLIATON :
                if self.simulation.step < GlobalVariables.FIN_DEFOLIATON :
                    if self.phytomers[key].rank == GlobalVariables.RANG_DEFOLIATON :
                        self.phytomers[key].leaf.ablation = self.phytomers[key].leaf.defoliation_decision(GlobalVariables.POURC_DEFOLIATON)
                        
            ### Simulation de l'ablation des regimes
            if self.simulation.step > GlobalVariables.DEBUT_ABLATION_REGIME :
                if self.simulation.step < GlobalVariables.FIN_ABLATION_REGIME :
                    if self.phytomers[key].rank == GlobalVariables.RANG_D_ABLATION_REGIME:
                        if self.phytomers[key].bunch.sexe == "FEMELLE" :
                            if self.phytomers[key].bunch.avort == "NON_AVORTE" :
                                self.phytomers[key].bunch.ablation = self.phytomers[key].bunch.ablation_decision(GlobalVariables.POURC_ABLATION_REGIME)
                            
    def doStep(self, nbOfDaySinceBeginning, TT, TEff, day, month, year):
        if self.simulation.meteo.irrigation_day == "IRRIGATION" :
            pluie = self.simulation.meteo.Rain + self.simulation.meteo.irrigation[self.simulation.meteo.jour_julien]
        else :
            pluie = self.simulation.meteo.Rain
        
        self.BH.compute_FTSW(self.simulation.meteo.getET0(), pluie)
        
        self.tabmemorise_meteo = resize(self.tabmemorise_meteo,(nbOfDaySinceBeginning + 1,10))
        self.tabmemorise_meteo[nbOfDaySinceBeginning,0] = self.name
        self.tabmemorise_meteo[nbOfDaySinceBeginning,1] = self.simulation.meteo.day
        self.tabmemorise_meteo[nbOfDaySinceBeginning,2] = self.simulation.meteo.month
        self.tabmemorise_meteo[nbOfDaySinceBeginning,3] = self.simulation.meteo.year
        self.tabmemorise_meteo[nbOfDaySinceBeginning,4] = self.BH.FTSW
        self.tabmemorise_meteo[nbOfDaySinceBeginning,5] = self.BH.pluie_efficace
        self.tabmemorise_meteo[nbOfDaySinceBeginning,6] = self.simulation.meteo.ET0
        self.tabmemorise_meteo[nbOfDaySinceBeginning,7] = self.simulation.meteo.Rg
        self.tabmemorise_meteo[nbOfDaySinceBeginning,8] = self.simulation.meteo.TEff
        self.tabmemorise_meteo[nbOfDaySinceBeginning,9] = self.simulation.meteo.TT
        
        self.computeProductionSpeed(nbOfDaySinceBeginning)
        self.newPhytomerEmergence += TEff * self.productionSpeed * (self.IC**GlobalVariables.VITESSE_SENSITIVITY)* self.compute_decrease_FTSW(self.BH.FTSW, GlobalVariables.SEUIL_ORGANO)
        #print self.newPhytomerEmergence, self.simulation.step, self.productionSpeed, min(1,self.IC**GlobalVariables.VITESSE_SENSITIVITY), self.IC
        
        #### defiolation le jour de la mise en place des traitements
        
        if self.simulation.step == GlobalVariables.DEBUT_DEFOLIATON :
            for key in sorted(self.phytomers):
                if (self.phytomers[key].rank > GlobalVariables.RANG_DEFOLIATON) :
                    if (self.phytomers[key].state == "ACTIVE") :
                        self.phytomers[key].leaf.ablation = self.phytomers[key].leaf.defoliation_decision(GlobalVariables.POURC_DEFOLIATON)
        
        ### ablation des regimes le jour de la mise en place des traitements
        if self.simulation.step == GlobalVariables.DEBUT_ABLATION_REGIME :
            for key in sorted(self.phytomers):
                if (self.phytomers[key].rank >= GlobalVariables.RANG_D_ABLATION_REGIME) :
                    if (self.phytomers[key].bunch.statut != "RECOLTE") :
                        if self.phytomers[key].bunch.sexe == "FEMELLE" :
                            if self.phytomers[key].bunch.avort == "NON_AVORTE" :
                                self.phytomers[key].bunch.ablation = self.phytomers[key].bunch.ablation_decision(GlobalVariables.POURC_ABLATION_REGIME)
                            
                    
        
        if (self.newPhytomerEmergence >= 1):
            #print self.simulation.step, 'toto'
            phytomerName = self.newPhytomer(TT, day, month, year)
            self.newPhytomerEmergence -= 1
            self.nouvelle_structure(TT, day, month, year)
            self.NF += 1
        phytomerName = ''
        
        self.racines.growth(TEff)
        
        self.compute_SF()
        self.compute_height()
       
        self.compute_biomasse_prod(self.totalLeafArea, GlobalVariables.EFFICIENCE_BIOLOGIQUE,self.simulation.meteo.Rg, self.BH.FTSW)
        self.Assim = self.biomasse_prod
        self.compute_demand(TEff)
        self.IC = self.Assim/self.demand
        self.compute_sexe_abortion()
        self.reserve.compute_etat_reserve()
        self.compute_offre_nette()
        self.compute_fraction_oil_reste()
        self.compute_biomasse_non_structurale_allouee_aux_feuilles() 
        #print 'day =',day,'month =',month,'year=',year, 'FTSW', self.BH.FTSW, 'assim', self.Assim, 'demand', self.demand, 'growth_demand', self.growth_demand, 'respi', self.respi_maintenance,  'offre_pour_croissance', self.offre_pour_croissance,  'delta_pour_croissance', self.delta_avant_croissance, 'apport-reserve', self.reserve.apport, 'offre_nette_pour_croissance', self.offre_nette, 'biomasse_reserve', self.reserve.biomass,'reserve_min', self.reserve.reserve_min, 'reserve_max', self.reserve.reserve_max, 'fr_reste', self.fr_reste, 'fr_fruits', self.fr_fruits 
        #print self.offre_nette/self.demand
        self.compute_growth(day,month,year)
        #print self.BH.FTSW, self.Assim,  self.demand,  self.IC, self.offre_nette, self.respi_maintenance, self.internode_demand, self.leaf_demand, self.bunch_demand
        
        self.tabmemorise = resize(self.tabmemorise,(nbOfDaySinceBeginning+1,12))
        self.tabmemorise[nbOfDaySinceBeginning,0] = self.name
        self.tabmemorise[nbOfDaySinceBeginning,1] = day
        self.tabmemorise[nbOfDaySinceBeginning,2] = month
        self.tabmemorise[nbOfDaySinceBeginning,3] = year
        self.tabmemorise[nbOfDaySinceBeginning,4] = self.totalLeafArea
        self.tabmemorise[nbOfDaySinceBeginning,5] = self.ei
        self.tabmemorise[nbOfDaySinceBeginning,6] = self.IC
        self.tabmemorise[nbOfDaySinceBeginning,7] = self.biomasse_prod
        self.tabmemorise[nbOfDaySinceBeginning,8] = self.demand
        self.tabmemorise[nbOfDaySinceBeginning,9] = self.reserve.biomass 
        self.tabmemorise[nbOfDaySinceBeginning,10] = self.NF
        self.tabmemorise[nbOfDaySinceBeginning,11] = self.trunk_height
    
    def compute_biomasse_non_structurale_allouee_aux_feuilles(self) :
        TOT = 0
        FRAC = 0 
        if  self.reserve.delta_biomasse_reserve_leaf > 0 :
            
            for key in sorted(self.phytomers):
                if (self.phytomers[key].state == "ACTIVE") :
                    if (self.phytomers[key].leaf.capacite_reserve_max  - self.phytomers[key].leaf.non_structural_biomass)  >= 0 : 
                        TOT += (self.phytomers[key].leaf.capacite_reserve_max  - self.phytomers[key].leaf.non_structural_biomass) 
                        #print self.phytomers[key].leaf.capacite_reserve_max, self.phytomers[key].leaf.non_structural_biomass ,(self.phytomers[key].leaf.capacite_reserve_max  - self.phytomers[key].leaf.non_structural_biomass),  TOT
                    
            if TOT <= 0 :   
                for key in sorted(self.phytomers):
                    if (self.phytomers[key].leaf.leafArea != 0) :
                        self.phytomers[key].leaf.fraction_non_str_biomasse_allouee = 0
            else :
                for key in sorted(self.phytomers):
                    if (self.phytomers[key].state != 'DEAD') :   
                        if (self.phytomers[key].leaf.leafArea != 0) :    
                            if (self.phytomers[key].leaf.capacite_reserve_max  - self.phytomers[key].leaf.non_structural_biomass)  >= 0 :   
                                self.phytomers[key].leaf.fraction_non_str_biomasse_allouee =  (self.phytomers[key].leaf.capacite_reserve_max  - self.phytomers[key].leaf.non_structural_biomass ) / TOT
                                FRAC += self.phytomers[key].leaf.fraction_non_str_biomasse_allouee
 
            #print '--------------------------------------'
        
        else :
            
            for key in sorted(self.phytomers):
                if (self.phytomers[key].state == "ACTIVE") :
                    
                    if self.phytomers[key].leaf.non_structural_biomass  >= 0 : 
                        TOT +=  self.phytomers[key].leaf.non_structural_biomass 
                        #print self.phytomers[key].leaf.non_structural_biomass,  TOT

            if TOT <= 0 :               
                for key in sorted(self.phytomers):
                    if (self.phytomers[key].leaf.leafArea != 0) :
                        self.phytomers[key].leaf.fraction_non_str_biomasse_allouee = 0
            else :
                for key in sorted(self.phytomers):
                    if (self.phytomers[key].state != 'DEAD') :                    
                        if (self.phytomers[key].leaf.leafArea != 0) :
                            if self.phytomers[key].leaf.non_structural_biomass  >= 0 : 
                                self.phytomers[key].leaf.fraction_non_str_biomasse_allouee =  (self.phytomers[key].leaf.non_structural_biomass ) / TOT
                                FRAC += self.phytomers[key].leaf.fraction_non_str_biomasse_allouee
  
            #print '--------------------------------------'
            
        
    def compute_growth(self, day, month, year) :
        
        self.trunk_biomass = 0
        self.bunch_biomass = 0
        self.leaf_non_structural_biomass = 0
        self.leaf_structural_biomass = 0 
        self.respirable_bunch_biomass = 0
        self.male_biomass  = 0 
        for key in sorted(self.phytomers):
            if (self.phytomers[key].state == "ACTIVE") :
                self.phytomers[key].growth()
            #print 'day =',day,'month =',month,'year=',year,'rank=',self.phytomers[key].rank, 'statut', self.phytomers[key].bunch.statut,'sex',self.phytomers[key].bunch.sexe, 'avort',self.phytomers[key].bunch.avort, 'leaf_area', self.phytomers[key].leaf.leafArea, 'TT_feuille', self.phytomers[key].leaf.thermalTimeSinceAppearance  #, 'IC_setting', self.phytomers[key].bunch.IC_setting, 'masse_max',  self.phytomers[key].bunch.masse_max_regime, 'masse_pot', self.phytomers[key].bunch.masse_pot_regime, 'masse_pot', self.phytomers[key].bunch.potential_biomass,'masse', self.phytomers[key].bunch.biomass
                self.leaf_structural_biomass += self.phytomers[key].leaf.structural_biomass
                self.leaf_non_structural_biomass += self.phytomers[key].leaf.non_structural_biomass
                self.bunch_biomass +=self.phytomers[key].bunch.biomass ## attention pour la respi de maintenance !!!
                self.respirable_bunch_biomass += self.phytomers[key].bunch.respirable_biomass
                self.female_bunch_biomass += self.phytomers[key].bunch.femelle_biomass
                self.male_biomass += self.phytomers[key].bunch.male_biomass
            self.trunk_biomass += self.phytomers[key].internode.biomass
            self.biomass = self.leaf_structural_biomass + self.leaf_non_structural_biomass +  self.female_bunch_biomass + self.trunk_biomass + self.reserve.biomass + self.male_biomass
        #print (str(self.simulation.step) + '\t' +  str(self.respirable_bunch_biomass) + '\t' +  str(self.male_biomass) + "\n")  
            
        for key in sorted(self.phytomers):
            self.trunk_biomass += self.phytomers[key].internode.biomass
            #print 'day =',day,'month =',month,'year=',year,'rank=',self.phytomers[key].rank, 'internode_biomass', self.phytomers[key].internode.biomass, 'internode_lenght',   self.phytomers[key].internode.length 
            
        self.total_leaf_biomass = self.leaf_structural_biomass + self.leaf_non_structural_biomass 
        
    def compute_demand(self,TEff) :
        self.demand = 0
        self.leaf_demand = 0
        self.internode_demand = 0
        self.bunch_demand = 0
        self.fruit_demand = 0
        self.male_demand = 0
        self.peduncule_demand = 0
        self.growth_demand  = 0 
        self.compute_respi_maintenance()
        for key in sorted(self.phytomers):
            #if (key != phytomerName):   A MODIFIER POUR LIMITER LA CROISSANCE LE PREMIER JOUR
            if (self.phytomers[key].state == "ACTIVE") : ## A VERIFIER SI CETTE CONDITION NE CREE PAS DES PROBLEMES
                self.phytomers[key].growth_demand(TEff)
                self.internode_demand += self.phytomers[key].internode.demand
                self.leaf_demand += self.phytomers[key].leaf.demand
                self.bunch_demand += self.phytomers[key].bunch.demand
                self.fruit_demand += self.phytomers[key].bunch.fruit.demand
                self.male_demand += self.phytomers[key].bunch.male.demand
                self.peduncule_demand += self.phytomers[key].bunch.peduncule.demand
        self.demand += self.internode_demand + self.leaf_demand + self.respi_maintenance + self.bunch_demand
        #print  'day =', self.simulation.meteo.getDay(),'month =', self.simulation.meteo.getMonth(),'year=',self.simulation.meteo.getYear(), 'demand', self.bunch_demand
        self.growth_demand +=  self.internode_demand +   self.leaf_demand  +  self.bunch_demand    
    
    def compute_sexe_abortion(self) :
        #print '--------------------------------'
        
        for key in sorted(self.phytomers) :
            self.phytomers[key].bunch.compute_IC()
            #print 'day =', self.simulation.meteo.getDay(),'month =', self.simulation.meteo.getMonth(),'year=',self.simulation.meteo.getYear(),'rank=',self.phytomers[key].rank, "sexe",self.phytomers[key].bunch.sexe,  "avort", self.phytomers[key].bunch.avort,  "statut", self.phytomers[key].bunch.statut, "bunch_weight",  self.phytomers[key].bunch.biomass, "fruit_biomass",self.phytomers[key].bunch.fruit.oil_biomass + self.phytomers[key].bunch.fruit.nonoil_biomass , "peduncule_biomass", self.phytomers[key].bunch.peduncule.biomass  #"bunch_biomass", self.phytomers[key].bunch.biomass, "potential_bunch_biomass" , self.phytomers[key].bunch.potential_biomass, "structural_biomass", self.phytomers[key].leaf.structural_biomass, "non_structural_biomass", self.phytomers[key].leaf.non_structural_biomass, "SLW",self.phytomers[key].leaf.SLW
            #print  'jour' , self.simulation.meteo.getDay(), 'mois', self.simulation.meteo.getMonth(),'annee', self.simulation.meteo.getYear(),'rang', self.phytomers[key].rank, 'longueur', self.phytomers[key].internode.length, 'duree_allongement', self.phytomers[key].internode.duree_allongement, 'max_lenght', self.phytomers[key].internode.max_length, 'demand' , self.phytomers[key].internode.demand, 'max_lenght_increase', self.phytomers[key].internode.max_lenght, 'TT_corrige', self.phytomers[key].internode.TT_corrige, 'gain_TEff_jour', self.phytomers[key].internode.gain_TEff_jour, 'IC_tree_vege', self.fr_reste # self.phytomers[key].bunch.sexe,   self.phytomers[key].bunch.avort,   self.phytomers[key].bunch.statut,    self.phytomers[key].leaf.leafArea,   self.phytomers[key].bunch.biomass, self.phytomers[key].bunch.fruit.oil_biomass + self.phytomers[key].bunch.fruit.nonoil_biomass , self.phytomers[key].bunch.peduncule.biomass  #"bunch_biomass", self.phytomers[key].bunch.biomass, "potential_bunch_biomass" , self.phytomers[key].bunch.potential_biomass, "structural_biomass", self.phytomers[key].leaf.structural_biomass, "non_structural_biomass", self.phytomers[key].leaf.non_structural_biomass, "SLW",self.phytomers[key].leaf.SLW

            #if self.phytomers[key].name > 200 :
            #    if self.phytomers[key].name < 230 : 
            #        if self.phytomers[key].bunch.sexe == "FEMELLE" :
            #            if self.phytomers[key].bunch.avort == "NON_AVORTE" :
            #    
            #                print str(self.phytomers[key].name) + '\t' +  str(self.simulation.step) + '\t' +  str(self.simulation.meteo.getDay()) + '\t' + str(self.simulation.meteo.getMonth()) +'\t' + str(self.simulation.meteo.getYear()) + '\t' +  str(self.phytomers[key].bunch.peduncule.demand) + '\t' + str(self.phytomers[key].bunch.fruit.oil_demand) + '\t' +  str(self.phytomers[key].bunch.fruit.nonoil_demand)  
            #print 'day_of_simulation',self.simulation.step ,'day =', self.simulation.meteo.getDay(),'month =', self.simulation.meteo.getMonth(),'year=',self.simulation.meteo.getYear(),'name=', self.phytomers[key].name, 'rank=',self.phytomers[key].rank,  'state', self.phytomers[key].state, 'leaf_biomass', self.phytomers[key].leaf.total_biomass #, 'bunch_sexe', self.phytomers[key].bunch.sexe, 'avort', self.phytomers[key].bunch.avort,  'biomasse', self.phytomers[key].bunch.biomass # 'biomass_pot', self.phytomers[key].bunch.potential_biomass, 'TT', self.phytomers[key].bunch.thermalTimeSinceAppearance, 'TT_corrige', self.phytomers[key].bunch.TT_corrige  # 'gain_corrige', self.phytomers[key].bunch.gain_TEff_jour *  'bunch_biomass', self.phytomers[key].bunch.biomass, 'pot_biomass', self.phytomers[key].bunch.potential_biomass #, self.phytomers[key].bunch.fruit.duree_oleo # 'duree_oleo', self.phytomers[key].bunch.fruit.duree_oleo, 'absolute_duree', selfphytomers[key].bunch.fruit.absolue_duree_oleo # 'oil_demand', self.phytomers[key].bunch.fruit.oil_demand   #, 'bunch_statut', self.phytomers[key].bunch.statut, 'TT_corrige', self.phytomers[key].bunch.TT_corrige, 'TT_vrai', self.phytomers[key].bunch.thermalTimeSinceAppearance, 'bunch_sexe',self.phytomers[key].bunch.sexe,'bunch_avort',self.phytomers[key].bunch.avort, 'potential_biomass',  self.phytomers[key].bunch.potential_biomass,'biomass', self.phytomers[key].bunch.biomass, 'test_biomass',self.phytomers[key].bunch.test_biomass #'non_oil_biomass_test', self.phytomers[key].bunch.fruit.test_nonoil_biomass, 'non_oil_biomass',self.phytomers[key].bunch.fruit.nonoil_biomass, 'oil_biomass_test', self.phytomers[key].bunch.fruit.test_oil_biomass, 'oil_biomass', self.phytomers[key].bunch.fruit.oil_biomass   #  'TT_corrige_leaf_area', self.phytomers[key].leaf.TT_corrige,'TT_vrai_leaf_area', self.phytomers[key].leaf.thermalTimeSinceAppearance ,'leaf_area', self.phytomers[key].leaf.leafArea, 'pot_leaf_area', self.phytomers[key].leaf.potLeafArea
       
        if  self.simulation.step == GlobalVariables.SORTIE1 :
            self.make_output(GlobalVariables.SORTIE1 )
        if  self.simulation.step == GlobalVariables.SORTIE2 :
            self.make_output(GlobalVariables.SORTIE2 )
        if  self.simulation.step == GlobalVariables.SORTIE3 :
            self.make_output(GlobalVariables.SORTIE3)
        if  self.simulation.step == GlobalVariables.SORTIE4 :
            self.make_output(GlobalVariables.SORTIE4)
        if  self.simulation.step == GlobalVariables.SORTIE5 :
            self.make_output(GlobalVariables.SORTIE5)
            
    def make_output(self, num_sortie):
        output_name = GlobalVariables.DRIVE + GlobalVariables.OUTPUTDIRECTORYNAME
        nom = "sortie_complete_vege_arbre_" +  str(self.name) + "_step_" + str(num_sortie) + ".txt"
        f=open(output_name + "description/" +  nom,"w")
        f.write("day" + '\t' + "month" + '\t' + "year" + '\t'+ "name" + '\t' + "appearence_step" + '\t' + "rank" + '\t' + "date_app_phytomere" + '\t' + "relative_rank_position" + '\t' + "nb_jours_delin_pn" +  '\t' + "Assim_max" + '\t' + "declin-stress" + '\t' + "pourc_interception" + '\t' +  "prod_feuille" + '\t' + "leaf_state" + '\t' + "leaf_ablation" + '\t' +  "SF" + '\t' + "masse" + '\t' + "SLW" + '\t' + "internode_lenght" + "\n")
        for key in sorted(self.phytomers) : 
            phytomer = self.phytomers[key]
            #if phytomer.state != 'DEAD' :
            leaf = phytomer.leaf
            internode = phytomer.internode
            f.write(str(self.simulation.meteo.day)+'\t'+ str(self.simulation.meteo.month)+'\t'+ str(self.simulation.meteo.year) + '\t' + str(phytomer.appearanceDay) + str("/") + str(phytomer.appearanceMonth) + str("/") + str(phytomer.appearanceYear) +  '\t'+  str(phytomer.name) + '\t' + str(phytomer.step_apparition) + '\t' +  str(phytomer.rank) + '\t' + str(leaf.crown_position) + '\t' +  str(leaf.nb_jour_declin_photosynthese) + '\t' + str(leaf.assim_max) + '\t' + str(leaf.declin_photosynthese_stress) + '\t' +  str(leaf.light_pourc_interception) + '\t' + str(leaf.biomasse_prod) +  '\t' + str(phytomer.state) +'\t'+ str(leaf.ablation) +'\t'+ str(leaf.leafArea) +'\t'+ str(leaf.total_biomass) +'\t'+  str(leaf.SLW) +  '\t' +  str(internode.length) + "\n" )
        f.close()
        
        output_name = GlobalVariables.DRIVE + GlobalVariables.OUTPUTDIRECTORYNAME
        nom = "sortie_complete_regime_arbre_" +  str(self.name) + "_step_" + str(num_sortie) + ".txt"
        f=open(output_name + "description/" +  nom,"w")
        f.write("day" + '\t' + "month" + '\t' + "year" +  '\t'+ "name"  + '\t' + "appearence_step" + '\t' + "rank" + '\t' + "facteur_age" + '\t'+ "bunch_sex" + '\t' + "bunch_avorte" + '\t' + "IC_spikelet" + '\t' + "IC_setting" + '\t' + "fruit_number" + '\t' + "ind_fruit_weight" + '\t' + "ablation" + '\t' + "TT_corrige" + '\t' + "statut" + '\t' + "fruit_statut" + '\t'+ "temps_thermique" + '\t' + "pot_statut" + '\t'+ "biomass" + '\t' + "pot_biomass" + '\t' + "oil_biomass" + '\t' + "non_oil_biomass" + '\t' + "peduncule_biomass" + '\t' + "demande" + '\t' + "ratio_huile_mesocarp" + '\t' + "date_floraison" + '\t' + "date_harvest" + '\t' + "date_debut_oleo" + '\t' + "male_biomass" + '\t' + "peduncule_demand" +  '\t' + "non_ oil_demand" + '\t' + "oil_demand" +  "\n")
        for key in sorted(self.phytomers) :
            #if phytomer.state != 'DEAD' :
            date_oleo = self.phytomers[key].bunch.estimated_harvest_date - self.phytomers[key].bunch.fruit.duree_oleo
            phytomer = self.phytomers[key]
            bunch = phytomer.bunch
            fruit = bunch.fruit
            peduncule = bunch.peduncule
            male_biomass = bunch.male.biomass
            f.write(str(self.simulation.meteo.day)+'\t'+ str(self.simulation.meteo.month)+'\t'+ str(self.simulation.meteo.year) +'\t'+  str(phytomer.name) +'\t'+ str(phytomer.step_apparition) + '\t' + str(phytomer.rank) +'\t'+ str(bunch.facteur_age_regimes) + '\t' +  str(bunch.sexe) +'\t'+ str(bunch.avort) +'\t'+ str(bunch.IC_spikelet**GlobalVariables.SENSIVITY_IC_SPIKELET) +'\t'+  str(bunch.IC_spikelet**GlobalVariables.SENSIVITY_IC_SETTING) + '\t' + str(bunch.fruit_number) + '\t' + str(bunch.fruit.masse_ind) + '\t' + str(bunch.ablation) +  '\t'+ str(bunch.TT_corrige) + '\t' + str(bunch.statut) + '\t' + str(fruit.oil_statut) + '\t' + str(bunch.thermalTimeSinceAppearance)+ '\t' +  str(bunch.pot_statut) + '\t'+   str(bunch.biomass) +  '\t' + str(bunch.potential_biomass) + '\t' + str(fruit.oil_biomass) + '\t'+ str(fruit.nonoil_biomass) + '\t'+ str(peduncule.biomass) + '\t' + str(bunch.demand) + '\t' + str(fruit.ratio_huile_mesocarp) +'\t' +  str(bunch.estimated_flowering_date) + '\t' + str(bunch.estimated_harvest_date) + '\t' +  str(date_oleo) + '\t' + str(male_biomass) + '\t' + str(peduncule.demand) + '\t' + str(fruit.nonoil_demand) + '\t' + str(fruit.oil_demand) +  "\n" ) # 
        f.close() 
        
    def Q_10(self,Tmoy,param):
        return(param**((Tmoy-25)/10))
    
    
    def compute_respi_maintenance(self) :
        Q10 = self.Q_10((self.simulation.meteo.Tmin + self.simulation.meteo.Tmax)/2,2)
        self.respi_maintenance = Q10*(self.trunk_biomass * GlobalVariables.COUT_RESPI_MAINTENANCE_STIPE + self.respirable_bunch_biomass * GlobalVariables.COUT_RESPI_MAINTENANCE_BUNCH + self.leaf_structural_biomass * GlobalVariables.COUT_RESPI_MAINTENANCE_LEAF)
    
 
    def compute_offre_nette(self) :
        self.offre_nette = self.offre_pour_croissance + self.reserve.apport
        self.fraction_pour_croissance = self.offre_nette / (self.growth_demand)
        
    def compute_fraction_oil_reste(self) :
        demande_cr_recal = self.leaf_demand + self.internode_demand  + self.male_demand + GlobalVariables.AF_FRUITS * self.fruit_demand + self.peduncule_demand 
            
        if GlobalVariables.AF_FRUITS >= 1:
            self.offre_fruits = min( ( GlobalVariables.AF_FRUITS * self.fruit_demand * self.offre_nette / demande_cr_recal ), self.fruit_demand )
        
            if self.fruit_demand==0 :
                self.fr_fruits = 1
                self.offre_reste = self.offre_nette - self.offre_fruits
                self.fr_reste = self.offre_reste / ( self.leaf_demand + self.internode_demand  + self.male_demand + self.peduncule_demand  )
            else :     
                self.fr_fruits = self.offre_fruits / self.fruit_demand
                self.offre_reste = self.offre_nette - self.offre_fruits
                self.fr_reste = self.offre_reste / ( self.leaf_demand + self.internode_demand  + self.male_demand + self.peduncule_demand  )
           
        else:
            self.offre_reste = min( ( self.leaf_demand + self.internode_demand  + self.male_demand + self.peduncule_demand  ) * self.offre_reste / demande_cr_recal, self.leaf_demand + self.internode_demand  + self.male_demand + self.peduncule_demand  )
            self.fr_reste = self.offre_reste / ( self.leaf_demand + self.internode_demand  + self.male_demand + self.peduncule_demand  )
            self.offre_fruits = self.offre_nette - self.offre_reste
            self.fr_fruits = self.offre_fruits/ self.fruit_demand
        

    def writeRecord(self, bunch):
        month = self.simulation.meteo.month
        year = self.simulation.meteo.year
        biomass = bunch.biomass
        potential_biomass = bunch.potential_biomass
        IC_spikelet = bunch.IC_spikelet ** (GlobalVariables.SENSIVITY_IC_SPIKELET)
        IC_setting = bunch.IC_setting ** (GlobalVariables.SENSIVITY_IC_SETTING)
        fruit_number = bunch.fruit_number
        pot_fruit_number = bunch.pot_fruits_number
        fruit_weight = bunch.fruit.masse_ind
        ratio_huile = bunch.fruit.ratio_huile_mesocarp
        #print self.name
#        print(self.simulation.meteo.getMonth()), (self.record[self.recordIndex-1, 1])
        #print self.recordIndex
 
        if ((self.record[self.recordIndex - 1, 1]) != month):
            
            self.record = resize(self.record, (self.recordIndex + 1, 13))
            
            #print self.record.shape
            self.recordIndex += 1
            self.record[self.recordIndex - 1, 0] = self.name
            self.record[self.recordIndex - 1, 1] = month
            self.record[self.recordIndex - 1, 2] = year
            if bunch.sexe == "FEMELLE" :
                    if bunch.avort == "NON_AVORTE" :
                        if bunch.ablation == "NON_ABLATED" :
                            self.record[self.recordIndex - 1, 3] = biomass
                            self.record[self.recordIndex - 1, 4] = 1
                            self.record[self.recordIndex - 1, 5] = potential_biomass
                            self.record[self.recordIndex - 1, 6] = IC_spikelet
                            self.record[self.recordIndex - 1, 7] = IC_setting
                            self.record[self.recordIndex - 1, 9] = fruit_number
                            self.record[self.recordIndex - 1, 10] = pot_fruit_number
                            self.record[self.recordIndex - 1, 11] = fruit_weight
                            self.record[self.recordIndex - 1, 12] = ratio_huile
            self.record[self.recordIndex - 1, 8] = 1
            
        else:
            if ((self.record[self.recordIndex - 1, 2]) == year) :
                
                if bunch.sexe == "FEMELLE" :
                    if bunch.avort == "NON_AVORTE" :
                        if bunch.ablation == "NON_ABLATED" :
                            self.record[self.recordIndex - 1 ,3] += biomass
                            self.record[self.recordIndex - 1 ,4] += 1
                            self.record[self.recordIndex - 1, 5] += potential_biomass
                            self.record[self.recordIndex - 1, 6] += IC_spikelet
                            self.record[self.recordIndex - 1, 7] += IC_setting
                            self.record[self.recordIndex - 1, 9] += fruit_number
                            self.record[self.recordIndex - 1, 10] += pot_fruit_number
                            self.record[self.recordIndex - 1, 11] += fruit_weight
                            self.record[self.recordIndex - 1, 12] += ratio_huile
                self.record[self.recordIndex - 1, 8] += 1
                
            else :
                self.record = resize(self.record, (self.recordIndex + 1, 9))
            #print self.record.shape
                self.recordIndex += 1
                self.record[self.recordIndex - 1, 0] = self.name
                self.record[self.recordIndex - 1, 1] = month
                self.record[self.recordIndex - 1, 2] = year
                if bunch.sexe == "FEMELLE" :
                    if bunch.avort == "NON_AVORTE" :
                        if bunch.ablation == "NON_ABLATED" :
                            self.record[self.recordIndex - 1, 3] = biomass
                            self.record[self.recordIndex - 1, 4] = 1
                            self.record[self.recordIndex - 1, 5] = potential_biomass
                            self.record[self.recordIndex - 1, 6] = IC_spikelet
                            self.record[self.recordIndex - 1, 7] = IC_setting
                self.record[self.recordIndex - 1, 8] = 1
 
 
    def write_floweringRecord(self,bunch) :
        month = self.simulation.meteo.month
        year = self.simulation.meteo.year
        sexe = bunch.sexe
        avorte = bunch.avort
            
        if ((self.record_flowering[self.record_floweringIndex - 1, 1]) != month):
            self.record_flowering = resize(self.record_flowering, (self.record_floweringIndex + 1, 6))
            self.record_floweringIndex += 1
            self.record_flowering[self.record_floweringIndex - 1, 0] = self.name
            self.record_flowering[self.record_floweringIndex - 1, 1] = month
            self.record_flowering[self.record_floweringIndex - 1, 2] = year
            if avorte == "AVORTE" :
                self.record_flowering[self.record_floweringIndex- 1, 3] = 1
            else :
                if sexe == "FEMELLE" :
                    self.record_flowering[self.record_floweringIndex - 1, 4] = 1
                else :
                    self.record_flowering[self.record_floweringIndex - 1, 5] = 1
                        
        else :
            if ((self.record_flowering[self.record_floweringIndex - 1, 2]) == year) :
                if avorte == "AVORTE" :
                    self.record_flowering[self.record_floweringIndex- 1, 3] += 1
                else :
                    if sexe == "FEMELLE" :
                        self.record_flowering[self.record_floweringIndex - 1, 4] += 1
                    else :
                        self.record_flowering[self.record_floweringIndex - 1, 5] += 1
                            
            else :
                self.record_flowering = resize(self.record_flowering, (self.record_floweringIndex + 1, 6)) 
                self.record_floweringIndex += 1
                self.record_flowering[self.record_floweringIndex - 1, 0] = self.name
                self.record_flowering[self.record_floweringIndex - 1, 1] = month
                self.record_flowering[self.record_floweringIndex - 1, 2] = year
                if avorte == "AVORTE" :
                    self.record_flowering[self.record_floweringIndex- 1, 3] = 1
                else :
                    if sexe == "FEMELLE" :
                        self.record_flowering[self.record_floweringIndex - 1, 4] = 1
                    else :
                        self.record_flowering[self.record_floweringIndex - 1, 5] = 1
        
        #print month, year, self.record_flowering[self.record_floweringIndex - 1 ,3], self.record_flowering[self.record_floweringIndex - 1 ,4], self.record_flowering[self.record_floweringIndex - 1 ,5]