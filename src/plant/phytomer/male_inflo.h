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
    enum internals { BIOMASS, POTENTIAL_BIOMASS, ASSIMILATE_SUPPLY, DEMAND, TT_FLO_DURATION, INFLO_DEV_FACTOR };

    enum externals { FR_RESTE,
                     TEFF,
                     TT_INI_FLOWERING,
                     TT_CORRIGE};

private:
    //      parameters
    double MASSE_INFLO_MALE_ADULTE;
    double COUT_STRUCTURE_REGIME;

    //     internals
    double biomass;
    double potential_biomass;
    double assimilate_supply;
    double demand;
    double TT_flowering_duration;
    double inflo_dev_factor;


    //     externals
    double fr_reste;
    double Teff;
    double TT_ini_flowering;
//    double TT_since_appearance;
    double TT_corrige;


public:
    MaleInflo()
    {
        //         internals
        Internal(BIOMASS, &MaleInflo::biomass);
        Internal(POTENTIAL_BIOMASS, &MaleInflo::potential_biomass);
        Internal(ASSIMILATE_SUPPLY, &MaleInflo::assimilate_supply);
        Internal(DEMAND, &MaleInflo::demand);
        Internal(TT_FLO_DURATION, &MaleInflo::TT_flowering_duration);
        Internal(INFLO_DEV_FACTOR, &MaleInflo::inflo_dev_factor);

        //          externals
        External(FR_RESTE, &MaleInflo::fr_reste);
        External(TEFF, &MaleInflo::Teff);
        External(TT_CORRIGE, &MaleInflo::TT_corrige);
        External (TT_INI_FLOWERING, &MaleInflo::TT_ini_flowering);
//        External(TT_SINCE_APPEARANCE, &MaleInflo::TT_since_appearance);
    }

    virtual ~MaleInflo() {}

    void init(double t, const xpalm::ModelParameters& parameters) {}
    void init(double t, const xpalm::ModelParameters& parameters, double phytomer_age, double inflo_dev_factor_, double TT_ini_flowering_, double TT_ini_male_senescence)
    {
        //        AtomicModel<MaleInflo>::init(t, parameters);

        last_time = t-1;

        //        parameters
        MASSE_INFLO_MALE_ADULTE = parameters.get("MASSE_INFLO_MALE_ADULTE");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");

        //       internals
        biomass = 0;
        demand = 0;
        assimilate_supply = 0;
        potential_biomass = 0;
        inflo_dev_factor = inflo_dev_factor_;
        TT_ini_flowering = TT_ini_flowering_;

        //        double RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        //        TT_flowering_duration = RANG_DEBUT_CROISSANCE_PEDUNCULE / production_speed;

        TT_flowering_duration=(TT_ini_male_senescence-TT_ini_flowering);

        double TEff_ini = parameters.get("T_EFF_INI");
        double TT_corrige = TEff_ini * phytomer_age;

        //init structure
        if (TT_corrige <= TT_ini_flowering) { // !inflo_status.is(inflo::FLOWERING)
            biomass = 0;
            demand = 0;
        } else {
            double fr_growth = min(1.0, (TT_ini_flowering - TT_corrige) / TT_flowering_duration );
            biomass = MASSE_INFLO_MALE_ADULTE * inflo_dev_factor * fr_growth / COUT_STRUCTURE_REGIME;
            demand = MASSE_INFLO_MALE_ADULTE * COUT_STRUCTURE_REGIME * inflo_dev_factor * ( TEff_ini / TT_flowering_duration );
        }
        potential_biomass = biomass;
    }

    void compute(double t, bool /* update */)
    {

        if (TT_corrige > TT_ini_flowering){
            biomass += assimilate_supply / COUT_STRUCTURE_REGIME;
            potential_biomass += demand / COUT_STRUCTURE_REGIME;

            demand = MASSE_INFLO_MALE_ADULTE * COUT_STRUCTURE_REGIME * inflo_dev_factor * ( Teff / TT_flowering_duration );
            assimilate_supply = demand * fr_reste;
        }
        else{
            biomass = 0;
            demand = 0;
        }

    }
};

} // namespace model

#endif // MALEINFLO_H
