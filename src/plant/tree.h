#ifndef TREE_H
#define TREE_H

/*
 * Copyright (C) 2019-2020 Cirad http://www.cirad.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>

#include <plant/processes/meteo.h>
#include <plant/processes/reserve.h>
#include <plant/processes/waterbalance.h>
#include <plant/phytomer/phytomer.h>
#include <plant/phytomer/leaf.h>
#include <plant/phytomer/bunch.h>
#include <plant/phytomer/internode.h>
#include <plant/root.h>

#include <iostream>

using namespace model;

class Tree : public CoupledModel < Tree >
{
public:
    enum submodels {  PHYTOMERS, RACINES, RESERVE, BH, METEO };

    enum internals { ACTIVEPHYTOMERNUMBER,
                     ASSIM,
                     BIOMASS,
                     BIOMASSE_PROD,
                     BUNCH_BIOMASS,
                     BUNCH_DEMAND,
                     DATE_PLUS_JEUNE_FEUILLE,
                     DELTA_AVANT_CROISSANCE,
                     DELTA_AV_RESPI,
                     EI,
                     FEMALE_BUNCH_BIOMASS,
                     FRACTION_POUR_CROISSANCE,
                     FRUIT_DEMAND,
                     FR_AVANT_CROISSANCE,
                     FR_FRUITS,
                     FR_RESTE,
                     GROWTH_DEMAND,
                     IC,
                     INACTIVEPHYTOMERNUMBER,
                     INTERNODE_DEMAND,
                     LEAF_DEMAND,
                     LEAF_NON_STRUCTURAL_BIOMASS,
                     LEAF_STRUCTURAL_BIOMASS,
                     MALE_BIOMASS,
                     MALE_DEMAND,
                     NEWPHYTOMEREMERGENCE,
                     NF,
                     OFFRE_FRUITS,
                     OFFRE_NETTE,
                     OFFRE_POUR_CROISSANCE,
                     OFFRE_POUR_RESPI,
                     OFFRE_RESTE,
                     PEDUNCULE_DEMAND,
                     PHYTOMERNUMBER,
                     PRODUCTIONSPEED,
                     PRODUCTION_DECREASE,
                     RESPIRABLE_BUNCH_BIOMASS,
                     SIMULATION,
                     SLW,
                     TABMEMORISE,
                     TOTALLEAFAREA,
                     TOTAL_LEAF_BIOMASS,
                     TRUNK_BIOMASS,
                     TRUNK_HEIGHT,
                     LAI };

private:
    xpalm::ModelParameters _parameters;

    //      parameters
    double AF_FRUITS;
    double COUT_RESPI_MAINTENANCE_BUNCH;
    double COUT_RESPI_MAINTENANCE_LEAF;
    double COUT_RESPI_MAINTENANCE_STIPE;
    double DEBUT_ABLATION_REGIME;
    double DEBUT_DEFOLIATON;
    double DECREASE_OF_PRODUCTION_SPEED;
    double DENS;
    double DRIVE;
    double EFFICIENCE_BIOLOGIQUE;
    double FIN_ABLATION_REGIME;
    double FIN_DEFOLIATON;
    double ICabort_RANG_FIN;
    double ICsex_RANG_FIN;
    double INACTIVE_PHYTOMER_NUMBER;
    double INCREASE_OF_LEAF_AREA;
    double INITIAL_HEIGHT;
    double INITIAL_PRODUCTION_SPEED;
    double INITIAL_SFIND;
    double INI_SEX_RATIO;
    double INI_TAUX_D_AVORTEMENT;
    double K;
    double LOCAL_LIGHT_INTERCEPTION;
    double MAXIMAL_SFIND;
    double MINIMAL_PRODUCTION_SPEED;
    double OUTPUTDIRECTORYNAME;
    double POURC_ABLATION_REGIME;
    double POURC_DEFOLIATON;
    double POURC_FOLIOLE;
    double RANG_DEFOLIATON;
    double RANG_D_ABLATION;
    double RANG_D_ABLATION_REGIME;
    double REMANENCE_STRESS;
    double SENSITIVITY_ABORTION;
    double SENSITIVITY_SEX;
    double SENSIVITY_IC_SETTING;
    double SENSIVITY_IC_SPIKELET;
    double SEUIL_ORGANO;
    double SEUIL_PHOTO;
    double SLW_ini;
    double SLW_min;
    double SORTIE1;
    double SORTIE2;
    double SORTIE3;
    double SORTIE4;
    double SORTIE5;
    double STEM_APPARENT_DENSITY;
    double STEM_RAYON;
    double Seuil_IC_abort;
    double Seuil_IC_sex;
    double VITESSE_SENSITIVITY;
    double lai;

    //     submodels
    std::deque < Phytomer* > phytomers;
    std::unique_ptr < Reserve > reserve;
    std::unique_ptr < Racines > racines;
    std::unique_ptr < WaterBalance > bh;
    std::unique_ptr < Meteo > meteo;

    //     internals
    double activephytomerNumber;
    double Assim;
    double biomass;
    double biomasse_prod;
    double bunch_biomass;
    double bunch_demand;
    double date_plus_jeune_feuille;
    double delta_avant_croissance;
    double delta_av_respi;
    double ei;
    double female_bunch_biomass;
    double fraction_pour_croissance;
    double fruit_demand;
    double fr_avant_croissance;
    double fr_fruits;
    double fr_reste;
    double growth_demand;
    double ic;
    double inactivephytomerNumber;
    double internode_demand;
    double leaf_demand;
    double leaf_non_structural_biomass;
    double leaf_structural_biomass;
    double male_biomass;
    double male_demand;
    double newPhytomerEmergence;
    double nf;
    double offre_fruits;
    double offre_nette;
    double offre_pour_croissance;
    double offre_pour_respi;
    double offre_reste;
    double peduncule_demand;
    double phytomerNumber;
    double productionSpeed;
    double production_decrease;
    double respirable_bunch_biomass;
    double simulation;
    double slw;
    double tabmemorise;
    double totalLeafArea;
    double total_leaf_biomass;
    double trunk_biomass;
    double trunk_height;

    //     externals


public:

    Tree() :
        reserve(new Reserve),
        racines(new Racines),
        meteo(new Meteo),
        bh(new WaterBalance())
    {
        //         submodels
        setsubmodel(RESERVE, reserve.get());
        setsubmodel(RACINES, racines.get());
        setsubmodel(BH, bh.get());
        setsubmodel(METEO, meteo.get());

        //         internals
        Internal(ACTIVEPHYTOMERNUMBER, &Tree::activephytomerNumber);
        Internal(ASSIM, &Tree::Assim);
        Internal(BIOMASS, &Tree::biomass);
        Internal(BIOMASSE_PROD, &Tree::biomasse_prod);
        Internal(BUNCH_BIOMASS, &Tree::bunch_biomass);
        Internal(BUNCH_DEMAND, &Tree::bunch_demand);
        Internal(DATE_PLUS_JEUNE_FEUILLE, &Tree::date_plus_jeune_feuille);
        Internal(DELTA_AVANT_CROISSANCE, &Tree::delta_avant_croissance);
        Internal(DELTA_AV_RESPI, &Tree::delta_av_respi);
        Internal(EI, &Tree::ei);
        Internal(FEMALE_BUNCH_BIOMASS, &Tree::female_bunch_biomass);
        Internal(FRACTION_POUR_CROISSANCE, &Tree::fraction_pour_croissance);
        Internal(FRUIT_DEMAND, &Tree::fruit_demand);
        Internal(FR_AVANT_CROISSANCE, &Tree::fr_avant_croissance);
        Internal(FR_FRUITS, &Tree::fr_fruits);
        Internal(FR_RESTE, &Tree::fr_reste);
        Internal(GROWTH_DEMAND, &Tree::growth_demand);
        Internal(IC, &Tree::ic);
        Internal(INACTIVEPHYTOMERNUMBER, &Tree::inactivephytomerNumber);
        Internal(INTERNODE_DEMAND, &Tree::internode_demand);
        Internal(LEAF_DEMAND, &Tree::leaf_demand);
        Internal(LEAF_NON_STRUCTURAL_BIOMASS, &Tree::leaf_non_structural_biomass);
        Internal(LEAF_STRUCTURAL_BIOMASS, &Tree::leaf_structural_biomass);
        Internal(MALE_BIOMASS, &Tree::male_biomass);
        Internal(MALE_DEMAND, &Tree::male_demand);
        Internal(NEWPHYTOMEREMERGENCE, &Tree::newPhytomerEmergence);
        Internal(NF, &Tree::nf);
        Internal(OFFRE_FRUITS, &Tree::offre_fruits);
        Internal(OFFRE_NETTE, &Tree::offre_nette);
        Internal(OFFRE_POUR_CROISSANCE, &Tree::offre_pour_croissance);
        Internal(OFFRE_POUR_RESPI, &Tree::offre_pour_respi);
        Internal(OFFRE_RESTE, &Tree::offre_reste);
        Internal(PEDUNCULE_DEMAND, &Tree::peduncule_demand);
        Internal(PHYTOMERNUMBER, &Tree::phytomerNumber);
        Internal(PRODUCTIONSPEED, &Tree::productionSpeed);
        Internal(PRODUCTION_DECREASE, &Tree::production_decrease);
        Internal(RESPIRABLE_BUNCH_BIOMASS, &Tree::respirable_bunch_biomass);
        Internal(SIMULATION, &Tree::simulation);
        Internal(SLW, &Tree::slw);
        Internal(TABMEMORISE, &Tree::tabmemorise);
        Internal(TOTALLEAFAREA, &Tree::totalLeafArea);
        Internal(TOTAL_LEAF_BIOMASS, &Tree::total_leaf_biomass);
        Internal(TRUNK_BIOMASS, &Tree::trunk_biomass);
        Internal(TRUNK_HEIGHT, &Tree::trunk_height);
        Internal(LAI, &Tree::lai);
    }

    virtual ~Tree()
    {
        reserve.reset(nullptr);
        racines.reset(nullptr);
        bh.reset(nullptr);
        meteo.reset(nullptr);

        auto it = phytomers.begin();
        while (it != phytomers.end()) {
            delete *it;
            ++it;
        }
    }


    void init(double t, const xpalm::ModelParameters& parameters)
    {
        _parameters = parameters;

        //        parameters
        AF_FRUITS = parameters.get("AF_FRUITS");
        COUT_RESPI_MAINTENANCE_BUNCH = parameters.get("COUT_RESPI_MAINTENANCE_BUNCH");
        COUT_RESPI_MAINTENANCE_LEAF = parameters.get("COUT_RESPI_MAINTENANCE_LEAF");
        COUT_RESPI_MAINTENANCE_STIPE = parameters.get("COUT_RESPI_MAINTENANCE_STIPE");
        DEBUT_ABLATION_REGIME = parameters.get("DEBUT_ABLATION_REGIME");
        DEBUT_DEFOLIATON = parameters.get("DEBUT_DEFOLIATON");
        DECREASE_OF_PRODUCTION_SPEED = parameters.get("DECREASE_OF_PRODUCTION_SPEED");
        DENS = parameters.get("DENS");
        DRIVE = parameters.get("DRIVE");
        EFFICIENCE_BIOLOGIQUE = parameters.get("EFFICIENCE_BIOLOGIQUE");
        FIN_ABLATION_REGIME = parameters.get("FIN_ABLATION_REGIME");
        FIN_DEFOLIATON = parameters.get("FIN_DEFOLIATON");
        ICabort_RANG_FIN = parameters.get("ICabort_RANG_FIN");
        ICsex_RANG_FIN = parameters.get("ICsex_RANG_FIN");
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");
        INCREASE_OF_LEAF_AREA = parameters.get("INCREASE_OF_LEAF_AREA");
        INITIAL_HEIGHT = parameters.get("INITIAL_HEIGHT");
        INITIAL_PRODUCTION_SPEED = parameters.get("INITIAL_PRODUCTION_SPEED");
        INITIAL_SFIND = parameters.get("INITIAL_SFIND");
        INI_SEX_RATIO = parameters.get("INI_SEX_RATIO");
        INI_TAUX_D_AVORTEMENT = parameters.get("INI_TAUX_D_AVORTEMENT");
        K = parameters.get("K");
        LOCAL_LIGHT_INTERCEPTION = parameters.get("LOCAL_LIGHT_INTERCEPTION");
        MAXIMAL_SFIND = parameters.get("MAXIMAL_SFIND");
        MINIMAL_PRODUCTION_SPEED = parameters.get("MINIMAL_PRODUCTION_SPEED");
        OUTPUTDIRECTORYNAME = parameters.get("OUTPUTDIRECTORYNAME");
        POURC_ABLATION_REGIME = parameters.get("POURC_ABLATION_REGIME");
        POURC_DEFOLIATON = parameters.get("POURC_DEFOLIATON");
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        RANG_DEFOLIATON = parameters.get("RANG_DEFOLIATON");
        RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        RANG_D_ABLATION_REGIME = parameters.get("RANG_D_ABLATION_REGIME");
        REMANENCE_STRESS = parameters.get("REMANENCE_STRESS");
        SENSITIVITY_ABORTION = parameters.get("SENSITIVITY_ABORTION");
        SENSITIVITY_SEX = parameters.get("SENSITIVITY_SEX");
        SENSIVITY_IC_SETTING = parameters.get("SENSIVITY_IC_SETTING");
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        SEUIL_ORGANO = parameters.get("SEUIL_ORGANO");
        SEUIL_PHOTO = parameters.get("SEUIL_PHOTO");
        SLW_ini = parameters.get("SLW_ini");
        SLW_min = parameters.get("SLW_min");
        SORTIE1 = parameters.get("SORTIE1");
        SORTIE2 = parameters.get("SORTIE2");
        SORTIE3 = parameters.get("SORTIE3");
        SORTIE4 = parameters.get("SORTIE4");
        SORTIE5 = parameters.get("SORTIE5");
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        STEM_RAYON = parameters.get("STEM_RAYON");
        Seuil_IC_abort = parameters.get("Seuil_IC_abort");
        Seuil_IC_sex = parameters.get("Seuil_IC_sex");
        VITESSE_SENSITIVITY = parameters.get("VITESSE_SENSITIVITY");

        //        internals
        phytomerNumber = INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION;
        inactivephytomerNumber = INACTIVE_PHYTOMER_NUMBER;
        activephytomerNumber = RANG_D_ABLATION;
        date_plus_jeune_feuille = 0         ;
        nf = 0 ;
        newPhytomerEmergence = 0;
        productionSpeed = INITIAL_PRODUCTION_SPEED;
        trunk_height = INITIAL_HEIGHT;
        bunch_biomass = 0;
        respirable_bunch_biomass = 0;
        female_bunch_biomass = 0;
        male_biomass = 0 ;

        meteo->init(t, parameters);
        reserve->init(t, parameters);
        racines->init(t, parameters);
        bh->init(t, parameters);

//        init_structure(t);

//        compute_SF(t);
        lai = totalLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);
        Assim= 0;
        production_decrease = 0 ;
        ic = 1;
        fr_fruits = 0; // part de la demande des fruits satisfaite;
        fr_reste = 0; // part de la demande du reste statisfaite;
        internode_demand = 0;
        leaf_demand = 0;
        bunch_demand = 0;
        fruit_demand = 0;
        male_demand = 0;
        peduncule_demand = 0 ;
        offre_nette = 0  ;
        offre_pour_respi = 0;  // offre pour la respi prioritaire;
        delta_av_respi = 0; // c est la difference entre l assim et la demande pour la respi;
        offre_pour_croissance = 0; // c est l offre pour la croissance une fois que la respi est enlevee du bilan;
        growth_demand = 0; // c est la demande pour la croissance des organes;
        delta_avant_croissance = 0; // c est la difference entre l offre apres la respi et la demande pour la croissance;
        fr_avant_croissance = 0; // c est la meme chose que precedemment mais en fraction;
        fraction_pour_croissance = 0; // c est la fraction qui reste pour la croissance une fois que le stockage de biomasse dans les reserves a ete effectuee;
        offre_fruits = 0; // offre pour les fruits;
        offre_reste = 0; // offre pour le reste;
        trunk_biomass = 1000 * INITIAL_HEIGHT * STEM_APPARENT_DENSITY * _PI * pow( STEM_RAYON, 2);
        leaf_structural_biomass = totalLeafArea  * SLW_min * 10 / POURC_FOLIOLE  ;
        leaf_non_structural_biomass= totalLeafArea * (SLW_ini - SLW_min) * 10 / POURC_FOLIOLE;
        total_leaf_biomass = leaf_structural_biomass + leaf_non_structural_biomass;
//        biomass = leaf_structural_biomass + leaf_non_structural_biomass + reserve->get < double >(t, Reserve::BIOMASS) + trunk_biomass + female_bunch_biomass + male_biomass ;
        slw = (((leaf_non_structural_biomass + leaf_structural_biomass) * POURC_FOLIOLE) / totalLeafArea) / 10;
        biomasse_prod = 0;

    }

    void init_structure(double t) { //TODO fill
        Phytomer * first_phytomer = new Phytomer(RANG_D_ABLATION, 1, phytomer::ACTIVE, t);
        setsubmodel(PHYTOMERS, first_phytomer);
        first_phytomer->init(t, _parameters);
        phytomers.push_back(first_phytomer);
//        date_plus_jeune_feuille =  first_phytomer->get < double, Phytomer >(t, Phytomer::STEP_APP); //TODO remettre

                for (int key = 0; key < INACTIVE_PHYTOMER_NUMBER + 1; ++key) {
                    Phytomer * phytomer = new Phytomer(key + RANG_D_ABLATION, -key + 1, phytomer::INACTIVE, t);
                    setsubmodel(PHYTOMERS, phytomer);
                    phytomer->init(t, _parameters);
                    phytomers.push_back(phytomer);


        //            phytomer.compute_facteur_age()
        //            if phytomers[phytomer.name].rank > GlobalVariables.ICsex_RANG_FIN :
        //                phytomers[phytomer.name].bunch.sexe = phytomer.bunch.sexe_decision(GlobalVariables.INI_SEX_RATIO)
                }

        //        for key in range(1 , GlobalVariables.RANG_D_ABLATION + 1) :
        //            nb_jour_depuis_l_appari = ((GlobalVariables.RANG_D_ABLATION) - key)/( 10 * GlobalVariables.INITIAL_PRODUCTION_SPEED)                         # rang d'ablation = nbre ini
        //            TTfeuille = nb_jour_depuis_l_appari * 10  # 10 temps thermique moyen a calculer


        //            phytomer = Phytomer(key , GlobalVariables.RANG_D_ABLATION - key + 1  , "ACTIVE", self,  0, 0, 0, TTfeuille, 0, 0, 0, 0)


        //            phytomers[phytomer.name] = phytomer
        //            phytomers[phytomer.name].step_apparition = simulation.step - (GlobalVariables.INACTIVE_PHYTOMER_NUMBER + GlobalVariables.RANG_D_ABLATION - phytomers[phytomer.name].name) * (1/ (10 * GlobalVariables.INITIAL_PRODUCTION_SPEED))
        //            phytomer.compute_facteur_age()

        //            phytomers[phytomer.name].bunch.thermalTimeSinceAppearance = TTfeuille
        //            phytomers[phytomer.name].bunch.TT_corrige = TTfeuille
        //            SF = min(GlobalVariables.INCREASE_OF_LEAF_AREA * ( - nb_jour_depuis_l_appari ) + GlobalVariables.INITIAL_SFIND, GlobalVariables.MAXIMAL_SFIND)
        //            phytomers[phytomer.name].bunch.sexe = phytomer.bunch.sexe_decision(GlobalVariables.INI_SEX_RATIO)
        //            if phytomers[phytomer.name].rank > GlobalVariables.ICabort_RANG_FIN :
        //                phytomers[phytomer.name].bunch.avort = phytomer.bunch.abortion_decision(GlobalVariables.INI_TAUX_D_AVORTEMENT)
        //            phytomer.bunch.compute_structure_ini()
        //            phytomer.bunch.compute_flowering_date(GlobalVariables.INITIAL_PRODUCTION_SPEED)
        //            phytomer.bunch.compute_harvest_date(GlobalVariables.INITIAL_PRODUCTION_SPEED)

        //            if phytomers[phytomer.name].rank< 0 :
        //                phytomers[phytomer.name].bunch.statut = "INITIE"
        //            else :

        //                if (phytomers[phytomer.name].bunch.thermalTimeSinceAppearance > phytomers[phytomer.name].bunch.estimated_flowering_date)  : # on calcule une moyenne entre la dtae ? l'apparition et la date au jour j
        //                    if (phytomers[phytomer.name].bunch.thermalTimeSinceAppearance > phytomers[phytomer.name].bunch.estimated_harvest_date)  :
        //                        phytomers[phytomer.name].bunch.statut = "RECOLTE"
        //                    else :
        //                        phytomers[phytomer.name].bunch.statut = "FLORAISON_RECOLTE"
        //                else :
        //                    phytomers[phytomer.name].bunch.statut = "APPARITION_FLORAISON"

        //            phytomers[phytomer.name].leaf.thermalTimeSinceAppearance = TTfeuille
        //            phytomers[phytomer.name].leaf.TT_corrige = TTfeuille
        //            phytomers[phytomer.name].internode.TT_corrige = TTfeuille
        //            SF = min(GlobalVariables.INCREASE_OF_LEAF_AREA * ( - nb_jour_depuis_l_appari ) + GlobalVariables.INITIAL_SFIND, GlobalVariables.MAXIMAL_SFIND)


        //            phytomers[phytomer.name].leaf.leafArea = phytomer.leaf.computeSF_ind_finale(SF , TTfeuille)

        //            phytomers[phytomer.name].leaf.structural_biomass = phytomers[phytomer.name].leaf.leafArea * GlobalVariables.SLW_min * 10 / GlobalVariables.POURC_FOLIOLE
        //            phytomers[phytomer.name].leaf.non_structural_biomass = phytomers[phytomer.name].leaf.leafArea * (GlobalVariables.SLW_ini - GlobalVariables.SLW_min) * 10 / GlobalVariables.POURC_FOLIOLE

        //            if phytomers[phytomer.name].bunch.statut == "RECOLTE" :
        //                if phytomers[phytomer.name].bunch.avort == "NON_AVORTE" :
        //                    if phytomers[phytomer.name].bunch.sexe == "FEMELLE" :
        //                        phytomers[phytomer.name].leaf.leafArea = 0
        //                        phytomers[phytomer.name].leaf.structural_biomass = 0
        //                        phytomers[phytomer.name].leaf.non_structural_biomass = 0
        //            phytomers[phytomer.name].leaf.potLeafArea = phytomers[phytomer.name].leaf.leafArea

        //            if phytomers[phytomer.name].leaf.leafArea != 0 :
        //                phytomers[phytomer.name].leaf.SLW = (phytomers[phytomer.name].leaf.structural_biomass + phytomers[phytomer.name].leaf.non_structural_biomass) * GlobalVariables.POURC_FOLIOLE / phytomers[phytomer.name].leaf.leafArea / 10

        //            #print "STRUCTURE_INI", "sexe",phytomers[phytomer.name].bunch.sexe,  "avort", phytomers[phytomer.name].bunch.avort,  "statut", phytomers[phytomer.name].bunch.statut, "leafarea", phytomers[phytomer.name].leaf.leafArea, "structural_biomass", phytomers[phytomer.name].leaf.structural_biomass, "non_structural_biomass", phytomers[phytomer.name].leaf.non_structural_biomass, "SLW",phytomers[phytomer.name].leaf.SLW

        //            phytomers[phytomer.name].internode.duree_allongement = 1 / productionSpeed
        //            #phytomers[phytomer.name].internode.length = GlobalVariables.INITIAL_HEIGHT / GlobalVariables.RANG_D_ABLATION
        //            date_d_appar =  (GlobalVariables.RANG_D_ABLATION - phytomer.name) * 1 /(10 * productionSpeed )
        //            if phytomers[phytomer.name].internode.TT_corrige > phytomers[phytomer.name].internode.duree_allongement :
        //                phytomers[phytomer.name].internode.length = phytomer.internode.compute_final_length(- date_d_appar )  # 10Cj en moyenne
        //            else :
        //                phytomers[phytomer.name].internode.length = phytomer.internode.compute_final_length(- date_d_appar ) * ( phytomers[phytomer.name].internode.TT_corrige) / phytomers[phytomer.name].internode.duree_allongement

        //            phytomers[phytomer.name].internode.biomass = 1000 * phytomers[phytomer.name].internode.length * GlobalVariables.STEM_APPARENT_DENSITY * pi * ( GlobalVariables.STEM_RAYON )**2



    }

    void compute_SF( double t ){
        totalLeafArea = 0;

        //surface
        auto it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            if (phytomer->get< phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) != phytomer::DEAD)
                totalLeafArea += phytomer->leaf_model()->get < double >(t, Leaf::LEAFAREA);

        }
    }


//    void compute_biomasse_prod(double t) {
//        double Rg = meteo->get<double>(t, Meteo::RG);
//        double FTSW = bh->get<double>(t, WaterBalance::FTSW);
//        double SF = totalLeafArea;
//        lai = totalLeafArea * DENS / 10000;
//        ei = 1 - exp(- K * lai);

//        total_beta_interception_leaf = 0;


//        if (LOCAL_LIGHT_INTERCEPTION == 1) {
//            auto it = phytomers.begin();
//            while (it != phytomers.end()) {
//                Phytomer* phytomer = (*it);
//                total_beta_interception_leaf += phytomer->leaf_model()->get < double >(t, Leaf::NIVEAU_D_ECLAIREMENT_LOI_BETA);
//            }
//            biomasse_prod  = 0;

//            it = phytomers.begin();
//            while (it != phytomers.end()) {
//                leaf = phytomers[key].leaf;
//                leaf.light_pourc_interception = leaf.niveau_d_eclairement_loi_beta / total_beta_interception_leaf;

//                if (REMANENCE_STRESS == 1)
//                    leaf.compute_declin_photo(); //TODO
//                else
//                    leaf.assim_max =  EFFICIENCE_BIOLOGIQUE;

//                leaf.compute_biomasse_prod(leaf.light_pourc_interception, leaf.assim_max, ei, GlobalVariables.DENS, Rg)   // on cacule la biomasse produite par la feuille en absence de contrainte
//                        biomasse_prod += compute_decrease_FTSW(FTSW, GlobalVariables.SEUIL_PHOTO) * leaf.biomasse_prod;

//            }
//        } else {
//            biomasse_prod = compute_decrease_FTSW(FTSW, GlobalVariables.SEUIL_PHOTO) * 10 * ei * EFFICIENCE_BIOLOGIQUE * 0.48 * Rg / GlobalVariables.DENS  # on multiplie par 10 pour passer en kg
//        }

//    }

    void update_organs(double t, double newPhytomerEmergence) {
        //       #### defiolation le jour de la mise en place des traitements
        if (t == DEBUT_DEFOLIATON ) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                int rank = phytomer->get< double, Phytomer >(t, Phytomer::RANK);
                phytomer::phytomer_state state = phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE);
                if(rank > RANG_DEFOLIATON && state == phytomer::ACTIVE)
                    ;//TODO ABLATION  phytomers[key].leaf.ablation = phytomers[key].leaf.defoliation_decision(POURC_DEFOLIATON)
            }
        }

        // ### ablation des regimes le jour de la mise en place des traitements
        if (t == DEBUT_ABLATION_REGIME ) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                int rank = phytomer->get < double, Phytomer >(t, Phytomer::RANK);
                bunch::bunch_states bunch_state = phytomer->bunch_model()->get <bunch::bunch_states, Bunch >(t, Bunch::STATUS);
                bunch::bunch_sex sex = phytomer->bunch_model()->get <bunch::bunch_sex, Bunch >(t, Bunch::SEX);
                bunch::bunch_sex avort = phytomer->bunch_model()->get <bunch::bunch_sex, Bunch >(t, Bunch::AVORT);
                if(rank >= RANG_D_ABLATION_REGIME
                        && bunch_state.is(bunch::RECOLTE)
                        && sex ==  bunch::FEMALE
                        && avort == bunch::NON_ABORTED)
                    ;//TODO ABLATION  phytomers[key].leaf.ablation = phytomers[key].leaf.defoliation_decision(POURC_DEFOLIATON)

            }
        }

        if (newPhytomerEmergence >= 1) {
            Phytomer* phytomer = new Phytomer(phytomerNumber + 1, -INACTIVE_PHYTOMER_NUMBER, phytomer::INACTIVE, t);
            phytomer->init(t, _parameters);
            phytomers.push_back(phytomer);
            phytomerNumber += 1;
            nf += 1;
        }
    }

    void compute(double t, bool /* update */)
    {
        std::cout << t << std::endl;
        if(t == 0) {
            init_structure(t);

            (*racines)(t);
            return;
        } else {
            std::cout << "pas 0" << std::endl;
        }

        (*meteo)(t);

        //        if simulation.meteo.irrigation_day == "IRRIGATION" :
        //            pluie = simulation.meteo.Rain + simulation.meteo.irrigation[simulation.meteo.jour_julien]
        //        else :
        //            pluie = simulation.meteo.Rain
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC, racines->get<double>(t-1, Racines::TAILLEC));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1, racines->get<double>(t-1, Racines::TAILLEC1));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC2, racines->get<double>(t-1, Racines::TAILLEC2));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1MOINSVAP, racines->get<double>(t-1, Racines::TAILLEC1MOINSVAP));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEVAP, racines->get<double>(t-1, Racines::TAILLEVAP));
        bh->put<double>(t, WaterBalance::ET0, meteo->get<double>(t, Meteo::ET0));
        bh->put<double>(t, WaterBalance::TREE_EI, ei);
        (*bh)(t);

        double TEff = meteo->get<double>(t, Meteo::TEFF);
        double ftsw = bh->get<double>(t, WaterBalance::FTSW);

        //Execution phythomers
        auto it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            if(phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::ACTIVE) {
                phytomer->leaf_model()->put(t, Leaf::TEFF, TEff);
                phytomer->leaf_model()->put(t, Leaf::FTSW, ftsw);
                phytomer->leaf_model()->put(t, Leaf::FR_RESTE, fr_reste);
                phytomer->leaf_model()->put(t, Leaf::DELTA_BIOMASSE_RESERVE_LEAF, reserve->get<double>(t-1, Reserve::DELTA_BIOMASSE_RESERVE_LEAF));

                phytomer->internode_model()->put(t, Internode::TEFF, TEff);
                phytomer->internode_model()->put(t, Internode::PRODUCTIONSPEED, productionSpeed);
                phytomer->internode_model()->put(t, Internode::FR_RESTE, fr_reste);

                phytomer->bunch_model()->put(t, Bunch::TEFF, TEff);
                phytomer->bunch_model()->put(t, Bunch::FTSW, ftsw);
                phytomer->bunch_model()->put(t, Bunch::FR_FRUITS, fr_fruits);
                phytomer->bunch_model()->put(t, Bunch::DATE_PLUS_JEUNE_FEUILLE, date_plus_jeune_feuille);
                phytomer->bunch_model()->put(t, Bunch::TREE_PRODUCTIONSPEED, productionSpeed);
                phytomer->bunch_model()->put(t, Bunch::TREE_IC, ic);

                (*phytomer)(t);
            }
        }

        productionSpeed = max(MINIMAL_PRODUCTION_SPEED, (-DECREASE_OF_PRODUCTION_SPEED * t) + INITIAL_PRODUCTION_SPEED);

        newPhytomerEmergence += TEff * productionSpeed * pow(ic,VITESSE_SENSITIVITY) * ( ftsw > SEUIL_ORGANO ? 1 : ftsw / SEUIL_ORGANO);
        update_organs(t, newPhytomerEmergence);
        (*racines)(t);

        //Compute results
        //reset vars
        trunk_height = INITIAL_HEIGHT;
        leaf_demand = 0;
        internode_demand = 0;
        bunch_demand = 0;
        fruit_demand = 0;
        male_demand = 0;
        peduncule_demand = 0;
        growth_demand = 0;
        totalLeafArea = 0;

        it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            if(phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::ACTIVE) {
                leaf_structural_biomass += phytomer->leaf_model()->get <double>(t, Leaf::STRUCTURAL_BIOMASS);
                leaf_non_structural_biomass += phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS);
                bunch_biomass += phytomer->bunch_model()->get <double, Bunch>(t, Bunch::BIOMASS); //## attention pour la respi de maintenance !!!
                respirable_bunch_biomass += phytomer->bunch_model()->get <double, Bunch>(t, Bunch::RESPIRABLE_BIOMASS);
                female_bunch_biomass += phytomer->bunch_model()->get <double, Bunch>(t, Bunch::FEMELLE_BIOMASS);
                male_biomass += phytomer->bunch_model()->get <double, Bunch>(t, Bunch::MALE_BIOMASS);
                trunk_height += phytomer->internode_model()->get <double>(t, Internode::LENGTH);
                internode_demand +=  phytomer->internode_model()->get <double>(t, Internode::DEMAND);
                leaf_demand += phytomer->leaf_model()->get <double>(t, Leaf::DEMAND);
                bunch_demand += phytomer->bunch_model()->get <double, Bunch>(t, Bunch::DEMAND);
                fruit_demand += phytomer->bunch_model()->fruit_model()->get <double>(t, Fruit::DEMAND);
                male_demand += phytomer->bunch_model()->male_model()->get <double>(t, MaleInflo::DEMAND);
                peduncule_demand += phytomer->bunch_model()->peduncle_model()->get <double>(t, Peduncle::DEMAND);
            }
            trunk_biomass += phytomer->internode_model()->get <double>(t, Internode::BIOMASS);
            biomass = leaf_structural_biomass + leaf_non_structural_biomass +  female_bunch_biomass + trunk_biomass + reserve->get <double>(t, Reserve::BIOMASS) + male_biomass;

            if (phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) != phytomer::DEAD) {
                totalLeafArea += phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA);
            }
        }
        total_leaf_biomass = leaf_structural_biomass + leaf_non_structural_biomass;

        double TMoy = (_parameters.get(t).TMax + _parameters.get(t).TMin) / 2;
        double Q10 = pow(2, (TMoy - 25)/10);
        double respi_maintenance = Q10* (
                    trunk_biomass * COUT_RESPI_MAINTENANCE_STIPE +
                    respirable_bunch_biomass * COUT_RESPI_MAINTENANCE_BUNCH +
                    leaf_structural_biomass * COUT_RESPI_MAINTENANCE_LEAF);

        double demand = internode_demand + leaf_demand + respi_maintenance + bunch_demand;
        growth_demand +=  internode_demand +   leaf_demand  +  bunch_demand;
        ic = Assim/demand;
        Assim = biomasse_prod;

        // compute etat reserve
        (*reserve)(t);

        //       compute_offre_nette
        offre_nette = offre_pour_croissance + reserve->get<double>(t, Reserve::APPORT);
        fraction_pour_croissance = offre_nette / (growth_demand);

        //       compute_fraction_oil_reste
        double demande_cr_recal = leaf_demand + internode_demand  + male_demand + AF_FRUITS * fruit_demand + peduncule_demand;

        if (AF_FRUITS >= 1) {
            offre_fruits = min( ( AF_FRUITS * fruit_demand * offre_nette / demande_cr_recal ), fruit_demand );

            if (fruit_demand==0) {
                fr_fruits = 1;
                offre_reste = offre_nette - offre_fruits;
                fr_reste = offre_reste / ( leaf_demand + internode_demand  + male_demand + peduncule_demand );
            } else {
                fr_fruits = offre_fruits / fruit_demand;
                offre_reste = offre_nette - offre_fruits;
                fr_reste = offre_reste / ( leaf_demand + internode_demand  + male_demand + peduncule_demand );
            }
        } else {
            offre_reste = min( ( leaf_demand + internode_demand  + male_demand + peduncule_demand  )
                               * offre_reste / demande_cr_recal, leaf_demand + internode_demand  + male_demand + peduncule_demand  );
            fr_reste = offre_reste / ( leaf_demand + internode_demand  + male_demand + peduncule_demand  );
            offre_fruits = offre_nette - offre_reste;
            fr_fruits = offre_fruits/ fruit_demand;
        }

        compute_biomasse_non_structurale_allouee_aux_feuilles(t);

    }


    //    void compute_growth(double t) {
    //        trunk_biomass = 0;
    //        bunch_biomass = 0;
    //        leaf_non_structural_biomass = 0;
    //        leaf_structural_biomass = 0;
    //        respirable_bunch_biomass = 0;
    //        male_biomass = 0;


    //    }


    void compute_biomasse_non_structurale_allouee_aux_feuilles(double t) {
        double TOT = 0;
        double FRAC = 0;

        double delta_biomasse_reserve_leaf = reserve->get<double>(t, Reserve::DELTA_BIOMASSE_RESERVE_LEAF);
        if  (delta_biomasse_reserve_leaf > 0) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::ACTIVE)
                    continue;

                double capacite_reserve_max = phytomer->leaf_model()->get <double>(t, Leaf::CAPACITE_RESERVE_MAX);
                double non_structural_biomass = phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS);
                if(capacite_reserve_max > non_structural_biomass)
                    TOT += capacite_reserve_max - non_structural_biomass;
            }

            it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(TOT <= 0 && phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA) > 0)
                    phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                else if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::DEAD &&
                        phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA) > 0) {
                    double capacite_reserve_max = phytomer->leaf_model()->get <double>(t, Leaf::CAPACITE_RESERVE_MAX);
                    double non_structural_biomass = phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS);
                    if(capacite_reserve_max > non_structural_biomass) {
                        double fraction_non_str_biomasse_allouee = (capacite_reserve_max - non_structural_biomass) / TOT;
                        phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, fraction_non_str_biomasse_allouee);
                        FRAC += fraction_non_str_biomasse_allouee;
                    }
                }
            }
        } else {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::ACTIVE)
                    continue;
                TOT += phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS);
            }

            it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);

                if(TOT <= 0 && phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA) > 0)
                    phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                else if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::DEAD &&
                        phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA) > 0) {
                    double non_structural_biomass = phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS);
                    if(non_structural_biomass >= 0) {
                        double fraction_non_str_biomasse_allouee = non_structural_biomass / TOT;
                        phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, fraction_non_str_biomasse_allouee);
                        FRAC += fraction_non_str_biomasse_allouee;
                    }
                }
            }
        }
    }

};

#endif // TREE_H
