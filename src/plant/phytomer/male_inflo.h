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
    enum internals { BIOMASS, POTENTIAL_BIOMASS, ASSIMILATE_SUPPLY, DEMAND };
    enum externals { TT_SINCE_APPEARANCE,
//                     INI_FLO_DATE,
                     ESTIM_FLO_DATE,
                     FACTEUR_AGE_REGIME,
                     PROD_SPEED,
                     FR_RESTE,
                     BUNCH_STATUS,
                     TEFF };

private:
//      parameters
    double RANG_DEBUT_CROISSANCE_PEDUNCULE;
    double PRODUCTION_SPEED_INITIAL;
    double MASSE_INFLO_MALE_ADULTE;
    double COUT_STRUCTURE_REGIME;

//     internals
    double biomass;
    double potential_biomass;
    double assimilate_supply;
    double demand;

    int TT_ini_flowering;
    double inflo_dev_factor;
    double production_speed;
    double TT_flowering_duration;

//     externals
    double TT_since_rank1;
    inflo::inflo_states inflo_status;
    double fr_reste;
    double Teff;

public:
    MaleInflo(double prod_speed, double flo_tt, double inflo_factor):
        TT_ini_flowering(flo_tt),
        inflo_dev_factor(inflo_factor),
        production_speed(prod_speed)
    {
//         internals
        Internal(BIOMASS, &MaleInflo::biomass);
        Internal(POTENTIAL_BIOMASS, &MaleInflo::potential_biomass);
        Internal(ASSIMILATE_SUPPLY, &MaleInflo::assimilate_supply);
        Internal(DEMAND, &MaleInflo::demand);

//          externals
        External(TT_SINCE_APPEARANCE, &MaleInflo::TT_since_rank1);
//        External(INI_FLO_DATE, &MaleInflo::ini_flowering_date);
        External(ESTIM_FLO_DATE, &MaleInflo::TT_ini_flowering);
        External(FACTEUR_AGE_REGIME, &MaleInflo::inflo_dev_factor);
        External(PROD_SPEED, &MaleInflo::production_speed);
        External(FR_RESTE, &MaleInflo::fr_reste);
        External(BUNCH_STATUS, &MaleInflo::inflo_status);
        External(TEFF, &MaleInflo::Teff);
    }

    virtual ~MaleInflo() {}


    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;

//        parameters
        RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        PRODUCTION_SPEED_INITIAL = parameters.get("PRODUCTION_SPEED_INITIAL");
        MASSE_INFLO_MALE_ADULTE = parameters.get("MASSE_INFLO_MALE_ADULTE");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");

//       internals
        biomass = 0;
        demand = 0;
        assimilate_supply = 0;
        potential_biomass = 0;

        TT_flowering_duration = RANG_DEBUT_CROISSANCE_PEDUNCULE / production_speed;
        double TEff_ini = parameters.get("T_EFF_INI");
        double TT_since_rank1 = TEff_ini * phytomer_age;

        //init structure
        if (TT_since_rank1 <= TT_ini_flowering) { // !inflo_status.is(inflo::FLOWERING)
            biomass = 0;
            demand = 0;
        } else {
            double fr_growth = min(1, (TT_ini_flowering - TT_since_rank1) / TT_flowering_duration );
            biomass = MASSE_INFLO_MALE_ADULTE * inflo_dev_factor * fr_growth / COUT_STRUCTURE_REGIME;
            demand = MASSE_INFLO_MALE_ADULTE * inflo_dev_factor * ( TEff_ini / TT_flowering_duration );
        }
        potential_biomass = biomass;
    }

    void compute(double t, bool /* update */)
    {
        last_time = t-1;

        demand = MASSE_INFLO_MALE_ADULTE * inflo_dev_factor * ( Teff / TT_flowering_duration );
        assimilate_supply = demand * fr_reste;
        biomass += assimilate_supply / COUT_STRUCTURE_REGIME;
        potential_biomass += demand / COUT_STRUCTURE_REGIME;
    }
};

} // namespace model

#endif // MALEINFLO_H
