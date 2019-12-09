#ifndef PHYTOMER_H
#define PHYTOMER_H
/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>
#include <plant/phytomer/leaf.h>
#include <plant/phytomer/internode.h>
#include <plant/phytomer/inflo.h>

namespace model {

class Phytomer : public CoupledModel < Phytomer >
{
public:
    enum submodels { LEAF, INFLO, INTERNODE };

    enum internals { STATE,
                     RANK,
                     NUMBER,
                     AGE,
                     TT_SINCE_APPEARENCE,
                     PRODUCTION_SPEED,
                     TT_INI_FLOWERING
                   };

    enum external { YOUNGEST_PHYTOMER_NUMBER,
                    TREE_IC,
                    TEFF };

private:
    //     submodels
    std::unique_ptr < Leaf > leaf;
    std::unique_ptr < Internode > internode;
    std::unique_ptr < Inflo > inflo;

    //     parameters
    double INACTIVE_PHYTOMER_NUMBER;

    //     internals
    phytomer::phytomer_state state;
    double rank;
    double number;
    double age;
    double TT_since_appearence;

    //predim
    double production_speed;
    double TT_ini_flowering;

    //attr
    inflo::inflo_states inflo_status;

    //   externals
    double youngest_phytomer_number;
    double tree_IC;
    double TEff;

public:
    Phytomer() :
        internode(new Internode()),
        leaf(new Leaf()),
        inflo(new Inflo())
    {
        //         submodels
        submodel(LEAF, leaf.get());
        submodel(INTERNODE, internode.get());
        submodel(INFLO, inflo.get());

        //         internals
        Internal(STATE, &Phytomer::state);
        Internal(RANK, &Phytomer::rank);
        Internal(NUMBER, &Phytomer::number);
        Internal(AGE, &Phytomer::age);
        Internal(TT_SINCE_APPEARENCE, &Phytomer::TT_since_appearence);
        //predim
        Internal(PRODUCTION_SPEED, &Phytomer::production_speed);
        Internal(TT_INI_FLOWERING, &Phytomer::TT_ini_flowering);

        // externals
        External(YOUNGEST_PHYTOMER_NUMBER, &Phytomer::youngest_phytomer_number);
        External(TREE_IC, &Phytomer::tree_IC);
        External(TEFF, &Phytomer::TEff);
    }

    virtual ~Phytomer()
    {
        leaf.reset(nullptr);
        internode.reset(nullptr);
        inflo.reset(nullptr);
    }

    Leaf * leaf_model() const { return leaf.get(); }
    Inflo * inflo_model() const { return inflo.get(); }
    Internode * internode_model() const { return internode.get(); }


    void init(double t, const xpalm::ModelParameters& parameters){}
    void init(double t, const xpalm::ModelParameters& parameters, int nb, int rk, bool st, double tree_age_at_creation,
              double tree_age, double prod_speed, double flo_tt, double harv_tt, double tt_ini_sen, double inflo_factor)
    {
        last_time = t-1;

        //parameters
        INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");

        youngest_phytomer_number = 0;

        //predim
        number = nb;
        TT_ini_flowering = flo_tt;
        production_speed = prod_speed;

        //var
        rank = rk;
        state = st ? phytomer::INACTIVE : phytomer::ACTIVE;
        age = tree_age - tree_age_at_creation;
        TT_since_appearence = parameters.get("T_EFF_INI")*age;

        //submodels
        leaf->init(t, parameters, age);
        internode->init(t, parameters, age, tree_age_at_creation, production_speed);
        inflo->init(t, parameters, age, rank, production_speed, TT_ini_flowering, harv_tt, tt_ini_sen, inflo_factor);
        inflo_status = inflo->get<inflo::inflo_states, Inflo>(t, Inflo::STATUS);
    }


    void compute(double t, bool /* update */)
    {

        if(rank == 1 && state == phytomer::INACTIVE)
            state = phytomer::ACTIVE;

        if(rank == 60 && state == phytomer::ACTIVE)
            state = phytomer::DEAD;

//        if(state == phytomer::DEAD) //TODO remove to include leaf/internode in demand when inactive

//            return;

        age++;


        rank = youngest_phytomer_number - number - INACTIVE_PHYTOMER_NUMBER + 1;

         TT_since_appearence += TEff;



//        if(state != phytomer::ACTIVE) //TODO remove to include leaf/internode in demand when inactive
//            return;

        leaf->put<double>(t, Leaf::PHYTOMER_RANK, rank);
        leaf->put<phytomer::phytomer_state>(t, Leaf::PHYTOMER_STATE, state);
        leaf->put<inflo::inflo_states>(t, Leaf::INFLO_STATUT, inflo_status);
        leaf->put<double>(t, Leaf::TEFF, TEff);
        (*leaf)(t);

        internode->put<double>(t, Internode::TEFF, TEff); //TODO passer dans tree
        //set by tree
//        internode->put<double>(t, Internode::FR_RESTE, fr_reste);
        (*internode)(t);

        inflo->put<double>(t, Inflo::TEFF, TEff);
        inflo->put<double>(t, Inflo::RANK, rank);
        inflo->put<double>(t, Inflo::TT_SINCE_APPEARENCE, TT_since_appearence);
        inflo->put<double>(t, Inflo::TREE_IC, tree_IC);

        //set by tree
//        inflo->put<double>(t, Inflo::FTSW, ftsw);
//        inflo->put<double>(t, Inflo::FR_RESTE, fr_reste);
//        inflo->put<double>(t, Inflo::FR_FRUITS, fr_fruits);
        (*inflo)(t);
        inflo_status = inflo->get<inflo::inflo_states, Inflo>(t, Inflo::STATUS);
    }



};

} //namespace model
#endif // PHYTOMER_H
