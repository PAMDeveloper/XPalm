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
                     ASSIM_AVAI,
                     MOB_RATE,
                     LEAF_RES,
                     LEAF_RES_MIN,
                     LEAF_RES_MAX,
                     LEAF_RES_POT,
                     LEAF_RES_AVAI,
                     TRUNK_RES,
                     TRUNK_RES_MIN,
                     TRUNK_RES_MAX,
                     TRUNK_RES_POT,
                     TRUNK_RES_AVAI,
                     RESERVE_MAX,
                     RESERVE_MIN,
                     RESERVE_POT,
                     RESERVE_AVAI };

    enum externals { TOTALLEAFAREA,
                     TREE_TRUNK_BIOMASS,
                     LEAF_NON_STRUCTURAL_BIOMASS,
                     MAINTENANCE_RESPI,
                     TREE_GROWTH_DEMAND,
                     TREE_ASSIM };

private:

    //      parameters
    double POURCENT_NSC_ST_MAX;
    double POURCENT_NSC_ST_MIN;
    double SLW_max;
    double SLW_min;
    double POURC_FOLIOLE;
    double COUT_RESERVE;
    double REALL_COST;
    double MOB_RATE_MAX;

    //     internals
    double reserve;
    double assim_avai;
    double mob_rate;
    double leaf_res, leaf_res_min, leaf_res_max, leaf_res_pot, leaf_res_avai;
    double trunk_res, trunk_res_min, trunk_res_max, trunk_res_pot, trunk_res_avai;
    double reserve_max, reserve_min, reserve_pot, reserve_avai;

    //     externals
    double totalLeafArea;
    double tree_trunk_biomass;
    double leaf_non_structural_biomass;
    double maintenance_respi;
    double tree_growth_demand;
    double tree_Assim;

public:

    Reserve()
    {
        //         internals
        Internal(ASSIM_AVAI, &Reserve::assim_avai);
        Internal(MOB_RATE, &Reserve::mob_rate);
        Internal(LEAF_RES, &Reserve::leaf_res);
        Internal(LEAF_RES_MIN, &Reserve::leaf_res_min);
        Internal(LEAF_RES_MAX, &Reserve::leaf_res_max);
        Internal(LEAF_RES_POT, &Reserve::leaf_res_pot);
        Internal(LEAF_RES_AVAI, &Reserve::leaf_res_avai);
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
        External(TOTALLEAFAREA, &Reserve::totalLeafArea);
        External(TREE_TRUNK_BIOMASS, &Reserve::tree_trunk_biomass);
        External(LEAF_NON_STRUCTURAL_BIOMASS, &Reserve::leaf_non_structural_biomass);
        External(MAINTENANCE_RESPI, &Reserve::maintenance_respi);
        External(TREE_GROWTH_DEMAND, &Reserve::tree_growth_demand);
        External(TREE_ASSIM, &Reserve::tree_Assim);

    }

    virtual ~Reserve()
    {
    }


    void init(double t, const xpalm::ModelParameters& parameters) //TODO externals in init ???
    {
        last_time = t-1;

        //        parameters
        POURCENT_NSC_ST_MAX = parameters.get("POURCENT_NSC_ST_MAX");
        POURCENT_NSC_ST_MIN = parameters.get("POURCENT_NSC_ST_MIN");
        SLW_max = parameters.get("SLW_max") * 10; //kg.m-2
        SLW_min = parameters.get("SLW_min") * 10; //kg.m-2
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        COUT_RESERVE = parameters.get("COUT_RESERVE");
        REALL_COST = parameters.get("REALL_COST");
        MOB_RATE_MAX = parameters.get("MOB_RATE_MAX");

        //internals
        double POURCENT_NSC_ST_INI = parameters.get("POURCENT_NSC_ST_INI");
        reserve = POURCENT_NSC_ST_INI * tree_trunk_biomass + leaf_non_structural_biomass;
        trunk_res = POURCENT_NSC_ST_INI * tree_trunk_biomass;
        leaf_res_max = (SLW_max - SLW_min) * totalLeafArea / POURC_FOLIOLE;
        leaf_res_min = 0;
        trunk_res_min = POURCENT_NSC_ST_MIN * tree_trunk_biomass;
        trunk_res_max = POURCENT_NSC_ST_MAX * tree_trunk_biomass;
        reserve_min = trunk_res_min;
        reserve_max = leaf_res_max + trunk_res_max;
    }




    void compute(double /*t*/, bool /* update */)
    {
        //   |=======|--------|...............|
        //   |       |  avai  |      pot      |
        //   0      min      res             max
        //
        // schema du fonctionnement des réserves


        //            compute_reserve_max_min
        trunk_res_min = POURCENT_NSC_ST_MIN * tree_trunk_biomass; //TODO Check bug entre tree.trunk_biomass et trunk_biomass
        trunk_res_max = POURCENT_NSC_ST_MAX * tree_trunk_biomass;
        trunk_res_pot = trunk_res_max - trunk_res_min;

        leaf_res_min = 0;
        leaf_res_max = totalLeafArea * (SLW_max - SLW_min) / POURC_FOLIOLE; // m2 * (g.cm-2) * 10 -> kg.m-2
        leaf_res_pot = leaf_res_max - leaf_res_min;
        leaf_res = leaf_non_structural_biomass;

        reserve = leaf_res + trunk_res;
        reserve_max = leaf_res_max + trunk_res_max;
        reserve_min = leaf_res_min + trunk_res_min;
        reserve_pot = reserve_max - reserve_min;

        //compute_capacite_de_mobilisation_biomasse_feuille_stipe;

        double trunk_mob_pct = reserve > 0 ? trunk_res / reserve : 0;
        double leaf_mob_pct = reserve > 0 ? leaf_res / reserve : 0;

        //Maintenance respi
        //            compute_etat_apres_respiration
        assim_avai = tree_Assim;

        if(maintenance_respi > assim_avai) {
            double respi_reserve_consumed = min ( (maintenance_respi - assim_avai) / REALL_COST, reserve );
            assim_avai = 0;
            trunk_res -= (respi_reserve_consumed * trunk_mob_pct);
            leaf_res -= (respi_reserve_consumed * leaf_mob_pct);
            reserve = leaf_res + trunk_res;

            maintenance_respi -= ( respi_reserve_consumed * REALL_COST ); //TODO only kept to show maitnenance_repi deficit
        } else {
            assim_avai -= maintenance_respi;
            maintenance_respi = 0;
        }

       //   allocate to reserve
          //# on est oblige de prendre en compte sa taille pour pouvoir prendre en compte la chute des feuilles

//        double assim_to_res_pot = tree_Assim * COUT_RESERVE;
        trunk_res_avai = trunk_res - trunk_res_min;
        leaf_res_avai = leaf_res - leaf_res_min;
        double assim_res_avai = assim_avai * COUT_RESERVE;

        if(leaf_res_avai < 0 && assim_res_avai > 0) {
            double delta = min(-leaf_res_avai, assim_res_avai);
            assim_res_avai -= delta;
            leaf_res += delta;
            leaf_res_avai = leaf_res - leaf_res_min;
        }

        if(trunk_res_avai < 0 && assim_res_avai > 0) {
            double delta = min(-trunk_res_avai, assim_res_avai);
            assim_res_avai -= delta;
            trunk_res += delta;
            trunk_res_avai = trunk_res - trunk_res_min;
        }

        assim_avai = assim_res_avai / COUT_RESERVE;
        reserve = leaf_res + trunk_res;
        reserve_avai = leaf_res_avai + trunk_res_avai;

        //            compute_actual_mob_rate
        mob_rate = ( MOB_RATE_MAX / reserve_pot ) * reserve_avai;


        //compute_etat_reserve_apres_croissance

        if (assim_avai < tree_growth_demand) {

            //compute_capacite_de_mobilisation_biomasse_feuille_stipe()
            trunk_mob_pct = reserve_avai > 0 ? trunk_res_avai / reserve_avai : 0;
            leaf_mob_pct = reserve_avai > 0 ? leaf_res_avai / reserve_avai : 0;

            double growth_reserve_consumed = min ( (tree_growth_demand - assim_avai) / REALL_COST, reserve_avai );
            assim_avai = 0;
            trunk_res -= (growth_reserve_consumed * trunk_mob_pct);
            leaf_res -= (growth_reserve_consumed * leaf_mob_pct);

            reserve = leaf_res + trunk_res;
            leaf_res_avai = leaf_res - leaf_res_min;
            trunk_res_avai = trunk_res - trunk_res_min;
            reserve_avai = leaf_res_avai + trunk_res_avai;

        } else {

            // compute_capacite_d_attraction_biomasse_feuille_stipe()
            assim_avai -= tree_growth_demand;

            double trunk_res_pot = trunk_res_max - trunk_res;
            double leaf_res_pot = leaf_res_max - leaf_res;
            double reserve_pot = trunk_res_pot + leaf_res_pot;
            double trunk_sink_pct = trunk_res_pot / reserve_pot;
            double leaf_sink_pct = leaf_res_pot / reserve_pot;

            double assim_res_avai = min ( assim_avai * COUT_RESERVE, reserve_pot);
            trunk_res += (assim_res_avai * trunk_sink_pct);
            leaf_res += (assim_res_avai * leaf_sink_pct);

            assim_avai -= ( assim_res_avai / COUT_RESERVE );
        }
    }

    //    def compute_etat_test_min(self) :
    //     # avant tout il faut verifier qu'on est au dessus de la reserve mini. Ce cas-la peut arriver car
//         # la reserve mini augmente et qu'on a jamais rien stocke dedans. Il existe donc cette demande ... Mais dans le cas
//         # d'un deficit prolonge en assimilats d'ou peuvent bien venir ces assimilats ?
    //     # pour l'instant pour eviter des bugs si il n'y pas ces assimilats on les fait arriver ...
};

} // namespace model
#endif
