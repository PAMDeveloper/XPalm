#ifndef BUNCH_H
#define BUNCH_H

/** PHYTOMER_MODEL Ablation **/
//        ### ablation des regimes le jour de la mise en place des traitements
//        if simulation.step == DEBUT_ABLATION_REGIME :
//            for key in sorted(phytomers):
//                if (phytomers[key].rank >= RANG_D_ABLATION_REGIME) :
//                    if (phytomers[key].bunch.statut != "RECOLTE") :
//                        if phytomers[key].bunch.sexe == "FEMELLE" :
//                            if phytomers[key].bunch.avort == "NON_AVORTE" :
//                                phytomers[key].bunch.ablation = phytomers[key].bunch.ablation_decision(POURC_ABLATION_REGIME)
#include <defines.hpp>

#include <plant/phytomer/bunch.h>
#include <plant/phytomer/peduncle.h>
#include <plant/phytomer/male_inflo.h>

namespace model {

class Inflo : public CoupledModel < Inflo >
{
public:
    enum submodels { PEDUNCLE, FRUIT, MALEINFLO };

    enum internals { BIOMASS,
                     DEMAND,
                     ESTIMATED_FLOWERING_DATE,
                     ESTIMATED_HARVEST_DATE,
                     BUNCH_DEV_FACTOR,
                     FEMELLE_BIOMASS,
                     FLOWERING_DATE,
                     FRUIT_NUMBER,
                     GAIN_TEFF_JOUR,
                     HARVEST_DATE,
                     ICABORT,
                     ICABORT_TOT,
                     ICSEX,
                     ICSEX_TOT,
                     IC_SETTING,
                     IC_SETTING_TOT,
                     IC_SPIKELET,
                     IC_SPIKELET_TOT,
                     INDEX,
                     INI_FLOWERING_DATE,
                     INI_HARVEST_DATE,
                     MALE_BIOMASS,
                     MASSE_MAX_REGIME,
                     MASSE_POT_REGIME,
                     NB_JOURSICABORT,
                     NB_JOURSICSEX,
                     NB_JOURSIC_SETTING,
                     NB_JOURSIC_SPIKELET,
                     POTENTIAL_BIOMASS,
                     POT_FRUITS_NUMBER,
                     RED_VITESSE_FTSW,
                     RESPIRABLE_BIOMASS,
                     STATUS, POT_STATUS, SEX, AVORT,
                     TEST_BIOMASS,
                     TT_SINCE_RANK1,
                     TT_CORRIGE
                   };

    enum externals { DATE_PLUS_JEUNE_FEUILLE,
                     TREE_PRODUCTIONSPEED,
                     PHYTOMER_RANK,
                     TEFF,
                     FR_FRUITS,
                     TREE_IC,
                     FTSW,
                     FR_RESTE
                   };

private:

    //     externals
    //    double age;
    //    double date_plus_jeune_feuille;
    double rank;

    double TT_ini_flowering;
    double TT_ini_harvest;
    double TT_ini_oleo;

    double inflo_dev_factor;
    double production_speed;

    double fr_fruits;
    double fr_reste;
    double tree_IC;

    double TEff;
    double ftsw;



    //      parameters
    double DEBUT_RANG_SENSITIVITY_NOUAISON;
    double FACTEUR_AGE_INI;
    double FIN_RANG_SENSITIVITY_NOUAISON;
    double IC_spikelet_RANG_DEBUT;
    double IC_spikelet_RANG_FIN;
    double ICabort_RANG_DEBUT;
    double ICabort_RANG_FIN;
    double ICsex_RANG_DEBUT;
    double ICsex_RANG_FIN;
    //    double INCREASE_OF_BUNCH_MASS;
    double INCREASE_TAILLE_REGIMES;
    double PRODUCTION_SPEED_INITIAL;
    //    double MASSE_REGIMES_ADULTE;
    //    double MASSE_REGIME_INI;
    double MEAN_FRUIT_NUMBER_ADULTE;
    double MINIMAL_PRODUCTION_SPEED;
    double PLASTICITY_BUNCH_IC_APRES_FLORAISON;
    double PLASTICITY_BUNCH_IC_AVANT_FLORAISON;
    double RATIO_DUREE_JEUNES_FLOWERING;
    double RATIO_DUREE_JEUNES_HARVEST;
    double SENSIVITY_IC_SETTING;
    double SENSIVITY_IC_SPIKELET;
    double SENS_FTSW;
    double SEUIL_MEDIAN_FTSW;
    //    double TT_FLOWERING_ADULTE;
    //    double TT_HARVEST_ADULTE;

    double AGE_ADULT;
    double AGE_PLANTING;
    double AGE_START_PROD;

    double TT_FLOWERING_DAILY_INCREMENT;
    double TT_HARVEST_DAILY_INCREMENT;
    double TT_FLOWERING_ADULT;
    double TT_HARVEST_ADULT;
    double TT_FLOWERING_INITIAL;
    double TT_HARVEST_INITIAL;

    //     submodels
    std::unique_ptr < Peduncle > peduncle;
    std::unique_ptr < Bunch > bunch;
    std::unique_ptr < MaleInflo > male;

    //     internals
    inflo::inflo_states status;
    inflo::inflo_states status_pot;
    double TT_oleo_duration;


    //    inflo::inflo_sex sex;

    double IC_setting;
    double IC_setting_tot;
    double IC_spikelet;
    double IC_spikelet_tot;
    double ICabort;
    double ICabort_tot;
    double ICsex;
    double ICsex_tot;
    double TT_corrige;
    double biomass;
    double demand;

    double femelle_biomass;
    double flowering_date;
    double fruit_number;
    double gain_TEff_jour;
    double harvest_date;
    double index;
    double ini_flowering_date;
    double ini_harvest_date;
    double male_biomass;
    double masse_max_regime;
    double masse_pot_regime;
    double nb_joursIC_setting;
    double nb_joursIC_spikelet;
    double nb_joursICabort;
    double nb_joursICsex;
    double pot_fruits_number;
    double potential_biomass;
    double red_vitesse_FTSW;
    double respirable_biomass;
    double test_biomass;
    double TT_since_rank1;


public:

    Inflo(double prod_speed, double flo_tt, double harv_tt, double inflo_factor):
        TT_ini_flowering(flo_tt),
        TT_ini_harvest(harv_tt),
        inflo_dev_factor(inflo_factor),
        production_speed(prod_speed),
        peduncle(new Peduncle()),
        bunch(new Bunch()),
        male(new MaleInflo())
    {
        //         submodels
        setsubmodel(PEDUNCLE, peduncle.get());
        setsubmodel(FRUIT, bunch.get());
        setsubmodel(MALEINFLO, male.get());

        //         internals
        Internal(STATUS, &Inflo::status);
        Internal(POT_STATUS, &Inflo::status_pot);
        Internal(SEX, &Inflo::sex);
        Internal(AVORT, &Inflo::avort);
        Internal(ESTIMATED_FLOWERING_DATE, &Inflo::TT_ini_flowering);
        Internal(ESTIMATED_HARVEST_DATE, &Inflo::TT_ini_harvest);
        Internal(TT_CORRIGE, &Inflo::TT_corrige);
        Internal(DEMAND, &Inflo::demand);
        Internal(MASSE_POT_REGIME, &Inflo::masse_pot_regime);
        Internal(ICSEX, &Inflo::ICsex);
        Internal(NB_JOURSICSEX, &Inflo::nb_joursICsex);
        Internal(ICSEX_TOT, &Inflo::ICsex_tot);
        Internal(ICABORT, &Inflo::ICabort);
        Internal(NB_JOURSICABORT, &Inflo::nb_joursICabort);
        Internal(ICABORT_TOT, &Inflo::ICabort_tot);
        Internal(NB_JOURSIC_SPIKELET, &Inflo::nb_joursIC_spikelet);
        Internal(IC_SPIKELET_TOT, &Inflo::IC_spikelet_tot);
        Internal(IC_SPIKELET, &Inflo::IC_spikelet);
        Internal(BIOMASS, &Inflo::biomass);
        Internal(FEMELLE_BIOMASS, &Inflo::femelle_biomass);
        Internal(RESPIRABLE_BIOMASS, &Inflo::respirable_biomass);
        Internal(INI_FLOWERING_DATE, &Inflo::ini_flowering_date);
        Internal(MASSE_MAX_REGIME, &Inflo::masse_max_regime);
        Internal(NB_JOURSIC_SETTING, &Inflo::nb_joursIC_setting);
        Internal(IC_SETTING_TOT, &Inflo::IC_setting_tot);
        Internal(IC_SETTING, &Inflo::IC_setting);
        Internal(INDEX, &Inflo::index);
        Internal(RED_VITESSE_FTSW, &Inflo::red_vitesse_FTSW);
        Internal(POTENTIAL_BIOMASS, &Inflo::potential_biomass);
        Internal(TEST_BIOMASS, &Inflo::test_biomass);
        Internal(GAIN_TEFF_JOUR, &Inflo::gain_TEff_jour);
        Internal(FRUIT_NUMBER, &Inflo::fruit_number);
        Internal(FLOWERING_DATE, &Inflo::flowering_date);
        Internal(HARVEST_DATE, &Inflo::harvest_date);
        Internal(BUNCH_DEV_FACTOR, &Inflo::inflo_dev_factor);
        Internal(POT_FRUITS_NUMBER, &Inflo::pot_fruits_number);
        Internal(MALE_BIOMASS, &Inflo::male_biomass);
        Internal(TT_SINCE_RANK1, &Inflo::TT_since_rank1);
        Internal(INI_HARVEST_DATE, &Inflo::ini_harvest_date);

        //          externals
        External(PHYTOMER_RANK, &Inflo::rank);
        External(TEFF, &Inflo::TEff);
        External(FR_FRUITS, &Inflo::fr_fruits);
        External(FR_RESTE, &Inflo::fr_reste);
        External(TREE_IC, &Inflo::tree_IC);
        External(FTSW, &Inflo::ftsw);

        //        External(TREE_PRODUCTIONSPEED, &Inflo::production_speed);
    }

    virtual ~Inflo()
    {
        peduncle.reset(nullptr);
        bunch.reset(nullptr);
        male.reset(nullptr);
    }

    Peduncle * peduncle_model() {
        return peduncle.get();
    }

    Bunch * bunch_model() {
        return bunch.get();
    }

    MaleInflo * male_model() {
        return male.get();
    }


    //TODO réactiver

    //    void compute_facteur_age(t) {
    //        if (INCREASE_TAILLE_REGIMES * (t + phytomer.step_apparition  - tree.date_plus_jeune_feuille) + FACTEUR_AGE_INI) > 1)
    //            inflo_dev_factor = 1;
    //        else
    //            inflo_dev_factor = (INCREASE_TAILLE_REGIMES * t + phytomer.step_apparition - tree.date_plus_jeune_feuille) + FACTEUR_AGE_INI);

    //        pot_fruits_number = inflo_dev_factor * MEAN_FRUIT_NUMBER_ADULTE;
    //    }



    void init(double t, double age, const xpalm::ModelParameters& parameters)
    {
        //        parameters
        DEBUT_RANG_SENSITIVITY_NOUAISON = parameters.get("DEBUT_RANG_SENSITIVITY_NOUAISON");
        FIN_RANG_SENSITIVITY_NOUAISON = parameters.get("FIN_RANG_SENSITIVITY_NOUAISON");
        IC_spikelet_RANG_DEBUT = parameters.get("IC_spikelet_RANG_DEBUT");
        IC_spikelet_RANG_FIN = parameters.get("IC_spikelet_RANG_FIN");
        ICabort_RANG_DEBUT = parameters.get("ICabort_RANG_DEBUT");
        ICabort_RANG_FIN = parameters.get("ICabort_RANG_FIN");
        ICsex_RANG_DEBUT = parameters.get("ICsex_RANG_DEBUT");
        ICsex_RANG_FIN = parameters.get("ICsex_RANG_FIN");
        //        INCREASE_OF_BUNCH_MASS = parameters.get("INCREASE_OF_BUNCH_MASS");
        FACTEUR_AGE_INI = parameters.get("AGE") / parameters.get("AGE_ADULTE");
        INCREASE_TAILLE_REGIMES = 1 / (parameters.get("AGE_ADULTE") * 365);

        TT_FLOWERING_DAILY_INCREMENT = ( TT_FLOWERING_ADULT - TT_FLOWERING_INITIAL ) / ( AGE_ADULT - AGE_PLANTING );
        TT_HARVEST_DAILY_INCREMENT = ( TT_HARVEST_ADULT - TT_HARVEST_INITIAL ) / ( AGE_ADULT - AGE_PLANTING );


        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");
        //        MASSE_REGIMES_ADULTE = parameters.get("MASSE_REGIMES_ADULTE");
        //        MASSE_REGIME_INI = parameters.get("MASSE_REGIME_INI");
        MEAN_FRUIT_NUMBER_ADULTE = parameters.get("MEAN_FRUIT_NUMBER_ADULTE");
        MINIMAL_PRODUCTION_SPEED = parameters.get("MINIMAL_PRODUCTION_SPEED");
        PLASTICITY_BUNCH_IC_APRES_FLORAISON = parameters.get("PLASTICITY_BUNCH_IC_APRES_FLORAISON");
        PLASTICITY_BUNCH_IC_AVANT_FLORAISON = parameters.get("PLASTICITY_BUNCH_IC_AVANT_FLORAISON");
        RATIO_DUREE_JEUNES_FLOWERING = parameters.get("RATIO_DUREE_JEUNES_FLOWERING");
        RATIO_DUREE_JEUNES_HARVEST = parameters.get("RATIO_DUREE_JEUNES_HARVEST");
        SENSIVITY_IC_SETTING = parameters.get("SENSIVITY_IC_SETTING");
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        SENS_FTSW = parameters.get("SENS_FTSW");
        SEUIL_MEDIAN_FTSW = parameters.get("SEUIL_MEDIAN_FTSW");

        TT_corrige = 0; //a set pour le state



        demand = 0 ;
        masse_pot_regime = 0 ;
        ICsex = 0;
        nb_joursICsex = 0;
        ICsex_tot = 0 ;
        ICabort = 0;
        nb_joursICabort = 0;
        ICabort_tot = 0;
        nb_joursIC_spikelet = 0;
        IC_spikelet_tot = 0;
        IC_spikelet = 1  ;
        biomass =  0 ;
        femelle_biomass = 0;
        respirable_biomass = 0;
        ini_flowering_date = 0 ;
        masse_max_regime = 0;
        nb_joursIC_setting = 0;
        IC_setting_tot = 0;
        IC_setting = 1;
        index = 0;
        red_vitesse_FTSW = 0 ;
        potential_biomass = 0;
        test_biomass = 0 ;
        gain_TEff_jour = 0;
        fruit_number = 0;

        double PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        double DUREE_OLEO = parameters.get("DUREE_OLEO");
        double RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        TT_oleo_duration = DUREE_OLEO * pow(PRODUCTION_SPEED_ADULT / production_speed, RATIO_DUREE_JEUNES_OLEO); //TODO filer depuis l'arbre à la création du phytomer
        TT_ini_oleo = TT_ini_harvest - TT_oleo_duration;

        male_biomass = 0;
        TT_since_rank1 = 0;
        ini_harvest_date = 0;

        pot_fruits_number = inflo_dev_factor * MEAN_FRUIT_NUMBER_ADULTE;

        double rd = ((double) rand() / (RAND_MAX)) + 1;
        if (rank > parameters.get("ICsex_RANG_FIN")) {
            if(rd < parameters.get("INI_SEX_RATIO")) {
                status.add(inflo::FEMALE);
                peduncle->init(t, parameters);
                bunch->init(t, parameters);
            } else {
                status.add(inflo::MALE);
                 male->init(t, parameters);
            }
        }

        rd = ((double) rand() / (RAND_MAX)) + 1;
        if (rank > parameters.get("ICabort_RANG_FIN")) {
            if( rd < parameters.get("INI_TAUX_D_AVORTEMENT"))
                status.add(inflo::ABORTED);
        }

        step_state();
        //TODO compute first growth ?
    }



    void step_state() {

        if( TT_corrige > TT_ini_harvest) {
            if(!status.is(inflo::HARVEST))
                status.replace(inflo::FLOWERING, inflo::HARVEST);
        } else if( TT_corrige > TT_ini_flowering) {
            if(!status.is(inflo::FLOWERING))
                status.replace(inflo::INITIATED, inflo::FLOWERING);
        } else {
            if(!status.is(inflo::INITIATED))
                status.add(inflo::INITIATED);
        }

        if(TT_corrige > TT_ini_harvest - TT_oleo_duration && TT_corrige < TT_ini_harvest)
            status.add(inflo::OLEOSYNTHESIS);
        else
            status.del(inflo::OLEOSYNTHESIS);

        if(TT_corrige > TT_ini_senescence && status.is(inflo::MALE))
            status.add(inflo::SENESCENCE);
        else
            status.del(inflo::SENESCENCE);
    }


    void compute(double t, bool /* update */)
    {
        if(status.is(inflo::ABORTED) || status.is(inflo::DEAD) || status.is(inflo::HARVEST))
            return;

        step_state();

        if (status.is(inflo::FEMALE)) {
            if(!status.is(inflo::HARVEST))
                (*peduncle)(t);

            if (status.is(inflo::FLOWERING))
                (*bunch)(t);

        } else if (status.is(inflo::MALE) && status.is(inflo::INITIATED))
            (*male)(t);

        //        compute_growth(t);
        if (!status.is(inflo::HARVEST)) {
            double peduncle_biomass = peduncle->get< double >(t, Peduncle::BIOMASS);
            double bunch_oil_biomass = status.is(inflo::FEMALE) && status.is(inflo::FLOWERING)? bunch->get< double >(t, Bunch::OIL_BIOMASS) : 0;
            double bunch_nonoil_biomass = status.is(inflo::FEMALE) && status.is(inflo::FLOWERING) ?  bunch->get< double >(t, Bunch::NONOIL_BIOMASS) : 0;
            male_biomass = status.is(inflo::MALE) ?
                        status.is(inflo::FLOWERING) && !status.is(inflo::SENESCENCE)
                        ? male_biomass
                        : male->get< double >(t, MaleInflo::BIOMASS)
                        : 0;


            femelle_biomass = bunch_oil_biomass  + bunch_nonoil_biomass + peduncle_biomass;
            biomass = femelle_biomass + male_biomass;
            respirable_biomass = bunch_nonoil_biomass + peduncle_biomass + male_biomass;
        } else {
            male_biomass = 0;
            biomass = 0;
            femelle_biomass = 0;
            respirable_biomass = 0;
        }

//        compute_demand(t);

        //Demand //TODO get from tree
        if (status.is(inflo::INITIATED))
            red_vitesse_FTSW = pow((ftsw + 1 - SEUIL_MEDIAN_FTSW ), SENS_FTSW );
        else
            red_vitesse_FTSW = 1;

        double correctedTEff = red_vitesse_FTSW * TEff;
        gain_TEff_jour = correctedTEff;

        //        Organ.growth_demand(self, correctedTEff);
        if(rank > 0)
            TT_since_rank1 += gain_TEff_jour;

        demand = 0;
        demand += status.is(inflo::FLOWERING) ? bunch->get< double >(t, Bunch::DEMAND) : 0;
        demand += !status.is(inflo::HARVEST) ? peduncle->get< double >(t, Bunch::DEMAND) : 0;
        demand += status.is(inflo::INITIATED) ? male->get< double >(t, MaleInflo::DEMAND) : 0;

        //        compute_TT_corrige() //TODO get from phytomer
        if (status.is(inflo::INITIATED))
            TT_corrige += pow(fr_fruits, PLASTICITY_BUNCH_IC_AVANT_FLORAISON) * gain_TEff_jour;
        else if (status.is(inflo::FLOWERING))
            TT_corrige += pow(fr_fruits, PLASTICITY_BUNCH_IC_APRES_FLORAISON) * gain_TEff_jour;
    }

};
} //namespace model
#endif // BUNCH_H
