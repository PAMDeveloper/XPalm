#ifndef PEDUNCLE_H
#define PEDUNCLE_H

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

namespace model {

class Peduncle : public AtomicModel < Peduncle >
{
public:
    enum internals { DEMAND,
                     ASSIMILATE_SUPPLY,
                     BIOMASS ,
                     POTENTIAL_BIOMASS ,
                     DEMAND_POT,
                     TEST_BIOMASS
                   };

    enum externals {  ABLATION_STATUS,
                      BUNCH_STATUS,
                      BUNCH_POT_STATUS,
                      ESTIMATED_FLOWERING_DATE,
                      FACTEUR_AGE_REGIMES,
                      IC_SPIKELET,
                      TREE_PRODUCTION_SPEED,
                      TEFF,
                      TT_CORRIGE,
                      THERMALTIMESINCEAPPEARANCE,
                      FR_RESTE,
//                      INI_HARVEST_DATE,
//                      INI_FLOWERING_DATE
                   };

private:

    //      parameters
    double RANG_DEBUT_CROISSANCE_PEDUNCULE;
    double SENSIVITY_IC_SPIKELET;
    double MASSE_MEAN_PEDUNCULE_ADULTE;
    double COUT_STRUCTURE_REGIME;
    double PRODUCTION_SPEED_INITIAL;
    double TT_FLO;
    double TT_HARVEST;

    //     internals
    double demand;
    double assimilate_supply;
    double biomass ;
    double potential_biomass;
    double demand_pot;
    double test_biomass;

    //     externals
    inflo::inflo_states ablation_statut;
    inflo::inflo_states inflo_status;
    inflo::inflo_states inflo_pot_statut;
    double estimated_flowering_date;
    double facteur_age_regimes;
    double IC_spikelet;
    double production_speed;
    double Teff;
    double TT_corrige;
    double thermalTimeSinceAppearance;
    double fr_reste;
    double ini_harvest_date;
    double ini_flowering_date;

public:

    Peduncle()
    {
        //         internals
        Internal(DEMAND, &Peduncle::demand);
        Internal(ASSIMILATE_SUPPLY, &Peduncle::assimilate_supply);
        Internal(BIOMASS , &Peduncle::biomass );
        Internal(POTENTIAL_BIOMASS , &Peduncle::potential_biomass );
        Internal(DEMAND_POT, &Peduncle::demand_pot);
        Internal(TEST_BIOMASS, &Peduncle::test_biomass);

        //          externals
        External(ABLATION_STATUS, &Peduncle::ablation_statut);
        External(BUNCH_STATUS, &Peduncle::inflo_status);
        External(BUNCH_POT_STATUS, &Peduncle::inflo_status);
        External(ESTIMATED_FLOWERING_DATE, &Peduncle::estimated_flowering_date);
        External(FACTEUR_AGE_REGIMES, &Peduncle::facteur_age_regimes);
        External(IC_SPIKELET, &Peduncle::IC_spikelet);
        External(TREE_PRODUCTION_SPEED, &Peduncle::production_speed);
        External(TEFF, &Peduncle::Teff);
        External(TT_CORRIGE, &Peduncle::TT_corrige);
        External(THERMALTIMESINCEAPPEARANCE, &Peduncle::thermalTimeSinceAppearance);
        External(FR_RESTE, &Peduncle::fr_reste);
        External(INI_HARVEST_DATE, &Peduncle::ini_harvest_date);
        External(INI_FLOWERING_DATE, &Peduncle::ini_flowering_date);


    }

    virtual ~Peduncle()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;

        //        parameters
        RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        MASSE_MEAN_PEDUNCULE_ADULTE = parameters.get("MASSE_MEAN_PEDUNCULE_ADULTE");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");
        TT_FLO = parameters.get("TT_FLO");
        TT_HARVEST = parameters.get("TT_HARVEST");

        //internals
        demand = 0;
        assimilate_supply = 0;
        demand_pot = 0;
        test_biomass = 0;


        //init structure
        if (thermalTimeSinceAppearance < TT_FLO - RANG_DEBUT_CROISSANCE_PEDUNCULE / production_speed) {
            biomass = 0 ;
        } else {
            if (thermalTimeSinceAppearance > TT_HARVEST)
                biomass = 0;
            else {
                if ( thermalTimeSinceAppearance > TT_FLO )
                    biomass = facteur_age_regimes * MASSE_MEAN_PEDUNCULE_ADULTE;
                else
                    biomass = facteur_age_regimes * MASSE_MEAN_PEDUNCULE_ADULTE /
                            ( RANG_DEBUT_CROISSANCE_PEDUNCULE / PRODUCTION_SPEED_INITIAL  ) *
                            ( thermalTimeSinceAppearance - ( TT_FLO - RANG_DEBUT_CROISSANCE_PEDUNCULE /
                                                             PRODUCTION_SPEED_INITIAL ) );
            }
        }
        potential_biomass = biomass;
    }


    void growth_demand() {
        if (ablation_statut.is(inflo::ABLATED)) {
            demand = 0;
            demand_pot = 0;
        } else {
            //compute potential biomass
            potential_biomass = facteur_age_regimes * (pow(IC_spikelet, SENSIVITY_IC_SPIKELET)) * MASSE_MEAN_PEDUNCULE_ADULTE;

            //compute_demand
            if (inflo_status.is(inflo::APPARITION_FLORAISON))
                demand = 0;
            else {
                if (TT_corrige > estimated_flowering_date - RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed)
                    demand = facteur_age_regimes * COUT_STRUCTURE_REGIME * potential_biomass / (RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed) * Teff;
                else
                    demand = 0;
            }

            //compute_pot_demand
            if (inflo_pot_statut.is(inflo::APPARITION_FLORAISON))
                demand_pot = 0;
            else {
                if (thermalTimeSinceAppearance > estimated_flowering_date - RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed)
                    demand_pot = facteur_age_regimes * COUT_STRUCTURE_REGIME * potential_biomass / (RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed) * Teff;
                else
                    demand_pot = 0;
            }
        }
    }

    void growth() {
        //compute assimilate supply
        assimilate_supply = demand * fr_reste;

        //compute biomass
        if (inflo_status.is(inflo::RECOLTE) || ablation_statut.is(inflo::ABLATED))
            biomass = 0;
        else
            biomass = assimilate_supply * (1 / COUT_STRUCTURE_REGIME);

        potential_biomass += demand_pot *(1 / COUT_STRUCTURE_REGIME);
        test_biomass += assimilate_supply * (1 / COUT_STRUCTURE_REGIME);
    }





    void compute(double t, bool /* update */)
    {
        growth();
        growth_demand();
    }


};
} //namespace model
#endif // PEDUNCLE_H
