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

    enum externals {
                      INFLO_STATUS,
                      INFLO_POT_STATUS,
                      FLOWERING_TT,
                      HARVEST_TT,
                      INFLO_DEV_FACTOR,
                      IC_SPIKELET,
                      TREE_PRODUCTION_SPEED,
                      TEFF,
                      TT_CORRIGE,
                      TT_SINCE_RANK1,
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
//    double PRODUCTION_SPEED_INITIAL;
//    double TT_FLO;
//    double TT_HARVEST;

    //     internals
    double demand;
    double assimilate_supply;
    double biomass ;
    double potential_biomass;
    double demand_pot;
    double test_biomass;

    double TT_ini_flowering;
    double TT_ini_harvest;
    double inflo_dev_factor;
    double production_speed;

    //     externals
    inflo::inflo_states inflo_status;
    inflo::inflo_states inflo_pot_status;
    double IC_spikelet;
    double Teff;
    double TT_corrige;
    double TT_since_rank1;
    double fr_reste;

public:

    Peduncle(double prod_speed, double flo_tt, double harv_tt, double inflo_factor):
        TT_ini_flowering(flo_tt),
        TT_ini_harvest(harv_tt),
        inflo_dev_factor(inflo_factor),
        production_speed(prod_speed)
    {
        //         internals
        Internal(DEMAND, &Peduncle::demand);
        Internal(ASSIMILATE_SUPPLY, &Peduncle::assimilate_supply);
        Internal(BIOMASS , &Peduncle::biomass );
        Internal(POTENTIAL_BIOMASS , &Peduncle::potential_biomass );
        Internal(DEMAND_POT, &Peduncle::demand_pot);
        Internal(TEST_BIOMASS, &Peduncle::test_biomass);

        //          externals
        External(IC_SPIKELET, &Peduncle::IC_spikelet);
        External(TEFF, &Peduncle::Teff);
        External(TT_CORRIGE, &Peduncle::TT_corrige);
        External(TT_SINCE_RANK1, &Peduncle::TT_since_rank1);
        External(FR_RESTE, &Peduncle::fr_reste);
    }

    virtual ~Peduncle()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;

        //        parameters
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        MASSE_MEAN_PEDUNCULE_ADULTE = parameters.get("MASSE_MEAN_PEDUNCULE_ADULTE");
        RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");
        double PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");

        //internals
        demand = 0;
        assimilate_supply = 0;
        demand_pot = 0;

        //init structure
        if (TT_since_rank1 < TT_ini_flowering - RANG_DEBUT_CROISSANCE_PEDUNCULE / production_speed) {
            biomass = 0 ;
        } else {
            if (TT_since_rank1 > TT_ini_harvest)
                biomass = 0;
            else {
                double coeff = 1;
                if ( TT_since_rank1 <= TT_ini_flowering ) {
                    double TT_since_growth = RANG_DEBUT_CROISSANCE_PEDUNCULE / PRODUCTION_SPEED_INITIAL;
                    coeff = ( TT_since_rank1 - (TT_ini_flowering - TT_since_growth)) / TT_since_growth;
                }

                biomass = inflo_dev_factor * MASSE_MEAN_PEDUNCULE_ADULTE * coeff;
            }
        }
        potential_biomass = biomass;
    }


    void compute(double t, bool /* update */)
    {
//        growth();
        //compute assimilate supply
        assimilate_supply = demand * fr_reste;
        biomass += assimilate_supply / COUT_STRUCTURE_REGIME;

//        growth_demand();
        //compute potential biomass
        potential_biomass = inflo_dev_factor * pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * MASSE_MEAN_PEDUNCULE_ADULTE;

        //compute_demand
        if (TT_corrige > TT_ini_flowering - RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed)
            demand = inflo_dev_factor * COUT_STRUCTURE_REGIME * potential_biomass / (RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed) * Teff;
        else
            demand = 0;

        //compute_pot_demand
        if (TT_since_rank1 > TT_ini_flowering - RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed)
            demand_pot = inflo_dev_factor * COUT_STRUCTURE_REGIME * potential_biomass / (RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed) * Teff;
        else
            demand_pot = 0;
    }


};
} //namespace model
#endif // PEDUNCLE_H
