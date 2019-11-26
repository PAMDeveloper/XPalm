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

    enum internals { STATUS,
                     STATUS_POT,
                     TT_OLEO_DURATION,
                     TT_INI_FLOWERING,
                     TT_INI_HARVEST,
                     TT_INI_SENESCENCE,
                     BIOMASS,
                     FEMELLE_BIOMASS,
                     MALE_BIOMASS,
                     RED_VITESSE_FTSW,
                     RESPIRABLE_BIOMASS,
                     GAIN_TEFF_JOUR,
                     TT_CORRIGE,
                     DEMAND,
                     IC_SETTING
                   };

    enum externals { FTSW,
                     RANK,
                     TEFF,
                     FR_RESTE,
                     FR_FRUITS,
                     TT_SINCE_RANK1,
                     IC_SPIKELET,
                     TREE_IC
                   };

private:

    //      parameters
    double PLASTICITY_BUNCH_IC_APRES_FLORAISON;
    double PLASTICITY_BUNCH_IC_AVANT_FLORAISON;
    double SENS_FTSW;
    double SEUIL_MEDIAN_FTSW;
    double DEBUT_RANG_SENSITIVITY_NOUAISON;
    double FIN_RANG_SENSITIVITY_NOUAISON;
    double IC_spikelet_RANG_DEBUT;
    double IC_spikelet_RANG_FIN;

    //     submodels
    std::unique_ptr < Peduncle > peduncle;
    std::unique_ptr < Bunch > bunch;
    std::unique_ptr < MaleInflo > male;

    //     internals
    inflo::inflo_states status;
    inflo::inflo_states status_pot;

    //predim
    double TT_oleo_duration;
    double TT_ini_flowering;
    double TT_ini_harvest;
    double TT_ini_senescence;
    double production_speed;
    //var
    double biomass;
    double femelle_biomass;
    double male_biomass;
    double red_vitesse_FTSW;
    double respirable_biomass;
    double gain_TEff_jour;
    double TT_corrige;
    double demand;

    //ic
    double nb_joursIC_setting;
    double IC_setting_tot;
    double IC_setting;
    double nb_joursIC_spikelet;
    double IC_spikelet_tot;
    double IC_spikelet;


    //     externals
    double ftsw;
    double rank;
    double TEff;
    double fr_reste;
    double fr_fruits;
    double TT_since_rank1;
    double tree_IC;

public:

    Inflo():
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
        Internal(STATUS_POT, &Inflo::status_pot);
        Internal(TT_OLEO_DURATION, &Inflo::TT_oleo_duration);
        Internal(TT_INI_FLOWERING, &Inflo::TT_ini_flowering);
        Internal(TT_INI_HARVEST, &Inflo::TT_ini_harvest);
        Internal(TT_INI_SENESCENCE, &Inflo::TT_ini_senescence);
        Internal(BIOMASS, &Inflo::biomass);
        Internal(FEMELLE_BIOMASS, &Inflo::femelle_biomass);
        Internal(MALE_BIOMASS, &Inflo::male_biomass);
        Internal(RED_VITESSE_FTSW, &Inflo::red_vitesse_FTSW);
        Internal(RESPIRABLE_BIOMASS, &Inflo::respirable_biomass);
        Internal(GAIN_TEFF_JOUR, &Inflo::gain_TEff_jour);
        Internal(TT_CORRIGE, &Inflo::TT_corrige);
        Internal(DEMAND, &Inflo::demand);
        Internal(IC_SETTING, &Inflo::IC_setting);
        Internal(IC_SPIKELET, &Inflo::IC_spikelet);

        //          externals
        External(FTSW, &Inflo::ftsw);
        External(RANK, &Inflo::rank);
        External(TEFF, &Inflo::TEff);
        External(FR_RESTE, &Inflo::fr_reste);
        External(FR_FRUITS, &Inflo::fr_fruits);
        External(TT_SINCE_RANK1, &Inflo::TT_since_rank1);
        External(TREE_IC, &Inflo::tree_IC);
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


    void init(double t, const xpalm::ModelParameters& parameters){}
    void init(double t, const xpalm::ModelParameters& parameters, double phytomer_age, double rk, double prod_speed,
              double TT_ini_flo, double TT_ini_harv, double TT_ini_senec, double inflo_dev_factor)
    {
        last_time = t - 1;
        //        parameters
        PLASTICITY_BUNCH_IC_APRES_FLORAISON = parameters.get("PLASTICITY_BUNCH_IC_APRES_FLORAISON");
        PLASTICITY_BUNCH_IC_AVANT_FLORAISON = parameters.get("PLASTICITY_BUNCH_IC_AVANT_FLORAISON");
        SENS_FTSW = parameters.get("SENS_FTSW");
        SEUIL_MEDIAN_FTSW = parameters.get("SEUIL_MEDIAN_FTSW");
        DEBUT_RANG_SENSITIVITY_NOUAISON = parameters.get("DEBUT_RANG_SENSITIVITY_NOUAISON");
        DEBUT_RANG_SENSITIVITY_NOUAISON = parameters.get("FIN_RANG_SENSITIVITY_NOUAISON");
        IC_spikelet_RANG_DEBUT = parameters.get("IC_spikelet_RANG_DEBUT");
        IC_spikelet_RANG_FIN = parameters.get("IC_spikelet_RANG_FIN");

        //predim
        TT_ini_flowering = TT_ini_flo;
        TT_ini_harvest = TT_ini_harv;
        TT_ini_senescence = TT_ini_senec;
        rank = rk;
        production_speed = prod_speed;

        //ic
        nb_joursIC_setting = 0;
        IC_setting_tot = 0;
        IC_setting = 0;
//        nb_joursICsex = 0;
//        ICsex_tot = 0;
//        ICsex = 0;
//        nb_joursICabort = 0;
//        ICabort_tot = 0;
//        ICabort = 0;
        nb_joursIC_spikelet = 0;
        IC_spikelet_tot = 0;
        IC_spikelet = 0;

        //var
        biomass= 0;
        femelle_biomass= 0;
        male_biomass= 0;
        red_vitesse_FTSW= 0;
        respirable_biomass= 0;
        gain_TEff_jour= 0;
        demand= 0;


        double PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        double DUREE_OLEO = parameters.get("DUREE_OLEO");
        double RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        TT_oleo_duration = DUREE_OLEO * pow(PRODUCTION_SPEED_ADULT / production_speed, RATIO_DUREE_JEUNES_OLEO); //TODO filer depuis l'arbre à la création du phytomer
        double TT_ini_oleo = TT_ini_harvest - TT_oleo_duration;

        double rd = ((double) rand() / (RAND_MAX)) + 1;
        if (rank > parameters.get("ICsex_RANG_FIN")) {
            if(rd < parameters.get("INI_SEX_RATIO")) {
                status.add(inflo::FEMALE);
                peduncle->init(t, parameters, production_speed, TT_ini_flowering, TT_ini_harvest, inflo_dev_factor);
                bunch->init(t, parameters, production_speed, TT_ini_flowering, TT_ini_harvest, TT_ini_oleo, inflo_dev_factor);
            } else {
                status.add(inflo::MALE);
                male->init(t, parameters, phytomer_age, production_speed, inflo_dev_factor, TT_ini_flowering);
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
            if(!status.is(inflo::HARVEST)) {
                peduncle->put<double>(t, Peduncle::IC_SPIKELET, IC_spikelet);
                peduncle->put<double>(t, Peduncle::TEFF, TEff);
                peduncle->put<double>(t, Peduncle::TT_CORRIGE, TT_corrige);
                peduncle->put<double>(t, Peduncle::TT_SINCE_RANK1, TT_since_rank1);
                peduncle->put<double>(t, Peduncle::FR_RESTE, fr_reste);
                (*peduncle)(t);
            }

            if (status.is(inflo::FLOWERING)) {
                bunch->put<double>(t, Bunch::TEFF, TEff);
                bunch->put<inflo::inflo_states>(t, Bunch::INFLO_STATUS_POT, status);
                bunch->put<inflo::inflo_states>(t, Bunch::INFLO_STATUS, status_pot);
                bunch->put<double>(t, Bunch::TT_SINCE_RANK1, TT_since_rank1);
                bunch->put<double>(t, Bunch::FR_FRUITS, fr_fruits);
                bunch->put<double>(t, Bunch::IC_SPIKELET, IC_spikelet);
                bunch->put<double>(t, Bunch::IC_SETTING, IC_setting);
                (*bunch)(t);
            }

        } else if (status.is(inflo::MALE) && status.is(inflo::INITIATED)) {
            male->put<double>(t, MaleInflo::TEFF, TEff);
            male->put<double>(t, MaleInflo::FR_RESTE, fr_reste);
            (*male)(t);
        }

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


        //IC
        if (TT_corrige > TT_ini_flowering - DEBUT_RANG_SENSITIVITY_NOUAISON / production_speed &&
                TT_corrige < TT_ini_flowering - FIN_RANG_SENSITIVITY_NOUAISON / production_speed &&
                TT_corrige < TT_ini_flowering) {
            nb_joursIC_setting += 1;
            IC_setting_tot += tree_IC;
            IC_setting = (IC_setting_tot) / nb_joursIC_setting;
        }

        if (rank > IC_spikelet_RANG_DEBUT && rank < IC_spikelet_RANG_FIN){
            nb_joursIC_spikelet += 1;
            IC_spikelet_tot += tree_IC;
            IC_spikelet = IC_spikelet_tot / nb_joursIC_spikelet;
        }
    }



};
} //namespace model
#endif // BUNCH_H
