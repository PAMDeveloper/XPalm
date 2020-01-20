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

    enum internals { AGE,
                     LAI,
                     EI,
                     IC,
                     ICS,
                     TOTALLEAFAREA,
                     SLW,
                     TRUNK_HEIGHT,
                     BIOMASS,
                     TOTAL_LEAF_BIOMASS,
                     TOTAL_LEAF_BIOMASS_HARVESTED,
                     TRUNK_BIOMASS,
                     LEAF_NON_STRUCTURAL_BIOMASS,
                     LEAF_STRUCTURAL_BIOMASS,
                     LEAF_NON_STRUCTURAL_BIOMASS_HARVESTED,
                     LEAF_STRUCTURAL_BIOMASS_HARVESTED,
                     INFLO_BIOMASS,
                     RESPIRABLE_BUNCH_BIOMASS,
                     BUNCH_BIOMASS,
                     BUNCH_BIOMASS_HARVESTED,
                     ASSIM,
                     PHYTOMERNUMBER,
                     NEWPHYTOMEREMERGENCE,
                     FR_FRUITS,
                     FR_RESTE,
                     OFFRE_FRUITS,
                     OFFRE_NETTE,
                     GROWTH_DEMAND,
                     BUNCH_DEMAND,
                     INTERNODE_DEMAND,
                     LEAF_DEMAND,
                     MALE_DEMAND,
                     INFLO_DEMAND,
                     MALE_BIOMASS,
                     MALE_BIOMASS_HARVESTED,
                     PEDUNCULE_DEMAND };

private:
    xpalm::ModelParameters _parameters;

    //      parameters
    double RANG_D_ABLATION;
    double INACTIVE_PHYTOMER_NUMBER;
    double INITIAL_HEIGHT;

    double PRODUCTION_SPEED_ADULT;
    double PRODUCTION_SPEED_INITIAL;
    double AGE_ADULT;
    double AGE_PLANTING;
    double AGE_START_PROD;
    double TT_FLOWERING_INITIAL;
    double TT_FLOWERING_ADULT;
    double TT_HARVEST_INITIAL;
    double TT_HARVEST_ADULT;
    double TT_SENESCENCE;
    double MAXIMAL_SFIND;
    double INITIAL_SFIND;
    double DENS;
    double K;
    double AF_FRUITS;
    double COUT_RESPI_MAINTENANCE_BUNCH;
    double COUT_RESPI_MAINTENANCE_LEAF;
    double COUT_RESPI_MAINTENANCE_STIPE;
    double EFFICIENCE_BIOLOGIQUE;
    double SEUIL_ORGANO;
    double SEUIL_PHOTO;
    double VITESSE_SENSITIVITY;

    //     submodels
    std::deque < Phytomer* > phytomers;
    std::unique_ptr < Reserve > reserve;
    std::unique_ptr < Racines > racines;
    std::unique_ptr < WaterBalance > bh;
    std::unique_ptr < Meteo > meteo;

    //     internals
    std::vector<double> ics;

    double age;
    double lai;
    double ei;
    double ic;
    double totalLeafArea;
    double slw;
    double trunk_height;
    double biomass;
    double total_leaf_biomass;
    double total_leaf_biomass_harvested;
    double trunk_biomass;
    double leaf_non_structural_biomass;
    double leaf_structural_biomass;
    double leaf_non_structural_biomass_harvested;
    double leaf_structural_biomass_harvested;
    double inflo_biomass;
    double respirable_bunch_biomass;
    double bunch_biomass;
    double bunch_biomass_harvested;
    double Assim;
    double phytomerNumber;
    double newPhytomerEmergence;
    double fr_fruits;
    double fr_reste;
    double offre_fruits;
    double offre_nette;
    double growth_demand;
    double bunch_demand;
    double internode_demand;
    double leaf_demand;
    double male_demand;
    double inflo_demand;
    double male_biomass;
    double male_biomass_harvested;
    double peduncule_demand;


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
        Internal(AGE, &Tree::age);
        Internal(LAI, &Tree::lai);
        Internal(EI, &Tree::ei);
        Internal(IC, &Tree::ic);
        Internal(ICS, &Tree::ics);
        Internal(TOTALLEAFAREA, &Tree::totalLeafArea);
        Internal(SLW, &Tree::slw);
        Internal(TRUNK_HEIGHT, &Tree::trunk_height);
        Internal(BIOMASS, &Tree::biomass);
        Internal(TOTAL_LEAF_BIOMASS, &Tree::total_leaf_biomass);
        Internal(TOTAL_LEAF_BIOMASS_HARVESTED, &Tree::total_leaf_biomass_harvested);
        Internal(TRUNK_BIOMASS, &Tree::trunk_biomass);
        Internal(LEAF_NON_STRUCTURAL_BIOMASS, &Tree::leaf_non_structural_biomass);
        Internal(LEAF_STRUCTURAL_BIOMASS, &Tree::leaf_structural_biomass);
        Internal(LEAF_NON_STRUCTURAL_BIOMASS_HARVESTED, &Tree::leaf_non_structural_biomass_harvested);
        Internal(LEAF_STRUCTURAL_BIOMASS_HARVESTED, &Tree::leaf_structural_biomass_harvested);
        Internal(INFLO_BIOMASS, &Tree::inflo_biomass);
        Internal(RESPIRABLE_BUNCH_BIOMASS, &Tree::respirable_bunch_biomass);
        Internal(BUNCH_BIOMASS, &Tree::bunch_biomass);
        Internal(BUNCH_BIOMASS_HARVESTED, &Tree::bunch_biomass_harvested);
        Internal(ASSIM, &Tree::Assim);
        Internal(PHYTOMERNUMBER, &Tree::phytomerNumber);
        Internal(NEWPHYTOMEREMERGENCE, &Tree::newPhytomerEmergence);
        Internal(FR_FRUITS, &Tree::fr_fruits);
        Internal(FR_RESTE, &Tree::fr_reste);
        Internal(OFFRE_FRUITS, &Tree::offre_fruits);
        Internal(OFFRE_NETTE, &Tree::offre_nette);
        Internal(GROWTH_DEMAND, &Tree::growth_demand);
        Internal(BUNCH_DEMAND, &Tree::bunch_demand);
        Internal(INTERNODE_DEMAND, &Tree::internode_demand);
        Internal(LEAF_DEMAND, &Tree::leaf_demand);
        Internal(MALE_DEMAND, &Tree::male_demand);
        Internal(INFLO_DEMAND, &Tree::inflo_demand);
        Internal(MALE_BIOMASS, &Tree::male_biomass);
        Internal(MALE_BIOMASS_HARVESTED, &Tree::male_biomass_harvested);
        Internal(PEDUNCULE_DEMAND, &Tree::peduncule_demand);
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

        age = parameters.get("AGE") * 365;

        //        parameters
        PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");
        AGE_ADULT = parameters.get("AGE_ADULT") * 365; // days
        AGE_PLANTING = parameters.get("AGE_PLANTING") * 365; // days (age when production speed starts to decrease
        AGE_START_PROD = parameters.get("AGE_START_PROD") * 365;
        //        PRODUCTION_DAILY_DECREMENT = ( PRODUCTION_SPEED_ADULT - PRODUCTION_SPEED_INITIAL ) / ( AGE_ADULT - AGE_PLANTING );

        TT_FLOWERING_INITIAL = parameters.get("TT_FLOWERING_INITIAL");
        TT_FLOWERING_ADULT = parameters.get("TT_FLOWERING_ADULT");
        TT_HARVEST_INITIAL = parameters.get("TT_HARVEST_INITIAL");
        TT_HARVEST_ADULT = parameters.get("TT_HARVEST_ADULT");
        TT_SENESCENCE = parameters.get("TT_SENESCENCE");
        MAXIMAL_SFIND  = parameters.get("MAXIMAL_SFIND");
        INITIAL_SFIND  = parameters.get("INITIAL_SFIND");

        AF_FRUITS = parameters.get("AF_FRUITS");
        COUT_RESPI_MAINTENANCE_BUNCH = parameters.get("COUT_RESPI_MAINTENANCE_BUNCH");
        COUT_RESPI_MAINTENANCE_LEAF = parameters.get("COUT_RESPI_MAINTENANCE_LEAF");
        COUT_RESPI_MAINTENANCE_STIPE = parameters.get("COUT_RESPI_MAINTENANCE_STIPE");
        ////        DECREASE_OF_PRODUCTION_SPEED = parameters.get("DECREASE_OF_PRODUCTION_SPEED");
        DENS = parameters.get("DENS");
        K=parameters.get("K");
        EFFICIENCE_BIOLOGIQUE = parameters.get("EFFICIENCE_BIOLOGIQUE");
        INITIAL_HEIGHT = parameters.get("INITIAL_HEIGHT");
        RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");
        SEUIL_ORGANO = parameters.get("SEUIL_ORGANO");
        SEUIL_PHOTO = parameters.get("SEUIL_PHOTO");
        VITESSE_SENSITIVITY = parameters.get("VITESSE_SENSITIVITY");


        //        internals
        newPhytomerEmergence = 0;
        phytomerNumber = INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION;
        ic = 1;
        ei = 0;

        totalLeafArea = slw = trunk_height = biomass = total_leaf_biomass = total_leaf_biomass_harvested = trunk_biomass =
                leaf_non_structural_biomass = leaf_structural_biomass = leaf_structural_biomass_harvested = leaf_non_structural_biomass_harvested =
                inflo_biomass = respirable_bunch_biomass = bunch_biomass = bunch_biomass_harvested = Assim =
                fr_fruits = fr_reste = offre_fruits = offre_nette = growth_demand = bunch_demand = internode_demand =
                leaf_demand = male_demand = inflo_demand = male_biomass = male_biomass_harvested = peduncule_demand = 0;


        //init structure
        double production_speed = age_relative_var(age, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
        double TTEff_init = parameters.get("T_EFF_INI");


        //        double age_at_creation = age;
        //        for ( int i = nb_phyto ; i >= 0 ; --i ) {
        //        create_phytomer(t, i, age_at_creation);
        //        age_at_creation -= 1 / (TTEff_init * production_speed);
        //    }

        int nb_phyto = INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION-1;
        double age_at_creation = age - (nb_phyto / (TTEff_init * production_speed));

        for ( int i = 0; i <= nb_phyto; ++i ) {
            create_phytomer(t-1, i, phytomerNumber, age_at_creation);
            age_at_creation += 1 / (TTEff_init * production_speed);
        }

        meteo->init(t, parameters);
        reserve->init(t, parameters);
        racines->init(t, parameters);
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC, racines->get<double>(t-1, Racines::TAILLEC));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1, racines->get<double>(t-1, Racines::TAILLEC1));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC2, racines->get<double>(t-1, Racines::TAILLEC2));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEC1MOINSVAP, racines->get<double>(t-1, Racines::TAILLEC1MOINSVAP));
        bh->put<double>(t, WaterBalance::RACINES_TAILLEVAP, racines->get<double>(t-1, Racines::TAILLEVAP));
        bh->put<double>(t, WaterBalance::ET0, meteo->get<double>(t-1, Meteo::ET0));
        bh->put<double>(t, WaterBalance::TREE_EI, ei);
        bh->init(t, parameters);

        //        init_structure(t);

        //        compute_SF(t);
        auto it = phytomers.begin();
        //        totalLeafArea = 0;
        while (it != phytomers.end()) {
            Phytomer* phytomer = (*it);
            totalLeafArea += phytomer->leaf_model()->get<double>(t-1, Leaf::LEAFAREA);
            ++it;
        }
        lai = totalLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);

        //        bh->put<double>(t, WaterBalance::TREE_EI, ei);

        double STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        double STEM_RAYON = parameters.get("STEM_RAYON");
        trunk_biomass = 1000  * STEM_APPARENT_DENSITY * _PI * pow( STEM_RAYON, 2) * INITIAL_HEIGHT; //TODO vérifier l'unité dans les calculs de masse x1000

        double SLW_ini = parameters.get("SLW_ini") * 10; //kg.m-2
        double SLW_min = parameters.get("SLW_min") * 10; //kg.m-2
        double POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        leaf_structural_biomass = totalLeafArea  * SLW_min / POURC_FOLIOLE  ;
        leaf_non_structural_biomass = totalLeafArea * (SLW_ini - SLW_min) / POURC_FOLIOLE;
        total_leaf_biomass = leaf_structural_biomass + leaf_non_structural_biomass;
        slw = ((leaf_non_structural_biomass + leaf_structural_biomass) * POURC_FOLIOLE) / totalLeafArea;

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
        double TT_ini_senescence = age_relative_var(age_at_creation, AGE_PLANTING, AGE_ADULT, TT_FLOWERING_INITIAL, TT_FLOWERING_INITIAL + TT_SENESCENCE);
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
                       TT_ini_senescence,
                       inflo_dev_factor,
                       SF_ind);

        phytomers.push_back(phytomer);
//        phytomerNumber += 1;
    }




    void compute(double t, bool /* update */)
    {
        age = age + 1 ;


        compute_biomasse_non_structurale_allouee_aux_feuilles(t);

        std::cout << "Age:" << age << std::endl;
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
            phytomer->put < double >(t, Phytomer::TOTAL_PHYTOMER_NUMBER, phytomers.size());
            phytomer->put < double >(t, Phytomer::TREE_IC, ic);
            phytomer->put < double >(t, Phytomer::TEFF, TEff);

            phytomer->leaf_model()->put<double>(t, Leaf::FTSW, ftsw);
            phytomer->leaf_model()->put<double>(t, Leaf::FR_RESTE, fr_reste);

            //            phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, fraction_non_str_biomasse_allouee);//done in compute struct biom for leaves
            phytomer->leaf_model()->put<double>(t, Leaf::LEAF_RES_AVAI, reserve->get<double>(t-1, Reserve::LEAF_RES_AVAI));

            phytomer->internode_model()->put<double>(t, Internode::FR_RESTE, fr_reste);

            phytomer->inflo_model()->put<double>(t, Inflo::FTSW, ftsw);
            phytomer->inflo_model()->put<double>(t, Inflo::FR_FRUITS, fr_fruits);
            phytomer->inflo_model()->put<double>(t, Inflo::FR_RESTE, fr_reste);

            (*phytomer)(t);
            ++it;
        }

        //        production_speed = max(MINIMAL_PRODUCTION_SPEED, (-DECREASE_OF_PRODUCTION_SPEED * (t - _parameters.beginDate) ) + PRODUCTION_SPEED_INITIAL); //DD.rang-1

        racines->put(t, Racines::TEFF, TEff);
        racines->put(t, Racines::FTSW, ftsw);
        (*racines)(t);


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

            //            if(phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) != phytomer::DEAD) {

            totalLeafArea += phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA);

            leaf_structural_biomass += phytomer->leaf_model()->get <double>(t, Leaf::STRUCTURAL_BIOMASS);
            leaf_non_structural_biomass += phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS);

            leaf_structural_biomass_harvested += phytomer->leaf_model()->get <double>(t, Leaf::STRUCTURAL_BIOMASS_HARVESTED);
            leaf_non_structural_biomass_harvested += phytomer->leaf_model()->get <double>(t, Leaf::NON_STRUCTURAL_BIOMASS_HARVESTED);

            leaf_demand += phytomer->leaf_model()->get <double>(t, Leaf::DEMAND);

            trunk_height += phytomer->internode_model()->get <double>(t, Internode::LENGTH);
            trunk_biomass += phytomer->internode_model()->get <double>(t, Internode::BIOMASS);
            internode_demand +=  phytomer->internode_model()->get <double>(t, Internode::DEMAND);

            inflo_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BIOMASS); //## attention pour la respi de maintenance !!!
            respirable_bunch_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::RESPIRABLE_BIOMASS);
            bunch_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::FEMELLE_BIOMASS);
            male_biomass += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_BIOMASS);

            bunch_biomass_harvested += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::FEMELLE_BIOMASS_HARVESTED);
            male_biomass_harvested += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_BIOMASS_HARVESTED);

            inflo_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::DEMAND);
            bunch_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::BUNCH_DEMAND);
            male_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::MALE_DEMAND);
            peduncule_demand += phytomer->inflo_model()->get <double, Inflo>(t, Inflo::PEDUNCLE_DEMAND);


            //            }

            double reserve_biomass = reserve->get <double>(t-1, Reserve::RESERVE);
            biomass = leaf_structural_biomass + leaf_non_structural_biomass +  bunch_biomass + trunk_biomass + reserve_biomass + male_biomass;

            //            if (phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE) == phytomer::ACTIVE) {
            //                totalLeafArea += phytomer->leaf_model()->get <double>(t, Leaf::LEAFAREA);
            //            }
            ++it;
        }
        total_leaf_biomass = leaf_structural_biomass + leaf_non_structural_biomass;
        total_leaf_biomass_harvested = leaf_structural_biomass_harvested + leaf_non_structural_biomass_harvested;


        double TMoy = (_parameters.get(t).TMax + _parameters.get(t).TMin) / 2;
        double Q10 = pow(2, (TMoy - 25)/10);
        double respi_maintenance = Q10 * (
                    trunk_biomass * COUT_RESPI_MAINTENANCE_STIPE +
                    respirable_bunch_biomass * COUT_RESPI_MAINTENANCE_BUNCH +
                    leaf_structural_biomass * COUT_RESPI_MAINTENANCE_LEAF);

        double demand = internode_demand + leaf_demand + respi_maintenance + inflo_demand;
        growth_demand =  internode_demand +   leaf_demand  +  inflo_demand;

        ic = Assim/demand;
        ics.push_back(ic);

        double Rg = meteo->get<double>(t, Meteo::RG);
        lai = totalLeafArea * DENS / 10000;
        ei = 1 - exp(- K * lai);
        double factor = ftsw > SEUIL_PHOTO ? 1 : ftsw / SEUIL_PHOTO; //reducing factor when FTSW < S
        Assim = factor * 10 * ei * EFFICIENCE_BIOLOGIQUE * 0.48 * Rg / DENS;//  # on multiplie par 10 pour passer en kg

        // compute etat reserve
        //set Tree Assim
        reserve->put(t, Reserve::TOTALLEAFAREA, totalLeafArea);
        reserve->put(t, Reserve::TREE_TRUNK_BIOMASS, trunk_biomass);
        reserve->put(t, Reserve::LEAF_NON_STRUCTURAL_BIOMASS, leaf_non_structural_biomass);
        reserve->put(t, Reserve::MAINTENANCE_RESPI, respi_maintenance);
        reserve->put(t, Reserve::TREE_GROWTH_DEMAND, demand);
        reserve->put(t, Reserve::TREE_ASSIM, Assim);
        (*reserve)(t);

        //       compute_offre_nette
        offre_nette = reserve->get<double>(t, Reserve::ASSIM_AVAI); //TODO appliquer REALL_COST
        //        fraction_pour_croissance = offre_nette / (growth_demand);

        //       compute_fraction_oil_reste
        double sum_organs_demand = leaf_demand + internode_demand + male_demand + peduncule_demand;
        double demand_growth_corrected = sum_organs_demand + (AF_FRUITS * bunch_demand);

        //        if (AF_FRUITS >= 1) {
        offre_fruits = min( ( (AF_FRUITS * bunch_demand) * offre_nette / demand_growth_corrected ), bunch_demand );

        //        if(bunch_demand==0) {
        //            fr_reste = offre_nette / sum_organs_demand;
        //            fr_fruits = 1;
        //        } else {

        //        }

        if (bunch_demand==0) {
            fr_fruits = 1;
        } else {
            fr_fruits = offre_fruits / bunch_demand;
        }
        double offre_reste = offre_nette - offre_fruits;
        fr_reste = offre_reste / sum_organs_demand;


        double production_speed = age_relative_var(age, AGE_PLANTING, AGE_ADULT, PRODUCTION_SPEED_INITIAL, PRODUCTION_SPEED_ADULT);
        newPhytomerEmergence += TEff * production_speed * pow(ic,VITESSE_SENSITIVITY) * ( ftsw > SEUIL_ORGANO ? 1 : ftsw / SEUIL_ORGANO);
        if (newPhytomerEmergence >= 1) {
//            create_phytomer(t, phytomerNumber + 1, age);
            create_phytomer(t, phytomerNumber, phytomerNumber+1, age);
             phytomerNumber += 1;
            newPhytomerEmergence -= 1;
        }
    }



    void compute_biomasse_non_structurale_allouee_aux_feuilles(double t) {
        double TOT = 0;
        double FRAC = 0;

        double delta_biomasse_reserve_leaf = reserve->get<double>(t-1, Reserve::LEAF_RES_AVAI);
        if  (delta_biomasse_reserve_leaf > 0) {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t-1, Phytomer::STATE) == phytomer::ACTIVE) {
                    ++it;
                    continue;
                }

                double capacite_reserve_max = phytomer->leaf_model()->get <double>(t-1, Leaf::CAPACITE_RESERVE_MAX);
                double non_structural_biomass = phytomer->leaf_model()->get <double>(t-1, Leaf::NON_STRUCTURAL_BIOMASS);
                if(capacite_reserve_max > non_structural_biomass)
                    TOT += capacite_reserve_max - non_structural_biomass;
                ++it;
            }

            it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(TOT <= 0 && phytomer->leaf_model()->get <double>(t-1, Leaf::LEAFAREA) > 0)
                    phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);

                else if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t-1, Phytomer::STATE) == phytomer::DEAD &&
                        phytomer->leaf_model()->get <double>(t-1, Leaf::LEAFAREA) > 0) {
                    double capacite_reserve_max = phytomer->leaf_model()->get <double>(t-1, Leaf::CAPACITE_RESERVE_MAX);
                    double non_structural_biomass = phytomer->leaf_model()->get <double>(t-1, Leaf::NON_STRUCTURAL_BIOMASS);
                    if(capacite_reserve_max > non_structural_biomass) {
                        double fraction_non_str_biomasse_allouee = (capacite_reserve_max - non_structural_biomass) / TOT;
                        phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, fraction_non_str_biomasse_allouee);
                        FRAC += fraction_non_str_biomasse_allouee;
                    } else
                        phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                } else {
                    phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                }
                ++it;
            }
        } else {
            auto it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);
                if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t-1, Phytomer::STATE) == phytomer::ACTIVE) {
                    ++it;
                    continue;
                }
                TOT += phytomer->leaf_model()->get <double>(t-1, Leaf::NON_STRUCTURAL_BIOMASS);
                ++it;
            }

            it = phytomers.begin();
            while (it != phytomers.end()) {
                Phytomer* phytomer = (*it);


                if(TOT <= 0 && phytomer->leaf_model()->get <double>(t-1, Leaf::LEAFAREA) > 0)
                    phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                else if(!phytomer->get < phytomer::phytomer_state, Phytomer >(t-1, Phytomer::STATE) == phytomer::DEAD &&
                        phytomer->leaf_model()->get <double>(t-1, Leaf::LEAFAREA) > 0) {
                    double non_structural_biomass = phytomer->leaf_model()->get <double>(t-1, Leaf::NON_STRUCTURAL_BIOMASS);
                    if(non_structural_biomass >= 0) {
                        double fraction_non_str_biomasse_allouee = non_structural_biomass / TOT;
                        phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, fraction_non_str_biomasse_allouee);
                        FRAC += fraction_non_str_biomasse_allouee;
                    } else
                        phytomer->leaf_model()->put(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                } else {
                    phytomer->leaf_model()->put<double>(t, Leaf::FRACTION_NON_STR_BIOMASSE_ALLOUEE, 0);
                }
                ++it;
            }
        }
    }

};

//    void update_organs(double t) {
//        //       #### defiolation le jour de la mise en place des traitements
//        if (t == DEBUT_DEFOLIATON ) {
//            auto it = phytomers.begin();
//            while (it != phytomers.end()) {
//                Phytomer* phytomer = (*it);
//                int rank = phytomer->get< double, Phytomer >(t, Phytomer::RANK);
//                phytomer::phytomer_state state = phytomer->get < phytomer::phytomer_state, Phytomer >(t, Phytomer::STATE);
//                if(rank > RANG_DEFOLIATON && state == phytomer::ACTIVE) {
//                    ;//TODO ABLATION  phytomers[key].leaf.ablation = phytomers[key].leaf.defoliation_decision(POURC_DEFOLIATON)
//                }
//            }
//        }

//        // ### ablation des regimes le jour de la mise en place des traitements
//        if (t == DEBUT_ABLATION_REGIME ) {
//            auto it = phytomers.begin();
//            while (it != phytomers.end()) {
//                Phytomer* phytomer = (*it);
//                int rank = phytomer->get < double, Phytomer >(t, Phytomer::RANK);
//                inflo::inflo_states bunch_state = phytomer->inflo_model()->get <inflo::inflo_states, Inflo >(t, Inflo::STATUS);
//                inflo::inflo_sex sex = phytomer->inflo_model()->get <inflo::inflo_sex, Inflo >(t, Inflo::SEX);
//                inflo::inflo_sex avort = phytomer->inflo_model()->get <inflo::inflo_sex, Inflo >(t, Inflo::AVORT);
//                if(rank >= RANG_D_ABLATION_REGIME
//                        && bunch_state.is(inflo::RECOLTE)
//                        && sex ==  inflo::FEMALE
//                        && avort == inflo::NON_ABORTED)
//                    ;//TODO ABLATION  phytomers[key].leaf.ablation = phytomers[key].leaf.defoliation_decision(POURC_DEFOLIATON)

//            }
//        }

//        if (newPhytomerEmergence >= 1) {
//            create_phytomer(phytomerNumber + 1, age);
//            newPhytomerEmergence -= 1;
//        }
//    }
#endif // TREE_H
