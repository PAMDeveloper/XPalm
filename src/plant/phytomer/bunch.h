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

#include <plant/phytomer/fruit.h>
#include <plant/phytomer/peduncle.h>
#include <plant/phytomer/male_inflo.h>

namespace model {

class Bunch : public CoupledModel < Bunch >
{
public:
    enum submodels { PEDUNCLE, FRUIT, MALEINFLO };

    enum internals { BIOMASS,
                     DEMAND,
                     ESTIMATED_FLOWERING_DATE,
                     ESTIMATED_HARVEST_DATE,
                     FACTEUR_AGE_REGIMES,
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
                     THERMALTIMESINCEAPPEARANCE,
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
    double INITIAL_PRODUCTION_SPEED;
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
    double TT_FLOWERING_ADULTE;
    double TT_HARVEST_ADULTE;

    //     submodels
    std::unique_ptr < Peduncle > peduncle;
    std::unique_ptr < Fruit > fruit;
    std::unique_ptr < MaleInflo > male;

    //     internals
    bunch::bunch_states statut;
    bunch::bunch_states pot_statut;
    bunch::bunch_sex sexe;
    bunch::bunch_sex avort;

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
    double estimated_flowering_date;
    double estimated_harvest_date;
    double facteur_age_regimes;
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
    double thermalTimeSinceAppearance;

    //     externals
    double date_plus_jeune_feuille;
    double tree_productionSpeed;
    double phytomer_rank;
    double TEff;
    double fr_fruits;
    double tree_IC;
    double ftsw;
    double fr_reste;

public:

    Bunch() :
        peduncle(new Peduncle()),
        fruit(new Fruit()),
        male(new MaleInflo())
    {
        //         submodels
        setsubmodel(PEDUNCLE, peduncle.get());
        setsubmodel(FRUIT, fruit.get());
        setsubmodel(MALEINFLO, male.get());

        //         internals
        Internal(STATUS, &Bunch::statut);
        Internal(POT_STATUS, &Bunch::pot_statut);
        Internal(SEX, &Bunch::sexe);
        Internal(AVORT, &Bunch::avort);
        Internal(ESTIMATED_FLOWERING_DATE, &Bunch::estimated_flowering_date);
        Internal(ESTIMATED_HARVEST_DATE, &Bunch::estimated_harvest_date);
        Internal(TT_CORRIGE, &Bunch::TT_corrige);
        Internal(DEMAND, &Bunch::demand);
        Internal(MASSE_POT_REGIME, &Bunch::masse_pot_regime);
        Internal(ICSEX, &Bunch::ICsex);
        Internal(NB_JOURSICSEX, &Bunch::nb_joursICsex);
        Internal(ICSEX_TOT, &Bunch::ICsex_tot);
        Internal(ICABORT, &Bunch::ICabort);
        Internal(NB_JOURSICABORT, &Bunch::nb_joursICabort);
        Internal(ICABORT_TOT, &Bunch::ICabort_tot);
        Internal(NB_JOURSIC_SPIKELET, &Bunch::nb_joursIC_spikelet);
        Internal(IC_SPIKELET_TOT, &Bunch::IC_spikelet_tot);
        Internal(IC_SPIKELET, &Bunch::IC_spikelet);
        Internal(BIOMASS, &Bunch::biomass);
        Internal(FEMELLE_BIOMASS, &Bunch::femelle_biomass);
        Internal(RESPIRABLE_BIOMASS, &Bunch::respirable_biomass);
        Internal(INI_FLOWERING_DATE, &Bunch::ini_flowering_date);
        Internal(MASSE_MAX_REGIME, &Bunch::masse_max_regime);
        Internal(NB_JOURSIC_SETTING, &Bunch::nb_joursIC_setting);
        Internal(IC_SETTING_TOT, &Bunch::IC_setting_tot);
        Internal(IC_SETTING, &Bunch::IC_setting);
        Internal(INDEX, &Bunch::index);
        Internal(RED_VITESSE_FTSW, &Bunch::red_vitesse_FTSW);
        Internal(POTENTIAL_BIOMASS, &Bunch::potential_biomass);
        Internal(TEST_BIOMASS, &Bunch::test_biomass);
        Internal(GAIN_TEFF_JOUR, &Bunch::gain_TEff_jour);
        Internal(FRUIT_NUMBER, &Bunch::fruit_number);
        Internal(FLOWERING_DATE, &Bunch::flowering_date);
        Internal(HARVEST_DATE, &Bunch::harvest_date);
        Internal(FACTEUR_AGE_REGIMES, &Bunch::facteur_age_regimes);
        Internal(POT_FRUITS_NUMBER, &Bunch::pot_fruits_number);
        Internal(MALE_BIOMASS, &Bunch::male_biomass);
        Internal(THERMALTIMESINCEAPPEARANCE, &Bunch::thermalTimeSinceAppearance);
        Internal(INI_HARVEST_DATE, &Bunch::ini_harvest_date);

        //          externals
        External(DATE_PLUS_JEUNE_FEUILLE, &Bunch::date_plus_jeune_feuille);
        External(TREE_PRODUCTIONSPEED, &Bunch::tree_productionSpeed);
        External(PHYTOMER_RANK, &Bunch::phytomer_rank);
        External(TEFF, &Bunch::TEff);
        External(FR_FRUITS, &Bunch::fr_fruits);
        External(FR_RESTE, &Bunch::fr_reste);
        External(TREE_IC, &Bunch::tree_IC);
        External(FTSW, &Bunch::ftsw);
    }

    virtual ~Bunch()
    {
        peduncle.reset(nullptr);
        fruit.reset(nullptr);
        male.reset(nullptr);
    }

    Peduncle * peduncle_model() {
        return peduncle.get();
    }

    Fruit * fruit_model() {
        return fruit.get();
    }

    MaleInflo * male_model() {
        return male.get();
    }


    //TODO réactiver

//    void compute_facteur_age(t) {
//        if (INCREASE_TAILLE_REGIMES * (t + phytomer.step_apparition  - tree.date_plus_jeune_feuille) + FACTEUR_AGE_INI) > 1)
//            facteur_age_regimes = 1;
//        else
//            facteur_age_regimes = (INCREASE_TAILLE_REGIMES * t + phytomer.step_apparition - tree.date_plus_jeune_feuille) + FACTEUR_AGE_INI);

//        pot_fruits_number = facteur_age_regimes * MEAN_FRUIT_NUMBER_ADULTE;
//    }



    void init(double t, const xpalm::ModelParameters& parameters)
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
        INCREASE_TAILLE_REGIMES = (1-0) * 1 / parameters.get("AGE_ADULTE") * 365;


        INITIAL_PRODUCTION_SPEED = parameters.get("INITIAL_PRODUCTION_SPEED");
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
        TT_FLOWERING_ADULTE = parameters.get("TT_FLOWERING_ADULTE");
        TT_HARVEST_ADULTE = parameters.get("TT_HARVEST_ADULTE");

        //         submodels
        peduncle->init(t, parameters);
        fruit->init(t, parameters);
        male->init(t, parameters);

        //        internals
        statut.add(bunch::UNKNOWN);
        statut.add(bunch::NON_ABLATED);
        pot_statut.add(bunch::UNKNOWN);
        sexe = bunch::NO_SEX;

        compute_statut(t); //TODO call in init

        estimated_flowering_date = 0;
        estimated_harvest_date = 0;
        TT_corrige = 0;
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
        flowering_date = -1;
        harvest_date = -1;
        facteur_age_regimes = 1;

        if ( (INCREASE_TAILLE_REGIMES * (t  - date_plus_jeune_feuille) + FACTEUR_AGE_INI) > 1)
            facteur_age_regimes = 1;
        else
            facteur_age_regimes = ( INCREASE_TAILLE_REGIMES * (t - date_plus_jeune_feuille) + FACTEUR_AGE_INI);

        pot_fruits_number = facteur_age_regimes * MEAN_FRUIT_NUMBER_ADULTE;

        male_biomass = 0;
        thermalTimeSinceAppearance = 0;
        ini_harvest_date = 0;


        //init structure
//        phytomer.compute_facteur_age() //TODO
//        if (GlobalVariables.INCREASE_TAILLE_REGIMES * (self.tree.simulation.step + self.step_apparition  - self.tree.date_plus_jeune_feuille) + GlobalVariables.FACTEUR_AGE_INI) > 1 :
//            self.bunch.facteur_age_regimes = 1
//        else:
//            self.bunch.facteur_age_regimes = (GlobalVariables.INCREASE_TAILLE_REGIMES * (self.tree.simulation.step + self.step_apparition - self.tree.date_plus_jeune_feuille) + GlobalVariables.FACTEUR_AGE_INI)
//        self.bunch.pot_fruits_number = self.bunch.facteur_age_regimes * GlobalVariables.MEAN_FRUIT_NUMBER_ADULTE

        double rd = ((double) rand() / (RAND_MAX)) + 1;
        if (phytomer_rank > parameters.get("ICsex_RANG_FIN")) {
           sexe = rd < parameters.get("INI_SEX_RATIO") ? bunch::FEMALE : bunch::MALE;
        }

    }

    void compute_statut(double t) {
        estimated_flowering_date = pow( (MINIMAL_PRODUCTION_SPEED / tree_productionSpeed), RATIO_DUREE_JEUNES_FLOWERING) * (TT_FLOWERING_ADULTE);
        estimated_harvest_date = pow((MINIMAL_PRODUCTION_SPEED / tree_productionSpeed), RATIO_DUREE_JEUNES_HARVEST) * (TT_HARVEST_ADULTE);
        bunch::bunch_states memstatut = statut;

        if(phytomer_rank < 0) {
            statut.add(bunch::INITIE);
            pot_statut.add(bunch::INITIE);
        } else {
            //TODO change Statut coz no removal
            statut.add( TT_corrige > estimated_flowering_date
                        ? (TT_corrige > estimated_harvest_date
                           ? bunch::RECOLTE
                           : bunch::FLORAISON_RECOLTE)
                        : bunch::APPARITION_FLORAISON );

            pot_statut.add( thermalTimeSinceAppearance > estimated_flowering_date
                            ? (thermalTimeSinceAppearance > estimated_harvest_date
                               ? bunch::RECOLTE
                               : bunch::FLORAISON_RECOLTE)
                            : bunch::APPARITION_FLORAISON );
        }

        //       if (statut.is(bunch::FLORAISON_RECOLTE) && memstatut.is(bunch::APPARITION_FLORAISON) {
        //               flowering_day = phytomer.tree.simulation.meteo.getDay()
        //               flowering_month = phytomer.tree.simulation.meteo.getMonth()
        //               flowering_year = phytomer.tree.simulation.meteo.getYear()
        //               phytomer.tree.write_floweringRecord()
        //       }

        if (statut.is(bunch::FLORAISON_RECOLTE) && memstatut.is(bunch::APPARITION_FLORAISON) ) {
            //               harvest_day = phytomer.tree.simulation.meteo.getDay()
            //               harvest_month = phytomer.tree.simulation.meteo.getMonth()
            //               harvest_year = phytomer.tree.simulation.meteo.getYear()
            //               phytomer.tree.writeRecord()
            biomass = 0;
            femelle_biomass = 0;
            respirable_biomass = 0;
            test_biomass = 0;

            //TODO check ces affectations depuis le bunch et les mettre dans les sous modeles
            male_biomass = 0;

            peduncle_model()->put(t, Peduncle::BIOMASS, 0);
            fruit_model()->put(t, Fruit::OIL_BIOMASS, 0);
            fruit_model()->put(t, Fruit::NONOIL_BIOMASS, 0);
        }
    }



    void growth_demand(double t) {
        //        compute_red_vitesse_FTSW();

    }

    void compute_biomass(double t) {
        if (!statut.is(bunch::RECOLTE)) {
            double fruit_oil_biomass = fruit->get< double >(t, Fruit::OIL_BIOMASS);
            double fruit_nonoil_biomass = fruit->get< double >(t, Fruit::NONOIL_BIOMASS);
            double peduncle_biomass = peduncle->get< double >(t, Peduncle::BIOMASS);
            male_biomass = male->get< double >(t, MaleInflo::BIOMASS);
            double fruit_oil_potential_biomass = fruit->get< double >(t, Fruit::OIL_POTENTIAL_BIOMASS);
            double fruit_nonoil_potential_biomass = fruit->get< double >(t, Fruit::NONOIL_POTENTIAL_BIOMASS);
            double peduncle_potential_biomass = peduncle->get< double >(t, Peduncle::POTENTIAL_BIOMASS);
            double male_potential_biomass = male->get< double >(t, MaleInflo::POTENTIAL_BIOMASS);
            double fruit_oil_test_biomass = fruit->get< double >(t, Fruit::TEST_OIL_BIOMASS);
            double fruit_nonoil_test_biomass = fruit->get< double >(t, Fruit::TEST_NONOIL_BIOMASS);
            double peduncle_test_biomass = peduncle->get< double >(t, Peduncle::TEST_BIOMASS);

            biomass = fruit_oil_biomass + fruit_nonoil_biomass + peduncle_biomass + male_biomass;
            femelle_biomass = fruit_oil_biomass  + fruit_nonoil_biomass + peduncle_biomass;
            respirable_biomass = fruit_nonoil_biomass + peduncle_biomass + male_biomass;
            potential_biomass = fruit_oil_potential_biomass  + fruit_nonoil_potential_biomass + peduncle_potential_biomass + male_potential_biomass;

            test_biomass = fruit_oil_test_biomass  + fruit_nonoil_test_biomass + peduncle_test_biomass + male_biomass;
        } else {
            biomass = 0;
            femelle_biomass = 0;
            male_biomass = 0;
            respirable_biomass = 0;
            test_biomass = 0;
        }
    }

    void init_structure(double t) {
          //bunch
//          phytomers[phytomer.name].bunch.thermalTimeSinceAppearance = TTfeuille
//          phytomers[phytomer.name].bunch.TT_corrige = TTfeuille
//          SF = min(GlobalVariables.INCREASE_OF_LEAF_AREA * ( - nb_jour_depuis_l_appari ) + GlobalVariables.INITIAL_SFIND, GlobalVariables.MAXIMAL_SFIND)

//          if phytomers[phytomer.name].rank > GlobalVariables.ICabort_RANG_FIN :
//              phytomers[phytomer.name].bunch.avort = phytomer.bunch.abortion_decision(GlobalVariables.INI_TAUX_D_AVORTEMENT)
//          phytomer.bunch.compute_structure_ini()
//          phytomer.bunch.compute_flowering_date(GlobalVariables.INITIAL_PRODUCTION_SPEED)
//          phytomer.bunch.compute_harvest_date(GlobalVariables.INITIAL_PRODUCTION_SPEED)

//          if phytomers[phytomer.name].rank< 0 :
//              phytomers[phytomer.name].bunch.statut = "INITIE"
//          else :

//              if (phytomers[phytomer.name].bunch.thermalTimeSinceAppearance > phytomers[phytomer.name].bunch.estimated_flowering_date)  : # on calcule une moyenne entre la dtae ? l'apparition et la date au jour j
//                  if (phytomers[phytomer.name].bunch.thermalTimeSinceAppearance > phytomers[phytomer.name].bunch.estimated_harvest_date)  :
//                      phytomers[phytomer.name].bunch.statut = "RECOLTE"
//                  else :
//                      phytomers[phytomer.name].bunch.statut = "FLORAISON_RECOLTE"
//              else :
//                  phytomers[phytomer.name].bunch.statut = "APPARITION_FLORAISON"




        ini_flowering_date = pow((MINIMAL_PRODUCTION_SPEED / INITIAL_PRODUCTION_SPEED), RATIO_DUREE_JEUNES_FLOWERING)  * (TT_FLOWERING_ADULTE);
        ini_harvest_date = pow((MINIMAL_PRODUCTION_SPEED / INITIAL_PRODUCTION_SPEED), RATIO_DUREE_JEUNES_HARVEST) * (TT_HARVEST_ADULTE);

        if (sexe == bunch::FEMALE && avort != bunch::ABORTED) {
            peduncle->init_structure();
            fruit->init_structure();
        }
        if (sexe == bunch::MALE && avort != bunch::ABORTED)
            male->init_structure(thermalTimeSinceAppearance, flowering_date, facteur_age_regimes);
        compute_biomass(0);
    }

    void compute_IC(){
        if (phytomer_rank > ICsex_RANG_DEBUT && phytomer_rank < ICsex_RANG_FIN ) {
            nb_joursICsex += 1;
            ICsex_tot += tree_IC;
            ICsex = ICsex_tot / nb_joursICsex;
        }

        if (phytomer_rank > ICabort_RANG_DEBUT && phytomer_rank < ICabort_RANG_FIN) {
            nb_joursICabort += 1;
            ICabort_tot += tree_IC;
            ICabort = ICabort_tot / nb_joursICabort;
        }

        if (phytomer_rank > IC_spikelet_RANG_DEBUT && phytomer_rank < IC_spikelet_RANG_FIN){
            nb_joursIC_spikelet += 1;
            IC_spikelet_tot += tree_IC;
            IC_spikelet = (IC_spikelet_tot) / nb_joursIC_spikelet;
        }

        if (TT_corrige > estimated_flowering_date - DEBUT_RANG_SENSITIVITY_NOUAISON / tree_productionSpeed &&
                TT_corrige < estimated_flowering_date - FIN_RANG_SENSITIVITY_NOUAISON / tree_productionSpeed &&
                TT_corrige < estimated_flowering_date) {
            nb_joursIC_setting += 1;
            IC_setting_tot += tree_IC;
            IC_setting = (IC_setting_tot) / nb_joursIC_setting;
        }
    }



    void compute(double t, bool /* update */)
    {

        if (avort != bunch::ABORTED and !statut.is(bunch::ABLATED)) {
            if (sexe == bunch::FEMALE) {
                peduncle->put(t, Peduncle::FR_RESTE, fr_reste);
                peduncle->put<bunch::bunch_states>(t, Peduncle::BUNCH_STATUS, statut);
                peduncle->put(t, Peduncle::ABLATION_STATUS, statut);//TODO Merge
                peduncle->put(t, Peduncle::IC_SPIKELET, IC_spikelet);
                peduncle->put(t, Peduncle::FACTEUR_AGE_REGIMES, facteur_age_regimes);
                peduncle->put(t, Peduncle::TEFF, gain_TEff_jour);
                peduncle->put(t, Peduncle::TT_CORRIGE, TT_corrige);
                peduncle->put(t, Peduncle::TREE_PRODUCTION_SPEED, tree_productionSpeed);
                peduncle->put(t, Peduncle::ESTIMATED_FLOWERING_DATE, estimated_flowering_date);
                peduncle->put(t, Peduncle::BUNCH_POT_STATUS, pot_statut);
                peduncle->put(t, Peduncle::THERMALTIMESINCEAPPEARANCE, thermalTimeSinceAppearance);
                (*peduncle)(t);

                fruit->put(t, Fruit::FR_FRUITS, fr_fruits);
                fruit->put(t, Fruit::BUNCH_STATUS, statut);
                fruit->put(t, Fruit::ABLATION_STATUS, statut);//TODO Merge
                fruit->put(t, Fruit::FRUIT_NUMBER, fruit_number);
                fruit->put(t, Peduncle::TEFF, gain_TEff_jour);
                fruit->put(t, Fruit::TT_CORRIGE, TT_corrige);
                fruit->put(t, Fruit::PROD_SPEED, tree_productionSpeed);
                fruit->put(t, Fruit::TT_SINCE_APP, thermalTimeSinceAppearance);
                fruit->put(t, Fruit::ESTIM_FLO_DATE, estimated_flowering_date);
                fruit->put(t, Fruit::ESTIM_HARVEST_DATE, estimated_harvest_date);
                fruit->put(t, Fruit::POT_STATUS, pot_statut);
                (*fruit)(t);
            }
            if (sexe == bunch::MALE) {
                male->put(t, MaleInflo::FR_RESTE, fr_reste);
                male->put(t, MaleInflo::BUNCH_STATUS, statut);
                male->put(t, MaleInflo::TEFF, gain_TEff_jour);
                male->put(t, MaleInflo::TT_SINCE_APPEARANCE, thermalTimeSinceAppearance);
                male->put(t, MaleInflo::ESTIM_FLO_DATE, estimated_flowering_date);
                male->put(t, MaleInflo::FACTEUR_AGE_REGIME, facteur_age_regimes);
                male->put(t, MaleInflo::PROD_SPEED, tree_productionSpeed);
                (*male)(t);
            }
        } else {
            //TODO reset submodel
            //            peduncule.biomass = 0;
            //            fruit.nonoil_biomass = 0;
            //            fruit.oil_biomass = 0;
            //            fruit.masse_ind  = 0;
            //            fruit.ratio_huile_mesocarp = 0;
            //            fruit.oil_potential_biomass = 0;
            //            fruit.nonoil_potential_biomass = 0;
            //            peduncule.potential_biomass = 0;
            //            male.potential_biomass = 0;
            //            male.biomass = 0;
        }
        compute_biomass(t);

        //Demand
        if (statut.is(bunch::APPARITION_FLORAISON))
            red_vitesse_FTSW = pow((ftsw + 1 - SEUIL_MEDIAN_FTSW ), SENS_FTSW );
        else
            red_vitesse_FTSW = 1;

        double correctedTEff = red_vitesse_FTSW * TEff;
        gain_TEff_jour = correctedTEff;

        //        Organ.growth_demand(self, correctedTEff);
        if(phytomer_rank > 0)
            thermalTimeSinceAppearance += gain_TEff_jour;

        if (!statut.is(bunch::RECOLTE))
            compute_statut(t);

        demand = fruit->get< double >(t, Fruit::DEMAND) +
                peduncle->get< double >(t, Peduncle::DEMAND) +
                male->get< double >(t, MaleInflo::DEMAND);


        //        compute_TT_corrige()
        if (statut.is(bunch::APPARITION_FLORAISON))
            TT_corrige += pow(fr_fruits, PLASTICITY_BUNCH_IC_AVANT_FLORAISON) * gain_TEff_jour;
        if (statut.is(bunch::FLORAISON_RECOLTE))
            TT_corrige += pow(fr_fruits, PLASTICITY_BUNCH_IC_APRES_FLORAISON) * gain_TEff_jour;

        //        compute_fruit_number()
        if (phytomer_rank > IC_spikelet_RANG_FIN) {
            if (TT_corrige < estimated_flowering_date)
                fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pot_fruits_number;
            else
                fruit_number =  pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pow(IC_setting, SENSIVITY_IC_SETTING)  * pot_fruits_number;
            if (sexe == bunch::MALE)
                fruit_number = 0;
            if (avort == bunch::ABORTED)
                fruit_number = 0;
            if (statut.is(bunch::ABLATED))
                fruit_number = 0;
        }

        compute_IC();

    }

};
} //namespace model
#endif // BUNCH_H
