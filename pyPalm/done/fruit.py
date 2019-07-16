#!/usr/bin/env python


from GlobalVariables import GlobalVariables

class Fruit(object):

    def __init__(self, bunchinstance):
        self.bunch = bunchinstance
     #   self.mass = 0
        self.demand = 0
        self.nonoil_demand = 0
        self.oil_demand = 0
        self.absolue_duree_oleo = GlobalVariables.MINIMAL_PRODUCTION_SPEED / self.bunch.phytomer.tree.productionSpeed * (GlobalVariables.DUREE_OLEO)
        self.nonoil_biomass = 0
        self.oil_biomass = 0 
        self.oil_assimilate_supply = 0
        self.nonoil_assimilate_supply = 0
        self.potential_biomass = 0
        self.nonoil_potential_biomass = 0
        self.oil_potential_biomass = 0
        self.oil_demand_pot = 0
        self.nonoil_demand_pot = 0
        self.test_oil_biomass = 0
        self.test_nonoil_biomass = 0
        self.duree_oleo = 0 
        self.oil_statut = ""
        self.oil_pot_statut =""
        self.masse_ind_max = float(GlobalVariables.IND_FRUIT_WEIGHT) / 1000 
        self.masse_ind =  0
        self.ratio_huile_mesocarp = 0 
        
    def growth_demand(self,Teff) :
        self.compute_duree_oleo()
        self.oil_demand = self.compute_oil_demand(self.bunch.statut,Teff, self.bunch.TT_corrige)
        self.oil_demand_pot = self.compute_oil_demand(self.bunch.pot_statut,Teff, self.bunch.thermalTimeSinceAppearance)
        self.oil_statut = self.compute_oil_statut(self.bunch.statut,self.bunch.TT_corrige)
        
        
        self.nonoil_demand = self.compute_nonoil_demand(self.bunch.statut,Teff)
        self.nonoil_demand_pot = self.compute_nonoil_demand(self.bunch.pot_statut,Teff)
        self.demand = self.oil_demand + self.nonoil_demand
    
    def compute_duree_oleo(self) : 
        #A = GlobalVariables.DUREE_OLEO * (GlobalVariables.MINIMAL_PRODUCTION_SPEED / self.bunch.phytomer.tree.productionSpeed)
        # self.duree_oleo = (A + self.absolue_duree_oleo) / 2 
        self.duree_oleo = GlobalVariables.DUREE_OLEO * ((GlobalVariables.MINIMAL_PRODUCTION_SPEED / self.bunch.phytomer.tree.productionSpeed) ** (GlobalVariables.RATIO_DUREE_JEUNES_OLEO))
        
    def compute_oil_demand(self, statut,Teff, TT) :     
        
        if self.bunch.ablation == "ABLATED" :
            demand = 0
        
        else :
            if statut != "FLORAISON_RECOLTE" :
                demand = 0
            else :
                if TT > self.bunch.estimated_harvest_date - self.duree_oleo :
                    demand = self.bunch.fruit_number *  GlobalVariables.OIL_CONTENT * GlobalVariables.COUT_OIL * self.masse_ind_max / (self.duree_oleo) * Teff
                    
                else :
                    demand = 0 
        return demand
    
    def compute_oil_statut(self,statut,TT) :
        if self.bunch.ablation == "ABLATED" :
            y = "PAS_DE_FRUITS"
        else :
            if statut == "APPARITION_FLORAISON" :
                y = "PAS_DE_FRUITS"
            else :
                if statut == "RECOLTE" :
                    y= "RECOLTE"
                else :
                    if TT > self.bunch.estimated_harvest_date - self.duree_oleo :
                        y="OLEOSYNTHESE"
                    else :
                        y="AVANT_OLEOSYNTHESE"
        return y 
    
    def compute_nonoil_demand(self, statut, Teff) :     
        
        if self.bunch.ablation == "ABLATED" :
            demand = 0
        else :
        
            if statut != "FLORAISON_RECOLTE" :
                demand = 0
            else :
                demand = self.bunch.fruit_number * (1 - GlobalVariables.OIL_CONTENT) * GlobalVariables.COUT_STRUCTURE_REGIME * self.masse_ind_max / (self.bunch.estimated_harvest_date  - self.bunch.estimated_flowering_date) * Teff
        return demand    
            
            
    def growth(self) :
        self.oil_compute_assimilate_supply()
        self.nonoil_compute_assimilate_supply()
        self.oil_compute_biomass()
        self.nonoil_compute_biomass()
        self.compute_ind_mass()
        
        
    def compute_ind_mass(self) :
        if self.bunch.fruit_number != 0 :  
            self.masse_ind  = ((self.oil_biomass + self.nonoil_biomass) / self.bunch.fruit_number) * 1000    
        else :
           self.masse_ind  = 0
        
        if self.oil_biomass == 0 :    
            self.ratio_huile_mesocarp =   0
        else :
            self.ratio_huile_mesocarp = self.oil_biomass / ( self.nonoil_biomass + self.oil_biomass)
           
    
    def oil_compute_assimilate_supply(self) :
        self.oil_assimilate_supply = self.oil_demand   * (self.bunch.phytomer.tree.fr_fruits)
        
    def oil_compute_biomass(self) :
        
        if self.bunch.statut == "RECOLTE" :
            self.oil_biomass = 0
            self.nonoil_biomass = 0 
             
            #self.oil_potential_biomass = 0
        else :
            self.oil_biomass += self.oil_assimilate_supply * (1 / GlobalVariables.COUT_OIL)      
        
        
        if self.bunch.ablation == "ABLATED" :
            self.oil_biomass = 0
            
            
        
        self.test_oil_biomass +=  self.oil_assimilate_supply * (1 / GlobalVariables.COUT_OIL)
            
        #if self.bunch.pot_statut != "RECOLTE" :
        self.oil_potential_biomass += self.oil_demand_pot * (1/GlobalVariables.COUT_OIL)
            
            
    def nonoil_compute_assimilate_supply(self) :
        self.nonoil_assimilate_supply = self.nonoil_demand_pot  * (self.bunch.phytomer.tree.fr_fruits)
        
        
    def nonoil_compute_biomass(self) :
        
        if self.bunch.statut == "RECOLTE" :
            self.nonoil_biomass = 0
            #self.nonoil_potential_biomass = 0
        else :
            self.nonoil_biomass += self.nonoil_assimilate_supply * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)
        
        
        
        if self.bunch.ablation == "ABLATED" :
            self.nonoil_biomass = 0
        
            
        self.nonoil_potential_biomass += self.nonoil_demand_pot * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)
        self.test_nonoil_biomass +=  self.nonoil_assimilate_supply * (1 / GlobalVariables.COUT_STRUCTURE_REGIME)
            
    def compute_structure_ini(self) :
        self.duree_oleo = GlobalVariables.DUREE_OLEO  * ((GlobalVariables.MINIMAL_PRODUCTION_SPEED / GlobalVariables.INITIAL_PRODUCTION_SPEED) ** (GlobalVariables.RATIO_DUREE_JEUNES_OLEO))
        if self.bunch.thermalTimeSinceAppearance < self.bunch.ini_flowering_date :
            self.oil_biomass = 0
            self.nonoil_biomass = 0
        else :
            if self.bunch.thermalTimeSinceAppearance > self.bunch.ini_harvest_date :
                self.oil_biomass = 0
                self.nonoil_biomass = 0
            else :
                self.nonoil_biomass = ( self.bunch.facteur_age_regimes ) * ( 1 - GlobalVariables.OIL_CONTENT ) * GlobalVariables.MEAN_FRUIT_NUMBER_ADULTE * (GlobalVariables.IND_FRUIT_WEIGHT/1000) / (self.bunch.ini_harvest_date - self.bunch.ini_flowering_date ) * (self.bunch.thermalTimeSinceAppearance - self.bunch.ini_flowering_date)
                if self.bunch.thermalTimeSinceAppearance > self.bunch.ini_harvest_date - self.duree_oleo  : 
                    masse_ini =  ( self.bunch.facteur_age_regimes ) *  GlobalVariables.MEAN_FRUIT_NUMBER_ADULTE * (GlobalVariables.IND_FRUIT_WEIGHT/1000) * GlobalVariables.OIL_CONTENT 
                    tps_passe_en_oleo =   self.bunch.thermalTimeSinceAppearance - (self.bunch.ini_harvest_date - self.duree_oleo) 
                    self.oil_biomass =  masse_ini / self.duree_oleo * tps_passe_en_oleo
                    #print 'on est la', self.oil_biomass, self.bunch.phytomer.rank
        self.oil_potential_biomass = self.oil_biomass
        self.nonoil_potential_biomass = self.nonoil_biomass
        self.compute_ind_mass()