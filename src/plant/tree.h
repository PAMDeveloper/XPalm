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
#include <plant/phytomer/inflo.h>
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
    double PRODUCTION_SPEED_ADULT;
    double PRODUCTION_SPEED_INITIAL;
    double AGE;
    double AGE_ADULT;
    double AGE_PLANTING;
    double PRODUCTION_DAILY_DECREMENT;


    double AF_FRUITS;
    double COUT_RESPI_MAINTENANCE_BUNCH;
    double COUT_RESPI_MAINTENANCE_LEAF;
    double COUT_RESPI_MAINTENANCE_STIPE;
    double DEBUT_ABLATION_REGIME;
    double DEBUT_DEFOLIATON;
//    double DECREASE_OF_PRODUCTION_SPEED;
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
    double INITIAL_SFIND;
    double INI_SEX_RATIO;
    double INI_TAUX_D_AVORTEMENT;
    double K;
    double LOCAL_LIGHT_INTERCEPTION;
    double MAXIMAL_SFIND;
//    double MINIMAL_PRODUCTION_SPEED;
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
    std::vector<double> ics;
    double age;
    double activephytomerNumber;
    double Assim;
    double biomass;
    double biomasse_prod;
    double inflo_biomass;
    double inflo_demand;
    double date_plus_jeune_feuille;
    double delta_avant_croissance;
    double delta_av_respi;
    double ei;
    double female_bunch_biomass;
    double fraction_pour_croissance;
    double bunch_demand;
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
    double offre_fruits;
    double offre_nette;
    double offre_pour_croissance;
    double offre_pour_respi;
    double offre_reste;
    double peduncule_demand;
    double phytomerNumber;
    double production_speed;
    double production_decrease;
    double respirable_bunch_biomass;
    double simulation;
    double slw;
    double tabmemorise;
    double totalLeafArea;
    double total_leaf_biomass;
    double trunk_biomass;
    double trunk_height;

    // attributes
    //t-1
    double rootTailleC;
    double rootTailleC1;
    double rootTailleC2;
    double rootTailleC1MVAP;
    double rootTailleVAP;


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
        Internal(BUNCH_BIOMASS, &Tree::inflo_biomass);
        Internal(BUNCH_DEMAND, &Tree::inflo_demand);
        Internal(DATE_PLUS_JEUNE_FEUILLE, &Tree::date_plus_jeune_feuille);
        Internal(DELTA_AVANT_CROISSANCE, &Tree::delta_avant_croissance);
        Internal(DELTA_AV_RESPI, &Tree::delta_av_respi);
        Internal(EI, &Tree::ei);
        Internal(FEMALE_BUNCH_BIOMASS, &Tree::female_bunch_biomass);
        Internal(FRACTION_POUR_CROISSANCE, &Tree::fraction_pour_croissance);
        Internal(FRUIT_DEMAND, &Tree::bunch_demand);
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
        Internal(OFFRE_FRUITS, &Tree::offre_fruits);
        Internal(OFFRE_NETTE, &Tree::offre_nette);
        Internal(OFFRE_POUR_CROISSANCE, &Tree::offre_pour_croissance);
        Internal(OFFRE_POUR_RESPI, &Tree::offre_pour_respi);
        Internal(OFFRE_RESTE, &Tree::offre_reste);
        Internal(PEDUNCULE_DEMAND, &Tree::peduncule_demand);
        Internal(PHYTOMERNUMBER, &Tree::phytomerNumber);
        Internal(PRODUCTIONSPEED, &Tree::production_speed);
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
        last_time = t-1;
        _parameters = parameters;

        age = parameters.get("AGE")  * 365;

        //        parameters
        PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");
        AGE_ADULT = parameters.get("AGE_ADULT") * 365; // days
        AGE_PLANTING = parameters.get("AGE_PLANTING") * 365; // days (age when production speed starts to decrease
        PRODUCTION_DAILY_DECREMENT = ( PRODUCTION_SPEED_ADULT - PRODUCTION_SPEED_INITIAL ) / ( AGE_ADULT - AGE_PLANTING );

        AF_FRUITS = parameters.get("AF_FRUITS");
        COUT_RESPI_MAINTENANCE_BUNCH = parameters.get("COUT_RESPI_MAINTENANCE_BUNCH");
        COUT_RESPI_MAINTENANCE_LEAF = parameters.get("COUT_RESPI_MAINTENANCE_LEAF");
        COUT_RESPI_MAINTENANCE_STIPE = parameters.get("COUT_RESPI_MAINTENANCE_STIPE");
        DEBUT_ABLATION_REGIME = parameters.get("DEBUT_ABLATION_REGIME");
        DEBUT_DEFOLIATON = parameters.get("DEBUT_DEFOLIATON");
//        DECREASE_OF_PRODUCTION_SPEED = parameters.get("DECREASE_OF_PRODUCTION_SPEED");
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
        INITIAL_SFIND = parameters.get("INITIAL_SFIND");
        INI_SEX_RATIO = parameters.get("INI_SEX_RATIO");
        INI_TAUX_D_AVORTEMENT = parameters.get("INI_TAUX_D_AVORTEMENT");
        K = parameters.get("K");
        LOCAL_LIGHT_INTERCEPTION = parameters.get("LOCAL_LIGHT_INTERCEPTION");
        MAXIMAL_SFIND = parameters.get("MAXIMAL_SFIND");
//        MINIMAL_PRODUCTION_SPEED = parameters.get("MINIMAL_PRODUCTION_SPEED");
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
        SLW_ini = parameters.get("SLW_ini") * 10; //kg.m-2
        SLW_min = parameters.get("SLW_min") * 10; //kg.m-2
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        STEM_RAYON = parameters.get("STEM_RAYON");
        Seuil_IC_abort = parameters.get("Seuil_IC_abort");
        Seuil_IC_sex = parameters.get("Seuil_IC_sex");
        VITESSE_SENSITIVITY = parameters.get("VITESSE_SENSITIVITY");

        //        internals
        phytomerNumber = 0;
        newPhytomerEmergence = 0;
        inactivephytomerNumber = INACTIVE_PHYTOMER_NUMBER;
        activephytomerNumber = RANG_D_ABLATION;

        production_speed = PRODUCTION_SPEED_INITIAL;
        trunk_height = INITIAL_HEIGHT;
        inflo_biomass = 0;
        respirable_bunch_biomass = 0;
        female_bunch_biomass = 0;
        male_biomass = 0 ;


        production_speed = age_relative_var(age, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);

        meteo->init(t, parameters);
        reserve->init(t, parameters);
        racines->init(t, parameters);
        bh->init(t, parameters);

//        init_structure(t);

        compute_SF(t);
        lai = totalLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);
        Assim= 0;
        production_decrease = 0 ;
        ic = 1;
        fr_fruits = 0; // part de la demande des fruits satisfaite;
        fr_reste = 0; // part de la demande du reste statisfaite;
        internode_demand = 0;
        leaf_demand = 0;
        inflo_demand = 0;
        bunch_demand = 0;
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
        trunk_biomass = 1000  * STEM_APPARENT_DENSITY * _PI * pow( STEM_RAYON, 2) * INITIAL_HEIGHT; //TODO vérifier l'unité dans les calculs de masse x1000


        leaf_structural_biomass = totalLeafArea  * SLW_min / POURC_FOLIOLE  ;
        leaf_non_structural_biomass= totalLeafArea * (SLW_ini - SLW_min) / POURC_FOLIOLE;
        total_leaf_biomass = leaf_structural_biomass + leaf_non_structural_biomass;
//        biomass = leaf_structural_biomass + leaf_non_structural_biomass + reserve->get < double >(t, Reserve::BIOMASS) + trunk_biomass + female_bunch_biomass + male_biomass ;
        slw = (((leaf_non_structural_biomass + leaf_structural_biomass) * POURC_FOLIOLE) / totalLeafArea);
        biomasse_prod = 0;


        //init structure
        double TTEff_init = parameters.get("T_EFF_INI");
        double age_at_creation = age;
        for ( int number = INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION - 1; number == 0 ; --number ) {
            create_phytomer(number, age_at_creation);
            age_at_creation -= 1 / (TTEff_init * production_speed);
        }

    }

    static double age_relative_var(double age, double age_ini, double age_fin, double val_ini, double val_fin) {
        if ( age > age_fin ) {
            return val_fin;
        } else if (age < age_ini ) {
            return val_ini;
        } else {
            double age_relative = age - age_ini;
            double inc = (val_fin - val_ini) / (age_fin - age_ini);
            return val_ini + age_relative * inc;
        }
    }


    void create_phytomer(double number, double age_at_creation) {
        int rank = RANG_D_ABLATION - number + 1;

        double production_speed = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
        double TT_ini_flowering = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, TT_FLOWERING_INITIAL, TT_FLOWERING_ADULT);
        double TT_ini_harvest = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, TT_HARVEST_INITIAL, TT_HARVEST_ADULT);
        double inflo_dev_factor = age_relative_var(age_at_creation, AGE_START_PROD, AGE_ADULT, 0, 1);
//        double TT_ini_oleo = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, TT_HARVEST_INITIAL, TT_HARVEST_ADULT);

        Phytomer * phytomer = new Phytomer(number,
                                           rank,
                                           number > RANG_D_ABLATION
                                               ? phytomer::INACTIVE
                                               : phytomer::ACTIVE,
                                           age_at_creation,
                                           age,
                                           production_speed,
                                           TT_ini_flowering,
                                           TT_ini_harvest,
                                           inflo_dev_factor
                                           );

        setsubmodel(PHYTOMERS, phytomer);
        phytomer->init(t, _parameters);
        phytomers.push_back(phytomer);
        phytomerNumber += 1;
    }

    void compute_SF( double t ){
        totalLeafArea = 0;

        //surface
//        auto it = phytomers.begin();
//        while (it != phytomers.end()) {
//            Phytomer* phytomer = (*it);
//            if (phytomer->get< phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) != phytomer::DEAD)
//                totalLeafArea += phytomer->leaf_model()->get < double >(t, Leaf::LEAFAREA);

//        }
    }



    void compute_biomasse_prod(double t) {
        double Rg = meteo->get<double>(t, Meteo::RG);
        double FTSW = bh->get<double>(t, WaterBalance::FTSW);
        lai = totalLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);
        double factor = FTSW > SEUIL_PHOTO ? 1 : FTSW / SEUIL_PHOTO;
        biomasse_prod = factor * 10 * ei * EFFICIENCE_BIOLOGIQUE * 0.48 * Rg / DENS;//  # on multiplie par 10 pour passer en kg
    }

    void update_organs(double t) {
        //       #### defiolation le jour de la mise en place des traitements
        if (t == DEBUT_DEFOLIATON ) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                int rank = phytomer->get< double, Phytomer >(t, Phytomer::RANK);
                phytomer::phytomer_state state = phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE);
                if(rank > RANG_DEFOLIATON && state == phytomer::ACTIVE) {
                    ;//TODO ABLATION  phytomers[key].leaf.ablation = phytomers[key].leaf.defoliation_decision(POURC_DEFOLIATON)
                }
            }
        }

        // ### ablation des regimes le jour de la mise en place des traitements
        if (t == DEBUT_ABLATION_REGIME ) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                int rank = phytomer->get < double, Phytomer >(t, Phytomer::RANK);
                inflo::inflo_states bunch_state = phytomer->inflo_model()->get <inflo::inflo_states, Inflo >(t, Inflo::STATUS);
                inflo::inflo_sex sex = phytomer->inflo_model()->get <inflo::inflo_sex, Inflo >(t, Inflo::SEX);
                inflo::inflo_sex avort = phytomer->inflo_model()->get <inflo::inflo_sex, Inflo >(t, Inflo::AVORT);
                if(rank >= RANG_D_ABLATION_REGIME
                        && bunch_state.is(inflo::RECOLTE)
                        && sex ==  inflo::FEMALE
                        && avort == inflo::NON_ABORTED)
                    ;//TODO ABLATION  phytomers[key].leaf.ablation = phytomers[key].leaf.defoliation_decision(POURC_DEFOLIATON)

            }
        }

        if (newPhytomerEmergence >= 1) {
            create_phytomer(phytomerNumber + 1, age);
            newPhytomerEmergence -= 1;
        }
    }

    void compute(double t, bool /* update */)
    {
        age = age + 1 ;

        (*meteo)(t);

        //        if simulation.meteo.irrigation_day == "IRRIGATION" :
        //            pluie = simulation.meteo.Rain + simulation.meteo.irrigation[simulation.meteo.jour_julien]
        //        else :
        //            pluie = simulation.meteo.Rain
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC, rootTailleC);
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1, rootTailleC1);
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC2, rootTailleC2);
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1MOINSVAP, rootTailleC1MVAP);
        bh->put<double>(t, WaterBalance::RACINES_TAILLEVAP, rootTailleVAP);
        bh->put<double>(t, WaterBalance::ET0, meteo->get<double>(t, Meteo::ET0));
        bh->put<double>(t, WaterBalance::TREE_EI, ei);
        (*bh)(t);

        double TEff = meteo->get<double>(t, Meteo::TEFF);
        double ftsw = bh->get<double>(t, WaterBalance::FTSW);

        //Execution phythomers
        auto it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            phytomer->put(t, Phytomer::YOUNG_PHYTO_NB, phytomers.size());

//            if(phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::ACTIVE) {
//                phytomer->leaf_model()->put(t, Leaf::TEFF, TEff);
//                phytomer->leaf_model()->put(t, Leaf::FTSW, ftsw);
//                phytomer->leaf_model()->put(t, Leaf::FR_RESTE, fr_reste);
//                phytomer->leaf_model()->put(t, Leaf::DELTA_BIOMASSE_RESERVE_LEAF, reserve->get<double>(t-1, Reserve::DELTA_BIOMASSE_RESERVE_LEAF));

//                phytomer->internode_model()->put(t, Internode::TEFF, TEff);
//                phytomer->internode_model()->put(t, Internode::PRODUCTIONSPEED, production_speed);
//                phytomer->internode_model()->put(t, Internode::FR_RESTE, fr_reste);

//                phytomer->inflo_model()->put(t, Inflo::TEFF, TEff);
//                phytomer->inflo_model()->put(t, Inflo::FTSW, ftsw);
//                phytomer->inflo_model()->put(t, Inflo::FR_FRUITS, fr_fruits);
//                phytomer->inflo_model()->put(t, Inflo::FR_RESTE, fr_reste);
//                phytomer->inflo_model()->put(t, Inflo::DATE_PLUS_JEUNE_FEUILLE, date_plus_jeune_feuille);
//                phytomer->inflo_model()->put(t, Inflo::TREE_PRODUCTIONSPEED, production_speed);
//                phytomer->inflo_model()->put(t, Inflo::TREE_IC, ic);
//            }
            (*phytomer)(t);
        }






//        production_speed = max(MINIMAL_PRODUCTION_SPEED, (-DECREASE_OF_PRODUCTION_SPEED * (t - _parameters.beginDate) ) + PRODUCTION_SPEED_INITIAL); //DD.rang-1

        newPhytomerEmergence += TEff * production_speed * pow(ic,VITESSE_SENSITIVITY) * ( ftsw > SEUIL_ORGANO ? 1 : ftsw / SEUIL_ORGANO);
        update_organs(t);
        (*racines)(t);
        rootTailleC = racines->get<double>(t, Racines::TAILLEC);
        rootTailleC1 = racines->get<double>(t, Racines::TAILLEC1);
        rootTailleC2 = racines->get<double>(t, Racines::TAILLEC2);
        rootTailleC1MVAP = racines->get<double>(t, Racines::TAILLEC1MOINSVAP);
        rootTailleVAP = racines->get<double>(t, Racines::TAILLEVAP);

        //Compute results
        //reset vars
        trunk_height = INITIAL_HEIGHT;
        leaf_demand = 0;
        internode_demand = 0;
        inflo_demand = 0;
        bunch_demand = 0;
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
                inflo_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BIOMASS); //## attention pour la respi de maintenance !!!
                respirable_bunch_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::RESPIRABLE_BIOMASS);
                female_bunch_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::FEMELLE_BIOMASS);
                male_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_BIOMASS);

                trunk_height += phytomer->internode_model()->get <double>(t, Internode::LENGTH);

                internode_demand +=  phytomer->internode_model()->get <double>(t, Internode::DEMAND);
                leaf_demand += phytomer->leaf_model()->get <double>(t, Leaf::DEMAND);
                inflo_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::DEMAND);
                bunch_demand += phytomer->inflo_model()->bunch_model()->get <double>(t, Bunch::DEMAND);
                male_demand += phytomer->inflo_model()->male_model()->get <double>(t, MaleInflo::DEMAND);
                peduncule_demand += phytomer->inflo_model()->peduncle_model()->get <double>(t, Peduncle::DEMAND);
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

        double demand = internode_demand + leaf_demand + respi_maintenance + inflo_demand;
        growth_demand =  internode_demand +   leaf_demand  +  inflo_demand;
        ic = Assim/demand;
        ics.push_back(ic);

        Assim = biomasse_prod;


        // compute etat reserve
        //set Tree Assim
        (*reserve)(t);

        //       compute_offre_nette
        offre_nette = reserve->get<double>(t, Reserve::ASSIM_AVAILABLE); //TODO appliquer REALL_COST
//        fraction_pour_croissance = offre_nette / (growth_demand);

        //       compute_fraction_oil_reste
        double sum_organs_demand = leaf_demand + internode_demand + male_demand + peduncule_demand;
        double demand_growth_corrected = sum_organs_demand + (AF_FRUITS * bunch_demand);

//        if (AF_FRUITS >= 1) {
        offre_fruits = min( ( (AF_FRUITS * bunch_demand) * offre_nette / demand_growth_corrected ), bunch_demand );

        if(bunch_demand==0) {
            fr_reste = offre_nette / sum_organs_demand;
            fr_fruits = 1;
        } else {

        }

        if (bunch_demand==0) {
            fr_fruits = 1;
            offre_reste = offre_nette - offre_fruits;
            fr_reste = offre_reste / sum_organs_demand;
        } else {
            fr_fruits = offre_fruits / bunch_demand;
            offre_reste = offre_nette - offre_fruits;
            fr_reste = offre_reste / sum_organs_demand;
        }
//        } else {

//            offre_reste = min( sum_organs_demand * ( offre_reste / demand_growth_corrected ),
//                               sum_organs_demand);

//            fr_reste = offre_reste / sum_organs_demand;
//            offre_fruits = offre_nette - offre_reste;
//            fr_fruits = offre_fruits/ fruit_demand;
//        }

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
