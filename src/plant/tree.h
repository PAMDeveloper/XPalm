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
                     TOTALPHYTOMERNUMBER,
                     NEWPHYTOMEREMERGENCE,
                     FR_FRUITS,
                     FR_RESTE,
                     OFFRE_FRUITS,
                     GROWTH_OFFER,
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
                     ASSIM_EXCESS,
                     DIF_DEMAND_OFFER,
                     RES_STEM,
                     RES_LEAVES,
                     RES_TOTAL,
                     NB_LEAVES,
                     NB_MALES,
                     NB_BUNCHES,
                     NB_ABORTED,
                     LASTLEAFAREA,
                     NEWPHYTOMER,
                     AV_SEXRATIO,
                     AV_ABORTION};

private:
    xpalm::ModelParameters _parameters;

    //      parameters
    double LEAF_PRUNING_RANK;
    double INACTIVE_PHYTOMER_NUMBER;
    double AGE_INTERNODE_START_GROWTH;
    double AGE_INTERNODE_STOP_GROWTH;
    double INTERNODE_LENGTH_ADULT;
    double INTERNODE_LENGTH_INI;
    double TEFF_INI;
    double PRODUCTION_SPEED_ADULT;
    double PRODUCTION_SPEED_INITIAL;
    double AGE_INI;
    double AGE_ADULT;
    //    double AGE_PLANTING;
    double AGE_START_PROD;
    double TT_FLOWERING_INITIAL;
    double TT_FLOWERING_ADULT;
    double TT_HARVEST_INITIAL;
    double TT_HARVEST_ADULT;
    double PERIOD_MALE_INFLO;
    double LEAFAREA_ADULT;
    //    double INITIAL_SFIND;
    double DENS;
    double K;
    double BUNCH_AFFINITY;
    double RESPIRATION_COST_INFLO;
    double RESPIRATION_COST_LEAF_MAINTENANCE;
    double RESPIRATION_COST_STEM;
    double RUE;
    double TRESH_FTSW_PHOTO_REDUC;
    double SEED;
    double TRESH_FTSW_SLOW_PHYLO;

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
    double total_phytomer_number;
    double newPhytomerEmergence;
    double fr_fruits;
    double fr_reste;
    double internode_demand;
    double offre_fruits;
    double growth_offer;
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
    double dif_demand_offer;
    double res_stem;
    double res_leaves;
    double res_total;
    double nb_leaves;
    double nb_males;
    double nb_bunches;
    double nb_aborted;
    double lastLeafArea;
    double newPhytomer;
    double av_sex_ratio;
    double av_abortion_rate;

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
        Internal(TOTALPHYTOMERNUMBER, &Tree::total_phytomer_number);
        Internal(NEWPHYTOMEREMERGENCE, &Tree::newPhytomerEmergence);
        Internal(FR_FRUITS, &Tree::fr_fruits);
        Internal(FR_RESTE, &Tree::fr_reste);
        Internal(OFFRE_FRUITS, &Tree::offre_fruits);
        Internal(GROWTH_OFFER, &Tree::growth_offer);
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
        Internal(DIF_DEMAND_OFFER, &Tree::dif_demand_offer);
        Internal(RES_STEM, &Tree::res_stem);
        Internal(RES_LEAVES, &Tree::res_leaves);
        Internal(RES_TOTAL, &Tree::res_total);
        Internal(NB_LEAVES, &Tree::nb_leaves);
        Internal(NB_MALES, &Tree::nb_males);
        Internal(NB_BUNCHES, &Tree::nb_bunches);
        Internal(NB_ABORTED, &Tree::nb_aborted);
        Internal(LASTLEAFAREA, &Tree::lastLeafArea);
        Internal(NEWPHYTOMER, &Tree::newPhytomer);
        Internal(AV_SEXRATIO, &Tree::av_sex_ratio);
        Internal(AV_ABORTION, &Tree::av_abortion_rate);


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

        PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT"); // (rank.DD-1)
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL"); // (rank.DD-1)
        //        AGE_ADULT = parameters.get("AGE_ADULT") * 365; // days
        //        AGE_START_PROD = parameters.get("AGE_START_PROD") * 365;
        //        AGE_PLANTING = parameters.get("AGE_PLANTING") * 365; // days (age when production speed starts to decrease)
        AGE_INI=parameters.get("AGE_INI")* 365;
        AGE_INTERNODE_START_GROWTH = parameters.get("AGE_INTERNODE_START_GROWTH")* 365;
        AGE_INTERNODE_STOP_GROWTH= parameters.get("AGE_INTERNODE_STOP_GROWTH")* 365;
        TEFF_INI = parameters.get("T_EFF_INI");
        INTERNODE_LENGTH_INI=parameters.get("INTERNODE_LENGTH_INI");
        INTERNODE_LENGTH_ADULT =parameters.get("INTERNODE_LENGTH_ADULT");
        TT_FLOWERING_INITIAL = parameters.get("TT_FLOWERING_INITIAL");
        TT_FLOWERING_ADULT = parameters.get("TT_FLOWERING_ADULT");
        TT_HARVEST_INITIAL = parameters.get("TT_HARVEST_INITIAL");
        TT_HARVEST_ADULT = parameters.get("TT_HARVEST_ADULT");
        PERIOD_MALE_INFLO =parameters.get("PERIOD_MALE_INFLO");
        LEAFAREA_ADULT  = parameters.get("LEAFAREA_ADULT");
        //        INITIAL_SFIND  = parameters.get("INITIAL_SFIND"); //SF at AGE_INI
        BUNCH_AFFINITY = parameters.get("BUNCH_AFFINITY");
        RESPIRATION_COST_INFLO = parameters.get("RESPIRATION_COST_INFLO");
        RESPIRATION_COST_LEAF_MAINTENANCE = parameters.get("RESPIRATION_COST_LEAF_MAINTENANCE");
        RESPIRATION_COST_STEM = parameters.get("RESPIRATION_COST_STEM");
        DENS = parameters.get("DENS");
        K=parameters.get("K");
        RUE = parameters.get("RUE");
        LEAF_PRUNING_RANK = parameters.get("LEAF_PRUNING_RANK");
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");
        TRESH_FTSW_PHOTO_REDUC = parameters.get("TRESH_FTSW_PHOTO_REDUC");
        SEED= parameters.get("SEED");
        TRESH_FTSW_SLOW_PHYLO=parameters.get("TRESH_FTSW_SLOW_PHYLO");

        //        internals
        newPhytomerEmergence = 0;


        ic = 1;
        ei = 0;

        plantLeafArea =lastLeafArea = trunk_height  = total_leaves_biomass = total_leaves_biomass_harvested = trunk_biomass =
                leaves_non_structural_biomass = leaves_structural_biomass = leaves_structural_biomass_harvested = leaves_non_structural_biomass_harvested =
                respirable_repro_biomass = bunch_oil_biomass =bunch_oil_biomass_harvested = bunch_non_oil_biomass= bunch_non_oil_biomass_harvested = assim =
                offre_fruits  = growth_demand = bunch_demand = internode_demand =
                leaves_demand = male_demand  = male_biomass = male_biomass_harvested = peduncle_demand =offre_reste =
                peduncle_biomass = peduncle_biomass_harvested=respi_maintenance= leaves_reserve_excess=leaves_reserve_max=
                dif_demand_offer=res_stem=res_leaves= res_total=nb_leaves=nb_males=nb_bunches=newPhytomer=av_sex_ratio=av_abortion_rate=0;

        fr_fruits = fr_reste = 0.5;

        // init random generator
        srand(SEED);

        //init structure

        AGE_ADULT = 5.0 * 365; // days
        //      AGE_PLANTING = 1.0 * 365;
        AGE_START_PROD = 0.0 * 365; // =60 phytomers without inflorescence


        age =AGE_INI;
        double production_speed = age_relative_var(age, 0.0, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);

        // estimate total phytomers
        total_phytomer_number = INACTIVE_PHYTOMER_NUMBER;
        double d=0;
        while(d<=AGE_INI){
            d+=1;
            production_speed = age_relative_var(d, 0.0, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
            total_phytomer_number+=production_speed*TEFF_INI;
        }

        total_phytomer_number=floor(total_phytomer_number);

        int nb_phyto = total_phytomer_number;

        double age_at_creation = age;
        for ( int i =0 ; i<nb_phyto ; ++i ) {
            create_phytomer(t-1, -i, newPhytomer, age_at_creation);
            //update production speed for each phytomer
            production_speed = age_relative_var(age_at_creation, 0.0, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
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


        double sum_sex_ratio=0;
        double sum_AB=0;

        // plant leaf area
        auto it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            plantLeafArea += phytomer->leaf_model()->get<double>(t-1, Leaf::LEAFAREA);



            // get number of leaves
            double LA= phytomer->leaf_model()->get <double>(t-1, Leaf::LEAFAREA);
            if (LA > 0){
                // last leaf area
                lastLeafArea=LA;
                nb_leaves+=1;
            }

            // get number of bunch & male inflo
            double BN= phytomer->inflo_model()->get <double, Inflo>(t-1, Inflo::BUNCH_DEMAND);
            if (BN > 0){
                nb_bunches+=1;
            }

            double male= phytomer->inflo_model()->get <double, Inflo>(t-1, Inflo::MALE_DEMAND);
            if (male > 0){
                nb_males+=1;
            }

            double sexRatio=phytomer->inflo_model()->get <double, Inflo>(t-1, Inflo::SEX_RATIO);
            if (LA>0){
                sum_sex_ratio+=sexRatio;
            }

            double AB=phytomer->inflo_model()->get <double, Inflo>(t-1, Inflo::ABORTION_RATE);
            if (LA>0){
                sum_AB+=AB;
            }



            ++it;
        }

        av_sex_ratio=sum_sex_ratio/nb_leaves;
        av_abortion_rate=sum_AB/nb_leaves;

        lai = plantLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);

        double Rg_init=18;
        assim = ei * RUE * 0.48 * Rg_init  / (DENS/10000);

        //init trunk dim
        trunk_initial_height=0;
        if (age < AGE_INTERNODE_START_GROWTH){
            trunk_initial_height = INTERNODE_LENGTH_INI * age * TEFF_INI * PRODUCTION_SPEED_INITIAL ;
        }
        else if (age < AGE_INTERNODE_STOP_GROWTH){
            trunk_initial_height= INTERNODE_LENGTH_INI * AGE_INTERNODE_START_GROWTH * TEFF_INI * PRODUCTION_SPEED_INITIAL +
                    (age-AGE_INTERNODE_START_GROWTH) * TEFF_INI * ((PRODUCTION_SPEED_INITIAL+PRODUCTION_SPEED_ADULT)/2);
        }
        else
            trunk_initial_height= INTERNODE_LENGTH_INI * AGE_INTERNODE_START_GROWTH * TEFF_INI * PRODUCTION_SPEED_INITIAL +
                    ((INTERNODE_LENGTH_INI+INTERNODE_LENGTH_ADULT)/2)*(AGE_INTERNODE_STOP_GROWTH-AGE_INTERNODE_START_GROWTH) * TEFF_INI * ((PRODUCTION_SPEED_INITIAL+PRODUCTION_SPEED_ADULT)/2)+
                    INTERNODE_LENGTH_ADULT*(age-AGE_INTERNODE_STOP_GROWTH)* TEFF_INI * PRODUCTION_SPEED_ADULT;


        double STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        double STEM_RADIUS = parameters.get("STEM_RADIUS"); // cm

        trunk_initial_biomass = STEM_APPARENT_DENSITY * _PI * pow( STEM_RADIUS, 2) * trunk_initial_height; //gDM

        double NSC_STEM_INI = parameters.get("NSC_STEM_INI") ;
        trunk_initial_res= NSC_STEM_INI * trunk_initial_biomass;


        double SPECIFIC_LEAFLET_WEIGHT_INI = parameters.get("SPECIFIC_LEAFLET_WEIGHT_INI")*10000; //g.m-2
        double SPECIFIC_LEAFLET_WEIGHT_MIN = parameters.get("SPECIFIC_LEAFLET_WEIGHT_MIN")*10000 ; //g.m-2
        double LEAFLET_BIOMASS_CONTRIBUTION = parameters.get("LEAFLET_BIOMASS_CONTRIBUTION");

        leaves_structural_biomass = plantLeafArea   * SPECIFIC_LEAFLET_WEIGHT_MIN / LEAFLET_BIOMASS_CONTRIBUTION;  //gDM
        leaves_non_structural_biomass = plantLeafArea  * (SPECIFIC_LEAFLET_WEIGHT_INI - SPECIFIC_LEAFLET_WEIGHT_MIN) / LEAFLET_BIOMASS_CONTRIBUTION; //gDM
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


    void create_phytomer(double t, double number, double newPhytomer, double age_at_creation) {
        //        double rank = total_phyto - INACTIVE_PHYTOMER_NUMBER - number - 1;

        double production_speed = age_relative_var(age_at_creation, 0.0, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
        double TT_ini_flowering = age_relative_var(age_at_creation, 0.0, AGE_ADULT, TT_FLOWERING_INITIAL, TT_FLOWERING_ADULT);
        double TT_ini_harvest = age_relative_var(age_at_creation, 0.0, AGE_ADULT, TT_HARVEST_INITIAL, TT_HARVEST_ADULT);
        double inflo_dev_factor =0;
        if (age_at_creation>=AGE_START_PROD){
            inflo_dev_factor=age_relative_var(age_at_creation, AGE_START_PROD, AGE_ADULT, 0.3, 1);
        }

        double TT_ini_male_senescence = TT_ini_flowering +PERIOD_MALE_INFLO;

        //        Leaf area increase with plant age
        double first_LA=0.05;

        double SF_ind = age_relative_var(age_at_creation, 0.0, AGE_ADULT, first_LA , LEAFAREA_ADULT);




        Phytomer * phytomer = new Phytomer();
        setsubmodel(PHYTOMERS, phytomer);
        phytomer->init(t, _parameters, number, newPhytomer,
                       age_at_creation,
                       age,
                       production_speed,
                       TT_ini_flowering,
                       TT_ini_harvest,
                       TT_ini_male_senescence,
                       inflo_dev_factor,
                       SF_ind);

        phytomers.push_back(phytomer);

    }




    void compute(double t, bool /* update */)
    {
        age = age + 1 ;

        // split leaves reserve: assign the fraction of reserve for each leaf

        compute_biomasse_non_structurale_allouee_aux_feuilles(t);

        std::cout << "Age:" << age << std::endl;
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
            phytomer->put < double >(t, Phytomer::TOTAL_PHYTOMER_NUMBER, total_phytomer_number);
            phytomer->put < double >(t, Phytomer::NEWPHYTOMER, newPhytomer);
            phytomer->put < double >(t, Phytomer::TREE_IC, ic);
            phytomer->put < double >(t, Phytomer::TREE_GROWTH_DEMAND, growth_demand);
            phytomer->put < double >(t, Phytomer::TREE_ASSIM, assim);
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
        lastLeafArea=0;
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
        nb_leaves=0;
        nb_bunches=0;
        nb_males=0;
        nb_aborted=0;

        double sum_sex_ratio=0;
        double sum_AB=0;


        it = phytomers.begin();
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);

            //            if(phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) != phytomer::DEAD) {
            //stem
            trunk_height += phytomer->internode_model()->get <double>(t, Internode::LENGTH);
            trunk_biomass += phytomer->internode_model()->get <double>(t, Internode::BIOMASS);


            //            phytomer::phytomer_state state;

            //            state=phytomer->get<phytomer::phytomer_state, Phytomer>(t, Phytomer::STATE);
            //            if (state.is(phytomer::INACTIVE)){
            //                nb_aborted+=1;
            //            }
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



            // get number of leaves
            double LA= phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA);
            if (LA > 0){
                nb_leaves+=1;

                // last leaf area
                lastLeafArea=LA;
            }

            // get number of bunch & male inflo
            //attr
            inflo::inflo_states inflo_status;

            inflo_status=phytomer->inflo_model()->get<inflo::inflo_states, Inflo>(t, Inflo::INFLO_STATUS);
            if (inflo_status.is(inflo::ABORTED) && LA>0){
                nb_aborted+=1;
            }

            double BN= phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_DEMAND);
            if (BN > 0){
                nb_bunches+=1;
            }

            double male= phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_DEMAND);
            if (male > 0){
                nb_males+=1;
            }

            double sexRatio=phytomer->inflo_model()->get <double, Inflo>(t, Inflo::SEX_RATIO);
            if (LA>0){
                sum_sex_ratio+=sexRatio;
            }

            double AB=phytomer->inflo_model()->get <double, Inflo>(t, Inflo::ABORTION_RATE);
            if (LA>0){
                sum_AB+=AB;
            }



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


        //average sex ratio
        av_sex_ratio=sum_sex_ratio/nb_leaves;
        av_abortion_rate=sum_AB/nb_leaves;

        // compute total harvested biomass
        total_leaves_biomass_harvested = leaves_structural_biomass_harvested + leaves_non_structural_biomass_harvested;
        total_leaves_biomass = leaves_structural_biomass + leaves_non_structural_biomass - total_leaves_biomass_harvested;


        // compute maintenance respiration
        double TMoy = (_parameters.get(t).TMax + _parameters.get(t).TMin) / 2;
        double Q10 = pow(2, (TMoy - 25)/10);
        respi_maintenance = Q10 * (
                    trunk_biomass * RESPIRATION_COST_STEM +
                    respirable_repro_biomass * RESPIRATION_COST_INFLO +
                    leaves_structural_biomass * RESPIRATION_COST_LEAF_MAINTENANCE);

        // compute growth demand
        growth_demand =  internode_demand +   leaves_demand  +  bunch_demand + male_demand + peduncle_demand;


        // compute competition index
        ic = assim/growth_demand;

        ics.push_back(ic);

        double Rg = meteo->get<double>(t, Meteo::RG);
        lai = plantLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);

        double photo_reduc = ftsw > TRESH_FTSW_PHOTO_REDUC ? 1 : ftsw / TRESH_FTSW_PHOTO_REDUC; //reducing factor of photosynthesis du to water stress

        assim = ei * RUE * 0.48 * Rg * photo_reduc / (DENS/10000);//  #gCH2O.day-1.plant-1



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
        growth_offer = reserve->get<double>(t, Reserve::GROWTH_OFFER);

        dif_demand_offer=growth_demand-growth_offer;

        //
        res_stem=reserve->get<double>(t, Reserve::TRUNK_RES_AVAI);
        res_leaves = reserve->get<double>(t, Reserve::LEAVES_RES_AVAI);
        res_total= res_stem + res_leaves;

        //        fraction_pour_croissance = offre_nette / (growth_demand);

        //       compute_fraction_oil_reste
        double sum_organs_demand = leaves_demand + internode_demand + male_demand + peduncle_demand;
        double bunch_demand_corrected = BUNCH_AFFINITY * bunch_demand;
        double fr_fruits_corrected= bunch_demand_corrected/(sum_organs_demand+bunch_demand_corrected);

        offre_fruits = min( fr_fruits_corrected * growth_offer, bunch_demand );
        offre_reste = growth_offer-offre_fruits;
        fr_fruits = offre_fruits/(sum_organs_demand+offre_fruits);
        fr_reste=1-fr_fruits;

        //        if (fr_reste<0 | fr_reste>1){
        //            fr_reste=fr_reste;
        //        }

        double production_speed = age_relative_var(age, 0.0, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);

        double phylo_slow=(ftsw > TRESH_FTSW_SLOW_PHYLO
                           ? 1
                           : ftsw / TRESH_FTSW_SLOW_PHYLO);

        newPhytomerEmergence += TEff * production_speed * phylo_slow;

        if (newPhytomerEmergence >= 1) {
            total_phytomer_number += 1;
            newPhytomer+=1;
            newPhytomerEmergence -= 1;
                create_phytomer(t, newPhytomer, newPhytomer, age);


        }

    }



    void compute_biomasse_non_structurale_allouee_aux_feuilles(double t) {

        double reserve_pot_total=0;
        fraction_non_str_biomass_total = 0; //check must equal 1

        // compute total leaf reserve
        double res_leaves_ = reserve->get<double>(t-1, Reserve::LEAVES_RES_AVAI);


        if  (res_leaves_ > 0) {
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
