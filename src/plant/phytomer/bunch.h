#ifndef FRUIT_H
#define FRUIT_H
#include <defines.hpp>

namespace model {

class Bunch : public AtomicModel < Bunch >
{
public:
    enum internals { MASSE_IND_MAX,
                     TT_OLEO_DURATION,
                     TT_BUNCH_DEV_DURATION,
                     POT_FRUITS_NUMBER,
                     TT_INI_HARVEST,
                     TT_INI_FLOWERING,
                     TT_INI_OLEO,
                     DEMAND,
                     OIL_DEMAND,
                     NONOIL_DEMAND,
//                     NONOIL_DEMAND_POT,
                     OIL_ASSIMILATE_SUPPLY,
                     NONOIL_ASSIMILATE_SUPPLY,
                     OIL_BIOMASS,
                     OIL_BIOMASS_HARVESTED,
                     NONOIL_BIOMASS,
                     NONOIL_BIOMASS_HARVESTED,
                     FINAL_OIL_MASS,
                     MASSE_IND,
                     RATIO_HUILE_MESOCARP,
                     FRUIT_NUMBER};

    enum externals { TEFF,
                     INFLO_STATUS_POT,
                     INFLO_STATUS,
                     TT_SINCE_APPEARANCE,
                     TT_CORRIGE,
                     FR_FRUITS,
                     IC_SPIKELET,
                     IC_SETTING,
                     RANK};

private:
    //      parameters
    double OIL_CONTENT;
    double COUT_OIL;
    double REPRO_CONSTRUCTION_COST;
    double SENSIVITY_IC_SPIKELET;
    //    double IC_spikelet_RANG_FIN;
    double SENSIVITY_IC_SETTING;
    double PERIOD_FRUIT_SET;

    //     internals
    //predim
    double masse_ind_max;
    double TT_oleo_duration;
    double TT_bunch_dev_duration;
    double pot_fruits_number;
    double TT_ini_harvest;
    double TT_ini_flowering;
    double TT_ini_oleo;

    //var
    double demand;
    double oil_demand;
    double nonoil_demand;
    //    double nonoil_demand_pot;
    double oil_assimilate_supply;
    double nonoil_assimilate_supply;
    double oil_biomass;
    double oil_biomass_harvested;
    double nonoil_biomass;
    double nonoil_biomass_harvested;
    double final_oil_mass;
    double masse_ind;
    double ratio_huile_mesocarp;
    int fruit_number;

    //     externals
    inflo::inflo_states inflo_status;
    inflo::inflo_states inflo_status_pot;
    double Teff;
    double TT_since_appearance;
    double TT_corrige;
    double fr_fruits;
    double IC_spikelet;
    double IC_setting;
    double rank;

public:

    Bunch()
    {
        //         internals
        Internal(MASSE_IND_MAX, &Bunch::masse_ind_max);
        Internal(TT_OLEO_DURATION, &Bunch::TT_oleo_duration);
        Internal(TT_BUNCH_DEV_DURATION, &Bunch::TT_bunch_dev_duration);
        Internal(POT_FRUITS_NUMBER, &Bunch::pot_fruits_number);
        Internal(TT_INI_HARVEST, &Bunch::TT_ini_harvest);
        Internal(TT_INI_FLOWERING, &Bunch::TT_ini_flowering);
        Internal(TT_INI_OLEO, &Bunch::TT_ini_oleo);
        Internal(DEMAND, &Bunch::demand);
        Internal(OIL_DEMAND, &Bunch::oil_demand);
        Internal(NONOIL_DEMAND, &Bunch::nonoil_demand);
        //        Internal(NONOIL_DEMAND_POT, &Bunch::nonoil_demand_pot);
        Internal(OIL_ASSIMILATE_SUPPLY, &Bunch::oil_assimilate_supply);
        Internal(NONOIL_ASSIMILATE_SUPPLY, &Bunch::nonoil_assimilate_supply);
        Internal(OIL_BIOMASS, &Bunch::oil_biomass);
        Internal(OIL_BIOMASS_HARVESTED, &Bunch::oil_biomass_harvested);
        Internal(NONOIL_BIOMASS, &Bunch::nonoil_biomass);
        Internal(NONOIL_BIOMASS_HARVESTED, &Bunch::nonoil_biomass_harvested);
        Internal(FINAL_OIL_MASS, &Bunch::final_oil_mass);
        Internal(MASSE_IND, &Bunch::masse_ind);
        Internal(RATIO_HUILE_MESOCARP, &Bunch::ratio_huile_mesocarp);
        Internal(FRUIT_NUMBER, &Bunch::fruit_number);


        //          externals
        External(TEFF, &Bunch::Teff);
        External(INFLO_STATUS_POT, &Bunch::inflo_status_pot);
        External(INFLO_STATUS, &Bunch::inflo_status);
        External(TT_SINCE_APPEARANCE, &Bunch::TT_since_appearance);
        External(TT_CORRIGE, &Bunch::TT_corrige);
        External(FR_FRUITS, &Bunch::fr_fruits);
        External(IC_SPIKELET, &Bunch::IC_spikelet);
        External(IC_SETTING, &Bunch::IC_setting);
        External(RANK, &Bunch::rank);



    }

    virtual ~Bunch()
    {
    }
    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double production_speed, double flo_tt, double harv_tt, double TT_ini_oleo_, double inflo_dev_factor)
    {
        //        AtomicModel<Bunch>::init(t, parameters);

        //default init
        last_time = t;

        //        parameters
        OIL_CONTENT = parameters.get("OIL_CONTENT");
        COUT_OIL = parameters.get("COUT_OIL");
        REPRO_CONSTRUCTION_COST = parameters.get("REPRO_CONSTRUCTION_COST");
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        //        IC_spikelet_RANG_FIN = parameters.get("IC_spikelet_RANG_FIN");
        SENSIVITY_IC_SETTING = parameters.get("SENSIVITY_IC_SETTING");
        PERIOD_FRUIT_SET= parameters.get("PERIOD_FRUIT_SET");

        TT_ini_harvest = harv_tt;
        TT_ini_flowering = flo_tt;
        TT_ini_oleo = TT_ini_oleo_;

        double IND_FRUIT_WEIGHT = parameters.get("IND_FRUIT_WEIGHT");
        double MEAN_FRUIT_NUMBER_ADULTE = parameters.get("MEAN_FRUIT_NUMBER_ADULTE");
        masse_ind_max = IND_FRUIT_WEIGHT / 1000;
        pot_fruits_number = int (inflo_dev_factor * MEAN_FRUIT_NUMBER_ADULTE);

        //        double RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        //        double PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        //        double DUREE_OLEO = parameters.get("DUREE_OLEO");
        //        TT_oleo_duration = DUREE_OLEO * pow(PRODUCTION_SPEED_ADULT / production_speed, RATIO_DUREE_JEUNES_OLEO);

        TT_oleo_duration = TT_ini_harvest - TT_ini_oleo;
        TT_bunch_dev_duration = TT_ini_harvest - TT_ini_flowering;

        //        oil_statut = oil::UNKNOWN;
        fruit_number=0;
        oil_assimilate_supply = 0;
        nonoil_assimilate_supply = 0;
        //        nonoil_demand_pot=0;
        oil_biomass = 0;
        oil_biomass_harvested = 0;
        nonoil_biomass = 0;
        nonoil_biomass_harvested = 0;
        masse_ind = 0;
        ratio_huile_mesocarp = 0;
        final_oil_mass=0;
        oil_demand = 0;
        nonoil_demand = 0;
        demand = 0;


        if (inflo_status.is(inflo::NON_ABORTED)){
            if (TT_corrige>=TT_ini_flowering)
                fruit_number = min (1.0, IC_spikelet) * min (1.0, IC_setting) * pot_fruits_number; //TODO change 1.0 in 1+x% of potential increase du to plasticity
            //            fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pot_fruits_number;
        }
        //        else
        //            fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pow(IC_setting, SENSIVITY_IC_SETTING) * pot_fruits_number;


        if (inflo_status.is(inflo::FLOWERING) | inflo_status.is(inflo::OLEOSYNTHESIS)) {
            double fr_bunch_dev = min (1.0 , (TT_since_appearance - TT_ini_flowering) / TT_bunch_dev_duration);
            nonoil_biomass = fruit_number * masse_ind_max * (1 - OIL_CONTENT ) * fr_bunch_dev;
            nonoil_demand = fruit_number * masse_ind_max * (1 - OIL_CONTENT ) * REPRO_CONSTRUCTION_COST  * ( Teff / TT_bunch_dev_duration );

            if(inflo_status.is(inflo::OLEOSYNTHESIS)) {
                double fr_oleo = min (1.0, (TT_since_appearance - TT_ini_oleo) / TT_oleo_duration);
                final_oil_mass = fruit_number * masse_ind_max * OIL_CONTENT;
                oil_biomass =  final_oil_mass * fr_oleo;
                oil_demand = final_oil_mass * COUT_OIL  * ( Teff / TT_oleo_duration );
            }

        }

        if (inflo_status.is(inflo::HARVEST)){
            oil_biomass_harvested=oil_biomass;
            nonoil_biomass_harvested=nonoil_biomass;
            oil_biomass=0;
            nonoil_biomass=0;
        }

    }


    void compute(double t, bool /* update */)
    {
        //        growth();
        //compute oil and nonoil biomass

        nonoil_assimilate_supply = nonoil_demand * fr_fruits;
        nonoil_biomass += nonoil_assimilate_supply / REPRO_CONSTRUCTION_COST;
        oil_assimilate_supply = oil_demand * fr_fruits;
        oil_biomass += oil_assimilate_supply / COUT_OIL;

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

        if (TT_corrige>=TT_ini_flowering)
            //            fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pow(IC_setting, SENSIVITY_IC_SETTING) * pot_fruits_number;
            fruit_number = min (1.0, IC_spikelet) * min (1.0, IC_setting) * pot_fruits_number;


        if (inflo_status.is(inflo::FLOWERING) | inflo_status.is(inflo::OLEOSYNTHESIS)) {
            double fr_bunch_dev = min (1.0 , (TT_corrige - TT_ini_flowering) / TT_bunch_dev_duration);
            nonoil_biomass = fruit_number * masse_ind_max * (1 - OIL_CONTENT ) * fr_bunch_dev;
            nonoil_demand = fruit_number * masse_ind_max * (1 - OIL_CONTENT ) * REPRO_CONSTRUCTION_COST  * ( Teff / TT_bunch_dev_duration );

            if(inflo_status.is(inflo::OLEOSYNTHESIS)) {
                double fr_oleo = min (1.0, (TT_corrige - TT_ini_oleo) / TT_oleo_duration);
                final_oil_mass = fruit_number * masse_ind_max * OIL_CONTENT;
                oil_biomass =  final_oil_mass * fr_oleo;
                oil_demand = final_oil_mass * COUT_OIL  * ( Teff / TT_oleo_duration );
            }

        }
        else  if (inflo_status.is(inflo::HARVEST)){
            oil_biomass_harvested=oil_biomass;
            nonoil_biomass_harvested=nonoil_biomass;
            oil_biomass=0;
            nonoil_biomass=0;
            oil_demand=0;
            nonoil_demand=0;
        }
        else{
            oil_demand=0;
        }

        demand = oil_demand + nonoil_demand;

    }

};

} // namespace model
#endif // FRUIT_H
