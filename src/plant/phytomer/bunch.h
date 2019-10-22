#ifndef FRUIT_H
#define FRUIT_H
#include <defines.hpp>

namespace model {

class Bunch : public AtomicModel < Bunch >
{
public:
    enum internals { OIL_DEMAND, OIL_POT_DEMAND, OIL_STATUS, NONOIL_DEMAND, NONOIL_POT_DEMAND, DEMAND,
                     FRUIT_DUREE_OLEO, OIL_ASSIMILATE_SUPPLY, NONOIL_ASSIMILATE_SUPPLY, OIL_BIOMASS,
                     NONOIL_BIOMASS, TEST_OIL_BIOMASS, OIL_POTENTIAL_BIOMASS, NONOIL_POTENTIAL_BIOMASS,
                     TEST_NONOIL_BIOMASS, MASSE_IND, RATIO_HUILE_MESOCARP, MASSE_INI, TPS_PASSE_EN_OLEO};

    enum externals { TEFF, TT_CORRIGE, POT_STATUS, BUNCH_STATUS, ABLATION_STATUS, TT_SINCE_APP, PROD_SPEED,
                     ESTIM_HARVEST_DATE, ESTIM_FLO_DATE, FRUIT_NUMBER, FR_FRUITS, INI_FLO_DATE, INI_HARVEST_DATE, FACTEUR_AGE_REGIMES};

private:
    //      parameters
    double PRODUCTION_SPEED_ADULT;
    double PRODUCTION_SPEED_INITIAL;

    double DUREE_OLEO;
    double IND_FRUIT_WEIGHT;
    double OIL_CONTENT;
    double COUT_OIL;
    double RATIO_DUREE_JEUNES_OLEO;
    double COUT_STRUCTURE_REGIME;
    double MEAN_FRUIT_NUMBER_ADULTE;

    //      attribute
    double MASSE_IND_MAX;

    //     internals
//    oil::oil_state oil_statut;

    double oil_demand;
    double nonoil_demand;
    double demand;
    double oil_assimilate_supply;
    double nonoil_assimilate_supply;
    double oil_biomass;
    double nonoil_biomass;
    double masse_ind;
    double masse_ini;
    double ratio_huile_mesocarp;
    double fruit_duree_oleo;

//    double tps_passe_en_oleo;
//    double oil_demand_pot;
//    double nonoil_demand_pot;
//    double oil_potential_biomass;
//    double nonoil_potential_biomass;

    //     externals
    inflo::inflo_states pot_statut;
    inflo::inflo_states inflo_status;
    inflo::inflo_states ablation_statut;

    double Teff;
    double TT_corrige;
    double thermalTimeSinceAppearance;
    double productionSpeed;
    double estimated_harvest_date;
    double estimated_flowering_date;
    int fruit_number;
    double fr_fruits;
    double ini_flowering_date;
    double ini_harvest_date;
    double facteur_age_regimes;

public:

    Bunch()
    {
        //         internals
        Internal(OIL_DEMAND, &Bunch::oil_demand);
        Internal(OIL_POT_DEMAND, &Bunch::oil_demand_pot);
        Internal(OIL_STATUS, &Bunch::oil_statut);
        Internal(NONOIL_DEMAND, &Bunch::nonoil_demand);
        Internal(DEMAND, &Bunch::demand);
        Internal(FRUIT_DUREE_OLEO, &Bunch::fruit_duree_oleo);
        Internal(FR_FRUITS, &Bunch::fr_fruits);
        Internal(OIL_ASSIMILATE_SUPPLY, &Bunch::oil_assimilate_supply);
        Internal(NONOIL_ASSIMILATE_SUPPLY, &Bunch::nonoil_assimilate_supply);
        Internal(OIL_BIOMASS, &Bunch::oil_biomass);
        Internal(NONOIL_BIOMASS, &Bunch::nonoil_biomass);
        Internal(MASSE_IND, &Bunch::masse_ind);
        Internal(RATIO_HUILE_MESOCARP, &Bunch::ratio_huile_mesocarp);
        Internal(MASSE_INI, &Bunch::masse_ini);

//        Internal(NONOIL_POT_DEMAND, &Bunch::nonoil_demand_pot);
//        Internal(TPS_PASSE_EN_OLEO, &Bunch::tps_passe_en_oleo);
//        Internal(TEST_OIL_BIOMASS, &Bunch::test_oil_biomass);
//        Internal(OIL_POTENTIAL_BIOMASS, &Bunch::oil_potential_biomass);
//        Internal(NONOIL_POTENTIAL_BIOMASS, &Bunch::nonoil_potential_biomass);
//        Internal(TEST_NONOIL_BIOMASS, &Bunch::test_nonoil_biomass);

        //          externals
        External(TEFF, &Bunch::Teff);
        External(TT_CORRIGE, &Bunch::TT_corrige);
        External(POT_STATUS, &Bunch::pot_statut);
        External(BUNCH_STATUS, &Bunch::inflo_status);
        External(ABLATION_STATUS, &Bunch::ablation_statut);
        External(TT_SINCE_APP, &Bunch::thermalTimeSinceAppearance);
        External(PROD_SPEED, &Bunch::productionSpeed);
        External(ESTIM_HARVEST_DATE, &Bunch::estimated_harvest_date);
        External(ESTIM_FLO_DATE, &Bunch::estimated_flowering_date);
        External(FRUIT_NUMBER, &Bunch::fruit_number);
        External(INI_FLO_DATE, &Bunch::ini_flowering_date);
        External(INI_HARVEST_DATE, &Bunch::ini_harvest_date);
        External(FACTEUR_AGE_REGIMES, &Bunch::facteur_age_regimes);

    }

    virtual ~Bunch()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        //default init
        last_time = t-1;

        //        parameters
        PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");

        DUREE_OLEO = parameters.get("DUREE_OLEO");
        IND_FRUIT_WEIGHT = parameters.get("IND_FRUIT_WEIGHT");
        OIL_CONTENT = parameters.get("OIL_CONTENT");
        COUT_OIL = parameters.get("COUT_OIL");
        RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");
        MEAN_FRUIT_NUMBER_ADULTE = parameters.get("MEAN_FRUIT_NUMBER_ADULTE");

        //        attributes
        MASSE_IND_MAX = IND_FRUIT_WEIGHT / 1000;

//        oil_statut = oil::UNKNOWN;
        oil_demand = 0;
        nonoil_demand = 0;
        demand = 0;
        fruit_duree_oleo = 0;
        oil_assimilate_supply = 0;
        nonoil_assimilate_supply = 0;
        oil_biomass = 0;
        nonoil_biomass = 0;
        masse_ind = 0;
        ratio_huile_mesocarp = 0;
        masse_ini = 0;

//        tps_passe_en_oleo = 0;
//        nonoil_demand_pot = 0;
//        oil_demand_pot = 0;
//        test_oil_biomass = 0;
//        oil_potential_biomass = 0;
//        nonoil_potential_biomass = 0;
//        test_nonoil_biomass = 0;
    }


//    void init_structure() {
//        fruit_duree_oleo = DUREE_OLEO  * pow(PRODUCTION_SPEED_ADULT / PRODUCTION_SPEED_INITIAL, RATIO_DUREE_JEUNES_OLEO);
//        if (thermalTimeSinceAppearance < ini_flowering_date) {
//            oil_biomass = 0;
//            nonoil_biomass = 0;
//        } else {
//            if (thermalTimeSinceAppearance > ini_harvest_date) {
//                oil_biomass = 0;
//                nonoil_biomass = 0;
//            } else {
//                nonoil_biomass = facteur_age_regimes * ( 1 - OIL_CONTENT ) * MEAN_FRUIT_NUMBER_ADULTE * (IND_FRUIT_WEIGHT/1000) / (ini_harvest_date - ini_flowering_date ) * (thermalTimeSinceAppearance - ini_flowering_date);
//                if (thermalTimeSinceAppearance > ini_harvest_date - fruit_duree_oleo) {
//                    masse_ini =  facteur_age_regimes *  MEAN_FRUIT_NUMBER_ADULTE * (IND_FRUIT_WEIGHT/1000) * OIL_CONTENT;
//                    tps_passe_en_oleo =   thermalTimeSinceAppearance - (ini_harvest_date - fruit_duree_oleo);
//                    oil_biomass =  masse_ini / fruit_duree_oleo * tps_passe_en_oleo;
//                }
//            }
//        }
//        oil_potential_biomass = oil_biomass;
//        nonoil_potential_biomass = nonoil_biomass;
//        compute_ind_mass();
//    }





//    oil::oil_state compute_oil_statut() { //TODO step state
//        if (ablation_statut.is(inflo::ABLATED)) {
//            return oil::PAS_DE_FRUITS;
//        } else {
//            if (inflo_status.is(inflo::APPARITION_FLORAISON))
//                return oil::PAS_DE_FRUITS;
//            else {
//                if (inflo_status.is(inflo::RECOLTE))
//                    return oil::RECOLTE;
//                else {
//                    if (TT_corrige > estimated_harvest_date - fruit_duree_oleo)
//                        return oil::OLEOSYNTHESE;
//                    else
//                        return oil::AVANT_OLEOSYNTHESE;
//                }
//            }
//        }
//    }




    double compute_oil_demand(double TT) {
        if (ablation_statut.is(inflo::ABLATED)) {
            demand = 0;
        } else {
            if (!inflo_status.is(inflo::FLORAISON_RECOLTE) ) {
                demand = 0;
            } else {
                if (TT > estimated_harvest_date - fruit_duree_oleo)
                    demand = Teff * fruit_number * OIL_CONTENT * COUT_OIL * MASSE_IND_MAX / fruit_duree_oleo ;
                else
                    demand = 0;
            }
        }
        return demand;
    }

    double compute_nonoil_demand(inflo::inflo_states statut) {
        if (ablation_statut.is(inflo::ABLATED))
            return 0;
        else {
            if (!statut.is(inflo::FLORAISON_RECOLTE))
                return 0;
            else
                return Teff * fruit_number * (1 - OIL_CONTENT) * COUT_STRUCTURE_REGIME * MASSE_IND_MAX / (estimated_harvest_date - estimated_flowering_date);
        }
    }



    void growth() {
        //compute oil and nonoil biomass
        oil_assimilate_supply = oil_demand * fr_fruits;
        nonoil_assimilate_supply = nonoil_demand_pot * fr_fruits;

        if (inflo_status.is(inflo::RECOLTE) ||  ablation_statut.is(inflo::ABLATED)) {
            oil_biomass = 0;
            nonoil_biomass = 0;
        } else {
            oil_biomass += oil_assimilate_supply / COUT_OIL;
            nonoil_biomass += nonoil_assimilate_supply / COUT_STRUCTURE_REGIME;
        }
//        oil_potential_biomass += oil_demand_pot / COUT_OIL;
//        nonoil_potential_biomass += nonoil_demand_pot / COUT_STRUCTURE_REGIME;


        //compute ind mass
        if (fruit_number != 0)
            masse_ind  = ((oil_biomass + nonoil_biomass) / fruit_number) * 1000; //en grammes
        else
            masse_ind  = 0;

        if (oil_biomass == 0)
            ratio_huile_mesocarp = 0;
        else
            ratio_huile_mesocarp = oil_biomass / (nonoil_biomass + oil_biomass);
    }

    void growth_demand(){
        fruit_duree_oleo = DUREE_OLEO * pow(PRODUCTION_SPEED_ADULT / productionSpeed, RATIO_DUREE_JEUNES_OLEO);

        oil_demand = compute_oil_demand(TT_corrige);
//        oil_demand_pot = compute_oil_demand(thermalTimeSinceAppearance);
//        oil_statut = compute_oil_statut();
        nonoil_demand = compute_nonoil_demand(inflo_status);
//        nonoil_demand_pot = compute_nonoil_demand(pot_statut);
        demand = oil_demand + nonoil_demand;
    }

    void compute(double t, bool /* update */)
    {
        last_time = t-1;

        //step_state()
        growth();
        growth_demand();
    }

};

} // namespace model
#endif // FRUIT_H
