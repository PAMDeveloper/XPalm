#ifndef LEAF_H
#define LEAF_H
#include <defines.hpp>

namespace model {

class Leaf : public AtomicModel < Leaf >
{
public:
    enum internals { ABLATION,
                     LEAFAREA,
                     VITESSE_EXP,
                     SFMAX,
                     DEMAND,
                     POT_INCREASE_LEAFAREA,
                     STRUCTURAL_BIOMASS,
                     NON_STRUCTURAL_BIOMASS,
                     TOTAL_BIOMASS,
                     CAPACITE_RESERVE_MAX,
                     ATTRACTION_BIOMASSE,
                     CAPACITE_RELAGARGE_BIOMASSE,
                     ASSIMILATE_SUPPLY,
                     POTLEAFAREA,
                     BIOMASSE_STRUCTURALE_ALLOUEE,
                     SLW,
                     GAIN_TEFF_JOUR,
                     TT_CORRIGE,
                     INCREASE_POTLEAFAREA,
                     CROWN_POSITION,
                     IRRADIANCE_FACTOR,
                     NIVEAU_D_ECLAIREMENT_LOI_BETA,
                     LIGHT_POURC_INTERCEPTION,
                     ASSIM_MAX,
                     NB_JOUR_DECLIN_PHOTOSYNTHESE,
                     DECLIN_PHOTOSYNTHESE_AGE,
                     DECLIN_PHOTOSYNTHESE_STRESS,
                     BIOMASSE_PROD,
                     RAYONNEMENT_INTERCEPTE_FEUILLE,
                     TT_SINCE_APP };

    enum externals { TEFF,
                     PHYTOMER_RANK,
                     FTSW,
                     BUNCH_SEXE,
                     BUNCH_AVORT,
                     BUNCH_STATUT,
                     PHYTOMER_STATE,
                     FR_RESTE,
                     FRACTION_NON_STR_BIOMASSE_ALLOUEE,
                     DELTA_BIOMASSE_RESERVE_LEAF };

private:

    //      parameters
    double SLW_min;
    double SLW_max;
    double COUT_RESPI_FEUILLE;
    double POURC_FOLIOLE;
    double SEUIL_DUREE;
    double SEUIL_EXPAN;
    double INCREASE_OF_LEAF_AREA;
    double INITIAL_SFIND;
    double MAXIMAL_SFIND;
    double INFLEXION;
    double COURBURE;
    double PLASTICITY_LEAF_IC;
    double RANG_DEBUT_SENS_PN_LONG_TERMES;
    double RANG_FIN_SENS_PN_LONG_TERMES;
    double seuil_FTSW_pour_red_a_long_termes_de_Pn;
    double SENS_REMANESCENCE;
    double REMANENCE_STRESS;
    double a_sigmoide_declin_pn;
    double b_sigmoide_declin_pn;
    double rang_chute_pn_age;
    double rang_pn_50;
    double EFFICIENCE_BIOLOGIQUE;
    double A_LOI_INTERC;
    double B_LOI_INTERC;
    double RANG_D_ABLATION;

    //     internals
    leaf::leaf_state ablation;

    double leafArea; //m²
    double vitesse_exp;
    double SFMax;
    double demand;
    double pot_increase_leafArea;
    double structural_biomass;
    double non_structural_biomass;
    double total_biomass;
    double capacite_reserve_max;
    double attraction_biomasse;
    double capacite_relagarge_biomasse;
    double assimilate_supply;
    double potLeafArea;
    double biomasse_structurale_allouee;
    double slw;
    double gain_TEff_jour;
    double TT_corrige;
    double increase_potleafArea;
    double crown_position;
    double irradiance_factor;
    double niveau_d_eclairement_loi_beta;
    double light_pourc_interception;
    double assim_max;
    double nb_jour_declin_photosynthese;
    double declin_photosynthese_age;
    double declin_photosynthese_stress;
    double biomasse_prod;
    double rayonnement_intercepte_feuille;
    double thermalTimeSinceAppearance;

    //     externals
    inflo::inflo_sex bunch_sexe;
    inflo::inflo_sex bunch_avort;
    inflo::inflo_states bunch_statut;
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
        Internal(ABLATION, &Leaf::ablation);
        Internal(LEAFAREA, &Leaf::leafArea);
        Internal(VITESSE_EXP, &Leaf::vitesse_exp);
        Internal(SFMAX, &Leaf::SFMax);
        Internal(DEMAND, &Leaf::demand);
        Internal(POT_INCREASE_LEAFAREA, &Leaf::pot_increase_leafArea);
        Internal(STRUCTURAL_BIOMASS, &Leaf::structural_biomass);
        Internal(NON_STRUCTURAL_BIOMASS, &Leaf::non_structural_biomass);
        Internal(TOTAL_BIOMASS, &Leaf::total_biomass);
        Internal(CAPACITE_RESERVE_MAX, &Leaf::capacite_reserve_max);
        Internal(ATTRACTION_BIOMASSE, &Leaf::attraction_biomasse);
        Internal(CAPACITE_RELAGARGE_BIOMASSE, &Leaf::capacite_relagarge_biomasse);
        Internal(ASSIMILATE_SUPPLY, &Leaf::assimilate_supply);
        Internal(POTLEAFAREA, &Leaf::potLeafArea);
        Internal(BIOMASSE_STRUCTURALE_ALLOUEE, &Leaf::biomasse_structurale_allouee);
        Internal(SLW, &Leaf::slw);
        Internal(GAIN_TEFF_JOUR, &Leaf::gain_TEff_jour);
        Internal(TT_CORRIGE, &Leaf::TT_corrige);
        Internal(INCREASE_POTLEAFAREA, &Leaf::increase_potleafArea);
        Internal(CROWN_POSITION, &Leaf::crown_position);
        Internal(IRRADIANCE_FACTOR, &Leaf::irradiance_factor);
        Internal(NIVEAU_D_ECLAIREMENT_LOI_BETA, &Leaf::niveau_d_eclairement_loi_beta);
        Internal(LIGHT_POURC_INTERCEPTION, &Leaf::light_pourc_interception);
        Internal(ASSIM_MAX, &Leaf::assim_max);
        Internal(NB_JOUR_DECLIN_PHOTOSYNTHESE, &Leaf::nb_jour_declin_photosynthese);
        Internal(DECLIN_PHOTOSYNTHESE_AGE, &Leaf::declin_photosynthese_age);
        Internal(DECLIN_PHOTOSYNTHESE_STRESS, &Leaf::declin_photosynthese_stress);
        Internal(BIOMASSE_PROD, &Leaf::biomasse_prod);
        Internal(RAYONNEMENT_INTERCEPTE_FEUILLE, &Leaf::rayonnement_intercepte_feuille);
        Internal(TT_SINCE_APP, &Leaf::thermalTimeSinceAppearance);

        //          externals
        External(TEFF, &Leaf::TEff);
        External(PHYTOMER_RANK, &Leaf::phytomer_rank);
        External(FTSW, &Leaf::ftsw);
        External(BUNCH_SEXE, &Leaf::bunch_sexe);
        External(BUNCH_AVORT, &Leaf::bunch_avort);
        External(BUNCH_STATUT, &Leaf::bunch_statut);
        External(PHYTOMER_STATE, &Leaf::phytomer_state);
        External(FR_RESTE, &Leaf::fr_reste);
        External(FRACTION_NON_STR_BIOMASSE_ALLOUEE, &Leaf::fraction_non_str_biomasse_allouee);
        External(DELTA_BIOMASSE_RESERVE_LEAF, &Leaf::delta_biomasse_reserve_leaf);

    }

    virtual ~Leaf()
    {
    }


    void init_structure(double t) {
        //leaf
        //            phytomers[phytomer.name].leaf.thermalTimeSinceAppearance = TTfeuille
        //            phytomers[phytomer.name].leaf.TT_corrige = TTfeuille
        //            phytomers[phytomer.name].internode.TT_corrige = TTfeuille
        //            SF = min(GlobalVariables.INCREASE_OF_LEAF_AREA * ( - nb_jour_depuis_l_appari ) + GlobalVariables.INITIAL_SFIND, GlobalVariables.MAXIMAL_SFIND)


        //            phytomers[phytomer.name].leaf.leafArea = phytomer.leaf.computeSF_ind_finale(SF , TTfeuille)

        //            phytomers[phytomer.name].leaf.structural_biomass = phytomers[phytomer.name].leaf.leafArea * GlobalVariables.SLW_min * 10 / GlobalVariables.POURC_FOLIOLE
        //            phytomers[phytomer.name].leaf.non_structural_biomass = phytomers[phytomer.name].leaf.leafArea * (GlobalVariables.SLW_ini - GlobalVariables.SLW_min) * 10 / GlobalVariables.POURC_FOLIOLE

        //            if phytomers[phytomer.name].bunch.statut == "RECOLTE" :
        //                if phytomers[phytomer.name].bunch.avort == "NON_AVORTE" :
        //                    if phytomers[phytomer.name].bunch.sexe == "FEMELLE" :
        //                        phytomers[phytomer.name].leaf.leafArea = 0
        //                        phytomers[phytomer.name].leaf.structural_biomass = 0
        //                        phytomers[phytomer.name].leaf.non_structural_biomass = 0
        //            phytomers[phytomer.name].leaf.potLeafArea = phytomers[phytomer.name].leaf.leafArea

        //            if phytomers[phytomer.name].leaf.leafArea != 0 :
        //                phytomers[phytomer.name].leaf.SLW = (phytomers[phytomer.name].leaf.structural_biomass + phytomers[phytomer.name].leaf.non_structural_biomass) * GlobalVariables.POURC_FOLIOLE / phytomers[phytomer.name].leaf.leafArea / 10

        //            #print "STRUCTURE_INI", "sexe",phytomers[phytomer.name].bunch.sexe,  "avort", phytomers[phytomer.name].bunch.avort,  "statut", phytomers[phytomer.name].bunch.statut, "leafarea", phytomers[phytomer.name].leaf.leafArea, "structural_biomass", phytomers[phytomer.name].leaf.structural_biomass, "non_structural_biomass", phytomers[phytomer.name].leaf.non_structural_biomass, "SLW",phytomers[phytomer.name].leaf.SLW

    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;

        //        parameters
        SLW_min = parameters.get("SLW_min") * 10; //kg.m-2
        SLW_max = parameters.get("SLW_max") * 10; //kg.m-2
        COUT_RESPI_FEUILLE = parameters.get("COUT_RESPI_FEUILLE");
        POURC_FOLIOLE = parameters.get("POURC_FOLIOLE");
        SEUIL_DUREE = parameters.get("SEUIL_DUREE");
        SEUIL_EXPAN = parameters.get("SEUIL_EXPAN");
        INCREASE_OF_LEAF_AREA = parameters.get("INCREASE_OF_LEAF_AREA");
        INITIAL_SFIND = parameters.get("INITIAL_SFIND");
        MAXIMAL_SFIND = parameters.get("MAXIMAL_SFIND");
        INFLEXION = parameters.get("INFLEXION");
        COURBURE = parameters.get("COURBURE");
        PLASTICITY_LEAF_IC = parameters.get("PLASTICITY_LEAF_IC");
        RANG_DEBUT_SENS_PN_LONG_TERMES = parameters.get("RANG_DEBUT_SENS_PN_LONG_TERMES");
        RANG_FIN_SENS_PN_LONG_TERMES = parameters.get("RANG_FIN_SENS_PN_LONG_TERMES");
        seuil_FTSW_pour_red_a_long_termes_de_Pn = parameters.get("seuil_FTSW_pour_red_a_long_termes_de_Pn");
        SENS_REMANESCENCE = parameters.get("SENS_REMANESCENCE");
        a_sigmoide_declin_pn = parameters.get("a_sigmoide_declin_pn");
        b_sigmoide_declin_pn = parameters.get("b_sigmoide_declin_pn");
        rang_chute_pn_age = parameters.get("rang_chute_pn_age");
        rang_pn_50 = parameters.get("rang_pn_50");
        EFFICIENCE_BIOLOGIQUE = parameters.get("EFFICIENCE_BIOLOGIQUE");
        A_LOI_INTERC = parameters.get("a_loi_beta_interception");
        B_LOI_INTERC = parameters.get("b_loi_beta_interception");
        RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        REMANENCE_STRESS = parameters.get("REMANENCE_STRESS");

        //        internals
        ablation = leaf::NON_COUPE;
        leafArea = 0;
        vitesse_exp = 0;
        SFMax = 0;
        demand = 0;
        pot_increase_leafArea = 0;
        structural_biomass = 0;
        non_structural_biomass = 0;
        total_biomass = 0;
        capacite_reserve_max = 0;
        attraction_biomasse = 0;
        capacite_relagarge_biomasse = 0;
        assimilate_supply = 0;
        potLeafArea = 0;
        biomasse_structurale_allouee = 0;
        slw = 0;
        gain_TEff_jour = 0;
        TT_corrige = 0;
        increase_potleafArea = 0;
        crown_position = 0;
        irradiance_factor = 0;
        niveau_d_eclairement_loi_beta = 0;
        light_pourc_interception = 0;
        assim_max = 0;
        nb_jour_declin_photosynthese = 0;
        declin_photosynthese_age = 0;
        declin_photosynthese_stress = 0;
        biomasse_prod = 0;
        rayonnement_intercepte_feuille = 0;

    }


    void growth_demand(double t) {
        //        compute_coupe_feuille_recolte
        if (bunch_sexe == inflo::FEMALE && bunch_avort == inflo::NON_ABORTED && bunch_statut.is(inflo::RECOLTE))
            ablation = leaf::COUPE;

        double correctedTEff = TEff * (ftsw > SEUIL_DUREE ? 1 : ftsw / SEUIL_DUREE);
        gain_TEff_jour = TEff * ( ftsw >  SEUIL_DUREE ? 1 : ftsw / SEUIL_DUREE);

        //       Organ.growth_demand(self, correctedTEff) //TODO faire le get dans l'organe après execution
        if (phytomer_rank > 0)
            thermalTimeSinceAppearance += TEff;

        //       compute_pot_inc_LeafArea(correctedTEff)
        if (phytomer_state == phytomer::DEAD)
            pot_increase_leafArea = 0;
        else {
            SFMax = min(MAXIMAL_SFIND, INCREASE_OF_LEAF_AREA * t + INITIAL_SFIND);
            //           computevitesse_exp(TT_corrige);
            vitesse_exp = -SFMax * (-exp(-(TT_corrige-INFLEXION)/COURBURE)/COURBURE)/pow( (1+exp(-(TT_corrige-INFLEXION)/COURBURE)),2);

            //compute_expansion_decrease
            pot_increase_leafArea = correctedTEff * vitesse_exp * ( ftsw >  SEUIL_EXPAN ? 1 : ftsw / SEUIL_EXPAN );

            //           computevitesse_exp(thermalTimeSinceAppearance);
            vitesse_exp = -SFMax * (-exp(-(thermalTimeSinceAppearance-INFLEXION)/COURBURE)/COURBURE)/pow( (1+exp(-(thermalTimeSinceAppearance-INFLEXION)/COURBURE)),2);

            increase_potleafArea = correctedTEff * vitesse_exp * ( ftsw >  SEUIL_EXPAN ? 1 : ftsw / SEUIL_EXPAN);
        }

        if (ablation == leaf::COUPE) {
            pot_increase_leafArea = 0;
            demand = 0;
        }

        if (leafArea == 0) {
            if (phytomer_rank != 1)
                demand = 0;
            else
                demand = pot_increase_leafArea*(SLW_min * COUT_RESPI_FEUILLE ) / POURC_FOLIOLE;
        } else
            demand = pot_increase_leafArea*(SLW_min * COUT_RESPI_FEUILLE ) / POURC_FOLIOLE;
    }


    void growth() {
        //      compute_assimilate_supply()
        assimilate_supply = demand * fr_reste;

        //        compute_leafArea()
        if (phytomer_state == phytomer::ACTIVE) {
            leafArea += pot_increase_leafArea   * fr_reste;
            potLeafArea += increase_potleafArea;
            if (ablation == leaf::COUPE){
                leafArea = 0;
                potLeafArea = 0;
            }
        } else
            leafArea = 0;

        //        compute_biomass()
        if (phytomer_state == phytomer::ACTIVE) {
            structural_biomass = 0;
            non_structural_biomass = 0;
        } else {
            structural_biomass += (1 / COUT_RESPI_FEUILLE) * assimilate_supply;
            if (non_structural_biomass + fraction_non_str_biomasse_allouee * delta_biomasse_reserve_leaf < 0 )
                non_structural_biomass = 0;
            else
                non_structural_biomass += fraction_non_str_biomasse_allouee * delta_biomasse_reserve_leaf;

            if (ablation == leaf::COUPE){
                structural_biomass = 0;
                non_structural_biomass = 0;
            }
        }
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
        growth();
        growth_demand();

//        crown_position = int( (phytomer_rank - 1) / 3 ) + 1;

//        double a = A_LOI_INTERC;
//        double b = B_LOI_INTERC;
//        double nombre_max_crown_position = int( (RANG_D_ABLATION-1) / 3 ) + 1;

//        if (phytomer_state == phytomer::DEAD || bunch_statut.is(inflo::ABLATED))
//            niveau_d_eclairement_loi_beta = 0;
//        else if(bunch_statut.is(inflo::NON_ABLATED) && phytomer_state == phytomer::ACTIVE)
//            niveau_d_eclairement_loi_beta = pow((crown_position - 0.5 )/ (nombre_max_crown_position), a-1) * pow(1-(crown_position - 0.5 )/ (nombre_max_crown_position), b-1);



        if (REMANENCE_STRESS == 1.)
            compute_declin_photo();
        else
            assim_max =  EFFICIENCE_BIOLOGIQUE;

    }


    void compute_declin_photo(){
        if (phytomer_rank >  RANG_DEBUT_SENS_PN_LONG_TERMES && phytomer_rank < RANG_FIN_SENS_PN_LONG_TERMES) {
            if (ftsw < seuil_FTSW_pour_red_a_long_termes_de_Pn)
                nb_jour_declin_photosynthese += 1 - 1/seuil_FTSW_pour_red_a_long_termes_de_Pn * ftsw;
            //                        ### nombre de jours en dessous de 0.4 module par l intensite du stress !
            //            on compte le nombre de jours ou il y a eu un stress fort endommageant la photosynthese
        }

        declin_photosynthese_stress = pow( (1 - fonction_impact_stress_fort_assim_pot(nb_jour_declin_photosynthese)), SENS_REMANESCENCE);
        //        ### on calcule le declin de la photosynthese en fonction de ce nombre de jours

        if (phytomer_state == phytomer::ACTIVE) //## ne sert rien de calculer si la feuille n est pas presente
            declin_photosynthese_age = fonction_impact_age_assim_pot(phytomer_rank, rang_chute_pn_age, rang_pn_50);
        //        ### on estime le declin de la pn en fonction de l age


        if (phytomer_state == phytomer::DEAD)
            assim_max = 0;
        if (ablation == leaf::COUPE)
            assim_max = 0;
        if (ablation == leaf::NON_COUPE && phytomer_state == phytomer::ACTIVE)
            assim_max = EFFICIENCE_BIOLOGIQUE * declin_photosynthese_stress * declin_photosynthese_age;
        //                # l assim max est l assimilation maximale permise par les jours de stress forts rencontres
        //                ### on multiplie ensuite celle-ci par sa quantite de lumiere absorbee pour avoir l assim de chacune des feuilles
        //                ### cette assim_max peut aussi etre modifiee par l age de la feuille

    }
    double fonction_impact_stress_fort_assim_pot(double nbjours) {
        double y;
        if (nbjours == 0)
            y = 0;
        else
            y = 1/(1 + exp(-(nbjours - a_sigmoide_declin_pn )/(b_sigmoide_declin_pn )));
        return y;
    }

    //          ### attention cette fonction ne passe pas par 0 !!!
    double fonction_impact_age_assim_pot(double rang, double rang_debut_chute, double rang_moitie_chute) {
        if (rang > rang_chute_pn_age) {
            if ( (0.5 - 1) / (rang_moitie_chute - rang_debut_chute) *(rang - rang_debut_chute ) + 1 < 0 )
                return 0;
            else
                return (0.5 - 1) / (rang_moitie_chute - rang_debut_chute) *(rang - rang_debut_chute ) + 1;
        } else
            return 1;
    }



};
} //namespace model
#endif // LEAF_H
