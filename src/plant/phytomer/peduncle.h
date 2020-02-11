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
                     BIOMASS_HARVESTED,
                     POTENTIAL_BIOMASS ,
                     DEMAND_POT,
                     TT_PED_DEV_DURATION
                   };

    enum externals {
        IC_SPIKELET,
        TEFF,
        TT_CORRIGE,
        TT_SINCE_APPEARANCE,
        FR_RESTE,
        INFLO_STATUS
    };

private:

    //      parameters
    //    double RANG_DEBUT_CROISSANCE_PEDUNCULE;
    double SENSIVITY_IC_SPIKELET;
    double MASSE_MEAN_PEDUNCULE_ADULTE;
    double REPRO_CONSTRUCTION_COST;
    double FRACTION_PERIOD_DEV_PEDUNCLE ;

    //     internals
    //predim
    double TT_ini_flowering;
    double TT_ini_harvest;
    double inflo_dev_factor;
    double production_speed;
    double TT_ped_dev_duration;
    //var
    double demand;
    double assimilate_supply;
    double biomass ;
    double biomass_harvested;
    double potential_biomass;
    double demand_pot;


    //     externals
    inflo::inflo_states inflo_status;
    double IC_spikelet;
    double Teff;
    double TT_corrige;
    double TT_since_appearance;
    double fr_reste;

public:

    Peduncle()
    {
        //         internals
        Internal(DEMAND, &Peduncle::demand);
        Internal(ASSIMILATE_SUPPLY, &Peduncle::assimilate_supply);
        Internal(BIOMASS , &Peduncle::biomass );
        Internal(BIOMASS_HARVESTED , &Peduncle::biomass_harvested );
        Internal(POTENTIAL_BIOMASS , &Peduncle::potential_biomass );
        Internal(DEMAND_POT, &Peduncle::demand_pot);
        Internal(TT_PED_DEV_DURATION, &Peduncle::TT_ped_dev_duration);


        //          externals
        External(IC_SPIKELET, &Peduncle::IC_spikelet);
        External(TEFF, &Peduncle::Teff);
        External(TT_CORRIGE, &Peduncle::TT_corrige);
        External(TT_SINCE_APPEARANCE, &Peduncle::TT_since_appearance);
        External(FR_RESTE, &Peduncle::fr_reste);
        External(INFLO_STATUS, &Peduncle::inflo_status);
    }

    virtual ~Peduncle()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double prod_speed, double flo_tt, double TT_ini_harvest_, double inflo_factor)
    {
        init(t, parameters);
        //        AtomicModel<Peduncle>::init(t, parameters);
        last_time = t;

        TT_ini_flowering = flo_tt;
        TT_ini_harvest=TT_ini_harvest_;
        inflo_dev_factor = inflo_factor;
        production_speed = prod_speed;

        //        parameters
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        MASSE_MEAN_PEDUNCULE_ADULTE = parameters.get("MASSE_MEAN_PEDUNCULE_ADULTE");
        FRACTION_PERIOD_DEV_PEDUNCLE = parameters.get("FRACTION_PERIOD_DEV_PEDUNCLE");
        double TEff_ini = parameters.get("T_EFF_INI");

        //        RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        REPRO_CONSTRUCTION_COST = parameters.get("REPRO_CONSTRUCTION_COST");

        //        double PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");

        //internals
        demand = 0;
        assimilate_supply = 0;
        demand_pot = 0;
        biomass = 0 ;
        biomass_harvested=0;

        TT_ped_dev_duration = FRACTION_PERIOD_DEV_PEDUNCLE* (TT_ini_harvest - TT_ini_flowering);

        //init structure
        //        if (TT_since_appearance < TT_ini_flowering) {
        //            biomass = 0 ;
        //        } else {
        if (inflo_status.is(inflo::HARVEST)){
            biomass_harvested=biomass;
            biomass = 0;
        }
        else {
            if ( inflo_status.is(inflo::FLOWERING) | inflo_status.is(inflo::OLEOSYNTHESIS) ) {

                double fr_ped_dev = min(1.0, (TT_since_appearance - TT_ini_flowering) / TT_ped_dev_duration);

                //                double coeff = 1;
                //                if ( TT_since_appearance <= TT_ini_flowering ) {
                //                    double TT_since_growth = RANG_DEBUT_CROISSANCE_PEDUNCULE / prod_speed;
                //                    coeff = ( TT_since_appearance - (TT_ini_flowering - TT_since_growth)) / TT_since_growth;
                //                }

                //                biomass = inflo_dev_factor * MASSE_MEAN_PEDUNCULE_ADULTE * coeff;

                biomass = MASSE_MEAN_PEDUNCULE_ADULTE * inflo_dev_factor * fr_ped_dev;
                demand= MASSE_MEAN_PEDUNCULE_ADULTE * inflo_dev_factor * REPRO_CONSTRUCTION_COST * ( TEff_ini / TT_ped_dev_duration );

            }
        }

        potential_biomass = biomass;
        //    }

    }

    void compute(double t, bool /* update */)
    {


        // growth();
        assimilate_supply = demand * fr_reste;
        biomass += assimilate_supply / REPRO_CONSTRUCTION_COST;
        potential_biomass += demand/ REPRO_CONSTRUCTION_COST;
        demand=0;

        //        growth_demand();
        if (inflo_status.is(inflo::HARVEST)){
            biomass_harvested=biomass;
            biomass = 0;

        }
        else

            if (inflo_status.is(inflo::FLOWERING) | inflo_status.is(inflo::OLEOSYNTHESIS)){
                demand = inflo_dev_factor * REPRO_CONSTRUCTION_COST * min(1.0, IC_spikelet) * MASSE_MEAN_PEDUNCULE_ADULTE * Teff/TT_ped_dev_duration; //TODO, test with TT_corrige, correct 1.0 if plasticity
                /*demand = inflo_dev_factor * REPRO_CONSTRUCTION_COST * pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * MASSE_MEAN_PEDUNCULE_ADULTE * Teff/TT_ped_dev_duration;*/
            }


        //        if (TT_corrige > TT_ini_flowering - RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed)
        //            demand = inflo_dev_factor * COUT_STRUCTURE_REGIME * potential_biomass / (RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed) * Teff;
        //        else
        //            demand = 0;

        //        //compute_pot_demand
        //        if (TT_since_appearance > TT_ini_flowering - RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed)
        //            demand_pot = inflo_dev_factor * COUT_STRUCTURE_REGIME * potential_biomass / (RANG_DEBUT_CROISSANCE_PEDUNCULE/production_speed) * Teff;
        //        else
        //            demand_pot = 0;
    }


};
} //namespace model
#endif // PEDUNCLE_H
