#!/usr/bin/env python

from Leaf import Leaf
from Bunch import Bunch
from Internode import Internode
from GlobalVariables import GlobalVariables


class Phytomer(object):
    
    def __init__(self, phytomerNumber,  rank, state, treeInstance, day, month, year, TT, initiationday, initiationmonth, initiationyear, initiationTT):
        #self.name = "Phytomer_" + repr(phytomerNumber)
        self.name = phytomerNumber
        
        self.rank = rank
        self.state = state
        self.tree = treeInstance
        self.appearanceDay = day
        self.appearanceMonth = month
        self.appearanceYear = year
        self.appearanceTT = TT
        self.initiationDay= initiationday
        self.initiationMonth = initiationmonth
        self.initiationYear = initiationyear
        self.initiationTT = initiationTT
        self.deathday = 0
        self.deathmonth = 0
        self.deathyear = 0
        self.deathTT = 0 
        self.leaf = Leaf(self)
        self.bunch = Bunch(self)
        self.internode = Internode(self)
        self.demand = 0
        self.step_apparition = self.tree.simulation.step
        
        
    def getName(self):
        return self.name
        
    def getLeaf(self):
        return self.leaf
  
    def getBunch(self):
        return self.bunch
    
    def getRank(self) :
        return self.rank
    
    def getAppearanceDay(self):
        return self.appearanceDay
    
    def getAppearanceMonth(self):
        return self.appearanceMonth
    
    def getAppearanceYear(self):
        return self.appearanceYear
    
    def getInternode(self):
        return self.internode
    
    def getTree(self):
        return self.tree

    
    def growth_demand(self, TEff):
        #organs = self.ActiveOrgans()
        #for o in organs :
        #    o.growth_demand(TEff)
            
        self.leaf.growth_demand(TEff)
        self.internode.growth_demand(TEff)
        if self.bunch.statut != 'RECOLTE' : 
            self.bunch.growth_demand(TEff)
  
       # print self.tree.plus_vieille_date
        #print self.leaf.demand, self.demand
        
    def growth(self) :
        #organs = self.ActiveOrgans()
        #for o in organs :
        #    o.growth()
        self.leaf.growth()
        self.internode.growth()
        if self.bunch.statut != 'RECOLTE' : 
            self.bunch.growth()
        
    def ActiveOrgans(self) :     
        if self.state == 'DEAD' :
            organs_active = [self.internode]
        if self.state == 'INACTIVE' :
            organs_active = [self.leaf, self.bunch]
        if self.state == 'ACTIVE' :
            organs_active = [self.leaf, self.bunch, self.internode]
        return(organs_active)
        
    def compute_facteur_age(self) :
              
        if (GlobalVariables.INCREASE_TAILLE_REGIMES * (self.tree.simulation.step + self.step_apparition  - self.tree.date_plus_jeune_feuille) + GlobalVariables.FACTEUR_AGE_INI) > 1 :
            self.bunch.facteur_age_regimes = 1
        else:
            self.bunch.facteur_age_regimes = (GlobalVariables.INCREASE_TAILLE_REGIMES * (self.tree.simulation.step + self.step_apparition - self.tree.date_plus_jeune_feuille) + GlobalVariables.FACTEUR_AGE_INI)
           
        self.bunch.pot_fruits_number = self.bunch.facteur_age_regimes * GlobalVariables.MEAN_FRUIT_NUMBER_ADULTE  
          
  