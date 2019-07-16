#!/usr/bin/env python

from Organ import Organ
from peduncule import Peduncule
from male import Male
from GlobalVariables import GlobalVariables
from fruit import Fruit
from numpy import *

from random import *

class Bunch(Organ):
    
    def __init__(self, phytomerInstance):
        Organ.__init__(self, phytomerInstance)
        self.peduncule = Peduncule(self)
        self.fruit = Fruit(self)
        self.male = Male(self)
        self.ablation = "NON_ABLATED"
        self.statut = ""
        self.pot_statut = ""
        self.estimated_flowering_date = 0
        self.estimated_harvest_date = 0        
        self.absolute_flowering_date = GlobalVariables.MINIMAL_PRODUCTION_SPEED / self.phytomer.tree.productionSpeed * (GlobalVariables.TT_FLOWERING_ADULTE)
        self.absolute_harvest_date = GlobalVariables.MINIMAL_PRODUCTION_SPEED / self.phytomer.tree.productionSpeed * (GlobalVariables.TT_HARVEST_ADULTE)
        self.TT_corrige = 0
        self.demand = 0 
        self.compute_statut()
        self.sexe= ""
        self.avort = ""
        self.masse_pot_regime = 0 
        self.ICsex = 0
        self.nb_joursICsex = 0
        self.ICsex_tot = 0 
        self.ICabort = 0
        self.nb_joursICabort = 0
        self.ICabort_tot = 0
        self.nb_joursIC_spikelet = 0
        self.IC_spikelet_tot = 0
        self.IC_spikelet = 1  
        self.biomass =  0 
        self.femelle_biomass = 0
        self.respirable_biomass = 0
        self.ini_flowering_date = 0 
        self.masse_max_regime = 0
        self.nb_joursIC_setting = 0
        self.IC_setting_tot = 0
        self.IC_setting = 1
        self.flowering_day =""
        self.flowering_month = ""
        self.flowering_year = ""
        self.harvest_day = ""
        self.harvest_month = ""
        self.harvest_year = ""
        self.index = 0
        self.red_vitesse_FTSW = 0 
        #self.total_demand = 0
        self.potential_biomass = 0 ## au debut on initinialise la biomasse potentielle a la biomasse reell
        self.test_biomass = 0 
        self.gain_TEff_jour = 0
        self.fruit_number = 0
        
        if (GlobalVariables.INCREASE_TAILLE_REGIMES * (self.phytomer.tree.simulation.step  - self.phytomer.tree.date_plus_jeune_feuille) + GlobalVariables.FACTEUR_AGE_INI) > 1 :
           
            self.facteur_age_regimes = 1
        else:
            self.facteur_age_regimes = ( GlobalVariables.INCREASE_TAILLE_REGIMES * (self.phytomer.tree.simulation.step - self.phytomer.tree.date_plus_jeune_feuille) + GlobalVariables.FACTEUR_AGE_INI) 
            
        #print self.phytomer.name, self.phytomer.tree.simulation.step , GlobalVariables.INCREASE_TAILLE_REGIMES, GlobalVariables.FACTEUR_AGE_INI , self.facteur_age_regimes   
        
        self.pot_fruits_number = self.facteur_age_regimes * GlobalVariables.MEAN_FRUIT_NUMBER_ADULTE
        
        
        
        
    def growth_demand(self, TEff):
        self.compute_red_vitesse_FTSW()     # voir a modifier avec V0
        correctedTEff = self.computeTEffCorrection(TEff)
        self.gain_TEff_jour = correctedTEff 
        Organ.growth_demand(self, correctedTEff)
        if self.statut != "RECOLTE" :         
            self.compute_statut()
        #print 'day =',self.phytomer.tree.simulation.meteo.getDay(),'month =', self.phytomer.tree.simulation.meteo.getMonth(),'year=', self.phytomer.tree.simulation.meteo.getYear(),'rank=',self.phytomer.rank, 'statut', self.statut, 'TEff', TEff, 'red_vitesse', self.red_vitesse_FTSW, 'corrected_Teff', correctedTEff, 'TT',  self.thermalTimeSinceAppearance    
   #    self.compute_masse_pot_reg(self.phytomer.tree.simulation.step)
        if  self.sexe == "FEMELLE" :
            if self.avort != "AVORTE"  : # and self.ablation != "ABLATED")
                self.peduncule.growth_demand(correctedTEff)
                self.fruit.growth_demand(correctedTEff)
            else :
                self.peduncule.demand = 0
                self.fruit.nonoil_demand = 0
                self.fruit.oil_demand = 0
                self.fruit.demand = 0
                
                
        if self.sexe == "MALE" :
            if self.avort != "AVORTE" :
                self.male.growth_demand(correctedTEff)
            else :
                self.male.demand = 0
                
                
        self.demand = self.fruit.demand + self.peduncule.demand + self.male.demand
        #self.total_demand += self.demand
        #kernel.growth(self,)
        #mesocarp.growth(self,)
        
    def growth(self) :
        if (self.avort != "AVORTE" and self.ablation != "ABLATED") :   #and self.ablation != "ABLATED"
        
            self.peduncule.growth()
            self.fruit.growth()
            self.male.growth()
        else : 
            self.peduncule.biomass = 0
            self.fruit.nonoil_biomass = 0
            self.fruit.oil_biomass = 0
            self.fruit.masse_ind  = 0
            self.fruit.ratio_huile_mesocarp = 0 
            self.fruit.oil_potential_biomass = 0
            self.fruit.nonoil_potential_biomass = 0
            self.peduncule.potential_biomass = 0
            self.male.potential_biomass = 0
            self.male.biomass = 0 
        
        self.compute_biomass()
        self.compute_TT_corrige()
        self.compute_fruit_number()
    
        
    def compute_biomass(self) :
        
        if self.statut != "RECOLTE" :   
            self.biomass = self.fruit.oil_biomass  + self.fruit.nonoil_biomass + self.peduncule.biomass + self.male.biomass
            self.femelle_biomass = self.fruit.oil_biomass  + self.fruit.nonoil_biomass + self.peduncule.biomass
            self.male_biomass = self.male.biomass
            self.respirable_biomass = self.fruit.nonoil_biomass + self.peduncule.biomass + self.male.biomass
            self.potential_biomass = self.fruit.oil_potential_biomass  + self.fruit.nonoil_potential_biomass + self.peduncule.potential_biomass + self.male.potential_biomass
            self.test_biomass = self.fruit.test_oil_biomass  + self.fruit.test_nonoil_biomass + self.peduncule.test_biomass + self.male.biomass
        #print 'day =', self.phytomer.tree.simulation.meteo.getDay(),'month =', self.phytomer.tree.simulation.meteo.getMonth(),'year=',self.phytomer.tree.simulation.meteo.getYear(),'rank=',self.phytomer.rank, 'statut', self.statut,'sex',self.bunch.sexe, 'avort',self.avort, 'biomass', self.biomass, 'potential_biomass',self.potential_biomass, 'leafarea', self.phytomers[key].leaf.leafArea 
        else :
            self.biomass = 0
            self.femelle_biomass = 0
            self.male_biomass = 0
            self.respirable_biomass = 0
            #self.potential_biomass = 0
            self.test_biomass = 0
        #print 'day =', self.
        
    def computeTEffCorrection(self, TEff):
        return  (TEff * self.red_vitesse_FTSW)
    
    def compute_TT_corrige(self) :
        if self.statut ==  "APPARITION_FLORAISON" :
            self.TT_corrige +=   ( ( self.phytomer.tree.fr_fruits ) ** GlobalVariables.PLASTICITY_BUNCH_IC_AVANT_FLORAISON ) * self.gain_TEff_jour
        if self.statut ==  "FLORAISON_RECOLTE" :
            self.TT_corrige += ( ( self.phytomer.tree.fr_fruits ) ** GlobalVariables.PLASTICITY_BUNCH_IC_APRES_FLORAISON ) * self.gain_TEff_jour
        
        #if self.statut ==  "RECOLTE" :
        #    self.TT_corrige +=   ( ( self.phytomer.tree.fr_fruits ) ** GlobalVariables.PLASTICITY_BUNCH_IC_AVANT_FLORAISON ) * self.gain_TEff_jour
        #else :
        #    self.TT_corrige += ( ( self.phytomer.tree.fr_fruits ) ** GlobalVariables.PLASTICITY_BUNCH_IC_APRES_FLORAISON ) * self.gain_TEff_jour
    
    def compute_structure_ini(self) :
        self.compute_ini_flowering_date()
        self.compute_ini_harvest_date()
        if (self.sexe == "FEMELLE") :
            if (self.avort != "AVORTE") :
                self.peduncule.compute_structure_ini()
                self.fruit.compute_structure_ini()
        if (self.sexe == "MALE") :
            if (self.avort != "AVORTE") :
                self.male.compute_structure_ini()
        self.compute_biomass()
    
    def compute_IC(self):
        if (self.phytomer.rank > GlobalVariables.ICsex_RANG_DEBUT):
            if (self.phytomer.rank < GlobalVariables.ICsex_RANG_FIN ):
                self.nb_joursICsex += 1
                self.ICsex_tot += self.phytomer.tree.IC
                self.ICsex = self.ICsex_tot / self.nb_joursICsex
        
        if self.phytomer.rank > GlobalVariables.ICabort_RANG_DEBUT :
            if self.phytomer.rank < GlobalVariables.ICabort_RANG_FIN : 
                self.nb_joursICabort += 1
                self.ICabort_tot += self.phytomer.tree.IC
                self.ICabort = self.ICabort_tot / self.nb_joursICabort 
        
        if self.phytomer.rank > GlobalVariables.IC_spikelet_RANG_DEBUT : 
            if self.phytomer.rank < GlobalVariables.IC_spikelet_RANG_FIN :
                self.nb_joursIC_spikelet += 1
                self.IC_spikelet_tot += self.phytomer.tree.IC
                self.IC_spikelet = (self.IC_spikelet_tot) / self.nb_joursIC_spikelet
                
        if self.TT_corrige > self.estimated_flowering_date - GlobalVariables.DEBUT_RANG_SENSITIVITY_NOUAISON / self.phytomer.tree.productionSpeed  :
            if self.TT_corrige < self.estimated_flowering_date - GlobalVariables.FIN_RANG_SENSITIVITY_NOUAISON / self.phytomer.tree.productionSpeed  :   
                if self.TT_corrige < self.estimated_flowering_date :
                    self.nb_joursIC_setting += 1
                    self.IC_setting_tot += self.phytomer.tree.IC
                    self.IC_setting = (self.IC_setting_tot) / self.nb_joursIC_setting  
        
        
    def compute_fruit_number(self) :
        if self.phytomer.rank > GlobalVariables.IC_spikelet_RANG_FIN :
            if self.TT_corrige < self.estimated_flowering_date :
                self.fruit_number = (self.IC_spikelet ** GlobalVariables.SENSIVITY_IC_SPIKELET) * self.pot_fruits_number
            else :
                self.fruit_number =  (self.IC_spikelet ** GlobalVariables.SENSIVITY_IC_SPIKELET) * (self.IC_setting ** GlobalVariables.SENSIVITY_IC_SETTING)  * self.pot_fruits_number
            if self.sexe == "MALE" :
                self.fruit_number = 0
            if self.avort == "AVORTE" :
                self.fruit_number = 0
            if self.ablation == "ABLATED" :
                self.fruit_number = 0
    
    def compute_statut(self) :
        self.compute_flowering_date(self.phytomer.tree.productionSpeed)
        self.compute_harvest_date(self.phytomer.tree.productionSpeed)
        memstatut = self.statut
      
        self.statut = self.compute_stade(self.TT_corrige, self.phytomer.rank, self.estimated_flowering_date, self.estimated_harvest_date )
        self.pot_statut = self.compute_stade(self.thermalTimeSinceAppearance, self.phytomer.rank, self.estimated_flowering_date, self.estimated_harvest_date )
        
        if self.statut == "FLORAISON_RECOLTE" :
            if memstatut == "APPARITION_FLORAISON" : 
                self.flowering_day = self.phytomer.tree.simulation.meteo.getDay()
                self.flowering_month = self.phytomer.tree.simulation.meteo.getMonth()
                self.flowering_year = self.phytomer.tree.simulation.meteo.getYear()
                self.phytomer.tree.write_floweringRecord(self)
                
        if self.statut == "RECOLTE" :
            
            if memstatut == "FLORAISON_RECOLTE" : 
                #print 'OK'
                self.harvest_day = self.phytomer.tree.simulation.meteo.getDay()
                self.harvest_month = self.phytomer.tree.simulation.meteo.getMonth()
                self.harvest_year = self.phytomer.tree.simulation.meteo.getYear()
               # print self.phytomer.tree.simulation.meteo.getDay(),self.phytomer.tree.simulation.meteo.getMonth(), self.phytomer.tree.simulation.meteo.getYear(),  self.phytomer.name 
                #if self.sexe == "FEMELLE" :
                #    if self.avort == "NON_AVORTE" :
                #        if self.ablation == "NON_ABLATED" :
                self.phytomer.tree.writeRecord(self)
          #              print 'tt_corrige', self.TT_corrige, 'biomass', self.biomass, 'tt_vrai',  self.thermalTimeSinceAppearance, 'pot_biomasse', self.potential_biomass
               # self.biomass = 0
                self.biomass = 0
                self.femelle_biomass = 0
                self.male_biomass = 0
                self.peduncule.biomass = 0
                self.fruit.oil_biomass = 0
                self.fruit.nonoil_biomass = 0 
                self.respirable_biomass = 0
            #self.potential_biomass = 0
                self.test_biomass = 0
        
    def ablation_decision(self,pourc_abl) :
        t=random()
        if (t < 1 - pourc_abl):
            y = "NON_ABLATED"
        else :
            y ="ABLATED"
        return y 
    
    
    def compute_stade(self, TT, rank, flowering_date, harvest_date)   :
        
        if rank < 0 :
            statut =  "INITIE"
        else :
            if (TT > flowering_date)  : # on calcule une moyenne entre la dtae ? l'apparition et la date au jour j
                if (TT > harvest_date)  :
                        statut = "RECOLTE"
                else :
                        statut = "FLORAISON_RECOLTE"
            else :     
                statut =  "APPARITION_FLORAISON"
                
        return statut    
        
        
                
                
    def compute_flowering_date(self, vitesse_prod) :
        #A = GlobalVariables.MINIMAL_PRODUCTION_SPEED / vitesse_prod * (GlobalVariables.TT_FLOWERING_ADULTE)
        #self.estimated_flowering_date = (A + self.absolute_flowering_date) / 2
        self.estimated_flowering_date = ((GlobalVariables.MINIMAL_PRODUCTION_SPEED / vitesse_prod)**GlobalVariables.RATIO_DUREE_JEUNES_FLOWERING) * (GlobalVariables.TT_FLOWERING_ADULTE)
        
    def compute_harvest_date(self, vitesse_prod) :
        #A =  (GlobalVariables.MINIMAL_PRODUCTION_SPEED / vitesse_prod) * (GlobalVariables.TT_HARVEST_ADULTE)
        #self.estimated_harvest_date = (A + self.absolute_harvest_date) / 2
        self.estimated_harvest_date = ((GlobalVariables.MINIMAL_PRODUCTION_SPEED / vitesse_prod) ** GlobalVariables.RATIO_DUREE_JEUNES_HARVEST) * (GlobalVariables.TT_HARVEST_ADULTE)
        
    def compute_ini_flowering_date(self) :
        self.ini_flowering_date = ((GlobalVariables.MINIMAL_PRODUCTION_SPEED / GlobalVariables.INITIAL_PRODUCTION_SPEED)**GlobalVariables.RATIO_DUREE_JEUNES_FLOWERING)  * (GlobalVariables.TT_FLOWERING_ADULTE)
    
    def compute_ini_harvest_date(self) :
        self.ini_harvest_date = ((GlobalVariables.MINIMAL_PRODUCTION_SPEED / GlobalVariables.INITIAL_PRODUCTION_SPEED) ** GlobalVariables.RATIO_DUREE_JEUNES_HARVEST) * (GlobalVariables.TT_HARVEST_ADULTE)
        
    def sexe_decision(self, proba) :
        t=random()
        if (t < proba) :
            y = "FEMELLE"
        else :
            y = "MALE"
        return(y)
    
    def abortion_decision(self , proba) :        
        t=random()
        if (t < proba) :
            y = "AVORTE"
        else:
            y = "NON_AVORTE"
        return(y)

    def compute_masse_pot_reg(self, NbJoursSinceBeginning) :
        
        if (GlobalVariables.INCREASE_OF_BUNCH_MASS * NbJoursSinceBeginning + GlobalVariables.MASSE_REGIME_INI) > GlobalVariables.MASSE_REGIMES_ADULTE :
            self.masse_max_regime= GlobalVariables.MASSE_REGIMES_ADULTE
            
        else:
            self.masse_max_regime = ( GlobalVariables.INCREASE_OF_BUNCH_MASS * NbJoursSinceBeginning  + GlobalVariables.MASSE_REGIME_INI ) 
            
        
        if  self.phytomer.rank < GlobalVariables.IC_spikelet_RANG_FIN :
            self.masse_pot_regime = self.masse_max_regime
        else :
            if self.thermalTimeSinceAppearance < self.estimated_flowering_date :
                self.masse_pot_regime = self.masse_max_regime * self.IC_spikelet**GlobalVariables.SENSIVITY_IC_SPIKELET 
            else :
                self.masse_pot_regime = self.masse_max_regime * self.IC_spikelet**GlobalVariables.SENSIVITY_IC_SPIKELET * self.IC_setting ** GlobalVariables.SENSIVITY_IC_SETTING 
     
    def compute_red_vitesse_FTSW(self) :
        if self.statut == "APPARITION_FLORAISON" : 
            self.red_vitesse_FTSW = (self.phytomer.tree.BH.FTSW + 1 - GlobalVariables.SEUIL_MEDIAN_FTSW )**( GlobalVariables.SENS_FTSW )
        else :
            self.red_vitesse_FTSW = 1
        
     
     
        
    
    