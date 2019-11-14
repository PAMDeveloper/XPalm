#ifndef FRUIT_H
#define FRUIT_H
#include <defines.hpp>

namespace model {

class Bunch : public AtomicModel < Bunch >
{
public:
    enum internals { MASSE_IND_MAX,
                     TT_OLEO_DURATION,
                     POT_FRUITS_NUMBER,
                     TT_INI_HARVEST,
                     TT_INI_FLOWERING,
                     DEMAND,
                     OIL_DEMAND,
                     NONOIL_DEMAND,
                     NONOIL_DEMAND_POT,
                     OIL_ASSIMILATE_SUPPLY,
                     NONOIL_ASSIMILATE_SUPPLY,
                     OIL_BIOMASS,
                     NONOIL_BIOMASS,
                     FINAL_OIL_MASS,
                     MASSE_IND,
                     RATIO_HUILE_MESOCARP,
                     FRUIT_NUMBER};

    enum externals { TEFF, INFLO_STATUS_POT, INFLO_STATUS, TT_SINCE_RANK1,
                     FR_FRUITS, IC_SPIKELET, IC_SETTING, RANK};

private:
    //      parameters
    double OIL_CONTENT;
    double COUT_OIL;
    double COUT_STRUCTURE_REGIME;
    double SENSIVITY_IC_SPIKELET;
    double IC_spikelet_RANG_FIN;
    double SENSIVITY_IC_SETTING;

    //     internals
    //predim
    double masse_ind_max;
    double TT_oleo_duration;
    double pot_fruits_number;
    double TT_ini_harvest;
    double TT_ini_flowering;
    //var
    double demand;
    double oil_demand;
    double nonoil_demand;
    double nonoil_demand_pot;
    double oil_assimilate_supply;
    double nonoil_assimilate_supply;
    double oil_biomass;
    double nonoil_biomass;
    double final_oil_mass;
    double masse_ind;
    double ratio_huile_mesocarp;
    int fruit_number;

    //     externals
    inflo::inflo_states inflo_status;
    inflo::inflo_states inflo_status_pot;
    double Teff;
    double TT_since_rank1;
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
        Internal(POT_FRUITS_NUMBER, &Bunch::pot_fruits_number);
        Internal(TT_INI_HARVEST, &Bunch::TT_ini_harvest);
        Internal(TT_INI_FLOWERING, &Bunch::TT_ini_flowering);
        Internal(DEMAND, &Bunch::demand);
        Internal(OIL_DEMAND, &Bunch::oil_demand);
        Internal(NONOIL_DEMAND, &Bunch::nonoil_demand);
        Internal(NONOIL_DEMAND_POT, &Bunch::nonoil_demand_pot);
        Internal(OIL_ASSIMILATE_SUPPLY, &Bunch::oil_assimilate_supply);
        Internal(NONOIL_ASSIMILATE_SUPPLY, &Bunch::nonoil_assimilate_supply);
        Internal(OIL_BIOMASS, &Bunch::oil_biomass);
        Internal(NONOIL_BIOMASS, &Bunch::nonoil_biomass);
        Internal(FINAL_OIL_MASS, &Bunch::final_oil_mass);
        Internal(MASSE_IND, &Bunch::masse_ind);
        Internal(RATIO_HUILE_MESOCARP, &Bunch::ratio_huile_mesocarp);
        Internal(FRUIT_NUMBER, &Bunch::fruit_number);


        //          externals
        External(TEFF, &Bunch::Teff);
        External(INFLO_STATUS_POT, &Bunch::inflo_status_pot);
        External(INFLO_STATUS, &Bunch::inflo_status);
        External(TT_SINCE_RANK1, &Bunch::TT_since_rank1);
        External(FR_FRUITS, &Bunch::fr_fruits);
        External(IC_SPIKELET, &Bunch::IC_spikelet);
        External(IC_SETTING, &Bunch::IC_setting);
        External(RANK, &Bunch::rank);



    }

    virtual ~Bunch()
    {
    }
    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double production_speed, double flo_tt, double harv_tt, double TT_ini_oleo, double inflo_dev_factor)
    {
//        AtomicModel<Bunch>::init(t, parameters);

        //default init
        last_time = t-1;

        //        parameters
        OIL_CONTENT = parameters.get("OIL_CONTENT");
        COUT_OIL = parameters.get("COUT_OIL");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");
        SENSIVITY_IC_SPIKELET = parameters.get("SENSIVITY_IC_SPIKELET");
        IC_spikelet_RANG_FIN = parameters.get("IC_spikelet_RANG_FIN");
        SENSIVITY_IC_SETTING = parameters.get("SENSIVITY_IC_SETTING");

        TT_ini_harvest = harv_tt;
        TT_ini_flowering = flo_tt;

        double IND_FRUIT_WEIGHT = parameters.get("IND_FRUIT_WEIGHT");
        double MEAN_FRUIT_NUMBER_ADULTE = parameters.get("MEAN_FRUIT_NUMBER_ADULTE");
        masse_ind_max = IND_FRUIT_WEIGHT / 1000;
        pot_fruits_number = inflo_dev_factor * MEAN_FRUIT_NUMBER_ADULTE;

        double RATIO_DUREE_JEUNES_OLEO = parameters.get("RATIO_DUREE_JEUNES_OLEO");
        double PRODUCTION_SPEED_ADULT = parameters.get("PRODUCTION_SPEED_ADULT");
        double DUREE_OLEO = parameters.get("DUREE_OLEO");
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
        oil_demand = Teff * fruit_number * OIL_CONTENT * COUT_OIL * masse_ind_max / TT_oleo_duration ;
        nonoil_demand =  Teff * fruit_number * (1 - OIL_CONTENT) * COUT_STRUCTURE_REGIME * masse_ind_max / (TT_ini_harvest - TT_ini_flowering);
        demand = oil_demand + nonoil_demand;

        if (rank > IC_spikelet_RANG_FIN) {
            if (inflo_status.is(inflo::INITIATED))
                fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pot_fruits_number;
            else
                fruit_number = pow(IC_spikelet, SENSIVITY_IC_SPIKELET) * pow(IC_setting, SENSIVITY_IC_SETTING) * pot_fruits_number;
        }
    }

};

} // namespace model
#endif // FRUIT_H
