/**
 * @file xpalm/plant/stock/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
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


#ifndef STOCK_MODEL_HPP
#define STOCK_MODEL_HPP
#include <defines.hpp>

namespace model {

class Reserve : public AtomicModel < Reserve >
{
public:
    enum internals { RESERVE,
                     GROWTH_OFFER,
                     LEAVES_RES,
                     LEAVES_RES_MIN,
                     LEAVES_RES_POT,
                     LEAVES_RES_AVAI,
                     TRUNK_RES,
                     TRUNK_RES_MIN,
                     TRUNK_RES_MAX,
                     TRUNK_RES_POT,
                     TRUNK_RES_AVAI,
                     RESERVE_MAX,
                     RESERVE_MIN,
                     RESERVE_POT,
                     RESERVE_AVAI,
                     ASSIM_TO_MIN_RES,
                     ASSIM_TO_RESPI,
                     RES_TO_RESPI,
                     RES_TO_GROWTH,
                     ASSIM_TO_GROWTH,
                     ASSIM_TO_RES,
                     ASSIM_EXCESS,
                     MAINTENANCE_RESPI_RES,
                     GROWTH_DEMAND_RES,
                     LEAVES_RES_EXCESS};

    enum externals { PLANTLEAFAREA,
                     TRUNK_BIOMASS,
                     LEAVES_RESERVE_MAX,
                     LEAVES_NON_STRUCTURAL_BIOMASS,
                     MAINTENANCE_RESPI,
                     GROWTH_DEMAND,
                     ASSIM,
                     LEAVES_RESERVE_EXCESS};

private:

    //      parameters
    double NSC_STEM_MAX;
    double NSC_STEM_MIN;
    double SPECIFIC_LEAFLET_WEIGHT_MAX;
    double SPECIFIC_LEAFLET_WEIGHT_MIN;
    double SPECIFIC_LEAFLET_WEIGHT_INI;
    double LEAFLET_BIOMASS_CONTRIBUTION;
    double COST_RESERVE_STORAGE;
    double COST_RESERVE_MOBILISATION;
//    double MOB_RATE_MAX;
    double NSC_STEM_INI;
    double STEM_APPARENT_DENSITY;
    double STEM_RADIUS;

    //     internals
    double reserve;
    double assim_avai;
    double leaves_res, leaves_res_min, leaves_res_pot, leaves_res_avai;
    double trunk_res, trunk_res_min, trunk_res_max, trunk_res_pot, trunk_res_avai;
    double reserve_max, reserve_min, reserve_pot, reserve_avai;
    double growth_offer;
    double assim_to_respi;
    double res_to_respi;
    double assim_to_min_res;
    double assim_to_growth;
    double assim_to_res;
    double res_to_growth;
    double assim_excess;
    double maintenance_respi_res;
    double growth_demand_res;
    double leaves_res_excess;

    //     externals
    double plantLeafArea;
    double leaves_res_max;
    double trunk_initial_height;
    double leaves_non_structural_biomass;
    double maintenance_respi;
    double growth_demand;
    double assim;
    double trunk_biomass;
    double leaves_reserve_excess;

public:

    Reserve()
    {
        //         internals
        Internal(RESERVE, &Reserve::reserve);
        Internal(GROWTH_OFFER, &Reserve::growth_offer);
        Internal(LEAVES_RES, &Reserve::leaves_res);
        Internal(LEAVES_RES_MIN, &Reserve::leaves_res_min);
        Internal(LEAVES_RES_POT, &Reserve::leaves_res_pot);
        Internal(LEAVES_RES_AVAI, &Reserve::leaves_res_avai);
        Internal(LEAVES_RES_EXCESS, &Reserve::leaves_res_excess);
        Internal(TRUNK_RES, &Reserve::trunk_res);
        Internal(TRUNK_RES_MIN, &Reserve::trunk_res_min);
        Internal(TRUNK_RES_MAX, &Reserve::trunk_res_max);
        Internal(TRUNK_RES_POT, &Reserve::trunk_res_pot);
        Internal(TRUNK_RES_AVAI, &Reserve::trunk_res_avai);
        Internal(RESERVE_MAX, &Reserve::reserve_max);
        Internal(RESERVE_MIN, &Reserve::reserve_min);
        Internal(RESERVE_POT, &Reserve::reserve_pot);
        Internal(RESERVE_AVAI, &Reserve::reserve_avai);
        Internal(ASSIM_TO_MIN_RES, &Reserve::assim_to_min_res);
        Internal(ASSIM_TO_RESPI, &Reserve::assim_to_respi);
        Internal(RES_TO_RESPI, &Reserve::res_to_respi);
        Internal(RES_TO_GROWTH, &Reserve::res_to_growth);
        Internal(ASSIM_TO_GROWTH, &Reserve::assim_to_growth);
        Internal(ASSIM_TO_RES, &Reserve::assim_to_res);
        Internal(ASSIM_EXCESS, &Reserve::assim_excess);
        Internal(MAINTENANCE_RESPI_RES, &Reserve::maintenance_respi_res);
        Internal(GROWTH_DEMAND_RES, &Reserve::growth_demand_res);


        //          externals
        External(PLANTLEAFAREA, &Reserve::plantLeafArea);
        External(TRUNK_BIOMASS, &Reserve::trunk_biomass);
        External(LEAVES_NON_STRUCTURAL_BIOMASS, &Reserve::leaves_non_structural_biomass);
        External(LEAVES_RESERVE_MAX, &Reserve::leaves_res_max);
        External(MAINTENANCE_RESPI, &Reserve::maintenance_respi);
        External(GROWTH_DEMAND, &Reserve::growth_demand);
        External(ASSIM, &Reserve::assim);
        External(LEAVES_RESERVE_EXCESS, &Reserve::leaves_reserve_excess);

    }

    virtual ~Reserve()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double plantLeafArea_, double trunk_initial_height_,double total_leaves_initial_res_) //TODO externals in init ???
    {
        init(t, parameters);
        last_time = t-1;

        //        parameters
        NSC_STEM_INI = parameters.get("NSC_STEM_INI");
        NSC_STEM_MAX = parameters.get("NSC_STEM_MAX");
        NSC_STEM_MIN = parameters.get("NSC_STEM_MIN");
        SPECIFIC_LEAFLET_WEIGHT_MAX = parameters.get("SPECIFIC_LEAFLET_WEIGHT_MAX") * 10000; //g.cm-2 *100000 = g.m-2
        SPECIFIC_LEAFLET_WEIGHT_INI = parameters.get("SPECIFIC_LEAFLET_WEIGHT_INI") * 10000; //g.m-2
        SPECIFIC_LEAFLET_WEIGHT_MIN = parameters.get("SPECIFIC_LEAFLET_WEIGHT_MIN") * 10000; //g.m-2
        LEAFLET_BIOMASS_CONTRIBUTION = parameters.get("LEAFLET_BIOMASS_CONTRIBUTION");
        COST_RESERVE_STORAGE = parameters.get("COST_RESERVE_STORAGE");
        COST_RESERVE_MOBILISATION = parameters.get("COST_RESERVE_MOBILISATION");
//        MOB_RATE_MAX = parameters.get("MOB_RATE_MAX");
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        STEM_RADIUS = parameters.get("STEM_RADIUS");

        //externals
        trunk_initial_height= trunk_initial_height_;
        trunk_biomass= STEM_APPARENT_DENSITY * _PI * pow( STEM_RADIUS, 2) * trunk_initial_height; //g.cm-3 * cm2 * cm = g
        assim_avai=0;
        leaves_non_structural_biomass=0;
        plantLeafArea=plantLeafArea_;
        growth_offer=0;

        //internals
        //        leaves_non_structural_biomass=0;
        trunk_res = NSC_STEM_INI * trunk_biomass;
        leaves_res = total_leaves_initial_res_;
        reserve = trunk_res + leaves_res;
        leaves_res_max = (SPECIFIC_LEAFLET_WEIGHT_MAX - SPECIFIC_LEAFLET_WEIGHT_MIN) * plantLeafArea/ LEAFLET_BIOMASS_CONTRIBUTION; //g.m-2 x m 2 = gDM
        leaves_res_min = 0;
        leaves_res_pot = leaves_res_max - leaves_res;
        leaves_res_avai=leaves_res - leaves_res_min;
        trunk_res_min = NSC_STEM_MIN * trunk_biomass;
        trunk_res_max = NSC_STEM_MAX * trunk_biomass;
        trunk_res_pot = trunk_res_max - trunk_res;
        trunk_res_avai = trunk_res - trunk_res_min;
        reserve_min = trunk_res_min;
        reserve_max = leaves_res_max + trunk_res_max;
        reserve_pot = reserve_max - reserve_min;
        reserve_avai = leaves_res_avai + trunk_res_avai;
        assim_to_min_res=0;
        assim_to_respi=0;
        assim_to_res=0;
        res_to_respi=0;
        res_to_growth=0;
        assim_to_growth=0;
        assim_excess=0;
        maintenance_respi_res=0;
        growth_demand_res=0;

        // externals
        leaves_reserve_excess=leaves_res_excess=0;
    }




    void compute(double /*t*/, bool /* update */)
    {
        //   |=======|--------|...............|
        //   |       |  avai  |      pot      |
        //   0      min      res             max
        //
        // scheme of reserves structure


        //            compute_reserve_max_min

        leaves_res_min = 0;
        leaves_res_max = plantLeafArea * (SPECIFIC_LEAFLET_WEIGHT_MAX - SPECIFIC_LEAFLET_WEIGHT_MIN) / LEAFLET_BIOMASS_CONTRIBUTION; // m2  g.m-2) = g
        leaves_res = leaves_non_structural_biomass; // update to take into account the removed leaf and the new openned leaf

        trunk_res_min = NSC_STEM_MIN * trunk_biomass;
        trunk_res_max = NSC_STEM_MAX * trunk_biomass;
        reserve_max = leaves_res_max + trunk_res_max;
        reserve_min = leaves_res_min + trunk_res_min;
        reserve_pot = reserve_max - reserve;

        trunk_res_pot=trunk_res_max-trunk_res;

        // reallocate leaves reserve excess toward tree reserve
        leaves_res_excess=leaves_reserve_excess;
        double excess= min (leaves_res_excess, trunk_res_pot);
        trunk_res += excess;
        leaves_res_excess -=excess;

        //Maintenance respiration
        reserve=leaves_res+trunk_res;
        double trunk_mob_pct = reserve > 0
                ? trunk_res / reserve
                : 0;
        double leaves_mob_pct = reserve > 0
                ? leaves_res / reserve
                : 0;

        //compute_etat_apres_respiration
        res_to_respi=0;
        assim_to_respi=0;

        assim_avai = assim;
        maintenance_respi_res=maintenance_respi;

        if(maintenance_respi_res > assim_avai) {

            assim_to_respi=assim_avai;
            maintenance_respi_res-=assim_avai;
            assim_avai = 0;

            res_to_respi = min ( maintenance_respi_res/ COST_RESERVE_MOBILISATION, reserve ) * COST_RESERVE_MOBILISATION;
            maintenance_respi_res -= (res_to_respi);

            trunk_res -= (res_to_respi * trunk_mob_pct);
            leaves_res -= (res_to_respi * leaves_mob_pct);
            reserve = leaves_res + trunk_res;

        } else {
            assim_to_respi = maintenance_respi_res;
            assim_avai -= maintenance_respi_res;
            maintenance_respi_res = 0;
        }


        //   allocate assim left to fulfill minimale reserves

        trunk_res_avai = trunk_res - trunk_res_min;
        leaves_res_avai = leaves_res - leaves_res_min;

        double delta_L=0;
        if(leaves_res_avai < 0 && assim_avai > 0) {
            delta_L = min(-leaves_res_avai, assim_avai * COST_RESERVE_STORAGE);
            assim_avai -= delta_L;
            leaves_res += delta_L;
        }

        double delta_T=0;
        if(trunk_res_avai < 0 && assim_avai > 0) {
            delta_T = min(-trunk_res_avai, assim_avai * COST_RESERVE_STORAGE);
            assim_avai -= delta_T;
            trunk_res += delta_T;
        }

        assim_to_min_res =delta_L + delta_T;

        reserve = leaves_res + trunk_res;

        leaves_res_avai = leaves_res - leaves_res_min;
        trunk_res_avai = trunk_res - trunk_res_min;
        reserve_avai = leaves_res_avai + trunk_res_avai;

//        if (trunk_res_avai<0){
//            trunk_res_avai=trunk_res_avai;
//        }

        //            compute_actual_mob_rate
        //        mob_rate = ( MOB_RATE_MAX / reserve_pot ) * reserve_avai;


        growth_offer = assim_avai;

        //compute reserve after growth

        growth_demand_res = growth_demand;
        assim_to_growth=0;
        res_to_growth=0;

        if (growth_offer < growth_demand) {

            //assim consumed for growth
            assim_to_growth = assim_avai;
            growth_demand_res -= assim_avai;
            assim_avai = 0;
            assim_to_res=0;

            //reserve consumed for growth
            //rate of reserve per compartment (trunk and leaved) than can be allocate for growth
            trunk_mob_pct = reserve_avai > 0 ? trunk_res_avai / reserve_avai : 0;
            leaves_mob_pct = reserve_avai > 0 ? leaves_res_avai / reserve_avai : 0;
            res_to_growth = min ( growth_demand_res / COST_RESERVE_MOBILISATION, reserve_avai) * COST_RESERVE_MOBILISATION;

            growth_demand_res -= res_to_growth;

            trunk_res -= (res_to_growth * trunk_mob_pct);
            leaves_res -= (res_to_growth * leaves_mob_pct);

        } else {

            assim_to_growth = growth_demand_res;
            res_to_growth=0;

            assim_avai -= growth_demand_res;
            growth_demand_res = 0;

            trunk_res_pot = trunk_res_max - trunk_res;
            leaves_res_pot = leaves_res_max - leaves_res;

            //version Benoit (no hierarchy)
            //            reserve_pot = trunk_res_pot + leaves_res_pot;
            //            double trunk_sink_pct = trunk_res_pot / reserve_pot;
            //            double leaves_sink_pct = leaves_res_pot / reserve_pot;
            //            assim_res_avai = min ( assim_avai * COST_RESERVE_STORAGE, reserve_pot);
            //            trunk_res += (assim_res_avai * trunk_sink_pct);
            //            leaves_res += (assim_res_avai * leaves_sink_pct);
            //            assim_to_res = assim_res_avai;
            //            assim_avai -= assim_to_res;


            // version avec hierarchy
            //first fill reserves in leaves
            double assim_avai_res_L= min (assim_avai * COST_RESERVE_STORAGE, leaves_res_pot);
            leaves_res += assim_avai_res_L;
            assim_avai -= assim_avai_res_L;

            //then fill reserves in trunk

            double assim_avai_res_T= min (assim_avai * COST_RESERVE_STORAGE, trunk_res_pot);
            trunk_res += assim_avai_res_T;
            assim_avai -= assim_avai_res_T;

            assim_to_res = assim_avai_res_L + assim_avai_res_T;

        }

        // reserve excess (roots??)
        assim_excess = assim_avai;

        reserve = leaves_res + trunk_res;
        leaves_res_avai = leaves_res - leaves_res_min;
        reserve_avai = leaves_res_avai + trunk_res_avai;

        leaves_res_pot = leaves_res_max - leaves_res;
        trunk_res_avai = trunk_res - trunk_res_min;
        trunk_res_pot = trunk_res_max - trunk_res;


    }

};

} // namespace model
#endif
