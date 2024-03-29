#ifndef MALEINFLO_H
#define MALEINFLO_H


/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>

namespace model {

class MaleInflo : public AtomicModel < MaleInflo >
{
public:
    enum internals { BIOMASS_MALE_INFLO,
                     BIOMASS_MALE_INFLO_HARVESTED,
                     POTENTIAL_BIOMASS,
                     ASSIMILATE_SUPPLY,
                     MALE_INFLO_DEMAND,
                     TT_FLO_DURATION,
                     INFLO_DEV_FACTOR };

    enum externals { FR_RESTE,
                     TEFF,
                     //                     TT_INI_FLOWERING,
                     TT_SINCE_APPEARANCE,
                     INFLO_STATUS,
//                     RANK
                   };

private:
    //      parameters
    double MALE_INFLO_FINAL_BIOMASS;
    double CONSTRUCTION_COST_INFLO;
    double TEFF_INI;

    //     internals
    double biomass;
    double biomass_harvested;
    double potential_biomass;
    double assimilate_supply;
    double demand;
    double TT_flowering_duration;
    double inflo_dev_factor;


    //     externals
    inflo::inflo_states inflo_status;
    double fr_reste;
    double Teff;
    double TT_ini_flowering;
    double TT_since_appearance;
//    double rank;
    //    double TT_corrige;



public:
    MaleInflo()
    {
        //         internals
        Internal(BIOMASS_MALE_INFLO, &MaleInflo::biomass);
        Internal(BIOMASS_MALE_INFLO_HARVESTED, &MaleInflo::biomass_harvested);
        Internal(POTENTIAL_BIOMASS, &MaleInflo::potential_biomass);
        Internal(ASSIMILATE_SUPPLY, &MaleInflo::assimilate_supply);
        Internal(MALE_INFLO_DEMAND, &MaleInflo::demand);
        Internal(TT_FLO_DURATION, &MaleInflo::TT_flowering_duration);
        Internal(INFLO_DEV_FACTOR, &MaleInflo::inflo_dev_factor);

        //          externals
        External(INFLO_STATUS, &MaleInflo::inflo_status);
        External(FR_RESTE, &MaleInflo::fr_reste);
        External(TEFF, &MaleInflo::Teff);
        //        External(TT_CORRIGE, &MaleInflo::TT_corrige);
        //        External (TT_INI_FLOWERING, &MaleInflo::TT_ini_flowering);
        External(TT_SINCE_APPEARANCE, &MaleInflo::TT_since_appearance);
//        External(RANK, &MaleInflo::rank);
    }

    virtual ~MaleInflo() {}

    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double TT_since_appearance_, double inflo_dev_factor_, double TT_ini_flowering_, double TT_ini_male_senescence)
    {
        //        AtomicModel<MaleInflo>::init(t, parameters);

        last_time = t;

        //        parameters
        MALE_INFLO_FINAL_BIOMASS = parameters.get("MALE_INFLO_FINAL_BIOMASS");
        CONSTRUCTION_COST_INFLO = parameters.get("CONSTRUCTION_COST_INFLO");
        TEFF_INI = parameters.get("T_EFF_INI");
        //       internals
        biomass = 0;
        biomass_harvested=0;
        demand = 0;
        assimilate_supply = 0;
        potential_biomass = 0;
        inflo_dev_factor = inflo_dev_factor_;
        TT_ini_flowering = TT_ini_flowering_;
        TT_since_appearance=TT_since_appearance_;
        TT_flowering_duration=(TT_ini_male_senescence-TT_ini_flowering);

        //        double RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        //        TT_flowering_duration = RANG_DEBUT_CROISSANCE_PEDUNCULE / production_speed;




        //        double TT_since_appearan = TEff_ini * phytomer_age;

        //init structure
        if (inflo_status.is(inflo::FLOWERING) ) {
            double fr_growth = min(1.0, (TT_since_appearance - TT_ini_flowering) / TT_flowering_duration );
            biomass = MALE_INFLO_FINAL_BIOMASS * inflo_dev_factor * fr_growth;
            demand = MALE_INFLO_FINAL_BIOMASS * CONSTRUCTION_COST_INFLO * inflo_dev_factor * ( TEFF_INI / TT_flowering_duration );
        }

        if (inflo_status.is(inflo::SENESCENCE) ) {
            biomass_harvested=biomass;
            biomass=0;
        }

        potential_biomass = biomass;
    }

    void compute(double t, bool /* update */)
    {


        // growth();
        assimilate_supply = demand * fr_reste;
        biomass += assimilate_supply / CONSTRUCTION_COST_INFLO;
        potential_biomass += demand/ CONSTRUCTION_COST_INFLO;
        demand=0;

        if (inflo_status.is(inflo::FLOWERING)){
            demand = MALE_INFLO_FINAL_BIOMASS * CONSTRUCTION_COST_INFLO * inflo_dev_factor * ( Teff / TT_flowering_duration );

        }
        else if (inflo_status.is(inflo::SENESCENCE)){
            biomass_harvested=biomass;
            biomass = 0;
        }
    }
};

} // namespace model

#endif // MALEINFLO_H
