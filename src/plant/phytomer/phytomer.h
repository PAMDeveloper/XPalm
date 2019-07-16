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
#include <plant/phytomer/bunch.h>

namespace model {

class Phytomer : public CoupledModel < Phytomer >
{
public:
    enum submodels { LEAF, BUNCH, INTERNODE };

    enum internals { RANK,
                     STATE,
                     DEMAND,
                     NUMBER,
                     DATE,
                     STEP_APP};
private:
//     submodels
    std::unique_ptr < Leaf > leaf;
    std::unique_ptr < Internode > internode;
    std::unique_ptr < Bunch > bunch;

//     internals
    double rank;
    phytomer::phytomer_state state;
    double demand;
    double number;
    double date;
    double step_apparition;

public:
    Phytomer(int rank, int number, phytomer::phytomer_state state, double date) :
        rank(rank), state(state), number(number),
        date(date),
        leaf(new Leaf()),
        internode(new Internode()),
        bunch(new Bunch())
    {
//         submodels
        setsubmodel(LEAF, leaf.get());
        setsubmodel(INTERNODE, internode.get());
        setsubmodel(BUNCH, bunch.get());

//         internals
        Internal(RANK, &Phytomer::rank);
        Internal(STATE, &Phytomer::state);
        Internal(DEMAND, &Phytomer::demand);
        Internal(NUMBER, &Phytomer::number);
        Internal(DATE, &Phytomer::date);
        Internal(STEP_APP, &Phytomer::step_apparition);
    }

    virtual ~Phytomer()
    {
        leaf.reset(nullptr);
        internode.reset(nullptr);
        bunch.reset(nullptr);
    }

    void init(double t, const xpalm::ModelParameters& parameters)
    {
//        parameters
        double INACTIVE_PHYTOMER_NUMBER = parameters.get("INACTIVE_PHYTOMER_NUMBER");
        double RANG_D_ABLATION = parameters.get("RANG_D_ABLATION");
        double INITIAL_PRODUCTION_SPEED = parameters.get("INITIAL_PRODUCTION_SPEED");
        step_apparition = t - (INACTIVE_PHYTOMER_NUMBER + RANG_D_ABLATION - number) * (1.0/ (10.0 * INITIAL_PRODUCTION_SPEED));

//         submodels
        leaf->init(t, parameters);
        internode->init(t, parameters);
        bunch->init(t, parameters);
    }

    void compute(double t, bool /* update */)
    {

        leaf->put(t, Leaf::PHYTOMER_RANK, rank);
        leaf->put(t, Leaf::PHYTOMER_STATE, state);
        leaf->put(t, Leaf::BUNCH_SEXE, bunch->get<bunch::bunch_sex, Bunch>(t, Bunch::SEX));
        leaf->put(t, Leaf::BUNCH_AVORT, bunch->get<bunch::bunch_sex, Bunch>(t, Bunch::AVORT));
        leaf->put(t, Leaf::BUNCH_STATUT, bunch->get<bunch::bunch_states, Bunch>(t, Bunch::STATUS));
        (*leaf)(t);

        (*internode)(t);

        bunch::bunch_states bunch_statut = bunch.get()->get< bunch::bunch_states, Bunch >(t, Bunch::STATUS);
        if(!bunch_statut.is(bunch::RECOLTE)) {
            bunch->put(t, Bunch::PHYTOMER_RANK, rank);
            (*bunch)(t);
        }
    }

    Leaf * leaf_model() const { return leaf.get(); }
    Bunch * bunch_model() const { return bunch.get(); }
    Internode * internode_model() const { return internode.get(); }

};

} //namespace model
#endif // PHYTOMER_H
