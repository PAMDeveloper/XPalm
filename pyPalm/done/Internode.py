#!/usr/bin/env python

from Organ import Organ
from GlobalVariables import GlobalVariables
from math import *

class Internode(Organ):
    
    def __init__(self, phytomerInstance):
        Organ.__init__(self, phytomerInstance)
        self.age = 0
        self.length_max_increase = 0 
        self.demand = 0 
        self.max_length = 0
        self.length = 0
        self.biomass =  0
        self.assimilate_supply = 0
        self.duree_allongement = 0 #1 / self.phytomer.tree.productionSpeed
        self.TT_corrige = 0
        self.gain_TEff_jour = 0 
        
      
        
    def growth_demand(self,TEff):
        self.gain_TEff_jour = TEff
        self.age += 1
        self.duree_allongement = 1 / self.phytomer.tree.productionSpeed
        if self.TT_corrige < self.duree_allongement :
            self.max_length = self.compute_final_length(self.phytomer.tree.simulation.step)
        self.compute_length_max_increase(TEff)
        volume = pi * ( GlobalVariables.STEM_RAYON )**2 * self.length_max_increase      
        self.demand =  GlobalVariables.STEM_APPARENT_DENSITY * volume * GlobalVariables.COUT_RESPI_INTERNODE * 1000
        
    def growth(self) :
        self.compute_assimilate_supply()
        self.compute_length()
        self.compute_biomass()
        self.compute_TT_new()
    
    def compute_assimilate_supply(self) :
        self.assimilate_supply = self.demand   * (self.phytomer.tree.fr_reste)
      
    def compute_biomass(self) :
        self.biomass += self.assimilate_supply * (1 / GlobalVariables.COUT_RESPI_INTERNODE)
        
    def compute_length(self) :
        self.length += self.length_max_increase  * (self.phytomer.tree.fr_reste)
        
    def getlength(self) :
        return self.length
    
    def getbiomass(self) :
        return self.biomass
    
    def getassimilate_supply(self) :
        return self.assimilate_supply
       
    def compute_length_max_increase(self,TEff) :
        if self.TT_corrige < self.duree_allongement :
            self.length_max_increase = self.max_length * TEff * 1 / self.duree_allongement
        else :
            self.length_max_increase = 0
        
        
        #if self.phytomer.rank == 1 :
        #    self.length_max_increase = self.max_length * TEff * self.phytomer.tree.productionSpeed
        #else :
        #    self.length_max_increase = 0
        #
        
    #def duree_allongement(self, vitesse_prod):
    #    return 1 / vitesse_prod
    #
    
    def compute_final_length(self,nbOfDaySinceBeginning) :
        #y = 0
        
        age_actuel = GlobalVariables.AGE + float(nbOfDaySinceBeginning)/365
        if  age_actuel < GlobalVariables.DEBUT_CROISSANCE_EN :
            y = float(GlobalVariables.EN_LENGHT_INI)
            
        else :
            if age_actuel < GlobalVariables.FIN_CROISSANCE_EN :
                y = (GlobalVariables.LENGHT_ADULTE - GlobalVariables.EN_LENGHT_INI) / (GlobalVariables.FIN_CROISSANCE_EN - GlobalVariables.DEBUT_CROISSANCE_EN) * (age_actuel - GlobalVariables.DEBUT_CROISSANCE_EN) +  GlobalVariables.EN_LENGHT_INI
                
            else :
                y = float(GlobalVariables.LENGHT_ADULTE)
                
        return y / 100      
   #     return (GlobalVariables.b_EN * GlobalVariables.c_EN * (GlobalVariables.AGE + nbOfDaySinceBeginning/365)**(-GlobalVariables.k_EN-1)*exp(-GlobalVariables.b_EN*(GlobalVariables.AGE + nbOfDaySinceBeginning/365)**(-GlobalVariables.k_EN))) / ( self.phytomer.tree.productionSpeed * 10 * 365 )
   
    def get_length_mas_increase(self) :
        return self.length_max_increase
    
    def compute_TT_new(self) :
        self.TT_corrige +=  ( ( self.phytomer.tree.fr_reste ) ** GlobalVariables.PLASTICITY_INTERNODE_IC ) * self.gain_TEff_jour
    
    #def getlength_increase(self) :
    #    return self.length_increase
    
    #def computelength(self) :
    #    
    #    self.length =+ self.getlength_increase()
            #print 'length= ', self.length