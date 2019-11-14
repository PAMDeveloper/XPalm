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
                     DEMAND_POT
                   };

    enum externals {
                      IC_SPIKELET,
                      TEFF,
                      TT_CORRIGE,
                      TT_SINCE_RANK1,
                      FR_RESTE
                   };

private:

    //      parameters
    double RANG_DEBUT_CROISSANCE_PEDUNCULE;
    double SENSIVITY_IC_SPIKELET;
    double MASSE_MEAN_PEDUNCULE_ADULTE;
    double COUT_STRUCTURE_REGIME;

    //     internals
    //predim
    double TT_ini_flowering;
    double inflo_dev_factor;
    double production_speed;
    //var
    double demand;
    double assimilate_supply;
    double biomass ;
    double potential_biomass;
    double demand_pot;


    //     externals
    double IC_spikelet;
    double Teff;
    double TT_corrige;
    double TT_since_rank1;
    double fr_reste;

public:

    Peduncle()
    {
        //         internals
        Internal(DEMAND, &Peduncle::demand);
        Internal(ASSIMILATE_SUPPLY, &Peduncle::assimilate_supply);
        Internal(BIOMASS , &Peduncle::biomass );
        Internal(POTENTIAL_BIOMASS , &Peduncle::potential_biomass );
        Internal(DEMAND_POT, &Peduncle::demand_pot);

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

    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double prod_speed, double flo_tt, double TT_ini_harvest, double inflo_factor)
    {
        init(t, parameters);
//        AtomicModel<Peduncle>::init(t, parameters);
        last_time = t-1;

        TT_ini_flowering = flo_tt;
        inflo_dev_factor = inflo_factor;
        production_speed = prod_speed;

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
