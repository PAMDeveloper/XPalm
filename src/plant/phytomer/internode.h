#ifndef INTERNODE_H
#define INTERNODE_H
/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>

namespace model {

class Internode : public AtomicModel < Internode >
{
public:
    enum internals { LENGTH_INCREASE_POT,
                     LENGTH,
                     BIOMASS,
                     DEMAND,
                     MAX_LENGTH_POT,
                     ASSIMILATE_SUPPLY,
                     DUREE_ALLONGEMENT,
                     TT_CORRIGE,
                     GAIN_TEFF_JOUR };

    enum externals { TEFF,
                     FR_RESTE };

private:
    xpalm::ModelParameters _parameters;

    //      parameters
    double STEM_RAYON;
    double STEM_APPARENT_DENSITY;
    double COUT_RESPI_INTERNODE;
    double PLASTICITY_INTERNODE_IC;

    //     internals
    double length_increase_pot;
    double length;
    double biomass;
    double demand;
    double max_length_pot;
    double assimilate_supply;
    double duree_allongement;
    double TT_corrige;
    double gain_TEff_jour;

    //     externals
    double TEff;
    double fr_reste;


public:

    Internode()
    {
//        //         internals
        Internal(LENGTH_INCREASE_POT, &Internode::length_increase_pot);
        Internal(LENGTH, &Internode::length);
        Internal(BIOMASS, &Internode::biomass);
        Internal(DEMAND, &Internode::demand);
        Internal(MAX_LENGTH_POT, &Internode::max_length_pot);
        Internal(ASSIMILATE_SUPPLY, &Internode::assimilate_supply);
        Internal(DUREE_ALLONGEMENT, &Internode::duree_allongement);
        Internal(TT_CORRIGE, &Internode::TT_corrige);
        Internal(GAIN_TEFF_JOUR, &Internode::gain_TEff_jour);


//        //          externals
        External(TEFF, &Internode::TEff);
        External(FR_RESTE, &Internode::fr_reste);

    }

    virtual ~Internode()
    {
    }

    void init(double t, const xpalm::ModelParameters& parameters){}
    void init(double t, const xpalm::ModelParameters& parameters, double phytomer_age, double tree_age_at_phyto_creation, double production_speed)
    {
//        AtomicModel<Internode>::init(t, parameters);

        //default init
        last_time = t;
        _parameters = parameters;

        //        parameters
        COUT_RESPI_INTERNODE = parameters.get("COUT_RESPI_INTERNODE");
        PLASTICITY_INTERNODE_IC = parameters.get("PLASTICITY_INTERNODE_IC");
        STEM_RAYON = parameters.get("STEM_RAYON"); //cm
        STEM_APPARENT_DENSITY = parameters.get("STEM_APPARENT_DENSITY"); //(g cm-3)

        //        internals
        length_increase_pot = 0;
        demand = 0;
        length = 0;
        biomass = 0;
        assimilate_supply = 0;
        TT_corrige = 0;
        gain_TEff_jour = 0;

        // ini
        double DEBUT_CROISSANCE_EN = parameters.get("DEBUT_CROISSANCE_EN") * 365;
        double FIN_CROISSANCE_EN = parameters.get("FIN_CROISSANCE_EN") * 365;
        double EN_LENGTH_ADULTE = parameters.get("EN_LENGTH_ADULTE") ; //cm
        double EN_LENGTH_INI = parameters.get("EN_LENGTH_INI"); //cm
        max_length_pot = age_relative_var(tree_age_at_phyto_creation + phytomer_age, DEBUT_CROISSANCE_EN, FIN_CROISSANCE_EN, EN_LENGTH_INI, EN_LENGTH_ADULTE); // cm

        duree_allongement = 1 / production_speed;

        // init structure
        double TEff_ini = parameters.get("T_EFF_INI");
        double fr_allongement = min(1.0, (phytomer_age * TEff_ini) / duree_allongement );
        length = fr_allongement * max_length_pot; //cm
        double volume = _PI * pow( STEM_RAYON, 2) * length; //cm3
        biomass =  volume * STEM_APPARENT_DENSITY; //gCH2O
    }

    void compute(double /*t*/, bool /* update */)
    {
//        age += 1;

        //        growth();
        assimilate_supply = demand * fr_reste; //compute_assimilate_supply
        length += length_increase_pot * fr_reste; //compute_length
        biomass += assimilate_supply / COUT_RESPI_INTERNODE; //compute_biomass
        TT_corrige +=  gain_TEff_jour * pow( fr_reste, PLASTICITY_INTERNODE_IC ); //compute_TT_new

        //        growth_demand(t);

        //compute_length_max_increase
        if (TT_corrige < duree_allongement) {
            length_increase_pot = ( max_length_pot * TEff ) / duree_allongement;
        } else {
            length_increase_pot = 0;
        }

        double volume_pot = _PI * pow( STEM_RAYON, 2) * length_increase_pot; //cm3
        demand =  volume_pot * STEM_APPARENT_DENSITY * COUT_RESPI_INTERNODE ; //gCH20

        gain_TEff_jour = TEff;
    }
};
} //namespace model
#endif // INTERNODE_H
