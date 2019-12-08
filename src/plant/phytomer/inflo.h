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
                     PEDUNCLE_BIOMASS,
                     FEMELLE_BIOMASS,
                     BUNCH_OIL_BIOMASS,
                     BUNCH_NONOIL_BIOMASS,
                     MALE_BIOMASS,
                     PEDUNCLE_BIOMASS_HARVESTED,
                     FEMELLE_BIOMASS_HARVESTED,
                     BUNCH_OIL_BIOMASS_HARVESTED,
                     BUNCH_NONOIL_BIOMASS_HARVESTED,
                     MALE_BIOMASS_HARVESTED,
                     RED_VITESSE_FTSW,
                     RESPIRABLE_BIOMASS,
                     GAIN_TEFF_JOUR,
                     TT_CORRIGE,
                     DEMAND,
                     IC_SETTING,
                     IC_SPIKELET,
                     IC_SEX,
                     IC_ABORT,
                     BUNCH_DEMAND,
                     PEDUNCLE_DEMAND,
                     MALE_DEMAND
                   };

    enum externals { FTSW,
                     RANK,
                     TEFF,
                     FR_RESTE,
                     FR_FRUITS,
                     TT_SINCE_APPEARENCE,
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
    double ICsex_RANG_DEBUT;
    double ICsex_RANG_FIN;
    double ICabort_RANG_DEBUT;
    double ICabort_RANG_FIN;
    double Seuil_IC_sex;
    double SENSITIVITY_SEX;
    double Seuil_IC_abort;
    double SENSITIVITY_ABORTION;


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
    double peduncle_biomass;
    double femelle_biomass;
    double bunch_oil_biomass;
    double bunch_nonoil_biomass;
    double male_biomass;

    double peduncle_biomass_harvested;
    double femelle_biomass_harvested;
    double bunch_oil_biomass_harvested;
    double bunch_nonoil_biomass_harvested;
    double male_biomass_harvested;

    double red_vitesse_FTSW;
    double respirable_biomass;
    double gain_TEff_jour;
    double TT_corrige;
    double demand;
    double bunch_demand;
    double peduncle_demand;
    double male_demand;

    //ic
    double nb_joursIC_setting;
    double IC_setting_tot;
    double IC_setting;
    double nb_joursIC_spikelet;
    double IC_spikelet_tot;
    double IC_spikelet;
    double nb_joursICsex;
    double IC_sex_tot;
    double IC_sex;
    double nb_joursICabort;
    double IC_abort_tot;
    double IC_abort;


    //     externals
    double ftsw;
    double rank;
    double TEff;
    double fr_reste;
    double fr_fruits;
    //    double TT_since_rank1;
    double TT_since_appearence;
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
        Internal(PEDUNCLE_BIOMASS, &Inflo::peduncle_biomass);
        Internal(FEMELLE_BIOMASS, &Inflo::femelle_biomass);
        Internal(BUNCH_OIL_BIOMASS, &Inflo::bunch_oil_biomass);
        Internal(BUNCH_NONOIL_BIOMASS, &Inflo::bunch_nonoil_biomass);
        Internal(MALE_BIOMASS, &Inflo::male_biomass);
        Internal(PEDUNCLE_BIOMASS_HARVESTED, &Inflo::peduncle_biomass_harvested);
        Internal(FEMELLE_BIOMASS_HARVESTED, &Inflo::femelle_biomass_harvested);
        Internal(BUNCH_OIL_BIOMASS_HARVESTED, &Inflo::bunch_oil_biomass_harvested);
        Internal(BUNCH_NONOIL_BIOMASS_HARVESTED, &Inflo::bunch_nonoil_biomass_harvested);
        Internal(MALE_BIOMASS_HARVESTED, &Inflo::male_biomass_harvested);
        Internal(RED_VITESSE_FTSW, &Inflo::red_vitesse_FTSW);
        Internal(RESPIRABLE_BIOMASS, &Inflo::respirable_biomass);
        Internal(GAIN_TEFF_JOUR, &Inflo::gain_TEff_jour);
        Internal(TT_CORRIGE, &Inflo::TT_corrige);
        Internal(DEMAND, &Inflo::demand);
        Internal(IC_SETTING, &Inflo::IC_setting);
        Internal(IC_SPIKELET, &Inflo::IC_spikelet);
        Internal(IC_SEX, &Inflo::IC_sex);
        Internal(IC_ABORT, &Inflo::IC_abort);
        Internal(BUNCH_DEMAND, &Inflo::bunch_demand);
        Internal(PEDUNCLE_DEMAND, &Inflo::peduncle_demand);
        Internal(MALE_DEMAND, &Inflo::male_demand);

        //          externals
        External(FTSW, &Inflo::ftsw);
        External(RANK, &Inflo::rank);
        External(TEFF, &Inflo::TEff);
        External(FR_RESTE, &Inflo::fr_reste);
        External(FR_FRUITS, &Inflo::fr_fruits);
        External(TT_SINCE_APPEARENCE, &Inflo::TT_since_appearence);
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
    void init(double t, const xpalm::ModelParameters& parameters, double phytomer_age,
              double rk, double prod_speed,
              double TT_ini_flo, double TT_ini_harv,
              double TT_ini_senec, double inflo_dev_factor)
    {
        last_time = t-1;

        //        parameters
        PLASTICITY_BUNCH_IC_APRES_FLORAISON = parameters.get("PLASTICITY_BUNCH_IC_APRES_FLORAISON");
        PLASTICITY_BUNCH_IC_AVANT_FLORAISON = parameters.get("PLASTICITY_BUNCH_IC_AVANT_FLORAISON");
        SENS_FTSW = parameters.get("SENS_FTSW");
        SEUIL_MEDIAN_FTSW = parameters.get("SEUIL_MEDIAN_FTSW");
        DEBUT_RANG_SENSITIVITY_NOUAISON = parameters.get("DEBUT_RANG_SENSITIVITY_NOUAISON");
        FIN_RANG_SENSITIVITY_NOUAISON = parameters.get("FIN_RANG_SENSITIVITY_NOUAISON");
        IC_spikelet_RANG_DEBUT = parameters.get("IC_spikelet_RANG_DEBUT");
        IC_spikelet_RANG_FIN = parameters.get("IC_spikelet_RANG_FIN");
        ICsex_RANG_DEBUT = parameters.get("ICsex_RANG_DEBUT");
        ICsex_RANG_FIN = parameters.get("ICsex_RANG_FIN");
        ICabort_RANG_DEBUT = parameters.get("ICabort_RANG_DEBUT");
        ICabort_RANG_FIN = parameters.get("ICabort_RANG_FIN");
        Seuil_IC_sex = parameters.get("Seuil_IC_sex");
        SENSITIVITY_SEX = parameters.get("SENSITIVITY_SEX");
        Seuil_IC_abort = parameters.get("Seuil_IC_abort");
        SENSITIVITY_ABORTION = parameters.get("SENSITIVITY_ABORTION");

        //predim
        TT_ini_flowering = TT_ini_flo;
        TT_ini_harvest = TT_ini_harv;
        TT_ini_senescence = TT_ini_senec;
        rank = rk;
        production_speed = prod_speed;
        TT_corrige = 0;

        status.states = 0;
        status.add(inflo::INITIATED);
        status_pot.states = 0;
        status_pot.add(inflo::INITIATED);
        bunch_demand = 0;
        male_demand = 0;
        peduncle_demand = 0;
        //ic
        nb_joursIC_setting = 0;
        IC_setting_tot = 0;
        IC_setting = 0;
        nb_joursICsex = 0;
        IC_sex_tot = 0;
        IC_sex = 0;
        nb_joursICabort = 0;
        IC_abort_tot = 0;
        IC_abort = 0;
        nb_joursIC_spikelet = 0;
        IC_spikelet_tot = 0;
        IC_spikelet = 0;

        //var
        biomass= 0;
        peduncle_biomass=0;
        femelle_biomass= 0;
        bunch_oil_biomass=0;
        bunch_nonoil_biomass=0;
        male_biomass= 0;

        peduncle_biomass_harvested=0;
        femelle_biomass_harvested= 0;
        bunch_oil_biomass_harvested=0;
        bunch_nonoil_biomass_harvested=0;
        male_biomass_harvested= 0;

        red_vitesse_FTSW= 0;
        respirable_biomass= 0;
        gain_TEff_jour= 0;
        demand= 0;


        double PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        double DUREE_OLEO = parameters.get("DUREE_OLEO");
        double RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        TT_oleo_duration = DUREE_OLEO * pow(PRODUCTION_SPEED_ADULT / production_speed, RATIO_DUREE_JEUNES_OLEO);
        double TT_ini_oleo = TT_ini_harvest - TT_oleo_duration;

        peduncle->init(t, parameters, production_speed, TT_ini_flowering, TT_ini_harvest, inflo_dev_factor);
        bunch->init(t, parameters, production_speed, TT_ini_flowering, TT_ini_harvest, TT_ini_oleo, inflo_dev_factor);
        male->init(t, parameters, phytomer_age, production_speed, inflo_dev_factor, TT_ini_flowering);

        double rd = (double) rand() / RAND_MAX;
        if (rank > ICsex_RANG_FIN) {
            if(rd < parameters.get("INI_SEX_RATIO")) {
                status.add(inflo::FEMALE);
            } else {
                status.add(inflo::MALE);
            }
        }

        rd = (double) rand() / RAND_MAX;
        if (rank > ICabort_RANG_FIN) {
            if( rd < parameters.get("INI_TAUX_D_AVORTEMENT"))
                status.add(inflo::ABORTED);
        }
        TT_corrige= parameters.get("T_EFF_INI") * phytomer_age;

        step_state();
    }



    void step_state() {
        if( TT_corrige > TT_ini_harvest) {
            if(!status.is(inflo::HARVEST) && status.is(inflo::FEMALE)){
                status.replace(inflo::FLOWERING, inflo::HARVEST);
                status.del(inflo::INITIATED);
            }
        } else if( TT_corrige > TT_ini_flowering) {
            if(!status.is(inflo::FLOWERING))
                status.replace(inflo::INITIATED, inflo::FLOWERING);
        } else {
            if(!status.is(inflo::INITIATED))
                status.add(inflo::INITIATED);
        }

        if(TT_corrige > TT_ini_harvest - TT_oleo_duration && TT_corrige < TT_ini_harvest && status.is(inflo::FEMALE))
            status.add(inflo::OLEOSYNTHESIS);
        else
            status.del(inflo::OLEOSYNTHESIS);

        if(TT_corrige > TT_ini_senescence && status.is(inflo::MALE))
            status.replace(inflo::INITIATED, inflo::SENESCENCE);
        else
            status.del(inflo::SENESCENCE);
    }


    void compute(double t, bool /* update */)
    {
//        if(status.is(inflo::ABORTED) || status.is(inflo::DEAD) || status.is(inflo::HARVEST))
//            return;
        if(status.is(inflo::ABORTED))
            return;

        if (rank == ICabort_RANG_FIN) {
            double rd = (double) rand() / RAND_MAX;
            if(rd < min(1.,pow(IC_abort/Seuil_IC_abort, SENSITIVITY_ABORTION))) {
                status.add(inflo::ABORTED);
            }
        }

        if (rank > ICsex_RANG_FIN && !status.is(inflo::FEMALE) && !status.is(inflo::MALE) && !status.is(inflo::ABORTED)) {
            double rd = (double) rand() / RAND_MAX;
            if(rd < min(1.,pow(IC_sex/Seuil_IC_sex, SENSITIVITY_SEX))) {
                status.add(inflo::FEMALE);
            } else {
                status.add(inflo::MALE);
            }
        }


        step_state();

        if(!status.is(inflo::HARVEST)) {
            if (status.is(inflo::FEMALE)) {
                peduncle->put<double>(t, Peduncle::IC_SPIKELET, IC_spikelet);
                peduncle->put<double>(t, Peduncle::TEFF, TEff);
                peduncle->put<double>(t, Peduncle::TT_CORRIGE, TT_corrige);
                peduncle->put<double>(t, Peduncle::TT_SINCE_APPEARENCE, TT_since_appearence);
                peduncle->put<double>(t, Peduncle::FR_RESTE, fr_reste);
                (*peduncle)(t);

                peduncle_biomass =  peduncle->get< double >(t, Peduncle::BIOMASS);
            }

            if (status.is(inflo::FEMALE) && status.is(inflo::FLOWERING)) {
                bunch->put<double>(t, Bunch::TEFF, TEff);
                bunch->put<inflo::inflo_states>(t, Bunch::INFLO_STATUS_POT, status);
                bunch->put<inflo::inflo_states>(t, Bunch::INFLO_STATUS, status_pot);
                bunch->put<double>(t, Bunch::TT_SINCE_APPEARENCE, TT_since_appearence);
                bunch->put<double>(t, Bunch::FR_FRUITS, fr_fruits);
                bunch->put<double>(t, Bunch::IC_SPIKELET, IC_spikelet);
                bunch->put<double>(t, Bunch::IC_SETTING, IC_setting);
                bunch->put<double>(t, Bunch::RANK, rank);
                (*bunch)(t);

                bunch_oil_biomass = bunch->get< double >(t, Bunch::OIL_BIOMASS);
                bunch_nonoil_biomass =  bunch->get< double >(t, Bunch::NONOIL_BIOMASS);

            }
        }

        if(status.is(inflo::HARVEST)) { // TODO check for carbon balance --> potential error from aborted satus
            peduncle->put<double>(t, Peduncle::IC_SPIKELET, IC_spikelet);
            peduncle->put<double>(t, Peduncle::TEFF, TEff);
            peduncle->put<double>(t, Peduncle::TT_CORRIGE, TT_corrige);
            peduncle->put<double>(t, Peduncle::TT_SINCE_APPEARENCE, TT_since_appearence);
            peduncle->put<double>(t, Peduncle::FR_RESTE, fr_reste);
            (*peduncle)(t);

            bunch->put<double>(t, Bunch::TEFF, TEff);
            bunch->put<inflo::inflo_states>(t, Bunch::INFLO_STATUS_POT, status);
            bunch->put<inflo::inflo_states>(t, Bunch::INFLO_STATUS, status_pot);
            bunch->put<double>(t, Bunch::TT_SINCE_APPEARENCE, TT_since_appearence);
            bunch->put<double>(t, Bunch::FR_FRUITS, fr_fruits);
            bunch->put<double>(t, Bunch::IC_SPIKELET, IC_spikelet);
            bunch->put<double>(t, Bunch::IC_SETTING, IC_setting);
            bunch->put<double>(t, Bunch::RANK, rank);
            (*bunch)(t);

            bunch_oil_biomass_harvested = bunch->get< double >(t, Bunch::OIL_BIOMASS);
            bunch_nonoil_biomass_harvested =  bunch->get< double >(t, Bunch::NONOIL_BIOMASS);
            peduncle_biomass_harvested =  peduncle->get< double >(t, Peduncle::BIOMASS);

        }

        if (status.is(inflo::MALE) && status.is(inflo::INITIATED)) {
            male->put<double>(t, MaleInflo::TEFF, TEff);
            male->put<double>(t, MaleInflo::FR_RESTE, fr_reste);
            (*male)(t);

            male_biomass = male->get< double >(t, MaleInflo::BIOMASS);
        }

        if (status.is(inflo::MALE) && status.is(inflo::SENESCENCE)) {
            male->put<double>(t, MaleInflo::TEFF, TEff);
            male->put<double>(t, MaleInflo::FR_RESTE, fr_reste);
            (*male)(t);

            male_biomass_harvested = male->get< double >(t, MaleInflo::BIOMASS);
        }



        //        compute_growth(t);
        femelle_biomass = bunch_oil_biomass  + bunch_nonoil_biomass + peduncle_biomass;
        biomass = femelle_biomass + male_biomass;
        respirable_biomass = bunch_nonoil_biomass + peduncle_biomass + male_biomass;

        femelle_biomass_harvested = bunch_oil_biomass_harvested  + bunch_nonoil_biomass_harvested + peduncle_biomass_harvested;


        //        compute_demand(t);

        //Demand //TODO get from tree
        if (status.is(inflo::INITIATED))
            red_vitesse_FTSW = pow((ftsw + 1 - SEUIL_MEDIAN_FTSW ), SENS_FTSW );
        else
            red_vitesse_FTSW = 1;

        double correctedTEff = red_vitesse_FTSW * TEff;
        gain_TEff_jour = correctedTEff;

        //        Organ.growth_demand(self, correctedTEff);
        //        if(rank > 0)
        TT_since_appearence += gain_TEff_jour;


        bunch_demand = status.is(inflo::FEMALE) && status.is(inflo::FLOWERING) ? bunch->get< double >(t, Bunch::DEMAND) : 0;
        peduncle_demand = status.is(inflo::FEMALE) && !status.is(inflo::HARVEST) ? peduncle->get< double >(t, Peduncle::DEMAND) : 0;
        male_demand = status.is(inflo::MALE) && status.is(inflo::INITIATED) ? male->get< double >(t, MaleInflo::DEMAND) : 0;
        demand = bunch_demand + peduncle_demand + male_demand;

        //        compute_TT_corrige()
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

        if (rank > ICsex_RANG_DEBUT && rank < ICsex_RANG_FIN ) {
            nb_joursICsex += 1;
            IC_sex_tot += tree_IC;
            IC_sex = IC_sex_tot / nb_joursICsex;
        }

        if (rank > ICabort_RANG_DEBUT && rank < ICabort_RANG_FIN) {
            nb_joursICabort += 1;
            IC_abort_tot += tree_IC;
            IC_abort = IC_abort_tot / nb_joursICabort;
        }
    }



};
} //namespace model
#endif // BUNCH_H
