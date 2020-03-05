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
                     MOB_RATE,
                     LEAVES_RES,
                     LEAVES_RES_MIN,
                     LEAVES_RES_MAX,
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
                     RESERVE_AVAI};

    enum externals { PLANTLEAFAREA,
                     TRUNK_BIOMASS,
                     LEAVES_NON_STRUCTURAL_BIOMASS,
                     MAINTENANCE_RESPI,
                     TREE_GROWTH_DEMAND,
                     TREE_ASSIM,
                     LEAVES_RESERVE_SURPLUS};

private:

    //      parameters
    double POURCENT_NSC_ST_MAX;
    double POURCENT_NSC_ST_MIN;
    double SLW_max;
    double SLW_min;
    double SLW_ini;
    double POURC_FOLIOLE;
    double COUT_RESERVE;
    double REALL_COST;
    double MOB_RATE_MAX;
    double POURCENT_NSC_ST_INI;

    //     internals
    double reserve;
    double assim_avai;
    double assim_res_avai;
    double mob_rate;
    double leaves_res, leaves_res_min, leaves_res_max, leaves_res_pot, leaves_res_avai;
    double trunk_res, trunk_res_min, trunk_res_max, trunk_res_pot, trunk_res_avai;
    double reserve_max, reserve_min, reserve_pot, reserve_avai;
    double growth_offer;

    //     externals
    double plantLeafArea;
    double trunk_initial_height;
    double leaves_non_structural_biomass;
    double maintenance_respi;
    double tree_growth_demand;
    double tree_Assim;
    double trunk_biomass;
    double leaves_reserve_surplus;

public:

    Reserve()
    {
        //         internals
        Internal(GROWTH_OFFER, &Reserve::growth_offer);
        Internal(MOB_RATE, &Reserve::mob_rate);
        Internal(LEAVES_RES, &Reserve::leaves_res);
        Internal(LEAVES_RES_MIN, &Reserve::leaves_res_min);
        Internal(LEAVES_RES_MAX, &Reserve::leaves_res_max);
        Internal(LEAVES_RES_POT, &Reserve::leaves_res_pot);
        Internal(LEAVES_RES_AVAI, &Reserve::leaves_res_avai);
        Internal(TRUNK_RES, &Reserve::trunk_res);
        Internal(TRUNK_RES_MIN, &Reserve::trunk_res_min);
        Internal(TRUNK_RES_MAX, &Reserve::trunk_res_max);
        Internal(TRUNK_RES_POT, &Reserve::trunk_res_pot);
        Internal(TRUNK_RES_AVAI, &Reserve::trunk_res_avai);
        Internal(RESERVE_MAX, &Reserve::reserve_max);
        Internal(RESERVE_MIN, &Reserve::reserve_min);
        Internal(RESERVE_POT, &Reserve::reserve_pot);
        Internal(RESERVE_AVAI, &Reserve::reserve_avai);
        Internal(RESERVE, &Reserve::reserve);

        //          externals
        External(PLANTLEAFAREA, &Reserve::plantLeafArea);
        External(TRUNK_BIOMASS, &Reserve::trunk_biomass);
        External(LEAVES_NON_STRUCTURAL_BIOMASS, &Reserve::leaves_non_structural_biomass);
        External(MAINTENANCE_RESPI, &Reserve::maintenance_respi);
        External(TREE_GROWTH_DEMAND, &Reserve::tree_growth_demand);
        External(TREE_ASSIM, &Reserve::tree_Assim);
        External(LEAVES_RESERVE_SURPLUS, &Reserve::leaves_reserve_surplus);

    }

    virtual ~Reserve()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double plantLeafArea_, double trunk_initial_height_) //TODO externals in init ???
    {
        init(t, parameters);
        last_time = t-1;

        //        parameters
        POURCENT_NSC_ST_INI = parameters.get("POURCENT_NSC_ST_INI");
        POURCENT_NSC_ST_MAX = parameters.get("POURCENT_NSC_ST_MAX");
        POURCENT_NSC_ST_MIN = parameters.get("POURCENT_NSC_ST_MIN");
        SLW_max = parameters.get("SLW_max") * 10; //kg.m-2
        SLW_ini = parameters.get("SLW_ini") * 10; //kg.m-2
        SLW_min = parameters.get("SLW_min") * 10; //kg.m-2
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        COUT_RESERVE = parameters.get("COUT_RESERVE");
        REALL_COST = parameters.get("REALL_COST");
        MOB_RATE_MAX = parameters.get("MOB_RATE_MAX");

        //externals
        double STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY");
        double STEM_RAYON = parameters.get("STEM_RAYON");
        //        double INITIAL_HEIGHT = parameters.get("INITIAL_HEIGHT"); //cm

        //        trunk_biomass= STEM_APPARENT_DENSITY * _PI * pow( STEM_RAYON, 2) * INITIAL_HEIGHT; //gDM

        trunk_initial_height= trunk_initial_height_;
        trunk_biomass= STEM_APPARENT_DENSITY * _PI * pow( STEM_RAYON, 2) * trunk_initial_height;
        assim_avai=0;
        assim_res_avai=0;
        leaves_res=0;
        plantLeafArea=plantLeafArea_;
        growth_offer=0;

        //internals

        leaves_non_structural_biomass=0;
        leaves_reserve_surplus=0;
        trunk_res = POURCENT_NSC_ST_INI * trunk_biomass;
        reserve = trunk_res + leaves_non_structural_biomass;
        leaves_res_max = (SLW_max - SLW_min) * plantLeafArea *10000 / POURC_FOLIOLE; //gDM
        leaves_res_min = 0;
        leaves_res_pot = leaves_res_max - leaves_res_min;
        leaves_res_avai=0;
        trunk_res_min = POURCENT_NSC_ST_MIN * trunk_biomass;
        trunk_res_max = POURCENT_NSC_ST_MAX * trunk_biomass;
        trunk_res_pot = trunk_res_max - trunk_res_min;
        trunk_res_avai = trunk_res - trunk_res_min;
        reserve_min = trunk_res_min;
        reserve_max = leaves_res_max + trunk_res_max;
        reserve_pot = reserve_max - reserve_min;
        reserve_avai = leaves_res_avai + trunk_res_avai;
        mob_rate = ( MOB_RATE_MAX / reserve_pot ) * reserve_avai;

    }




    void compute(double /*t*/, bool /* update */)
    {
        //   |=======|--------|...............|
        //   |       |  avai  |      pot      |
        //   0      min      res             max
        //
        // scheme of reserves structure


        //            compute_reserve_max_min
        trunk_res_min = POURCENT_NSC_ST_MIN * trunk_biomass;
        trunk_res_max = POURCENT_NSC_ST_MAX * trunk_biomass;
        trunk_res_pot = trunk_res_max - trunk_res_min;

        leaves_res_min = 0;
        leaves_res_max = plantLeafArea * 10000 * (SLW_max - SLW_min) / POURC_FOLIOLE; // m2 *10000 * (g.cm-2) = g
        leaves_res_pot = leaves_res_max - leaves_res_min;

        leaves_res = leaves_non_structural_biomass;

        trunk_res = trunk_res + leaves_reserve_surplus;
        reserve = leaves_res + trunk_res;
        reserve_max = leaves_res_max + trunk_res_max;
        reserve_min = leaves_res_min + trunk_res_min;
        reserve_pot = reserve_max - reserve_min;


        //Maintenance respiration

        //compute_capacite_de_mobilisation_biomasse_feuille_stipe;

        double trunk_mob_pct = reserve > 0 ? trunk_res / reserve : 0;
        double leaves_mob_pct = reserve > 0 ? leaves_res / reserve : 0;

        //            compute_etat_apres_respiration
        assim_avai = tree_Assim;

        if(maintenance_respi > assim_avai) {

            maintenance_respi-=assim_avai;
            assim_avai = 0;

            double respi_reserve_consumed = min ( maintenance_respi/ REALL_COST, reserve );

            trunk_res -= (respi_reserve_consumed * trunk_mob_pct);
            leaves_res -= (respi_reserve_consumed * leaves_mob_pct);
            reserve = leaves_res + trunk_res;

            maintenance_respi -= ( respi_reserve_consumed * REALL_COST );
        } else {
            assim_avai -= maintenance_respi;
            maintenance_respi = 0;
        }


        //   allocate assim left to fulfill minimale reserves

        //        double assim_to_res_pot = tree_Assim * COUT_RESERVE;
        trunk_res_avai = trunk_res - trunk_res_min;
        leaves_res_avai = leaves_res - leaves_res_min;


        assim_res_avai = assim_avai * COUT_RESERVE;


        if(leaves_res_avai < 0 && assim_res_avai > 0) {
            double delta = min(-leaves_res_avai, assim_res_avai);
            assim_res_avai -= delta;
            leaves_res += delta;
        }

        if(trunk_res_avai < 0 && assim_res_avai > 0) {
            double delta = min(-trunk_res_avai, assim_res_avai);
            assim_res_avai -= delta;
            trunk_res += delta;
        }

        assim_avai = assim_res_avai / COUT_RESERVE;
        reserve = leaves_res + trunk_res;

        leaves_res_avai = leaves_res - leaves_res_min;
        trunk_res_avai = trunk_res - trunk_res_min;
        reserve_avai = leaves_res_avai + trunk_res_avai;

        //            compute_actual_mob_rate
        //        mob_rate = ( MOB_RATE_MAX / reserve_pot ) * reserve_avai;


        growth_offer=assim_avai;

        //compute reserve after growth

        if (assim_avai < tree_growth_demand) {

            //rate of reserve per compartment (trunk and leaved) than can be allocate for growth
            trunk_mob_pct = reserve_avai > 0 ? trunk_res_avai / reserve_avai : 0;
            leaves_mob_pct = reserve_avai > 0 ? leaves_res_avai / reserve_avai : 0;

            //reserve consumed for growth
            tree_growth_demand-=assim_avai;
            assim_avai = 0;

            double growth_reserve_consumed = min ( tree_growth_demand / REALL_COST, reserve_avai );

            trunk_res -= (growth_reserve_consumed * trunk_mob_pct);
            leaves_res -= (growth_reserve_consumed * leaves_mob_pct);

            reserve = leaves_res + trunk_res;

        } else {

            assim_avai -= tree_growth_demand;
            tree_growth_demand=0;

            trunk_res_pot = trunk_res_max - trunk_res;
            leaves_res_pot = leaves_res_max - leaves_res;
            reserve_pot = trunk_res_pot + leaves_res_pot;

            double trunk_sink_pct = trunk_res_pot / reserve_pot;
            double leaves_sink_pct = leaves_res_pot / reserve_pot;

            assim_res_avai = min ( assim_avai * COUT_RESERVE, reserve_pot);

            //            if (assim_res_avai==reserve_pot){
            //                reserve_pot=reserve_pot;
            //            }

            trunk_res += (assim_res_avai * trunk_sink_pct);
            leaves_res += (assim_res_avai * leaves_sink_pct);

            assim_avai -= ( assim_res_avai / COUT_RESERVE );
        }


        leaves_res_avai = leaves_res - leaves_res_min;
        trunk_res_avai = trunk_res - trunk_res_min;
        reserve_avai = leaves_res_avai + trunk_res_avai;
    }

    //    def compute_etat_test_min(self) :
    //     # avant tout il faut verifier qu'on est au dessus de la reserve mini. Ce cas-la peut arriver car
    //         # la reserve mini augmente et qu'on a jamais rien stocke dedans. Il existe donc cette demande ... Mais dans le cas
    //         # d'un deficit prolonge en assimilats d'ou peuvent bien venir ces assimilats ?
    //     # pour l'instant pour eviter des bugs si il n'y pas ces assimilats on les fait arriver ...
};

} // namespace model
#endif
