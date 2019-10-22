#!/usr/bin/env python

from Organ import Organ
from math import *
from GlobalVariables import GlobalVariables
#from Phytomer import Phytomer
from random import *

class Leaf(Organ):
    
    def __init__(self, phytomerInstance):
        Organ.__init__(self, phytomerInstance)
        self.leafArea = 0
        self.vitesse_exp = 0
        self.SFMax = 0
        self.demand = 1
        self.pot_increase_leafArea = 0
        self.structural_biomass = 0
        self.non_structural_biomass = 0
        self.total_biomass = 0
        self.capacite_reserve_max = 0
        self.attraction_biomasse = 0
        self.capacite_relagarge_biomasse = 0
        self.fraction_non_str_biomasse_allouee = 0 
        self.assimilate_supply = 0
        self.potLeafArea = 0
        self.biomasse_structurale_allouee = 0 
        self.SLW = 0
        self.gain_TEff_jour = 0
        self.TT_corrige = 0 
        self.increase_potleafArea = 0
        self.crown_position = 0
        self.irradiance_factor = 0
        self.niveau_d_eclairement_loi_beta = 0 
        self.ablation = "NON_COUPE"
        self.light_pourc_interception = 0
        self.assim_max = 0
        self.nb_jour_declin_photosynthese = 0 
        self.declin_photosynthese_age  = 0  
        self.declin_photosynthese_stress = 0
        self.biomasse_prod = 0
        self.rayonnement_intercepte_feuille = 0 
        
    def growth_demand(self, TEff):
        #self.TT_jour_precedent = self.TT_corrige
        self.compute_coupe_feuille_recolte()
        correctedTEff = self.computeTEffCorrection(TEff)
        self.gain_TEff_jour = correctedTEff
        Organ.growth_demand(self, correctedTEff)
        self.compute_pot_inc_LeafArea(correctedTEff)
        
          
        #if self.phytomer.rank != 1 :
         
        if self.ablation == "COUPE" :
            self.demand = 0
        
        if self.leafArea == 0 :
            if self.phytomer.rank != 1 :
                self.demand = 0
            else :
                self.demand = self.pot_increase_leafArea*(GlobalVariables.SLW_min * 10 * GlobalVariables.COUT_RESPI_FEUILLE ) / GlobalVariables.POURC_FOLIOLE 
        else :
            self.demand = self.pot_increase_leafArea*(GlobalVariables.SLW_min * 10 * GlobalVariables.COUT_RESPI_FEUILLE ) / GlobalVariables.POURC_FOLIOLE 
        #print 'day =',self.phytomer.tree.simulation.meteo.getDay(),'month =',self.phytomer.tree.simulation.meteo.getMonth(),'year=',self.phytomer.tree.simulation.meteo.getYear(),'rank=',self.phytomer.rank, 'pot_increase', self.pot_increase_leafAreaself.pot_increase_leafArea, 'demand',        
    
    def compute_declin_photo(self) :
        
        if (self.phytomer.rank >  GlobalVariables.RANG_DEBUT_SENS_PN_LONG_TERMES) and  (self.phytomer.rank < GlobalVariables.RANG_FIN_SENS_PN_LONG_TERMES):
            if self.phytomer.tree.BH.FTSW < GlobalVariables.seuil_FTSW_pour_red_a_long_termes_de_Pn : 
                 
                self.nb_jour_declin_photosynthese += 1 - 1/GlobalVariables.seuil_FTSW_pour_red_a_long_termes_de_Pn * self.phytomer.tree.BH.FTSW  ### nombre de jours en dessous de 0.4 module par l intensite du stress !
        ### on compte le nombre de jours ou il y a eu un stress fort endommageant la photosynthese
        
        self.declin_photosynthese_stress = (1 - self.fonction_impact_stress_fort_assim_pot(self.nb_jour_declin_photosynthese))**GlobalVariables.SENS_REMANESCENCE
        ### on calcule le declin de la photosynthese en fonction de ce nombre de jours        
        
        if self.phytomer.state == "ACTIVE" : ## ne sert rien de calculer si la feuille n est pas presente
            self.declin_photosynthese_age = self.fonction_impact_age_assim_pot(self.phytomer.rank, GlobalVariables.rang_chute_pn_age, GlobalVariables.rang_pn_50)
        ### on estime le declin de la pn en fonction de l age
        
        
        if self.phytomer.state == "DEAD" :
            self.assim_max = 0
        if self.ablation == "COUPE" :
            self.assim_max = 0
        if self.ablation == "NON_COUPE" :
            if self.phytomer.state == "ACTIVE" :
                self.assim_max = GlobalVariables.EFFICIENCE_BIOLOGIQUE * (self.declin_photosynthese_stress ) * self.declin_photosynthese_age
                
                # l assim max est l assimilation maximale permise par les jours de stress forts rencontres
                ### on multiplie ensuite celle-ci par sa quantite de lumiere absorbee pour avoir l assim de chacune des feuilles
                ### cette assim_max peut aussi etre modifiee par l age de la feuille
    
                
    def fonction_impact_stress_fort_assim_pot(self, nbjours) :
        if nbjours == 0 :
            y = 0
        else :
            y = 1/(1 + exp(-(nbjours - GlobalVariables.a_sigmoide_declin_pn )/(GlobalVariables.b_sigmoide_declin_pn )))
        
        return(y)
        
        ### attention cette fonction ne passe pas par 0 !!! 
        
    def fonction_impact_age_assim_pot(self, rang, rang_debut_chute, rang_moitie_chute) :
        if rang > GlobalVariables.rang_chute_pn_age :
            if (0.5 - 1) / (rang_moitie_chute - rang_debut_chute) *(rang - rang_debut_chute ) + 1 < 0 : 
                y = 0
            else :     
                y = (0.5 - 1) / (rang_moitie_chute - rang_debut_chute) *(rang - rang_debut_chute ) + 1
        else :
            y = 1
        
        return(y)
        
        ### Corley dit dans Dufresnes 1989 p.43 que la photosynthese max est constante jusqu au rang 20 et ensuite decroit jusqu au rang 30 pour
        ### atteindre 50 pourcent de la valeur au rang 35 environ quid des rangs en dessous
        ### je fais une regression lineaire du coup que je continue par la suite.
    
    def compute_biomasse_prod(self, pourc_interception, assim_max, ei_tot, densite, Rg) :
        rayonnement_tot_intercepte_par_l_arbre =  ei_tot * Rg * 0.48 * 10000 / densite
        
        self.rayonnement_intercepte_feuille = pourc_interception * rayonnement_tot_intercepte_par_l_arbre
        
        #self.biomasse_prod = assim_max * self.rayonnement_intercepte_feuille / 1000  ### on passe de kg metre carre en g m? 
        self.biomasse_prod = assim_max * self.rayonnement_intercepte_feuille / 1000
        
    def compute_coupe_feuille_recolte(self) :
        if (self.phytomer.bunch.sexe == "FEMELLE") :
                if (self.phytomer.bunch.avort == "NON_AVORTE") :
                    if (self.phytomer.bunch.statut == "RECOLTE") :
                         self.ablation = "COUPE"
                         
    
    def defoliation_decision(self, pourc_abl):
        t=random()
        if (t < 1 - pourc_abl):
            y = "NON_COUPE"
        else :
            y ="COUPE"
        return y 
        
    
    
    def growth(self) :
        
        self.compute_assimilate_supply()
        self.compute_leafArea()
        self.compute_biomass()
        self.compute_capacite_reserve_max()
        self.compute_SLW()
        self.compute_TT_new()
    
    def compute_assimilate_supply(self) :
        self.assimilate_supply = self.demand * (self.phytomer.tree.fr_reste)
    
    def compute_leafArea(self) :      
        if self.phytomer.state == "ACTIVE" :
            self.leafArea += self.pot_increase_leafArea   * (self.phytomer.tree.fr_reste)  #(self.phytomer.getTree().IC)
            self.potLeafArea += self.increase_potleafArea
            
            if (self.ablation == "COUPE") :
                self.leafArea = 0
                self.potLeafArea = 0 
            
            #if (self.phytomer.bunch.sexe == "FEMELLE") :
            #    if (self.phytomer.bunch.avort == "NON_AVORTE") :
            #        if (self.phytomer.bunch.statut == "RECOLTE") :
            #             self.leafArea = 0
            #             self.potLeafArea = 0 
        
        else :
            #print 'toto', self.phytomer.state
            self.leafArea = 0
        #print 'day =',self.phytomer.tree.simulation.meteo.getDay(),'month =',self.phytomer.tree.simulation.meteo.getMonth(),'year=',self.phytomer.tree.simulation.meteo.getYear(),'rank=',self.phytomer.rank, 'TT', self.thermalTimeSinceAppearance, 'leaf_area',    self.leafArea     
       
    def compute_capacite_reserve_max(self) :
        self.capacite_reserve_max = (GlobalVariables.SLW_max - GlobalVariables.SLW_min) * 10 * self.leafArea / GlobalVariables.POURC_FOLIOLE
    
    def compute_TT_new(self) :
        self.TT_corrige +=  ( ( self.phytomer.tree.fr_reste ) ** GlobalVariables.PLASTICITY_LEAF_IC ) * self.gain_TEff_jour
    
    
    def compute_SLW(self) :
        if self.leafArea != 0 :
            self.SLW = (self.structural_biomass + self.non_structural_biomass) * GlobalVariables.POURC_FOLIOLE / self.leafArea / 10 
        else :
            self.SLW = 0 
        #print self.SLW, 'day =',self.phytomer.tree.simulation.meteo.getDay(),'month =',self.phytomer.tree.simulation.meteo.getMonth(),'year=',self.phytomer.tree.simulation.meteo.getYear(),'rank=',self.phytomer.rank, 'statut',self.phytomer.state,  'demande_biomasse_str', self.demand,   'leaf_area', self.leafArea, 'gain_biomasse_struct', (1 / GlobalVariables.COUT_RESPI_FEUILLE) * self.assimilate_supply,'potential_increase_leaf_area', self.pot_increase_leafArea, 'gain en leaf_area', self.pot_increase_leafArea   * (self.phytomer.tree.fr_reste), 'fraction', self.phytomer.tree.fr_reste, 'allocation_biomasse_str', (1 / GlobalVariables.COUT_RESPI_FEUILLE) * self.assimilate_supply, 'biomasse_structurale', self.structural_biomass,   'capacite de stockage_max', self.capacite_reserve_max, 'fraction_allouee', self.fraction_non_str_biomasse_allouee, 'allocation_biomasse_non_str', self.fraction_non_str_biomasse_allouee * ( self.phytomer.tree.reserve.delta_biomasse_reserve_leaf),   'SLW', self.SLW, 'biomasse_non_structurale', self.non_structural_biomass
    
    def compute_biomass(self) :
        #print self.phytomer.state
        if self.phytomer.state == "DEAD" :
            self.structural_biomass = 0 
            self.non_structural_biomass = 0
            #print 'toto', self.structural_biomass 
            
            #if (self.phytomer.bunch.sexe == "FEMELLE") :
            #    if (self.phytomer.bunch.avort == "NON_AVORTE") :
            #        if (self.phytomer.bunch.statut == "RECOLTE") :
            #             self.structural_biomass = 0
            #             self.non_structural_biomass = 0 
        
        else :
            self.structural_biomass += (1 / GlobalVariables.COUT_RESPI_FEUILLE) * self.assimilate_supply
            if self.non_structural_biomass + self.fraction_non_str_biomasse_allouee * ( self.phytomer.tree.reserve.delta_biomasse_reserve_leaf) < 0 : 
                self.non_structural_biomass  = 0 
            else :     
                self.non_structural_biomass += self.fraction_non_str_biomasse_allouee * ( self.phytomer.tree.reserve.delta_biomasse_reserve_leaf)
            
            
            
            if (self.ablation == "COUPE") :
                self.structural_biomass = 0
                self.non_structural_biomass = 0 
            
        self.total_biomass = self.structural_biomass + self.non_structural_biomass
               
    
    def getvitesse_exp(self) :
        return self.vitesse_exp

    def computeSF_ind_finale(self, SFMAX , TTfeuille) :
        return (SFMAX / (1 + exp(-(TTfeuille - GlobalVariables.INFLEXION) / GlobalVariables.COURBURE)))

    def getSFMax(self) :
        return self.SFMax
    
    def computeTEffCorrection(self, TEff):
        return TEff * self.compute_expansion_decrease(self.phytomer.tree.BH.FTSW, GlobalVariables.SEUIL_DUREE) 
        
    def computevitesse_exp(self,TTfeuille) :
        
        self.computeSFMAX(self.phytomer.tree.simulation.step)
        self.vitesse_exp = (-self.SFMax*(-exp(-(TTfeuille-GlobalVariables.INFLEXION)/GlobalVariables.COURBURE)/GlobalVariables.COURBURE)/(1+exp(-(TTfeuille-GlobalVariables.INFLEXION)/GlobalVariables.COURBURE))**2)
        
        
    def computeSFMAX(self, nbOfDaySinceBeginning):
        
        temp = (GlobalVariables.INCREASE_OF_LEAF_AREA * nbOfDaySinceBeginning) + GlobalVariables.INITIAL_SFIND
        self.SFMax = min(GlobalVariables.MAXIMAL_SFIND, temp)
        
    
    def getpot_inc_LeafArea(self):
        return self.pot_increase_leafArea
    
    def compute_pot_inc_LeafArea(self, correctedTEff):
        
        if (self.phytomer.state == "DEAD") :
            self.pot_increase_leafArea = 0
            #if (self.phytomer.bunch.sexe == "FEMELLE") :
            #    if (self.phytomer.bunch.avort == "NON_AVORTE") :
            #        if (self.phytomer.bunch.statut == "RECOLTE") :
            #             self.pot_increase_leafArea = 0 
        else : 
            self.computevitesse_exp(self.TT_corrige)
            self.pot_increase_leafArea = correctedTEff * self.compute_expansion_decrease(self.phytomer.tree.BH.FTSW, GlobalVariables.SEUIL_EXPAN) * self.vitesse_exp
            #print self.compute_expansion_decrease(self.phytomer.tree.BH.FTSW, GlobalVariables.SEUIL_EXPAN)
            self.computevitesse_exp(self.thermalTimeSinceAppearance)
            self.increase_potleafArea = correctedTEff * self.compute_expansion_decrease(self.phytomer.tree.BH.FTSW, GlobalVariables.SEUIL_EXPAN) * self.vitesse_exp
        
        if (self.ablation == "COUPE") :
            self.pot_increase_leafArea = 0
            
    def compute_expansion_decrease(self, FTSW, seuil) :
        
        if FTSW > seuil:
            y = 1
        else :   
            y = FTSW / seuil
        return(y)
       
        #print self.phytomer.getTree().simulation.getStep(),self.thermalTimeSinceAppearance,self.leafArea,self.SFMax
        #self.phytomer.getTree().increaseLeafArea(self.leafArea)
        #self.phytomer.tree().increaseLeafArea(self.leafArea)
        
    def compute_relative_position(self, rank) :
        self.crown_position = int((rank - 1)/3) + 1
        
    def compute_niveau_d_eclairement(self,crown_position) :
        a = GlobalVariables.a_loi_beta_interception
        b = GlobalVariables.b_loi_beta_interception
        
        nombre_max_crown_position = int((GlobalVariables.RANG_D_ABLATION-1)/3) + 1 
        if self.phytomer.state == "DEAD" :
            self.niveau_d_eclairement_loi_beta = 0
        if self.ablation == "COUPE" :
            self.niveau_d_eclairement_loi_beta = 0
        if self.ablation == "NON_COUPE" :
            if self.phytomer.state == "ACTIVE" :
                self.niveau_d_eclairement_loi_beta = ((crown_position - 0.5 )/ (nombre_max_crown_position)) **(a-1) * (1-(crown_position - 0.5 )/ (nombre_max_crown_position)) ** (b-1)
        
        
        
