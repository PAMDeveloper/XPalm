#!/usr/bin/env python


from GlobalVariables import GlobalVariables


class Male(object):
    
    def __init__(self, bunchinstance):
        self.bunch = bunchinstance
        self.biomass = 0
        self.demand = 0
        self.assimilate_supply = 0
        self.potential_biomass = 0 
        
  
    def growth_demand(self, Teff):
        if self.bunch.statut != "APPARITION_FLORAISON" :
            self.demand=0
        else :
            if (self.bunch.thermalTimeSinceAppearance > self.bunch.estimated_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.productionSpeed) :
                #print((self.bunch.thermalTimeSinceAppearance > self.bunch.estimated_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.getproductionSpeed()))
                self.demand = GlobalVariables.MASSE_INFLO_MALE_ADULTE * self.bunch.facteur_age_regimes / (GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.productionSpeed) * Teff
            else :
                self.demand = 0

    def growth(self) :
        self.compute_assimilate_supply()
        self.compute_biomass()
        
    def compute_assimilate_supply(self) :
        self.assimilate_supply = self.demand   * (self.bunch.phytomer.tree.fr_reste)
        
    def compute_biomass(self) :
        if self.bunch.statut == "FLORAISON_RECOLTE" :
            self.biomass = 0
            self.potential_biomass = 0 
        else :
            self.biomass += self.assimilate_supply * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)
            self.potential_biomass += self.demand * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)
            
            
    def compute_structure_ini(self) :
        if (self.bunch.thermalTimeSinceAppearance < self.bunch.ini_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE/self.bunch.phytomer.tree.productionSpeed) :
            self.biomass = 0
        else :
            if self.bunch.thermalTimeSinceAppearance > self.bunch.ini_flowering_date :
                self.biomass = 0
            else :
                if self.bunch.thermalTimeSinceAppearance > self.bunch.ini_flowering_date :
                    self.biomass = GlobalVariables.MASSE_INFLO_MALE_ADULTE * self.bunch.facteur_age_regimes
                else :    
                    self.biomass = GlobalVariables.MASSE_INFLO_MALE_ADULTE * self.bunch.facteur_age_regimes / ( GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE / GlobalVariables.INITIAL_PRODUCTION_SPEED  ) * ( self.bunch.thermalTimeSinceAppearance - ( self.bunch.ini_flowering_date - GlobalVariables.RANG_DEBUT_CROISSANCE_PEDUNCULE / GlobalVariables.INITIAL_PRODUCTION_SPEED ) )
        self.potential_biomass= self.biomass    
        