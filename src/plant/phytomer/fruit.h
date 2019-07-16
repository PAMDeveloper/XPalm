#ifndef FRUIT_H
#define FRUIT_H
#include <defines.hpp>

namespace model {

class Fruit : public AtomicModel < Fruit >
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
    double MINIMAL_PRODUCTION_SPEED;
    double INITIAL_PRODUCTION_SPEED;
    double DUREE_OLEO;
    double IND_FRUIT_WEIGHT;
    double OIL_CONTENT;
    double COUT_OIL;
    double RATIO_DUREE_JEUNES_OLEO;
    double COUT_STRUCTURE_REGIME;
    double MEAN_FRUIT_NUMBER_ADULTE;

    //      attribute
    double masse_ind_max;

    //     internals
    double oil_demand;
    double oil_demand_pot;
    oil::oil_state oil_statut;
    double nonoil_demand;
    double nonoil_demand_pot;
    double demand;
    double fruit_duree_oleo;
    double oil_assimilate_supply;
    double nonoil_assimilate_supply;
    double oil_biomass;
    double nonoil_biomass;
    double test_oil_biomass;
    double oil_potential_biomass;
    double nonoil_potential_biomass;
    double test_nonoil_biomass;
    double masse_ind;
    double ratio_huile_mesocarp;
    double masse_ini;
    double tps_passe_en_oleo;

    //     externals
    double Teff;
    double TT_corrige;
    bunch::bunch_states pot_statut;
    bunch::bunch_states bunch_statut;
    bunch::bunch_states ablation_statut;
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

    Fruit()
    {
        //         internals
        Internal(OIL_DEMAND, &Fruit::oil_demand);
        Internal(OIL_POT_DEMAND, &Fruit::oil_demand_pot);
        Internal(OIL_STATUS, &Fruit::oil_statut);
        Internal(NONOIL_DEMAND, &Fruit::nonoil_demand);
        Internal(NONOIL_POT_DEMAND, &Fruit::nonoil_demand_pot);
        Internal(DEMAND, &Fruit::demand);
        Internal(FRUIT_DUREE_OLEO, &Fruit::fruit_duree_oleo);
        Internal(FR_FRUITS, &Fruit::fr_fruits);
        Internal(OIL_ASSIMILATE_SUPPLY, &Fruit::oil_assimilate_supply);
        Internal(NONOIL_ASSIMILATE_SUPPLY, &Fruit::nonoil_assimilate_supply);
        Internal(OIL_BIOMASS, &Fruit::oil_biomass);
        Internal(NONOIL_BIOMASS, &Fruit::nonoil_biomass);
        Internal(TEST_OIL_BIOMASS, &Fruit::test_oil_biomass);
        Internal(OIL_POTENTIAL_BIOMASS, &Fruit::oil_potential_biomass);
        Internal(NONOIL_POTENTIAL_BIOMASS, &Fruit::nonoil_potential_biomass);
        Internal(TEST_NONOIL_BIOMASS, &Fruit::test_nonoil_biomass);
        Internal(MASSE_IND, &Fruit::masse_ind);
        Internal(RATIO_HUILE_MESOCARP, &Fruit::ratio_huile_mesocarp);
        Internal(MASSE_INI, &Fruit::masse_ini);
        Internal(TPS_PASSE_EN_OLEO, &Fruit::tps_passe_en_oleo);

        //          externals
        External(TEFF, &Fruit::Teff);
        External(TT_CORRIGE, &Fruit::TT_corrige);
        External(POT_STATUS, &Fruit::pot_statut);
        External(BUNCH_STATUS, &Fruit::bunch_statut);
        External(ABLATION_STATUS, &Fruit::ablation_statut);
        External(TT_SINCE_APP, &Fruit::thermalTimeSinceAppearance);
        External(PROD_SPEED, &Fruit::productionSpeed);
        External(ESTIM_HARVEST_DATE, &Fruit::estimated_harvest_date);
        External(ESTIM_FLO_DATE, &Fruit::estimated_flowering_date);
        External(FRUIT_NUMBER, &Fruit::fruit_number);
        External(INI_FLO_DATE, &Fruit::ini_flowering_date);
        External(INI_HARVEST_DATE, &Fruit::ini_harvest_date);
        External(FACTEUR_AGE_REGIMES, &Fruit::facteur_age_regimes);

    }

    virtual ~Fruit()
    {
    }


    double compute_oil_demand(double TT) {
        if (ablation_statut.is(bunch::ABLATED)) {
            demand = 0;
        } else {
            if (!bunch_statut.is(bunch::FLORAISON_RECOLTE) ) {
                demand = 0;
            }else {
                if (TT > estimated_harvest_date - fruit_duree_oleo)
                    demand = fruit_number *  OIL_CONTENT * COUT_OIL * masse_ind_max / (fruit_duree_oleo) * Teff;
                else
                    demand = 0;
            }
        }
        return demand;
    }

    oil::oil_state compute_oil_statut() { //TODO step state
        if (ablation_statut.is(bunch::ABLATED)) {
            return oil::PAS_DE_FRUITS;
        } else {
            if (bunch_statut.is(bunch::APPARITION_FLORAISON))
                return oil::PAS_DE_FRUITS;
            else {
                if (bunch_statut.is(bunch::RECOLTE))
                    return oil::RECOLTE;
                else {
                    if (TT_corrige > estimated_harvest_date - fruit_duree_oleo)
                        return oil::OLEOSYNTHESE;
                    else
                        return oil::AVANT_OLEOSYNTHESE;
                }
            }
        }
    }

    double compute_nonoil_demand(bunch::bunch_states statut) {
        if (ablation_statut.is(bunch::ABLATED))
            return 0;
        else {
            if (!statut.is(bunch::FLORAISON_RECOLTE))
                return 0;
            else
                return fruit_number * (1 - OIL_CONTENT) * COUT_STRUCTURE_REGIME * masse_ind_max / (estimated_harvest_date  - estimated_flowering_date) * Teff;
        }
    }


    void compute_ind_mass() {
        if (fruit_number != 0)
            masse_ind  = ((oil_biomass + nonoil_biomass) / fruit_number) * 1000;
        else
            masse_ind  = 0;

        if (oil_biomass == 0)
            ratio_huile_mesocarp = 0;
        else
            ratio_huile_mesocarp = oil_biomass / ( nonoil_biomass + oil_biomass);
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
        //default init
        last_time = t-1;

        //        parameters
        MINIMAL_PRODUCTION_SPEED = parameters.get("MINIMAL_PRODUCTION_SPEED");
        INITIAL_PRODUCTION_SPEED = parameters.get("INITIAL_PRODUCTION_SPEED");
        DUREE_OLEO = parameters.get("DUREE_OLEO");
        IND_FRUIT_WEIGHT = parameters.get("IND_FRUIT_WEIGHT");
        OIL_CONTENT = parameters.get("OIL_CONTENT");
        COUT_OIL = parameters.get("COUT_OIL");
        RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");
        MEAN_FRUIT_NUMBER_ADULTE = parameters.get("MEAN_FRUIT_NUMBER_ADULTE");

        //        attributes
        masse_ind_max = IND_FRUIT_WEIGHT / 1000;
        oil_statut = oil::UNKNOWN;
        oil_demand = 0;
        oil_demand_pot = 0;
        nonoil_demand = 0;
        nonoil_demand_pot = 0;
        demand = 0;
        fruit_duree_oleo = 0;
        oil_assimilate_supply = 0;
        nonoil_assimilate_supply = 0;
        oil_biomass = 0;
        nonoil_biomass = 0;
        test_oil_biomass = 0;
        oil_potential_biomass = 0;
        nonoil_potential_biomass = 0;
        test_nonoil_biomass = 0;
        masse_ind = 0;
        ratio_huile_mesocarp = 0;
        masse_ini = 0;
        tps_passe_en_oleo = 0;
    }


    void init_structure() {
        fruit_duree_oleo = DUREE_OLEO  * pow(MINIMAL_PRODUCTION_SPEED / INITIAL_PRODUCTION_SPEED, RATIO_DUREE_JEUNES_OLEO);
        if (thermalTimeSinceAppearance < ini_flowering_date) {
            oil_biomass = 0;
            nonoil_biomass = 0;
        } else {
            if (thermalTimeSinceAppearance > ini_harvest_date) {
                oil_biomass = 0;
                nonoil_biomass = 0;
            } else {
                nonoil_biomass = facteur_age_regimes * ( 1 - OIL_CONTENT ) * MEAN_FRUIT_NUMBER_ADULTE * (IND_FRUIT_WEIGHT/1000) / (ini_harvest_date - ini_flowering_date ) * (thermalTimeSinceAppearance - ini_flowering_date);
                if (thermalTimeSinceAppearance > ini_harvest_date - fruit_duree_oleo) {
                    masse_ini =  facteur_age_regimes *  MEAN_FRUIT_NUMBER_ADULTE * (IND_FRUIT_WEIGHT/1000) * OIL_CONTENT;
                    tps_passe_en_oleo =   thermalTimeSinceAppearance - (ini_harvest_date - fruit_duree_oleo);
                    oil_biomass =  masse_ini / fruit_duree_oleo * tps_passe_en_oleo;
                }
            }
        }
        oil_potential_biomass = oil_biomass;
        nonoil_potential_biomass = nonoil_biomass;
        compute_ind_mass();
    }



    void growth() {
        oil_assimilate_supply = oil_demand * (fr_fruits);
        nonoil_assimilate_supply = nonoil_demand_pot  * (fr_fruits);


        //oil_compute_biomass
        if (bunch_statut.is(bunch::RECOLTE)) {
            oil_biomass = 0;
            nonoil_biomass = 0;
        } else {
            oil_biomass += oil_assimilate_supply * (1 / COUT_OIL);
        }


        if (ablation_statut.is(bunch::ABLATED))
            oil_biomass = 0;

        test_oil_biomass +=  oil_assimilate_supply * (1 / COUT_OIL);
        oil_potential_biomass += oil_demand_pot * (1/COUT_OIL);


        //nonoil_compute_biomass
        if (bunch_statut.is(bunch::RECOLTE) ||  ablation_statut.is(bunch::ABLATED))
            nonoil_biomass = 0;
        else
            nonoil_biomass += nonoil_assimilate_supply * (1 / COUT_STRUCTURE_REGIME);

        nonoil_potential_biomass += nonoil_demand_pot * (1 / COUT_STRUCTURE_REGIME);
        test_nonoil_biomass +=  nonoil_assimilate_supply * (1 / COUT_STRUCTURE_REGIME);

        //compute_ind_mass
       compute_ind_mass();
    }

    void growth_demand(){
        fruit_duree_oleo = DUREE_OLEO * pow(MINIMAL_PRODUCTION_SPEED / productionSpeed, RATIO_DUREE_JEUNES_OLEO);
        oil_demand = compute_oil_demand(TT_corrige);
        oil_demand_pot = compute_oil_demand(thermalTimeSinceAppearance);
        oil_statut = compute_oil_statut();
        nonoil_demand = compute_nonoil_demand(bunch_statut);
        nonoil_demand_pot = compute_nonoil_demand(pot_statut);
        demand = oil_demand + nonoil_demand;
    }

    void compute(double t, bool /* update */)
    {
        last_time = t-1;
        growth();
        growth_demand();
    }

};

} // namespace model
#endif // FRUIT_H
