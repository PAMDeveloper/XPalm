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
    enum internals { BIOMASS,
                     TRUNK_BIOMASS,
                     LEAF_RESERVE_MAX,
                     LEAF_RESERVE_MIN,
                     TRUNK_RESERVE_MIN,
                     TRUNK_RESERVE_MAX,
                     RESERVE_MIN,
                     RESERVE_MAX,
                     APPORT,
                     ACTUAL_MOB_RATE,
                     LEAF_APPORT,
                     TRUNK_APPORT,
                     ATTRAC_TRUNK,
                     ATTRAC_LEAF,
                     DELTA_BIOMASSE_RESERVE_STIPE,
                     DELTA_BIOMASSE_RESERVE_LEAF,
                     DELTA_BIOMASS_TOT,
                     TREE_OFFRE_POUR_RESPI,
                     MOB_TRUNK,
                     MOB_LEAF,
                     TREE_DELTA_AV_RESPI,
                     TREE_OFFRE_POUR_CROISSANCE,
                     TREE_DELTA_AVANT_CROISSANCE,
                     TREE_FR_AVANT_CROISSANCE };

    enum externals { TOTALLEAFAREA,
                     TREE_TRUNK_BIOMASS,
                     LEAF_NON_STRUCTURAL_BIOMASS,
                     TREE_ASSIM,
                     FR_AVANT_CROISSANCE,
                     DELTA_AVANT_CROISSANCE,
                     RESPI_MANTENANCE,
                     TREE_GROWTH_DEMAN };

private:

    //      parameters
    double POURCENT_NSC_ST_INI;
    double POURCENT_NSC_ST_MAX;
    double POURCENT_NSC_ST_MIN;
    double SLW_max;
    double SLW_min;
    double POURC_FOLIOLE;
    double COUT_RESERVE;
    double REALL_COST;
    double MOB_RATE_MAX;

    //     internals
    double biomass;
    double trunk_biomass;
    double leaf_reserve_max;
    double leaf_reserve_min;
    double trunk_reserve_min;
    double trunk_reserve_max;
    double reserve_min;
    double reserve_max;
    double apport;
    double actual_mob_rate;
    double leaf_apport;
    double trunk_apport;
    double attrac_trunk;
    double attrac_leaf;
    double delta_biomasse_reserve_stipe;
    double delta_biomasse_reserve_leaf;
    double delta_biomass_tot;
    double tree_offre_pour_respi;
    double mob_trunk;
    double mob_leaf;
    double tree_delta_av_respi;
    double tree_offre_pour_croissance;
    double tree_delta_avant_croissance;
    double tree_fr_avant_croissance;

    //     externals
    double totalLeafArea;
    double tree_trunk_biomass;
    double leaf_non_structural_biomass;
    double tree_Assim;
    double fr_avant_croissance;
    double delta_avant_croissance;
    double respi_maintenance;
    double tree_growth_demand;

public:

    Reserve()
    {
        //         internals
        Internal(BIOMASS, &Reserve::biomass);
        Internal(TRUNK_BIOMASS, &Reserve::trunk_biomass);
        Internal(LEAF_RESERVE_MAX, &Reserve::leaf_reserve_max);
        Internal(LEAF_RESERVE_MIN, &Reserve::leaf_reserve_min);
        Internal(TRUNK_RESERVE_MIN, &Reserve::trunk_reserve_min);
        Internal(TRUNK_RESERVE_MAX, &Reserve::trunk_reserve_max);
        Internal(RESERVE_MIN, &Reserve::reserve_min);
        Internal(RESERVE_MAX, &Reserve::reserve_max);
        Internal(APPORT, &Reserve::apport);
        Internal(ACTUAL_MOB_RATE, &Reserve::actual_mob_rate);
        Internal(LEAF_APPORT, &Reserve::leaf_apport);
        Internal(TRUNK_APPORT, &Reserve::trunk_apport);
        Internal(ATTRAC_TRUNK, &Reserve::attrac_trunk);
        Internal(ATTRAC_LEAF, &Reserve::attrac_leaf);
        Internal(DELTA_BIOMASSE_RESERVE_STIPE, &Reserve::delta_biomasse_reserve_stipe);
        Internal(DELTA_BIOMASSE_RESERVE_LEAF, &Reserve::delta_biomasse_reserve_leaf);
        Internal(DELTA_BIOMASS_TOT, &Reserve::delta_biomass_tot);
        Internal(TREE_OFFRE_POUR_RESPI, &Reserve::tree_offre_pour_respi);
        Internal(MOB_TRUNK, &Reserve::mob_trunk);
        Internal(MOB_LEAF, &Reserve::mob_leaf);
        Internal(TREE_DELTA_AV_RESPI, &Reserve::tree_delta_av_respi);
        Internal(TREE_OFFRE_POUR_CROISSANCE, &Reserve::tree_offre_pour_croissance);
        Internal(TREE_OFFRE_POUR_CROISSANCE, &Reserve::tree_offre_pour_croissance);
        Internal(TREE_DELTA_AVANT_CROISSANCE, &Reserve::tree_delta_avant_croissance);
        Internal(TREE_FR_AVANT_CROISSANCE, &Reserve::tree_fr_avant_croissance);

        //          externals
        External(TOTALLEAFAREA, &Reserve::totalLeafArea);
        External(TREE_TRUNK_BIOMASS, &Reserve::tree_trunk_biomass);
        External(LEAF_NON_STRUCTURAL_BIOMASS, &Reserve::leaf_non_structural_biomass);
        External(TREE_ASSIM, &Reserve::tree_Assim);
        External(FR_AVANT_CROISSANCE, &Reserve::fr_avant_croissance);
        External(DELTA_AVANT_CROISSANCE, &Reserve::delta_avant_croissance);
        External(RESPI_MANTENANCE, &Reserve::respi_maintenance);
        External(TREE_GROWTH_DEMAN, &Reserve::tree_growth_demand);

    }

    virtual ~Reserve()
    {
    }


    void init(double t, const xpalm::ModelParameters& parameters) //TODO externals in init ???
    {
        last_time = t;

        //        parameters
        POURCENT_NSC_ST_INI = parameters.get("POURCENT_NSC_ST_INI");
        POURCENT_NSC_ST_MAX = parameters.get("POURCENT_NSC_ST_MAX");
        POURCENT_NSC_ST_MIN = parameters.get("POURCENT_NSC_ST_MIN");
        SLW_max = parameters.get("SLW_max");
        SLW_min = parameters.get("SLW_min");
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        COUT_RESERVE = parameters.get("COUT_RESERVE");
        REALL_COST = parameters.get("REALL_COST");
        MOB_RATE_MAX = parameters.get("MOB_RATE_MAX");

        //internals
        biomass = POURCENT_NSC_ST_INI * trunk_biomass + leaf_non_structural_biomass;
        trunk_biomass = POURCENT_NSC_ST_INI * trunk_biomass;
        leaf_reserve_max = ((SLW_max - SLW_min) * 10 ) * totalLeafArea / POURC_FOLIOLE;
        leaf_reserve_min = 0;
        trunk_reserve_min = POURCENT_NSC_ST_MIN * trunk_biomass;
        trunk_reserve_max = POURCENT_NSC_ST_MAX * trunk_biomass;
        reserve_min = trunk_reserve_min;
        reserve_max = leaf_reserve_max + trunk_reserve_max;
        apport = 0;
        actual_mob_rate = 0;
        leaf_apport = 0;
        trunk_apport = 0;
        attrac_trunk =  0;
        attrac_leaf = 0;
        delta_biomasse_reserve_stipe = 0;
        delta_biomasse_reserve_leaf = 0;
        delta_biomass_tot = 0;
        tree_offre_pour_respi = 0;
        mob_trunk = 0;
        mob_leaf = 0;
        tree_delta_av_respi = 0;
        tree_offre_pour_croissance = 0;
    }



    void compute_etat_reserve(){
        delta_biomasse_reserve_stipe = 0;
        delta_biomasse_reserve_leaf = 0;
        delta_biomass_tot = 0;

        //            compute_reserve_max_min
        trunk_reserve_min = POURCENT_NSC_ST_MIN * tree_trunk_biomass; //TODO Check bug entre tree.trunk_biomass et trunk_biomass
        trunk_reserve_max = POURCENT_NSC_ST_MAX * tree_trunk_biomass;
        leaf_reserve_min = 0;
        leaf_reserve_max = totalLeafArea * ((SLW_max - SLW_min) * 10 ) / POURC_FOLIOLE;
        reserve_max = leaf_reserve_max + trunk_reserve_max;

        //            compute_etat_test_min
        biomass = leaf_non_structural_biomass + trunk_biomass;  //# on est oblige de prendre en compte sa taille pour pouvoir prendre en compte la chute des feuilles
        if (biomass <   reserve_min) {
            if (biomass + COUT_RESERVE > reserve_min)
                tree_offre_pour_respi = tree_Assim - (reserve_min - biomass)/COUT_RESERVE;
            else
                tree_offre_pour_respi = 0; //TODO Affectation d'un external
            biomass = reserve_min;

            trunk_biomass = trunk_reserve_min;
            delta_biomasse_reserve_leaf += - (leaf_non_structural_biomass - leaf_reserve_min);
        } else
            tree_offre_pour_respi = tree_Assim;




        //            compute_etat_apres_respiration
            //compute_capacite_de_mobilisation_biomasse_feuille_stipe;
        if ( ((trunk_biomass - trunk_reserve_min) + (leaf_non_structural_biomass - leaf_reserve_min)) != 0 ) {
            mob_trunk = (trunk_biomass - trunk_reserve_min) / ((trunk_biomass - trunk_reserve_min) + (leaf_non_structural_biomass - leaf_reserve_min));
            mob_leaf = (leaf_non_structural_biomass - leaf_reserve_min) / ((trunk_biomass - trunk_reserve_min) + (leaf_non_structural_biomass - leaf_reserve_min));
        } else {
            mob_trunk = 0;
            mob_leaf = 0;
        }

        tree_delta_av_respi = tree_offre_pour_respi - respi_maintenance; //TODO tree.delta_av_respi external accessor

        if (tree_delta_av_respi < 0) {
            if (REALL_COST * (biomass - reserve_min) + tree_delta_av_respi < 0)
                apport =  REALL_COST * (biomass - reserve_min);
            else
                apport = - tree_delta_av_respi;
            tree_offre_pour_croissance = 0 ;//TODO tree.offre_pour_croissance external accessor
        } else {
            apport = 0;
            tree_offre_pour_croissance = tree_delta_av_respi;
        }
        biomass +=  - apport / REALL_COST;
        trunk_biomass += - apport / REALL_COST * mob_trunk;

        tree_delta_avant_croissance = tree_offre_pour_croissance - tree_growth_demand;
        tree_fr_avant_croissance = tree_offre_pour_croissance / tree_growth_demand;
        delta_biomasse_reserve_leaf += - apport / REALL_COST * mob_leaf;

        //            compute_actual_mob_rate
        actual_mob_rate = MOB_RATE_MAX / (reserve_max - reserve_min) * ( biomass - reserve_min );

        //            compute_etat_reserve_apres_croissance

        if (fr_avant_croissance < 1) {
            //            compute_capacite_de_mobilisation_biomasse_feuille_stipe();
            if ( ((trunk_biomass - trunk_reserve_min) + (leaf_non_structural_biomass - leaf_reserve_min)) != 0 ) {
                mob_trunk = (trunk_biomass - trunk_reserve_min) / ((trunk_biomass - trunk_reserve_min) + (leaf_non_structural_biomass - leaf_reserve_min));
                mob_leaf = (leaf_non_structural_biomass - leaf_reserve_min) / ((trunk_biomass - trunk_reserve_min) + (leaf_non_structural_biomass - leaf_reserve_min));
            } else {
                mob_trunk = 0;
                mob_leaf = 0;
            }
            if ((REALL_COST * actual_mob_rate * biomass ) > - delta_avant_croissance)
                apport =  - delta_avant_croissance;
            else
                apport = REALL_COST * actual_mob_rate * biomass;

            biomass +=  - apport / REALL_COST;
            trunk_biomass += - apport / REALL_COST * mob_trunk;

            delta_biomass_tot += - apport / REALL_COST;
            delta_biomasse_reserve_leaf += - apport / REALL_COST * mob_leaf;
            delta_biomasse_reserve_stipe += - apport / REALL_COST * mob_trunk;

        } else {
            //                    compute_capacite_d_attraction_biomasse_feuille_stipe()
            attrac_trunk = (trunk_reserve_max - trunk_biomass) /( (leaf_reserve_max - leaf_non_structural_biomass ) + (trunk_reserve_max - trunk_biomass));
            attrac_leaf = (leaf_reserve_max - leaf_non_structural_biomass ) /( (leaf_reserve_max - leaf_non_structural_biomass ) + (trunk_reserve_max - trunk_biomass));

            if ((biomass + COUT_RESERVE * delta_avant_croissance ) < reserve_max) {
                trunk_biomass += COUT_RESERVE * attrac_trunk *  delta_avant_croissance;
                biomass += COUT_RESERVE * delta_avant_croissance;
                delta_biomass_tot += COUT_RESERVE * delta_avant_croissance;
                delta_biomasse_reserve_leaf += COUT_RESERVE * attrac_leaf *  delta_avant_croissance;
                delta_biomasse_reserve_stipe += COUT_RESERVE * attrac_trunk *  delta_avant_croissance;
            } else {
                delta_biomass_tot = reserve_max - biomass;
                biomass = reserve_max;
                delta_biomasse_reserve_stipe += trunk_reserve_max - trunk_biomass;
                trunk_biomass = trunk_reserve_max;
                delta_biomasse_reserve_leaf += leaf_reserve_max - leaf_non_structural_biomass;
            }
            apport = - delta_avant_croissance;
        }
    }



    void compute(double t, bool /* update */)
    {
        compute_etat_reserve();
    }

    //    def compute_etat_test_min(self) :
    //     # avant tout il faut verifier qu'on est au dessus de la reserve mini. Ce cas-la peut arriver car la reserve mini augmente et qu'on a jamais rien stocke dedans. Il existe donc cette demande ... Mais dans le cas d'un deficit prolonge en assimilats d'ou peuvent bien venir ces assimilats ?
    //     #  # pour l'instant pour eviter des bugs si il n'y pas ces assimilats on les fait arriver ...
};

} // namespace model
#endif
