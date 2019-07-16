#!/usr/bin/env python

from GlobalVariables import GlobalVariables

class Reserve(object):
    
    def __init__(self, treeInstance) :
        self.tree = treeInstance
        self.biomass = GlobalVariables.POURCENT_NSC_ST_INI * self.tree.trunk_biomass + self.tree.leaf_non_structural_biomass
        
        self.trunk_biomass = GlobalVariables.POURCENT_NSC_ST_INI * self.tree.trunk_biomass
        
    
        self.leaf_reserve_max = ((GlobalVariables.SLW_max - GlobalVariables.SLW_min) * 10 ) * self.tree.totalLeafArea / GlobalVariables.POURC_FOLIOLE
        self.leaf_reserve_min = 0 
        self.trunk_reserve_min = GlobalVariables.POURCENT_NSC_ST_MIN * self.tree.trunk_biomass
        self.trunk_reserve_max = GlobalVariables.POURCENT_NSC_ST_MAX * self.tree.trunk_biomass
        self.reserve_min = self.trunk_reserve_min
        self.reserve_max = self.leaf_reserve_max + self.trunk_reserve_max        
        self.apport = 0
        self.actual_mob_rate = 0
        self.leaf_apport = 0 
        self.trunk_apport = 0 
        self.attrac_trunk =  0
        self.attrac_leaf = 0
        self.delta_biomasse_reserve_stipe = 0 
        self.delta_biomass_tot = 0
        
    def compute_etat_reserve(self) :
        self.delta_biomasse_reserve_leaf = 0
        self.delta_biomasse_reserve_stipe = 0
        self.delta_biomass_tot = 0
        self.compute_reserve_max_min()
        self.compute_etat_test_min()
        self.compute_etat_apres_respiration()
        self.compute_actual_mob_rate()
        self.compute_etat_reserve_apres_croissance()
       
        
    def compute_reserve_max_min(self) :
        
        self.trunk_reserve_min = GlobalVariables.POURCENT_NSC_ST_MIN * self.tree.trunk_biomass
        self.trunk_reserve_max = GlobalVariables.POURCENT_NSC_ST_MAX * self.tree.trunk_biomass
        self.leaf_reserve_min = 0
        self.leaf_reserve_max = self.tree.totalLeafArea * ((GlobalVariables.SLW_max - GlobalVariables.SLW_min) * 10 ) / GlobalVariables.POURC_FOLIOLE
        self.reserve_max = self.leaf_reserve_max + self.trunk_reserve_max
        
   
    
    def compute_etat_test_min(self) :
  # avant tout il faut verifier qu'on est au dessus de la reserve mini. Ce cas-la peut arriver car la reserve mini augmente et qu'on a jamais rien stocke dedans. Il existe donc cette demande ... Mais dans le cas d'un deficit prolonge en assimilats d'ou peuvent bien venir ces assimilats ? 
  #  # pour l'instant pour eviter des bugs si il n'y pas ces assimilats on les fait arriver ...
        
        self.biomass = self.tree.leaf_non_structural_biomass + self.trunk_biomass  # on est oblige de prendre en compte sa taille pour pouvoir prendre en compte la chute des feuilles
        if self.biomass <   self.reserve_min :
            if self.biomass + GlobalVariables.COUT_RESERVE > self.reserve_min :
                self.tree.offre_pour_respi = self.tree.Assim - (self.reserve_min - self.biomass)/GlobalVariables.COUT_RESERVE
            else :
                self.tree.offre_pour_respi = 0
            self.biomass = self.reserve_min
            
            self.trunk_biomass = self.trunk_reserve_min
            self.delta_biomasse_reserve_leaf += - (self.tree.leaf_non_structural_biomass - self.leaf_reserve_min)
            
        else :
            self.tree.offre_pour_respi = self.tree.Assim
        #print 'delta_biomasse_str_1' , self.delta_biomasse_reserve_leaf
 
    def compute_etat_apres_respiration(self) :
         
        self.compute_capacite_de_mobilisation_biomasse_feuille_stipe()
        
        self.tree.delta_av_respi = self.tree.offre_pour_respi - self.tree.respi_maintenance
        if self.tree.delta_av_respi < 0 :
            
            if GlobalVariables.REALL_COST * (self.biomass - self.reserve_min) + self.tree.delta_av_respi < 0 :
                self.apport =  GlobalVariables.REALL_COST * (self.biomass - self.reserve_min)
            else :
                self.apport = - self.tree.delta_av_respi
            self.tree.offre_pour_croissance = 0 
        else :
            self.apport = 0
            self.tree.offre_pour_croissance = self.tree.delta_av_respi
        self.biomass +=  - self.apport / GlobalVariables.REALL_COST
        self.trunk_biomass += - self.apport / GlobalVariables.REALL_COST * self.mob_trunk
       
        self.tree.delta_avant_croissance = self.tree.offre_pour_croissance - (self.tree.growth_demand)
        self.tree.fr_avant_croissance = self.tree.offre_pour_croissance/ (self.tree.growth_demand)
        self.delta_biomasse_reserve_leaf += - self.apport / GlobalVariables.REALL_COST * self.mob_leaf
        
        #print 'reserve_max_leaves', self.leaf_reserve_max, 'non_structural_biomass', self.tree.leaf_non_structural_biomass, 'reserve_stipe_max', self.trunk_reserve_max , 'reserve_stipe_min', self.trunk_reserve_min,  'stipe_reserve', self.trunk_biomass
        #print 'total_reserve', self.biomass, 'max_tot_reserve', self.reserve_max 
        #print 'delta_biomasse_str_2' , self.delta_biomasse_reserve_leaf, 'delta_biomasse_stipe', - self.apport / GlobalVariables.REALL_COST * self.mob_trunk
        
    def compute_actual_mob_rate(self) :
        self.actual_mob_rate = GlobalVariables.MOB_RATE_MAX / (self.reserve_max - self.reserve_min) * ( self.biomass - self.reserve_min )
  
  
    def compute_etat_reserve_apres_croissance(self) :
        
        #print 'assim', self.tree.Assim, 'demande_croissance', self.tree.growth_demand, 'demande_respi', self.tree.respi_maintenance
        #print 'manque_pour_la_croissance', self.tree.delta_avant_croissance, 
        if self.tree.fr_avant_croissance < 1 :
            self.compute_capacite_de_mobilisation_biomasse_feuille_stipe()
            #print 'mob_rate', self.actual_mob_rate, 'max_mob_rate',  GlobalVariables.MOB_RATE_MAX, 'mob_rate_trunk', self.mob_trunk, 'mob_rate_leaf', self.mob_leaf
            if (GlobalVariables.REALL_COST * self.actual_mob_rate * self.biomass ) > - self.tree.delta_avant_croissance :
                self.apport =  - self.tree.delta_avant_croissance
            else :
                self.apport = GlobalVariables.REALL_COST * self.actual_mob_rate * self.biomass
            self.biomass +=  - self.apport / GlobalVariables.REALL_COST
            self.trunk_biomass += - self.apport / GlobalVariables.REALL_COST * self.mob_trunk
            
            self.delta_biomass_tot += - self.apport / GlobalVariables.REALL_COST
            self.delta_biomasse_reserve_leaf += - self.apport / GlobalVariables.REALL_COST * self.mob_leaf
            self.delta_biomasse_reserve_stipe += - self.apport / GlobalVariables.REALL_COST * self.mob_trunk
               
        else :
            self.compute_capacite_d_attraction_biomasse_feuille_stipe()
            #print 'attrac_rate_trunk', self.attrac_trunk, 'attrac_rate_leaf', self.attrac_leaf
            if ((self.biomass + GlobalVariables.COUT_RESERVE * self.tree.delta_avant_croissance ) < self.reserve_max) :
               
                self.trunk_biomass += GlobalVariables.COUT_RESERVE * self.attrac_trunk *  self.tree.delta_avant_croissance
                
                self.biomass += GlobalVariables.COUT_RESERVE * self.tree.delta_avant_croissance
                self.delta_biomass_tot += GlobalVariables.COUT_RESERVE * self.tree.delta_avant_croissance
                self.delta_biomasse_reserve_leaf += GlobalVariables.COUT_RESERVE * self.attrac_leaf *  self.tree.delta_avant_croissance
                self.delta_biomasse_reserve_stipe += GlobalVariables.COUT_RESERVE * self.attrac_trunk *  self.tree.delta_avant_croissance
                #print 'CAS 1-1'
            else :
                
                self.delta_biomass_tot = self.reserve_max - self.biomass
                self.biomass = self.reserve_max
                #print 'trunk-reserve_max', self.trunk_reserve_max, 'trunk-reserve', self.trunk_biomass, 'leaf_reserve_max', self.leaf_reserve_max, 'leaf-reserve', self.tree.leaf_non_structural_biomass
                self.delta_biomasse_reserve_stipe += self.trunk_reserve_max - self.trunk_biomass
                self.trunk_biomass = self.trunk_reserve_max
               
                self.delta_biomasse_reserve_leaf += self.leaf_reserve_max - self.tree.leaf_non_structural_biomass
                #print 'CAS 1-2'
                
            self.apport = - self.tree.delta_avant_croissance  # dans ce cas la l apport est neg
            
        #print 'delta_biomasse_tot', self.delta_biomass_tot , 'delta_biomasse_str_3' , self.delta_biomasse_reserve_leaf, 'delta_biomasse_stipe', self.delta_biomasse_reserve_stipe
        

        
    def compute_capacite_d_attraction_biomasse_feuille_stipe(self) :
        self.attrac_trunk = (self.trunk_reserve_max - self.trunk_biomass) /( (self.leaf_reserve_max - self.tree.leaf_non_structural_biomass ) + (self.trunk_reserve_max - self.trunk_biomass))
        self.attrac_leaf = (self.leaf_reserve_max - self.tree.leaf_non_structural_biomass ) /( (self.leaf_reserve_max - self.tree.leaf_non_structural_biomass ) + (self.trunk_reserve_max - self.trunk_biomass))
    
    def compute_capacite_de_mobilisation_biomasse_feuille_stipe(self) : 
        if ((self.trunk_biomass - self.trunk_reserve_min) + (self.tree.leaf_non_structural_biomass - self.leaf_reserve_min)) != 0 :
            self.mob_trunk = (self.trunk_biomass - self.trunk_reserve_min) / ((self.trunk_biomass - self.trunk_reserve_min) + (self.tree.leaf_non_structural_biomass - self.leaf_reserve_min))
            self.mob_leaf = (self.tree.leaf_non_structural_biomass - self.leaf_reserve_min) / ((self.trunk_biomass - self.trunk_reserve_min) + (self.tree.leaf_non_structural_biomass - self.leaf_reserve_min))
        else : 
            self.mob_trunk = 0 
            self.mob_leaf = 0 
  