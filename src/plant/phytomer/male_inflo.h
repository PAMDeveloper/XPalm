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
    double INITIAL_PRODUCTION_SPEED;
    double MASSE_INFLO_MALE_ADULTE;
    double COUT_STRUCTURE_REGIME;

//     internals
    double biomass;
    double potential_biomass;
    double assimilate_supply;
    double demand;

//     externals
    double thermalTimeSinceAppearance;
//    int ini_flowering_date;
    int estimated_flowering_date;
    double facteur_age_regimes;
    double productionSpeed;// (bunch.phytomer.tree)
    double fr_reste; //bunch.phytomer.tree.fr_reste
    bunch::bunch_states bunch_status;
    double Teff;

public:
    MaleInflo()
    {
//         internals
        Internal(BIOMASS, &MaleInflo::biomass);
        Internal(POTENTIAL_BIOMASS, &MaleInflo::potential_biomass);
        Internal(ASSIMILATE_SUPPLY, &MaleInflo::assimilate_supply);
        Internal(DEMAND, &MaleInflo::demand);

//          externals
        External(TT_SINCE_APPEARANCE, &MaleInflo::thermalTimeSinceAppearance);
//        External(INI_FLO_DATE, &MaleInflo::ini_flowering_date);
        External(ESTIM_FLO_DATE, &MaleInflo::estimated_flowering_date);
        External(FACTEUR_AGE_REGIME, &MaleInflo::facteur_age_regimes);
        External(PROD_SPEED, &MaleInflo::productionSpeed);
        External(FR_RESTE, &MaleInflo::fr_reste);
        External(BUNCH_STATUS, &MaleInflo::bunch_status);
        External(TEFF, &MaleInflo::Teff);
    }

    virtual ~MaleInflo() {}


    void init_structure(double thermalTimeSinceAppearance, double ini_flowering_date, double facteur_age_regimes ) {
        if (thermalTimeSinceAppearance < ini_flowering_date - RANG_DEBUT_CROISSANCE_PEDUNCULE / productionSpeed)
            biomass = 0;
        else {
            if (thermalTimeSinceAppearance > ini_flowering_date)
                biomass = 0;
            else {
                if (thermalTimeSinceAppearance > ini_flowering_date)
                    biomass = MASSE_INFLO_MALE_ADULTE * facteur_age_regimes;
                else
                    biomass = MASSE_INFLO_MALE_ADULTE * facteur_age_regimes / ( RANG_DEBUT_CROISSANCE_PEDUNCULE / INITIAL_PRODUCTION_SPEED  ) *
                                     ( thermalTimeSinceAppearance - ( ini_flowering_date - RANG_DEBUT_CROISSANCE_PEDUNCULE / INITIAL_PRODUCTION_SPEED ) );
            }
        }
        potential_biomass= biomass;
    }



    void growth_demand() {
        if (!bunch_status.is(bunch::APPARITION_FLORAISON))
            demand = 0;
        else {
            if ( thermalTimeSinceAppearance > estimated_flowering_date - RANG_DEBUT_CROISSANCE_PEDUNCULE/productionSpeed )
                demand = MASSE_INFLO_MALE_ADULTE * facteur_age_regimes / (RANG_DEBUT_CROISSANCE_PEDUNCULE/productionSpeed) * Teff;
            else
                demand = 0;
        }
    }


    void growth() {
        assimilate_supply = demand   * fr_reste;
        if (bunch_status == bunch::FLORAISON_RECOLTE) {
            biomass = 0;
            potential_biomass = 0;
        } else {
            biomass += assimilate_supply * (1 / COUT_STRUCTURE_REGIME);
            potential_biomass += demand * (1 / COUT_STRUCTURE_REGIME);
        }
    }
    void init(double t, const xpalm::ModelParameters& parameters)
    {
        last_time = t-1;

//        parameters
        RANG_DEBUT_CROISSANCE_PEDUNCULE = parameters.get("RANG_DEBUT_CROISSANCE_PEDUNCULE");
        INITIAL_PRODUCTION_SPEED = parameters.get("INITIAL_PRODUCTION_SPEED");
        MASSE_INFLO_MALE_ADULTE = parameters.get("MASSE_INFLO_MALE_ADULTE");
        COUT_STRUCTURE_REGIME = parameters.get("COUT_STRUCTURE_REGIME");

//       internals
        biomass = 0;
        demand = 0;
        assimilate_supply = 0;
        potential_biomass = 0;

//        init_structure();//todo
    }

    void compute(double t, bool /* update */)
    {
        last_time = t-1;
        growth();
        growth_demand();
    }
};

} // namespace model

#endif // MALEINFLO_H
