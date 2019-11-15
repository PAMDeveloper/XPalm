#ifndef LEAF_H
#define LEAF_H
#include <defines.hpp>

namespace model {

class Leaf : public AtomicModel < Leaf >
{
public:
    enum internals { LEAFAREA,
                     SFMAX,
                     STRUCTURAL_BIOMASS,
                     NON_STRUCTURAL_BIOMASS,
                     TOTAL_BIOMASS,
                     SLW,
                     GAIN_TEFF_JOUR,
                     TT_CORRIGE,
                     TT_SINCE_RANK1,
                     VITESSE_EXP,
                     DEMAND,
                     CAPACITE_RESERVE_MAX,
                     ASSIMILATE_SUPPLY,
                     POTLEAFAREA,
                     INCREASE_POTLEAFAREA };

    enum externals { TEFF,
                     PHYTOMER_RANK,
                     FTSW,
                     INFLO_STATUT,
                     PHYTOMER_STATE,
                     FR_RESTE,
                     FRACTION_NON_STR_BIOMASSE_ALLOUEE,
                     LEAF_RES_AVAI };

private:

    //      parameters
    double SLW_min;
    double INITIAL_SFIND;
    double MAXIMAL_SFIND;
    double INFLEXION;
    double COURBURE;
    double POURC_FOLIOLE;
    double INCREASE_OF_LEAF_AREA;
    double SLW_ini;
    double PLASTICITY_LEAF_IC;
    double SLW_max;
    double COUT_RESPI_FEUILLE;
    double SEUIL_DUREE;
    double SEUIL_EXPAN;

    //     internals
    double leafArea; //m²
    double SFMax;
    double structural_biomass;
    double non_structural_biomass;
    double slw;
    double gain_TEff_jour;
    double TT_since_rank1;
    double TT_corrige;
    double vitesse_exp;
    double demand;
    double total_biomass;
    double capacite_reserve_max;
    double assimilate_supply;
    double potLeafArea;
    double increase_potleafArea;


    //     externals
    inflo::inflo_states inflo_status;
    phytomer::phytomer_state phytomer_state;
    double TEff;
    double phytomer_rank;
    double ftsw;
    double fr_reste;
    double delta_biomasse_reserve_leaf;
    double fraction_non_str_biomasse_allouee;

public:

    Leaf()
    {
        //         internals
        Internal(LEAFAREA, &Leaf::leafArea);
        Internal(SFMAX, &Leaf::SFMax);
        Internal(STRUCTURAL_BIOMASS, &Leaf::structural_biomass);
        Internal(NON_STRUCTURAL_BIOMASS, &Leaf::non_structural_biomass);
        Internal(TOTAL_BIOMASS, &Leaf::total_biomass);
        Internal(SLW, &Leaf::slw);
        Internal(GAIN_TEFF_JOUR, &Leaf::gain_TEff_jour);
        Internal(TT_CORRIGE, &Leaf::TT_corrige);
        Internal(TT_SINCE_RANK1, &Leaf::TT_since_rank1);
        Internal(VITESSE_EXP, &Leaf::vitesse_exp);
        Internal(DEMAND, &Leaf::demand);
        Internal(CAPACITE_RESERVE_MAX, &Leaf::capacite_reserve_max);
        Internal(ASSIMILATE_SUPPLY, &Leaf::assimilate_supply);
        Internal(POTLEAFAREA, &Leaf::potLeafArea);
        Internal(INCREASE_POTLEAFAREA, &Leaf::increase_potleafArea);

        //          externals
        External(INFLO_STATUT, &Leaf::inflo_status);
        External(PHYTOMER_STATE, &Leaf::phytomer_state);
        External(TEFF, &Leaf::TEff);
        External(PHYTOMER_RANK, &Leaf::phytomer_rank);
        External(FTSW, &Leaf::ftsw);
        External(FR_RESTE, &Leaf::fr_reste);
        External(LEAF_RES_AVAI, &Leaf::delta_biomasse_reserve_leaf);
        External(FRACTION_NON_STR_BIOMASSE_ALLOUEE, &Leaf::fraction_non_str_biomasse_allouee);

    }

    virtual ~Leaf()
    {
    }
    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double phytomer_age)
    {
//        AtomicModel<Leaf>::init(t, parameters);

        last_time = t-1;

        //        parameters
        SLW_min = parameters.get("SLW_min") * 10; //kg.m-2
        SLW_max = parameters.get("SLW_max") * 10; //kg.m-2
        SLW_ini = parameters.get("SLW_ini") * 10; //kg.m-2
        INITIAL_SFIND = parameters.get("INITIAL_SFIND");
        MAXIMAL_SFIND = parameters.get("MAXIMAL_SFIND");
        COUT_RESPI_FEUILLE = parameters.get("COUT_RESPI_FEUILLE");
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        INCREASE_OF_LEAF_AREA = parameters.get("INCREASE_OF_LEAF_AREA");
        SEUIL_DUREE = parameters.get("SEUIL_DUREE");
        SEUIL_EXPAN = parameters.get("SEUIL_EXPAN");
        INFLEXION = parameters.get("INFLEXION");
        COURBURE = parameters.get("COURBURE");
        PLASTICITY_LEAF_IC = parameters.get("PLASTICITY_LEAF_IC");


        //        internals
        gain_TEff_jour = 0;
        TT_since_rank1 = 0;
        TT_corrige = 0;
        vitesse_exp = 0;
        demand = 0;
        total_biomass = 0;
        capacite_reserve_max = 0;
        assimilate_supply = 0;
        potLeafArea = 0;
        increase_potleafArea = 0;
        fraction_non_str_biomasse_allouee = 1;

        // init structure
        double TTfeuille = phytomer_age * parameters.get("T_EFF_INI");
        SFMax = min(INCREASE_OF_LEAF_AREA * phytomer_age + INITIAL_SFIND, MAXIMAL_SFIND);
        leafArea = (SFMax / (1 + exp(-(TTfeuille - INFLEXION) / COURBURE)));
        structural_biomass = leafArea * SLW_min * 10 / POURC_FOLIOLE;
        non_structural_biomass = leafArea * (SLW_ini - SLW_min) * 10 / POURC_FOLIOLE;

        if (inflo_status.is(inflo::FEMALE)
                && !inflo_status.is(inflo::ABORTED)
                && inflo_status.is(inflo::HARVEST)){
            structural_biomass = 0;
            non_structural_biomass = 0;
            demand = 0;
            leafArea = 0;
        }

        if (leafArea != 0)
            slw = (structural_biomass + non_structural_biomass) * (POURC_FOLIOLE / leafArea) / 10;
    }


    void growth_demand(double /*t*/) {
        //        compute_coupe_feuille_recolte
        double correctedTEff = TEff * (ftsw > SEUIL_DUREE ? 1 : ftsw / SEUIL_DUREE);
        gain_TEff_jour = TEff * ( ftsw >  SEUIL_DUREE ? 1 : ftsw / SEUIL_DUREE);

        //       Organ.growth_demand(self, correctedTEff) //TODO faire le get dans l'organe après execution
        if (phytomer_rank > 0)
            TT_since_rank1 += TEff;

        //       compute_pot_inc_LeafArea(correctedTEff)
        SFMax = min(MAXIMAL_SFIND, INCREASE_OF_LEAF_AREA * TT_since_rank1 + INITIAL_SFIND);

        //           computevitesse_exp(TT_corrige);
        vitesse_exp = -SFMax *
                (-exp(-(TT_corrige-INFLEXION)/COURBURE)/COURBURE)
                / pow( (1+exp(-(TT_corrige-INFLEXION)/COURBURE)),2);

        //           computevitesse_exp(TT_since_rank1);
        vitesse_exp = -SFMax * (-exp(-(TT_since_rank1-INFLEXION)/COURBURE)/COURBURE)/pow( (1+exp(-(TT_since_rank1-INFLEXION)/COURBURE)),2);
        increase_potleafArea = correctedTEff * vitesse_exp * ( ftsw >  SEUIL_EXPAN ? 1 : ftsw / SEUIL_EXPAN);

        demand = (leafArea == 0 && phytomer_rank != 1) ? 0 : increase_potleafArea*(SLW_min * COUT_RESPI_FEUILLE ) / POURC_FOLIOLE; //TODO check condition sur rang 1
    }


    void growth() {
        //      compute_assimilate_supply()
        assimilate_supply = demand * fr_reste;

        //        compute_leafArea()
        if (phytomer_state == phytomer::ACTIVE) {
            leafArea += increase_potleafArea * fr_reste;
            potLeafArea += increase_potleafArea;
        } else
            leafArea = 0;

        //        compute_biomass()
        structural_biomass += assimilate_supply / COUT_RESPI_FEUILLE;
        if (non_structural_biomass + fraction_non_str_biomasse_allouee * delta_biomasse_reserve_leaf < 0 )
            non_structural_biomass = 0;
        else
            non_structural_biomass += fraction_non_str_biomasse_allouee * delta_biomasse_reserve_leaf;


        total_biomass = structural_biomass + non_structural_biomass;

        //        compute_capacite_reserve_max()
        capacite_reserve_max = (SLW_max - SLW_min) * leafArea / POURC_FOLIOLE;

        //        compute_SLW()
        slw = ( leafArea == 0 )
                ? 0
                : (structural_biomass + non_structural_biomass) * POURC_FOLIOLE / leafArea;

        //        compute_TT_new()
        TT_corrige +=  pow( fr_reste, PLASTICITY_LEAF_IC ) * gain_TEff_jour;
    }


    void compute(double t, bool /* update */)
    {
        if (inflo_status.is(inflo::FEMALE)
                && !inflo_status.is(inflo::ABORTED)
                && inflo_status.is(inflo::HARVEST)){
            structural_biomass = 0;
            non_structural_biomass = 0;
            demand = 0;
        }

        growth();
        growth_demand(t);

    }









    //    compute() {
    //    ...
    //        crown_position = int( (phytomer_rank - 1) / 3 ) + 1;

    //        double a = A_LOI_INTERC;
    //        double b = B_LOI_INTERC;
    //        double nombre_max_crown_position = int( (RANG_D_ABLATION-1) / 3 ) + 1;

    //        if (phytomer_state == phytomer::DEAD || bunch_statut.is(inflo::ABLATED))
    //            niveau_d_eclairement_loi_beta = 0;
    //        else if(bunch_statut.is(inflo::NON_ABLATED) && phytomer_state == phytomer::ACTIVE)
    //            niveau_d_eclairement_loi_beta = pow((crown_position - 0.5 )/ (nombre_max_crown_position), a-1) * pow(1-(crown_position - 0.5 )/ (nombre_max_crown_position), b-1);



    //        if (REMANENCE_STRESS == 1.)
    //            compute_declin_photo();
    //        else
    //            assim_max =  EFFICIENCE_BIOLOGIQUE;
    //    }

    //    void compute_declin_photo(){
    //        if (phytomer_rank >  RANG_DEBUT_SENS_PN_LONG_TERMES && phytomer_rank < RANG_FIN_SENS_PN_LONG_TERMES) {
    //            if (ftsw < seuil_FTSW_pour_red_a_long_termes_de_Pn)
    //                nb_jour_declin_photosynthese += 1 - 1/seuil_FTSW_pour_red_a_long_termes_de_Pn * ftsw;
    //            //                        ### nombre de jours en dessous de 0.4 module par l intensite du stress !
    //            //            on compte le nombre de jours ou il y a eu un stress fort endommageant la photosynthese
    //        }

    //        declin_photosynthese_stress = pow( (1 - fonction_impact_stress_fort_assim_pot(nb_jour_declin_photosynthese)), SENS_REMANESCENCE);
    //        //        ### on calcule le declin de la photosynthese en fonction de ce nombre de jours

    //        if (phytomer_state == phytomer::ACTIVE) //## ne sert rien de calculer si la feuille n est pas presente
    //            declin_photosynthese_age = fonction_impact_age_assim_pot(phytomer_rank, rang_chute_pn_age, rang_pn_50);
    //        //        ### on estime le declin de la pn en fonction de l age


    //        if (phytomer_state == phytomer::DEAD)
    //            assim_max = 0;
    //        if (ablation == leaf::COUPE)
    //            assim_max = 0;
    //        if (ablation == leaf::NON_COUPE && phytomer_state == phytomer::ACTIVE)
    //            assim_max = EFFICIENCE_BIOLOGIQUE * declin_photosynthese_stress * declin_photosynthese_age;
    //        //                # l assim max est l assimilation maximale permise par les jours de stress forts rencontres
    //        //                ### on multiplie ensuite celle-ci par sa quantite de lumiere absorbee pour avoir l assim de chacune des feuilles
    //        //                ### cette assim_max peut aussi etre modifiee par l age de la feuille

    //    }
    //    double fonction_impact_stress_fort_assim_pot(double nbjours) {
    //        double y;
    //        if (nbjours == 0)
    //            y = 0;
    //        else
    //            y = 1/(1 + exp(-(nbjours - a_sigmoide_declin_pn )/(b_sigmoide_declin_pn )));
    //        return y;
    //    }

    //    //          ### attention cette fonction ne passe pas par 0 !!!
    //    double fonction_impact_age_assim_pot(double rang, double rang_debut_chute, double rang_moitie_chute) {
    //        if (rang > rang_chute_pn_age) {
    //            if ( (0.5 - 1) / (rang_moitie_chute - rang_debut_chute) *(rang - rang_debut_chute ) + 1 < 0 )
    //                return 0;
    //            else
    //                return (0.5 - 1) / (rang_moitie_chute - rang_debut_chute) *(rang - rang_debut_chute ) + 1;
    //        } else
    //            return 1;
    //    }



};
} //namespace model
#endif // LEAF_H
