#ifndef SAMPLEATOMIC_H
#define SAMPLEATOMIC_H

/*
 * Copyright (C) 2019-2020 Cirad http://www.cirad.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file xpalm/phytomer/Model.hpp
 * @author The XPalm Development Team
 * See the AUTHORS or Authors.txt file
 */

#include <defines.hpp>

namespace model {

class SampleModel : public AtomicModel < SampleModel >
{
public:
    enum submodels { SUB_1, SUB_2 };

    enum internals { INTERNAL_1, INTERNAL_2 };

    enum externals { EXTERNAL_1, EXTERNAL_2 };

private:

//      parameters
    double PARAM_1;
    double PARAM_2;

//      attribute
    int _integerAttr;
    bool _boolAttr;
    double _doubleAttr;

//     submodels
    std::unique_ptr < SampleModel > _subsample_model_1;
    std::unique_ptr < SampleModel > _subsample_model_2;

//     internals
    double _int_1;
    int _int_2;

//     externals
    double _ext_1;
    int _ext_2;

public:

    SampleModel(int integerVar, bool boolVar, double doubleVar) :
        _integerAttr(integerVar),
        _boolAttr(boolVar),
        _doubleAttr(doubleVar),
        _subsample_model_1(new SampleModel(_index, _is_on_mainstem, _is_last_phytomer)),
        _subsample_model_2(nullptr)
    {
//         submodels
        setsubmodel(SUB_1, _subsample_model_1.get());
        setsubmodel(SUB_2, _subsample_model_2.get());

//         internals
        Internal(INTERNAL_1, &SampleModel::_int_1);
        Internal(INTERNAL_2, &SampleModel::_int_2);

//          externals
        External(EXTERNAL_1, &SampleModel::_ext_1);
        External(EXTERNAL_2, &SampleModel::_ext_2);

    }

    virtual ~SampleModel()
    {
        _subsample_model_1.reset(nullptr);
        _subsample_model_2.reset(nullptr);
    }


    double sample_function() {
        return _integerAttr;
    }


    void init(double t, const xpalm::ModelParameters& parameters)
    {

//        parameters
        PARAM_1 = _parameters.get("param_1");
        PARAM_2 = _parameters.get("param_2");

//         submodels
        _subsample_model_1->init(t, parameters);
        _subsample_model_2->init(t, parameters);

//        attributes
        _integerAttr = 0;
        _boolAttr = 0;
        _doubleAttr = 0;
        _int_1 = 0;
        _int_2 = 0;
    }

    void compute(double t, bool /* update */)
    {
        _doubleAttr = sample_function();
        _subsample_model_1->put(t, SampleModel::EXTERNAL_1, _doubleAttr);
        _subsample_model_1->put < int >(t, SampleModel::EXTERNAL_2, _integerAttr);
        _doubleAttr = _subsample_model_2->get< double >(t, SampleModel::INTERNAL_1);
        (*_subsample_model_1)(t);
        (*_subsample_model_2)(t);
    }

    SampleModel * sub_sample_1() const
    { return _subsample_model_1.get(); }

};

} // namespace model
#endif // SAMPLEATOMIC_H
