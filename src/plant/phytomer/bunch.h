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
                     ESTIM_HARVEST_DATE, ESTIM_FLO_DATE, FRUIT_NUMBER, FR_FRUITS, INI_FLO_DATE, INI_HARVEST_DATE, BUNCH_DEV_FACTOR};

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
    double final_oil_mass;
    double ratio_huile_mesocarp;
    double TT_oleo_duration;

//    double tps_passe_en_oleo;
//    double oil_demand_pot;
    double nonoil_demand_pot;
//    double oil_potential_biomass;
//    double nonoil_potential_biomass;

    //     externals
    inflo::inflo_states inflo_status_pot;
    inflo::inflo_states inflo_status;
//    inflo::inflo_states ablation_statut;

    double Teff;
    double TT_corrige;
    double TT_since_rank1;
    int fruit_number;
    double fr_fruits;


    double production_speed;
    double TT_ini_harvest;
    double TT_ini_flowering;
    double inflo_dev_factor;

public:

    Bunch(double prod_speed, double flo_tt, double harv_tt, double inflo_factor):
        TT_ini_flowering(flo_tt),
        TT_ini_harvest(harv_tt),
        inflo_dev_factor(inflo_factor),
        production_speed(prod_speed)
    {
        //         internals
        Internal(OIL_DEMAND, &Bunch::oil_demand);
//        Internal(OIL_POT_DEMAND, &Bunch::oil_demand_pot);
        Internal(OIL_STATUS, &Bunch::oil_statut);
        Internal(NONOIL_DEMAND, &Bunch::nonoil_demand);
        Internal(DEMAND, &Bunch::demand);
        Internal(FRUIT_DUREE_OLEO, &Bunch::TT_oleo_duration);
        Internal(FR_FRUITS, &Bunch::fr_fruits);
        Internal(OIL_ASSIMILATE_SUPPLY, &Bunch::oil_assimilate_supply);
        Internal(NONOIL_ASSIMILATE_SUPPLY, &Bunch::nonoil_assimilate_supply);
        Internal(OIL_BIOMASS, &Bunch::oil_biomass);
        Internal(NONOIL_BIOMASS, &Bunch::nonoil_biomass);
        Internal(MASSE_IND, &Bunch::masse_ind);
        Internal(RATIO_HUILE_MESOCARP, &Bunch::ratio_huile_mesocarp);
        Internal(MASSE_INI, &Bunch::final_oil_mass);


        //          externals
        External(TEFF, &Bunch::Teff);
        External(TT_CORRIGE, &Bunch::TT_corrige);
        External(POT_STATUS, &Bunch::inflo_status_pot);
        External(BUNCH_STATUS, &Bunch::inflo_status);
        External(ABLATION_STATUS, &Bunch::ablation_statut);
        External(TT_SINCE_APP, &Bunch::TT_since_rank1);
        External(FRUIT_NUMBER, &Bunch::fruit_number);



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
        pot_fruits_number = inflo_dev_factor * MEAN_FRUIT_NUMBER_ADULTE;

        TT_oleo_duration = DUREE_OLEO * pow(PRODUCTION_SPEED_ADULT / production_speed, RATIO_DUREE_JEUNES_OLEO);

//        oil_statut = oil::UNKNOWN;
        oil_assimilate_supply = 0;
        nonoil_assimilate_supply = 0;
        oil_biomass = 0;
        nonoil_biomass = 0;
        masse_ind = 0;
        ratio_huile_mesocarp = 0;

        if (!inflo_status.is(inflo::FLOWERING)) { //voire si etat dispo
            oil_biomass = 0;
            nonoil_biomass = 0;
            oil_demand = 0;
            nonoil_demand = 0;
            demand = 0;
        } else {
            double total_final_biomass = inflo_dev_factor *  MEAN_FRUIT_NUMBER_ADULTE * (IND_FRUIT_WEIGHT/1000);
            double TT_bunch_dev_duration = TT_ini_harvest - TT_ini_flowering;
            double fr_bunch_dev = (TT_since_rank1 - TT_ini_flowering) / TT_bunch_dev_duration;
            nonoil_biomass = total_final_biomass * (1 - OIL_CONTENT ) * fr_bunch_dev;

            if(inflo_status.is(inflo::OLEOSYNTHESIS)) {
//            if (TT_since_rank1 > TT_ini_oleo) {
                double fr_oleo = (TT_since_rank1 - TT_ini_oleo) / TT_oleo_duration;
                final_oil_mass = total_final_biomass * OIL_CONTENT;
                oil_biomass =  final_oil_mass * fr_oleo;
            }
        }

    }


    void compute(double t, bool /* update */)
    {
        last_time = t-1;

//        growth();
        //compute oil and nonoil biomass
        oil_assimilate_supply = oil_demand * fr_fruits;
        nonoil_assimilate_supply = nonoil_demand_pot * fr_fruits;

        oil_biomass += oil_assimilate_supply / COUT_OIL;
        nonoil_biomass += nonoil_assimilate_supply / COUT_STRUCTURE_REGIME;

        //compute ind mass
        if (fruit_number != 0)
            masse_ind  = ((oil_biomass + nonoil_biomass) / fruit_number) * 1000; //en grammes
        else
            masse_ind  = 0;

        if (oil_biomass == 0)
            ratio_huile_mesocarp = 0;
        else
            ratio_huile_mesocarp = oil_biomass / (nonoil_biomass + oil_biomass);


        //        growth_demand();
        oil_demand = Teff * fruit_number * OIL_CONTENT * COUT_OIL * MASSE_IND_MAX / TT_oleo_duration ;
        nonoil_demand =  Teff * fruit_number * (1 - OIL_CONTENT) * COUT_STRUCTURE_REGIME * MASSE_IND_MAX / (TT_ini_harvest - TT_ini_flowering);
        demand = oil_demand + nonoil_demand;

        if (rank > IC_spikelet_RANG_FIN) {
            if (status.is(inflo::INITIATED))
                fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pot_fruits_number;
            else
                fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pow(IC_setting, SENSIVITY_IC_SETTING) * pot_fruits_number;
        }
    }

};

} // namespace model
#endif // FRUIT_H
