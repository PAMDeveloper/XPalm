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
    enum submodels { RACINES, RESERVE, BH, METEO, PHYTOMERS };

    enum internals { TREE_AGE,
                     TRUNK_INITIAL_HEIGHT,
                     TRUNK_INITIAL_BIOMASS,
                     LAI,
                     EI,
                     IC,
                     ICS,
                     PLANTLEAFAREA,
                     TRUNK_HEIGHT,
                     TOTAL_LEAVES_BIOMASS,
                     TOTAL_LEAVES_BIOMASS_HARVESTED,
                     TRUNK_BIOMASS,
                     LEAVES_NON_STRUCTURAL_BIOMASS,
                     LEAVES_RESERVE_MAX,
                     LEAVES_RESERVE_EXCESS,
                     LEAVES_STRUCTURAL_BIOMASS,
                     LEAVES_NON_STRUCTURAL_BIOMASS_HARVESTED,
                     LEAVES_STRUCTURAL_BIOMASS_HARVESTED,
                     RESPIRABLE_REPRO_BIOMASS,
                     BUNCH_OIL_BIOMASS,
                     BUNCH_NONOIL_BIOMASS,
                     BUNCH_OIL_BIOMASS_HARVESTED,
                     BUNCH_NONOIL_BIOMASS_HARVESTED,
                     ASSIM,
                     RESPI_MAINTENANCE,
                     PHYTOMERNUMBER,
                     NEWPHYTOMEREMERGENCE,
                     FR_FRUITS,
                     FR_RESTE,
                     OFFRE_FRUITS,
                     OFFRE_NETTE,
                     OFFRE_RESTE,
                     GROWTH_DEMAND,
                     BUNCH_DEMAND,
                     INTERNODE_DEMAND,
                     LEAVES_DEMAND,
                     MALE_DEMAND,
                     MALE_BIOMASS,
                     MALE_BIOMASS_HARVESTED,
                     PEDUNCLE_BIOMASS,
                     PEDUNCLE_BIOMASS_HARVESTED,
                     PEDUNCLE_DEMAND,
                     C_BALANCE,
                     FRACTION_NON_STR_BIOMASS_TOTAL,
                     ASSIM_EXCESS};

private:
    xpalm::ModelParameters _parameters;

    //      parameters
    double RANG_D_ABLATION;
    double INACTIVE_PHYTOMER_NUMBER;
    double DEBUT_CROISSANCE_EN;
    double FIN_CROISSANCE_EN;
    double EN_LENGTH_ADULTE;
    double EN_LENGTH_INI;
    double TEFF_INI;
    double PRODUCTION_SPEED_ADULT;
    double PRODUCTION_SPEED_INITIAL;
    double AGE_INI;
    double AGE_ADULT;
    double AGE_PLANTING;
    double AGE_START_PROD;
    double TT_FLOWERING_INITIAL;
    double TT_FLOWERING_ADULT;
    double TT_HARVEST_INITIAL;
    double TT_HARVEST_ADULT;
    double PERIOD_MALE_INFLO;
    double MAXIMAL_SFIND;
    double INITIAL_SFIND;
    double DENS;
    double K;
    double AF_FRUITS;
    double COUT_RESPI_MAINTENANCE_REPRO;
    double COUT_RESPI_MAINTENANCE_LEAF;
    double COUT_RESPI_MAINTENANCE_STIPE;
    double EFFICIENCE_BIOLOGIQUE;
//    double SEUIL_ORGANO;
    //    double SEUIL_PHOTO;
    //    double VITESSE_SENSITIVITY;
    double SEED;
    double TRESH_SLOW_PHYLO;

    //     submodels
    std::deque < Phytomer* > phytomers;
    std::unique_ptr < Reserve > reserve;
    std::unique_ptr < Racines > racines;
    std::unique_ptr < WaterBalance > bh;
    std::unique_ptr < Meteo > meteo;

    //     internals
    std::vector<double> ics;

    double age;
    double trunk_initial_height;
    double trunk_initial_biomass;
    double trunk_height;
    double trunk_biomass;
    double trunk_initial_res;
    double plantLeafArea;
    double lai;
    double ei;
    double assim;
    double ic;
    double respi_maintenance;
    double phytomerNumber;
    double newPhytomerEmergence;
    double fr_fruits;
    double fr_reste;
    double internode_demand;
    double offre_fruits;
    double offre_nette;
    double offre_reste;
    double C_balance;
    double total_leaves_biomass;
    double total_leaves_biomass_harvested;
    double leaves_non_structural_biomass;
    double leaves_reserve_max;
    double leaves_reserve_excess;
    double leaves_structural_biomass;
    double leaves_non_structural_biomass_harvested;
    double leaves_structural_biomass_harvested;
    double leaves_demand;
    double male_biomass;
    double male_biomass_harvested;
    double male_demand;
    double peduncle_biomass;
    double peduncle_biomass_harvested;
    double peduncle_demand;
    double respirable_repro_biomass;
    double bunch_oil_biomass;
    double bunch_oil_biomass_harvested;
    double bunch_non_oil_biomass;
    double bunch_non_oil_biomass_harvested;
    double growth_demand;
    double bunch_demand;
    double fraction_non_str_biomass_total;
    double delta_reserve_leaves;
    double assim_excess;

public:

    Tree() :
        reserve(new Reserve),
        racines(new Racines),
        meteo(new Meteo),
        bh(new WaterBalance())
    {
        //         submodels
        submodel(RACINES, racines.get());
        submodel(RESERVE, reserve.get());
        submodel(BH, bh.get());
        submodel(METEO, meteo.get());

        //         internals
        Internal(TREE_AGE, &Tree::age);
        Internal(TRUNK_INITIAL_HEIGHT, &Tree::trunk_initial_height);
        Internal(TRUNK_INITIAL_BIOMASS, &Tree::trunk_initial_biomass);
        Internal(LAI, &Tree::lai);
        Internal(EI, &Tree::ei);
        Internal(IC, &Tree::ic);
        Internal(ICS, &Tree::ics);
        Internal(PLANTLEAFAREA, &Tree::plantLeafArea);
        Internal(TRUNK_HEIGHT, &Tree::trunk_height);
        Internal(TOTAL_LEAVES_BIOMASS, &Tree::total_leaves_biomass);
        Internal(TOTAL_LEAVES_BIOMASS_HARVESTED, &Tree::total_leaves_biomass_harvested);
        Internal(TRUNK_BIOMASS, &Tree::trunk_biomass);
        Internal(LEAVES_NON_STRUCTURAL_BIOMASS, &Tree::leaves_non_structural_biomass);
        Internal(LEAVES_RESERVE_MAX, &Tree::leaves_reserve_max);
        Internal(LEAVES_RESERVE_EXCESS, &Tree::leaves_reserve_excess);
        Internal(LEAVES_STRUCTURAL_BIOMASS, &Tree::leaves_structural_biomass);
        Internal(LEAVES_NON_STRUCTURAL_BIOMASS_HARVESTED, &Tree::leaves_non_structural_biomass_harvested);
        Internal(LEAVES_STRUCTURAL_BIOMASS_HARVESTED, &Tree::leaves_structural_biomass_harvested);
        Internal(RESPIRABLE_REPRO_BIOMASS, &Tree::respirable_repro_biomass);
        Internal(BUNCH_OIL_BIOMASS, &Tree::bunch_oil_biomass);
        Internal(BUNCH_NONOIL_BIOMASS, &Tree::bunch_non_oil_biomass);
        Internal(BUNCH_OIL_BIOMASS_HARVESTED, &Tree::bunch_oil_biomass_harvested);
        Internal(BUNCH_NONOIL_BIOMASS_HARVESTED, &Tree::bunch_non_oil_biomass_harvested);
        Internal(ASSIM, &Tree::assim);
        Internal(RESPI_MAINTENANCE, &Tree::respi_maintenance);
        Internal(PHYTOMERNUMBER, &Tree::phytomerNumber);
        Internal(NEWPHYTOMEREMERGENCE, &Tree::newPhytomerEmergence);
        Internal(FR_FRUITS, &Tree::fr_fruits);
        Internal(FR_RESTE, &Tree::fr_reste);
        Internal(OFFRE_FRUITS, &Tree::offre_fruits);
        Internal(OFFRE_NETTE, &Tree::offre_nette);
        Internal(OFFRE_RESTE, &Tree::offre_reste);
        Internal(GROWTH_DEMAND, &Tree::growth_demand);
        Internal(BUNCH_DEMAND, &Tree::bunch_demand);
        Internal(INTERNODE_DEMAND, &Tree::internode_demand);
        Internal(LEAVES_DEMAND, &Tree::leaves_demand);
        Internal(MALE_DEMAND, &Tree::male_demand);
        Internal(MALE_BIOMASS, &Tree::male_biomass);
        Internal(MALE_BIOMASS_HARVESTED, &Tree::male_biomass_harvested);
        Internal(PEDUNCLE_BIOMASS, &Tree::peduncle_biomass);
        Internal(PEDUNCLE_BIOMASS_HARVESTED, &Tree::peduncle_biomass_harvested);
        Internal(PEDUNCLE_DEMAND, &Tree::peduncle_demand);
        Internal(C_BALANCE, &Tree::C_balance);
        Internal(FRACTION_NON_STR_BIOMASS_TOTAL, &Tree::fraction_non_str_biomass_total);
        Internal(ASSIM_EXCESS, &Tree::assim_excess);

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

        //        parameters
        AGE_INI=parameters.get("AGE_INI");
        PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT"); // (rank.DD-1)
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL"); // (rank.DD-1)
        AGE_ADULT = parameters.get("AGE_ADULT") * 365; // days
        AGE_PLANTING = parameters.get("AGE_PLANTING") * 365; // days (age when production speed starts to decrease
        AGE_START_PROD = parameters.get("AGE_START_PROD") * 365;
        DEBUT_CROISSANCE_EN = parameters.get("DEBUT_CROISSANCE_EN")* 365;
        FIN_CROISSANCE_EN= parameters.get("FIN_CROISSANCE_EN")* 365;
        TEFF_INI = parameters.get("T_EFF_INI");
        EN_LENGTH_INI=parameters.get("EN_LENGTH_INI");
        EN_LENGTH_ADULTE =parameters.get("EN_LENGTH_ADULTE");
        TT_FLOWERING_INITIAL = parameters.get("TT_FLOWERING_INITIAL");
        TT_FLOWERING_ADULT = parameters.get("TT_FLOWERING_ADULT");
        TT_HARVEST_INITIAL = parameters.get("TT_HARVEST_INITIAL");
        TT_HARVEST_ADULT = parameters.get("TT_HARVEST_ADULT");
        PERIOD_MALE_INFLO =parameters.get("PERIOD_MALE_INFLO");
        MAXIMAL_SFIND  = parameters.get("MAXIMAL_SFIND");
        INITIAL_SFIND  = parameters.get("INITIAL_SFIND");
        AF_FRUITS = parameters.get("AF_FRUITS");
        COUT_RESPI_MAINTENANCE_REPRO = parameters.get("COUT_RESPI_MAINTENANCE_REPRO");
        COUT_RESPI_MAINTENANCE_LEAF = parameters.get("COUT_RESPI_MAINTENANCE_LEAF");
        COUT_RESPI_MAINTENANCE_STIPE = parameters.get("COUT_RESPI_MAINTENANCE_STIPE");
        DENS = parameters.get("DENS");
        K=parameters.get("K");
        EFFICIENCE_BIOLOGIQUE = parameters.get("EFFICIENCE_BIOLOGIQUE");
        RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");
        //        SEUIL_ORGANO = parameters.get("SEUIL_ORGANO");
        //        SEUIL_PHOTO = parameters.get("SEUIL_PHOTO");
        //        VITESSE_SENSITIVITY = parameters.get("VITESSE_SENSITIVITY");
        SEED= parameters.get("SEED");
        TRESH_SLOW_PHYLO=parameters.get("TRESH_SLOW_PHYLO");

        //        internals
        newPhytomerEmergence = 0;
        phytomerNumber = INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION;
        ic = 1;
        ei = 0;

        plantLeafArea = trunk_height  = total_leaves_biomass = total_leaves_biomass_harvested = trunk_biomass =
                leaves_non_structural_biomass = leaves_structural_biomass = leaves_structural_biomass_harvested = leaves_non_structural_biomass_harvested =
                respirable_repro_biomass = bunch_oil_biomass =bunch_oil_biomass_harvested = bunch_non_oil_biomass= bunch_non_oil_biomass_harvested = assim =
                offre_fruits = offre_nette = growth_demand = bunch_demand = internode_demand =
                leaves_demand = male_demand  = male_biomass = male_biomass_harvested = peduncle_demand =offre_reste =
                peduncle_biomass = peduncle_biomass_harvested=respi_maintenance= leaves_reserve_excess=leaves_reserve_max=0;

        fr_fruits = fr_reste = 0.5;

        // init random generator
        srand(SEED);

        //init structure
        age =AGE_INI * 365;
        double production_speed = age_relative_var(age, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);

        int nb_phyto = INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION;
        double age_at_creation = age;
        for ( int i =0 ; i<nb_phyto ; ++i ) {
            create_phytomer(t-1, -i, phytomerNumber, age_at_creation);
            //update production speed for each phytomer
            production_speed = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
            //update age at phytomer creation
            age_at_creation -= 1 / (TEFF_INI * production_speed);
        }

        meteo->init(t, parameters);

        racines->init(t, parameters);
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC, racines->get<double>(t-1, Racines::TAILLEC));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1, racines->get<double>(t-1, Racines::TAILLEC1));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC2, racines->get<double>(t-1, Racines::TAILLEC2));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1MOINSVAP, racines->get<double>(t-1, Racines::TAILLEC1MOINSVAP));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEVAP, racines->get<double>(t-1, Racines::TAILLEVAP));
        bh->put<double>(t, WaterBalance::ET0, meteo->get<double>(t-1, Meteo::ET0));
        bh->put<double>(t, WaterBalance::TREE_EI, ei);
        bh->init(t, parameters);


        // plant leaf area
        auto it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            plantLeafArea += phytomer->leaf_model()->get<double>(t-1, Leaf::LEAFAREA);
            ++it;
        }
        lai = plantLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);

        //init trunk dim
        trunk_initial_height=0;
        if (age < DEBUT_CROISSANCE_EN){
            trunk_initial_height = EN_LENGTH_INI * age * TEFF_INI * PRODUCTION_SPEED_INITIAL ;
        }
        else if (age < FIN_CROISSANCE_EN){
            trunk_initial_height= EN_LENGTH_INI * DEBUT_CROISSANCE_EN * TEFF_INI * PRODUCTION_SPEED_INITIAL +
                    (age-DEBUT_CROISSANCE_EN) * TEFF_INI * ((PRODUCTION_SPEED_INITIAL+PRODUCTION_SPEED_ADULT)/2);
        }
        else
            trunk_initial_height= EN_LENGTH_INI * DEBUT_CROISSANCE_EN * TEFF_INI * PRODUCTION_SPEED_INITIAL +
                    ((EN_LENGTH_INI+EN_LENGTH_ADULTE)/2)*(FIN_CROISSANCE_EN-DEBUT_CROISSANCE_EN) * TEFF_INI * ((PRODUCTION_SPEED_INITIAL+PRODUCTION_SPEED_ADULT)/2)+
                    EN_LENGTH_ADULTE*(age-FIN_CROISSANCE_EN)* TEFF_INI * PRODUCTION_SPEED_ADULT;


        double STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        double STEM_RAYON = parameters.get("STEM_RAYON"); // cm

        trunk_initial_biomass = STEM_APPARENT_DENSITY * _PI * pow( STEM_RAYON, 2) * trunk_initial_height; //gDM

        double POURCENT_NSC_ST_INI = parameters.get("POURCENT_NSC_ST_INI") ; //g.cm-2
        trunk_initial_res= POURCENT_NSC_ST_INI * trunk_initial_biomass;


        double SLW_ini = parameters.get("SLW_ini") ; //g.cm-2
        double SLW_min = parameters.get("SLW_min") ; //g.cm-2
        double POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");

        leaves_structural_biomass = plantLeafArea *10000  * SLW_min / POURC_FOLIOLE;  //gDM
        leaves_non_structural_biomass = plantLeafArea *10000 * (SLW_ini - SLW_min) / POURC_FOLIOLE; //gDM
        total_leaves_biomass = leaves_structural_biomass + leaves_non_structural_biomass;
        reserve->init(t, parameters, plantLeafArea, trunk_initial_height,leaves_non_structural_biomass);
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


    void create_phytomer(double t, double number, double total_phyto, double age_at_creation) {
        //        double rank = total_phyto - INACTIVE_PHYTOMER_NUMBER - number - 1;

        double production_speed = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
        double TT_ini_flowering = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, TT_FLOWERING_INITIAL, TT_FLOWERING_ADULT);
        double TT_ini_harvest = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, TT_HARVEST_INITIAL, TT_HARVEST_ADULT);
        double inflo_dev_factor = age_relative_var(age_at_creation, AGE_START_PROD, AGE_ADULT, 0, 1);
        double TT_ini_male_senescence = TT_ini_flowering +PERIOD_MALE_INFLO;

        //        Leaf area increase with plant age
        double SF_ind = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, INITIAL_SFIND , MAXIMAL_SFIND);

        Phytomer * phytomer = new Phytomer();
        setsubmodel(PHYTOMERS, phytomer);
        phytomer->init(t, _parameters, number, total_phyto,
                       (number > RANG_D_ABLATION),
                       age_at_creation,
                       age,
                       production_speed,
                       TT_ini_flowering,
                       TT_ini_harvest,
                       TT_ini_male_senescence,
                       inflo_dev_factor,
                       SF_ind);

        phytomers.push_back(phytomer);
        //        phytomerNumber += 1;
    }




    void compute(double t, bool /* update */)
    {
        age = age + 1 ;

        // split leaves reserve: assign the fraction of reserve for each leaf

        compute_biomasse_non_structurale_allouee_aux_feuilles(t);

//        std::cout << "Age:" << age << std::endl;
        (*meteo)(t);

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
            phytomer->put < double >(t, Phytomer::TOTAL_PHYTOMER_NUMBER, phytomers.size());
            phytomer->put < double >(t, Phytomer::TREE_IC, ic);
            phytomer->put < double >(t, Phytomer::TEFF, TEff);

            phytomer->internode_model()->put<double>(t, Internode::TEFF, TEff);
            phytomer->internode_model()->put<double>(t, Internode::FR_RESTE, fr_reste);

            phytomer->leaf_model()->put<double>(t, Leaf::FTSW, ftsw);
            phytomer->leaf_model()->put<double>(t, Leaf::FR_RESTE, fr_reste);
            phytomer->leaf_model()->put<double>(t, Leaf::TEFF, TEff);
            phytomer->leaf_model()->put<double>(t, Leaf::LEAVES_RES_AVAI, reserve->get<double>(t-1, Reserve::LEAVES_RES_AVAI));

            phytomer->inflo_model()->put<double>(t, Inflo::FTSW, ftsw);
            phytomer->inflo_model()->put<double>(t, Inflo::FR_FRUITS, fr_fruits);
            phytomer->inflo_model()->put<double>(t, Inflo::FR_RESTE, fr_reste);
            phytomer->inflo_model()->put<double>(t, Inflo::TEFF, TEff);

            (*phytomer)(t);
            ++it;
        }

        racines->put(t, Racines::TEFF, TEff);
        racines->put(t, Racines::FTSW, ftsw);
        (*racines)(t);


        //init structure
        plantLeafArea = 0;
        trunk_height=trunk_initial_height;

        //init biomass
        trunk_biomass= trunk_initial_biomass; //gDM;
        leaves_structural_biomass=0;
        leaves_reserve_max=0;
        leaves_reserve_excess=0;
        leaves_non_structural_biomass=0;
        leaves_structural_biomass_harvested=0;
        leaves_non_structural_biomass_harvested=0;

        male_biomass=0;
        male_biomass_harvested=0;
        peduncle_biomass=0;
        peduncle_biomass_harvested=0;
        respirable_repro_biomass=0;
        bunch_non_oil_biomass=0;
        bunch_non_oil_biomass_harvested=0;
        bunch_oil_biomass=0;
        bunch_oil_biomass_harvested=0;

        //init demand
        internode_demand = 0;
        leaves_demand = 0;
        bunch_demand = 0;
        male_demand = 0;
        peduncle_demand = 0;

        it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);

            //            if(phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) != phytomer::DEAD) {
            //stem
            trunk_height += phytomer->internode_model()->get <double>(t, Internode::LENGTH);
            trunk_biomass += phytomer->internode_model()->get <double>(t, Internode::BIOMASS);
            internode_demand +=  phytomer->internode_model()->get <double>(t, Internode::DEMAND);

            //leaves
            plantLeafArea += phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA);
            leaves_structural_biomass += phytomer->leaf_model()->get <double>(t, Leaf::LEAF_STRUCTURAL_BIOMASS); //g
            leaves_non_structural_biomass += phytomer->leaf_model()->get <double>(t, Leaf::LEAF_NON_STRUCTURAL_BIOMASS); //g
            leaves_reserve_max += phytomer->leaf_model()->get <double>(t, Leaf::LEAF_RESERVE_MAX); //g
            leaves_reserve_excess += phytomer->leaf_model()->get <double>(t, Leaf::RESERVE_EXCESS); //g
            leaves_structural_biomass_harvested += phytomer->leaf_model()->get <double>(t, Leaf::LEAF_STRUCTURAL_BIOMASS_HARVESTED);
            leaves_non_structural_biomass_harvested += phytomer->leaf_model()->get <double>(t, Leaf::LEAF_NON_STRUCTURAL_BIOMASS_HARVESTED);
            leaves_demand += phytomer->leaf_model()->get <double>(t, Leaf::LEAF_DEMAND);

            //inflo
            //            inflo_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BIOMASS); //## attention pour la respi de maintenance !!!
            respirable_repro_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::RESPIRABLE_BIOMASS);

            male_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_BIOMASS);
            male_biomass_harvested += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_BIOMASS_HARVESTED);
            male_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_DEMAND);

            peduncle_biomass+= phytomer->inflo_model()->get <double, Inflo>(t, Inflo::PEDUNCLE_BIOMASS);
            peduncle_biomass_harvested+= phytomer->inflo_model()->get <double, Inflo>(t, Inflo::PEDUNCLE_BIOMASS_HARVESTED);
            peduncle_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::PEDUNCLE_DEMAND);

            bunch_oil_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_OIL_BIOMASS);
            bunch_oil_biomass_harvested += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_OIL_BIOMASS_HARVESTED);
            bunch_non_oil_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_NONOIL_BIOMASS);
            bunch_non_oil_biomass_harvested += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_NONOIL_BIOMASS_HARVESTED);

            //            inflo_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::DEMAND);
            bunch_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_DEMAND);

            ++it;
        }

        // compute total harvested biomass
        total_leaves_biomass_harvested = leaves_structural_biomass_harvested + leaves_non_structural_biomass_harvested;
        total_leaves_biomass = leaves_structural_biomass + leaves_non_structural_biomass - total_leaves_biomass_harvested;


        // compute maintenance respiration
        double TMoy = (_parameters.get(t).TMax + _parameters.get(t).TMin) / 2;
        double Q10 = pow(2, (TMoy - 25)/10);
        respi_maintenance = Q10 * (
                    trunk_biomass * COUT_RESPI_MAINTENANCE_STIPE +
                    respirable_repro_biomass * COUT_RESPI_MAINTENANCE_REPRO +
                    leaves_structural_biomass * COUT_RESPI_MAINTENANCE_LEAF);

        // compute growth demand
        growth_demand =  internode_demand +   leaves_demand  +  bunch_demand + male_demand + peduncle_demand;


        // compute competition index
        ic = assim/growth_demand;
        ics.push_back(ic);

        double Rg = meteo->get<double>(t, Meteo::RG);
        lai = plantLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);
        //        double factor = ftsw > SEUIL_PHOTO ? 1 : ftsw / SEUIL_PHOTO; //reducing factor when FTSW < S
        //        Assim = factor * 10 * ei * EFFICIENCE_BIOLOGIQUE * 0.48 * Rg / DENS;//  # on multiplie par 10 pour passer en kg

        assim = ei * EFFICIENCE_BIOLOGIQUE * 0.48 * Rg / (DENS/10000);//  #gCH2O.day-1.plant-1

        // update reserve
        reserve->put(t, Reserve::PLANTLEAFAREA, plantLeafArea);
        reserve->put(t, Reserve::TRUNK_BIOMASS, trunk_biomass);
        reserve->put(t, Reserve::LEAVES_NON_STRUCTURAL_BIOMASS, leaves_non_structural_biomass);
        reserve->put(t, Reserve::LEAVES_RESERVE_MAX, leaves_reserve_max);
        reserve->put(t, Reserve::LEAVES_RESERVE_EXCESS, leaves_reserve_excess);
        reserve->put(t, Reserve::MAINTENANCE_RESPI, respi_maintenance);
        reserve->put(t, Reserve::GROWTH_DEMAND, growth_demand);
        reserve->put(t, Reserve::ASSIM, assim);
        (*reserve)(t);

        // check carbon balance
        double assim_to_min_res = reserve->get <double>(t, Reserve::ASSIM_TO_MIN_RES);
        double assim_to_respi=reserve->get <double>(t, Reserve::ASSIM_TO_RESPI);
        double res_to_respi=reserve->get <double>(t, Reserve::RES_TO_RESPI);
        double assim_to_growth=reserve->get <double>(t, Reserve::ASSIM_TO_GROWTH);
        double res_to_growth=reserve->get <double>(t, Reserve::RES_TO_GROWTH);
        double assim_to_res=reserve->get <double>(t, Reserve::ASSIM_TO_RES);
        assim_excess=reserve->get <double>(t, Reserve::ASSIM_EXCESS);


        C_balance= assim - assim_to_min_res - assim_to_respi - assim_to_growth - assim_to_res - assim_excess;



        //                C_balance= assim + res_to_growth + res_to_respi - growth_demand - respi_maintenance - surplus_assim - assim_to_res;

        //                if (C_balance>0.1){
        //                    C_balance=C_balance;
        //                }


        //       compute_offre_nette
        offre_nette = reserve->get<double>(t, Reserve::GROWTH_OFFER);
        //        fraction_pour_croissance = offre_nette / (growth_demand);

        //       compute_fraction_oil_reste
        double sum_organs_demand = leaves_demand + internode_demand + male_demand + peduncle_demand;
        double bunch_demand_corrected = AF_FRUITS * bunch_demand;
        double fr_fruits_corrected= bunch_demand_corrected/(sum_organs_demand+bunch_demand_corrected);

        offre_fruits = min( fr_fruits_corrected * offre_nette, bunch_demand );
        offre_reste = offre_nette-offre_fruits;
        fr_fruits = offre_fruits/(sum_organs_demand+offre_fruits);
        fr_reste=1-fr_fruits;

//        if (fr_reste<0 | fr_reste>1){
//            fr_reste=fr_reste;
//        }

        double production_speed = age_relative_var(age, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
        //        newPhytomerEmergence += TEff * production_speed * pow(ic,VITESSE_SENSITIVITY) * ( ftsw > SEUIL_ORGANO ? 1 : ftsw / SEUIL_ORGANO);

        double phylo_slow=(ftsw > TRESH_SLOW_PHYLO
                           ? 1
                           : ftsw / TRESH_SLOW_PHYLO);

        newPhytomerEmergence += TEff * production_speed * phylo_slow;

        if (newPhytomerEmergence >= 1) {
            create_phytomer(t, phytomerNumber-RANG_D_ABLATION-INACTIVE_PHYTOMER_NUMBER+1, phytomerNumber+1, age);
            phytomerNumber += 1;
            newPhytomerEmergence -= 1;
        }

    }



    void compute_biomasse_non_structurale_allouee_aux_feuilles(double t) {

        double reserve_pot_total=0;
        fraction_non_str_biomass_total = 0; //check must equal 1

        // compute total leaf reserve
        double biomasse_reserve_leaves = reserve->get<double>(t-1, Reserve::LEAVES_RES_AVAI);


        if  (biomasse_reserve_leaves > 0) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                double leaf_reserve_pot = phytomer->leaf_model()->get <double>(t-1, Leaf::LEAF_RESERVE_MAX) - phytomer->leaf_model()->get <double>(t-1, Leaf::LEAF_NON_STRUCTURAL_BIOMASS);
                reserve_pot_total += leaf_reserve_pot;
                ++it;
            }

            // estimate the fraction of non structrural biomass to allocate
            it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(reserve_pot_total<=0 | phytomer->leaf_model()->get<double>(t-1, Leaf::LEAFAREA)<=0)
                    phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                else {
                    double leaf_reserve_pot =phytomer->leaf_model()->get <double>(t-1, Leaf::LEAF_RESERVE_MAX) - phytomer->leaf_model()->get <double>(t-1, Leaf::LEAF_NON_STRUCTURAL_BIOMASS);
                    double fraction_non_str_biomasse_allouee = leaf_reserve_pot / reserve_pot_total;

                    phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, fraction_non_str_biomasse_allouee);
                    fraction_non_str_biomass_total += fraction_non_str_biomasse_allouee;
                }
                ++it;
            }
        }
        else {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                ++it;
            }
        }

        delta_reserve_leaves= reserve_pot_total;



    }

};
#endif // TREE_H
