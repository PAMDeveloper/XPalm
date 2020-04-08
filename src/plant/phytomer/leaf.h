#ifndef LEAF_H
#define LEAF_H
#include <defines.hpp>

namespace model {

class Leaf : public AtomicModel < Leaf >
{
public:
    enum internals { LEAFAREA,
                     LEAF_STRUCTURAL_BIOMASS,
                     LEAF_NON_STRUCTURAL_BIOMASS,
                     LEAF_STRUCTURAL_BIOMASS_HARVESTED,
                     LEAF_NON_STRUCTURAL_BIOMASS_HARVESTED,
                     LEAF_TOTAL_BIOMASS,
                     SLW,
                     GAIN_TEFF_JOUR,
                     TT_SINCE_LEAF_EXPAND,
                     VITESSE_EXP,
                     LEAF_DEMAND,
                     LEAF_RESERVE_MAX,
                     LEAF_RESERVE_POT,
                     ASSIMILATE_SUPPLY,
                     POTLEAFAREA,
                     INCREASE_POTLEAFAREA,
                     LEAF_STATE,
                     RESERVE_EXCESS,
                     RESERVE_ALLOCATED,
                     INFLEXION};

    enum externals { TEFF,
                     SF_FIN,
                     TT_SINCE_APPEARANCE,
                     PRODUCTION_SPEED,
                     RANK,
                     NUMBER,
                     FTSW,
                     INFLO_STATUT,
                     PHYTOMER_STATE,
                     FR_RESTE,
                     FRACTION_NON_STR_BIOMASSE_ALLOUEE,
                     LEAVES_RES_AVAI };

private:

    //      parameters
    double RANG_D_ABLATION;
    double SLW_min;
    double COURBURE;
    double POURC_FOLIOLE;
    //    double INCREASE_OF_LEAF_AREA;
    double SLW_ini;
//    double PLASTICITY_LEAF_IC;
    double SLW_max;
    double COUT_RESPI_FEUILLE;
    double TRESH_SLOW_PHYLO;
//    double SEUIL_EXPAN;
    double PRODUCTION_SPEED_ADULT;

    //     internals
    double leafArea; //m²
    double leafArea_cor;
    double leaf_structural_biomass;
    double leaf_non_structural_biomass;
    double leaf_total_biomass;
    double leaf_structural_biomass_harvested;
    double leaf_non_structural_biomass_harvested;
    double slw;
    double gain_TEff_jour;
    double TT_since_leaf_expand;
    //    double TT_corrige;
    double vitesse_exp;
    double leaf_demand;
    double leaf_reserve_max;
    double leaf_reserve_pot;
    double assimilate_supply;
    double potLeafArea;
    double increase_potleafArea;
    leaf::leaf_state leaf_state;
    double reserve_allocated;
    double reserve_excess;
    double inflexion;

    //     externals
    double SF_fin;
    inflo::inflo_states inflo_status;
    phytomer::phytomer_state phytomer_state;
    double TEff;
    double phytomer_rank;
    double phytomer_number;
    double TT_since_appearance;
    double ftsw;
    double fr_reste;
    double leaves_res_avai;
    double fraction_non_str_biomasse_allouee;
    double production_speed;

public:

    Leaf()
    {
        //         internals
        Internal(LEAFAREA, &Leaf::leafArea);
        Internal(LEAF_STRUCTURAL_BIOMASS, &Leaf::leaf_structural_biomass);
        Internal(LEAF_NON_STRUCTURAL_BIOMASS, &Leaf::leaf_non_structural_biomass);
        Internal(LEAF_STRUCTURAL_BIOMASS_HARVESTED, &Leaf::leaf_structural_biomass_harvested);
        Internal(LEAF_NON_STRUCTURAL_BIOMASS_HARVESTED, &Leaf::leaf_non_structural_biomass_harvested);
        Internal(LEAF_TOTAL_BIOMASS, &Leaf::leaf_total_biomass);
        Internal(SLW, &Leaf::slw);
        Internal(GAIN_TEFF_JOUR, &Leaf::gain_TEff_jour);
        Internal(TT_SINCE_LEAF_EXPAND, &Leaf::TT_since_leaf_expand);
        Internal(VITESSE_EXP, &Leaf::vitesse_exp);
        Internal(LEAF_DEMAND, &Leaf::leaf_demand);
        Internal(LEAF_RESERVE_MAX, &Leaf::leaf_reserve_max);
        Internal(LEAF_RESERVE_POT, &Leaf::leaf_reserve_pot);
        Internal(ASSIMILATE_SUPPLY, &Leaf::assimilate_supply);
        Internal(POTLEAFAREA, &Leaf::potLeafArea);
        Internal(INCREASE_POTLEAFAREA, &Leaf::increase_potleafArea);
        Internal(LEAF_STATE, &Leaf::leaf_state);
        Internal(RESERVE_EXCESS, &Leaf::reserve_excess);
        Internal(RESERVE_ALLOCATED, &Leaf::reserve_allocated);
        Internal(INFLEXION, &Leaf::inflexion);


        //          externals
        External(SF_FIN, &Leaf::SF_fin);
        External(INFLO_STATUT, &Leaf::inflo_status);
        External(PHYTOMER_STATE, &Leaf::phytomer_state);
        External(TEFF, &Leaf::TEff);
        External(RANK, &Leaf::phytomer_rank);
        External(PRODUCTION_SPEED, &Leaf::production_speed);
        External(NUMBER, &Leaf::phytomer_number);
        External(TT_SINCE_APPEARANCE, &Leaf::TT_since_appearance);
        External(FTSW, &Leaf::ftsw);
        External(FR_RESTE, &Leaf::fr_reste);
        External(LEAVES_RES_AVAI, &Leaf::leaves_res_avai);
        External(FRACTION_NON_STR_BIOMASSE_ALLOUEE, &Leaf::fraction_non_str_biomasse_allouee);

    }

    virtual ~Leaf()
    {
    }
    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, int rank, phytomer::phytomer_state state, double TT_since_appearance_, double SF_fin_leaf )
    {
        //        AtomicModel<Leaf>::init(t, parameters);

        last_time = t;

        //        parameters
        SLW_min = parameters.get("SLW_min"); //g.cm-2
        SLW_max = parameters.get("SLW_max"); //g.cm-2
        SLW_ini = parameters.get("SLW_ini"); //g.cm-2
        COUT_RESPI_FEUILLE = parameters.get("COUT_RESPI_FEUILLE");
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        TRESH_SLOW_PHYLO  = parameters.get("TRESH_SLOW_PHYLO");
        COURBURE = parameters.get("COURBURE");
//        PLASTICITY_LEAF_IC = parameters.get("PLASTICITY_LEAF_IC");
        RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT"); // (rank.DD-1)

        //        internals
        TT_since_leaf_expand=TT_since_appearance_;
        gain_TEff_jour = 0;
        leafArea_cor=0;
        vitesse_exp = 0;
        leaf_demand = 0;
        leaf_total_biomass = 0;
        leaf_reserve_max = 0;
        leaf_reserve_pot = 0;
        assimilate_supply = 0;
        potLeafArea = 0;
        increase_potleafArea = 0;
        leaf_non_structural_biomass=0;
        leaf_structural_biomass=0;
        leaf_structural_biomass_harvested = 0;
        leaf_non_structural_biomass_harvested =0;
        leaf_state=leaf::NON_COUPE;

        double tt_cor= production_speed/PRODUCTION_SPEED_ADULT; // correction factor to take into account varying production speed of leaves, enables to get leaf emmition around rank 0
        inflexion = parameters.get("INFLEXION")/tt_cor;

        //        externals
        SF_fin = SF_fin_leaf; //m2
        phytomer_rank = rank;
        phytomer_state = state;
        reserve_excess = 0;

        if ((phytomer_rank >= RANG_D_ABLATION) | (inflo_status.is(inflo::FEMALE) && !inflo_status.is(inflo::ABORTED)&& inflo_status.is(inflo::HARVEST))){
            leaf_state= leaf::COUPE;
            leaf_structural_biomass=0;
            leaf_non_structural_biomass=0;
            leafArea=0;
            slw=0;
        }


        if (leaf_state==leaf::NON_COUPE){
            potLeafArea = (SF_fin / (1 + exp(-(TT_since_leaf_expand - inflexion) / COURBURE)));


            leafArea_cor=potLeafArea;

            leafArea = ( phytomer_rank >0 )
                    ? potLeafArea
                    : 0;

            leaf_structural_biomass = potLeafArea * 10000 * SLW_min / POURC_FOLIOLE; // m2 x10000 x g.cm-2=g
            leaf_non_structural_biomass = leafArea * 10000 * (SLW_ini - SLW_min) / POURC_FOLIOLE;

            slw = (leafArea > 0)
                    ? leaf_total_biomass * POURC_FOLIOLE / (potLeafArea * 1000) //g.cm-2
                    : 0;


        }

        leaf_reserve_max = (SLW_max - SLW_min) * leafArea * 10000 / POURC_FOLIOLE;
        leaf_reserve_pot = leaf_reserve_max-leaf_non_structural_biomass;
        reserve_allocated=leaf_non_structural_biomass;
        leaf_total_biomass = leaf_structural_biomass + leaf_non_structural_biomass; //g

    }


    void compute(double t, bool /* update */)
    {
        //        GROWTH;
        reserve_excess=0;

        //        compute_coupe_feuille_recolte
        if ((phytomer_rank >= RANG_D_ABLATION) | (inflo_status.is(inflo::FEMALE) && !inflo_status.is(inflo::ABORTED) && inflo_status.is(inflo::HARVEST))){
            //         if ((phytomer_rank >= RANG_D_ABLATION)){
            leaf_state= leaf::COUPE;
            leaf_demand=0;
            assimilate_supply=0;
            leafArea=0;
            leafArea_cor=0;
            vitesse_exp=0;
            increase_potleafArea=0;
            potLeafArea=0;
            slw=0;
            leaf_structural_biomass_harvested = leaf_structural_biomass;
            leaf_non_structural_biomass_harvested = leaf_non_structural_biomass;
            leaf_structural_biomass=0;
            leaf_reserve_pot=0;
            leaf_reserve_max=0;
            reserve_allocated = 0;
        }

        else{


            //            double factor_decrease_expand=( ftsw >  SEUIL_EXPAN)
            //                    ? 1
            //                    : ftsw / SEUIL_EXPAN;

            potLeafArea += increase_potleafArea;
            //            leafArea_cor += increase_potleafArea * fr_reste * factor_decrease_expand;
            leafArea_cor += increase_potleafArea * fr_reste;
//            leafArea_cor += increase_potleafArea;
            leafArea = ( phytomer_rank >0 )
                    ? leafArea_cor
                    : 0;

            //            leafArea =leafArea_cor;
            assimilate_supply = leaf_demand * fr_reste;
            leaf_structural_biomass += assimilate_supply / COUT_RESPI_FEUILLE;

            leaf_reserve_max = (SLW_max - SLW_min) * leafArea * 10000 / POURC_FOLIOLE;
            reserve_allocated = fraction_non_str_biomasse_allouee * leaves_res_avai;

            if (reserve_allocated>=leaf_reserve_max){
                reserve_excess=reserve_allocated-leaf_reserve_max; // excess for reallocation
                reserve_allocated=leaf_reserve_max;
            }

            if (leafArea<=0){
                reserve_allocated = 0;
            }

            slw = ( leafArea > 0 )
                    ? leaf_total_biomass * POURC_FOLIOLE / (leafArea *10000)
                    : 0;


            //        GROWTH DEMAND;
            double expan_slow=(ftsw > TRESH_SLOW_PHYLO
                               ? 1
                               : ftsw / TRESH_SLOW_PHYLO );

            gain_TEff_jour = TEff * expan_slow;

//                        gain_TEff_jour = TEff;

            TT_since_leaf_expand += gain_TEff_jour;


            vitesse_exp = -(SF_fin /COURBURE) * (-exp(-(TT_since_leaf_expand-inflexion)/COURBURE))/(pow( (1+exp(-(TT_since_leaf_expand-inflexion)/COURBURE)),2));
            increase_potleafArea = gain_TEff_jour * vitesse_exp ;
            leaf_demand =  increase_potleafArea * 10000 *(SLW_min * COUT_RESPI_FEUILLE )/ POURC_FOLIOLE;

            //            }
        }

        leaf_non_structural_biomass = reserve_allocated;
        leaf_total_biomass = leaf_structural_biomass + leaf_non_structural_biomass;


    }

};
} //namespace model
#endif // LEAF_H
