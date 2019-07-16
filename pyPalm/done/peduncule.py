#!/usr/bin/env python

from GlobalVariables import GlobalVariables

class Peduncule(object):
    
    def __init__(self, bunchinstance):
        self.bunch = bunchinstance
        #self.mass = 0
        self.demand = 0
        self.assimilate_supply = 0
        self.biomass = 0 
        self.potential_biomass = 0 
        self.demand_pot = 0
        self.test_biomass = 0
        self.potential_biomass =  0

    def growth_demand(self,Teff) :
        if self.bunch.ablation == "ABLATED" :
            self.demand = 0
            self.demand_pot = 0
        else : 
            self.demand = self.compute_demand(self.bunch.statut, self.bunch.TT_corrige, Teff)
            self.demand_pot = self.compute_demand(self.bunch.pot_statut, self.bunch.thermalTimeSinceAppearance, Teff)
    
    
    def compute_demand(self,statut, TT, Teff) :
        self.compute_potential_biomass()  
        if statut != "APPARITION_FLORAISON" :
            demand = 0
        else :
            if (TT > self.bunch.estimated_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.getproductionSpeed()) :
                demand = self.bunch.facteur_age_regimes * GlobalVariables.COUT_STRUCTURE_REGIME * self.potential_biomass / (GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.getproductionSpeed()) * Teff
            else :
                demand = 0
        return demand        

    def compute_potential_biomass(self):
        self.potential_biomass = self.bunch.facteur_age_regimes * (self.bunch.IC_spikelet ** GlobalVariables.SENSIVITY_IC_SPIKELET) * GlobalVariables.MASSE_MEAN_PEDUNCULE_ADULTE
        
        
    def growth(self) :
        self.compute_assimilate_supply()
        self.compute_biomass()
        
    def compute_assimilate_supply(self) :
        self.assimilate_supply = self.demand   * (self.bunch.phytomer.tree.fr_reste)
        
    def compute_biomass(self) :
        
        if self.bunch.statut == "RECOLTE" :
            self.biomass = 0
            #self.potential_biomass =  0
        else :
            self.biomass += self.assimilate_supply * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)      
        
        
        if self.bunch.ablation == "ABLATED" :
            self.biomass = 0 
        
           
        self.potential_biomass += self.demand_pot *(1 / GlobalVariables.COUT_STRUCTURE_REGIME)
        self.test_biomass += self.assimilate_supply * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)
            
    def compute_structure_ini(self) :
        if (self.bunch.thermalTimeSinceAppearance < self.bunch.ini_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.getproductionSpeed()) :
            self.biomass = 0
        else :
            if self.bunch.thermalTimeSinceAppearance > self.bunch.ini_harvest_date :
                self.biomass = 0
            else :
                if self.bunch.thermalTimeSinceAppearance > self.bunch.ini_flowering_date :
                    self.biomass = (self.bunch.facteur_age_regimes) * GlobalVariables.MASSE_MEAN_PEDUNCULE_ADULTE
                else :    
                    self.biomass = (self.bunch.facteur_age_regimes) * GlobalVariables.MASSE_MEAN_PEDUNCULE_ADULTE / ( GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE / GlobalVariables.INITIAL_PRODUCTION_SPEED  ) * ( self.bunch.thermalTimeSinceAppearance - ( self.bunch.ini_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE / GlobalVariables.INITIAL_PRODUCTION_SPEED ) )
        self.potential_biomass = self.biomass    