#ifndef DEFINES_HPP
#define DEFINES_HPP

#define STRICT_R_HEADERS
#undef PI


#include "iso646.h"
#include <type_traits> //necessary for enum type checking on states

class Tree;
const double _PI = 3.141592653589793238462643383280;

struct GlobalParameters {
};


static double age_relative_var(double age, double age_ini, double age_fin, double val_ini, double val_fin) {
    if ( age > age_fin ) {
        return val_fin;
    } else if (age < age_ini ) {
        return val_ini;
    } else {
        double age_relative = age - age_ini;
        double inc = (val_fin - val_ini) / (age_fin - age_ini);
        return val_ini + age_relative * inc;
    }
}

//coeffIC(ics, rank, IC_spikelet_RANG_DEBUT, IC_spikelet_RANG_FIN);


#ifdef UNSAFE_RUN
namespace leaf {
typedef int leaf_state;
static const unsigned int COUPE = 0;
static const unsigned int NON_COUPE = 1;
static const unsigned int LIG = 2;
static const unsigned int DEAD = 3;
}


namespace phytomer {
typedef int phytomer_state;
static const unsigned int ACTIVE = 0;
static const unsigned int INACTIVE = 1;
static const unsigned int DEAD = 2;
}


namespace bunch {
typedef int bunch_state;
static const unsigned int OLEOSYNTHESE = 0;
static const unsigned int AVANT_OLEOSYNTHESE = 1;
static const unsigned int RECOLTE = 2;
static const unsigned int ABORTED = 3;
static const unsigned int ABLATED = 4;
}


struct flag {
    flag() = default;
    flag(int v): val(v) {}
    int val;
    inline flag& operator=(const flag& b) {
        val = b.val;
        return *this;
    }
    inline flag& operator=(const unsigned int& b) {
        val = b;
        return *this;
    }
    inline operator int() const {
        return val;
    }
    inline explicit operator int* () const {
        return nullptr;
    }

    bool is(const flag& b) {
        return (val & b) != 0;
    }

    void add(const flag& b) {
        val = val | b;
    }

    void del(const flag& b) {
        val = val & !b;
    }

    void replace(const flag& from, const flag& to) {
        del(from);
        add(to);
    }
};

inline bool operator&(const flag& a, const unsigned int& b) {
    return (a.val & b) != 0;
}
inline void operator<<(flag& a, const unsigned int& b) {
    a.val = a.val | b;
}
inline void operator>>(flag& a, const unsigned int& b) {
    a.val = a.val & !b;
}

// Plant enums
namespace inflo {
typedef flag inflo_states;
static const unsigned int ABORTED = 1;
static const unsigned int INITIATED = 2;
static const unsigned int MALE = 4;
static const unsigned int FEMALE = 8;
static const unsigned int FLOWERING = 16;
static const unsigned int OLEOSYNTHESIS = 32;
static const unsigned int HARVEST = 64;
static const unsigned int DEAD = 128;
static const unsigned int SENESCENCE = 256;
};

#include <utils/juliancalculator.h>
#include <artis_lite/simplemodel.h>
#include <memory>
#include <deque>
#include <limits>

namespace artis {
namespace utils {
namespace DateTime {
static string toJulianDayFmt(double, int) {
    return "";
}
}
}
}

struct DoubleTime {
    static constexpr double negative_infinity = -numeric_limits < double >::infinity();
    static constexpr double positive_infinity = numeric_limits < double >::infinity();
    static constexpr double null = 0;
};


using Model = SimpleModel < xpalm::ModelParameters >;
template < typename T > using AtomicModel = SimpleModel < T >;
template < typename T > using CoupledModel = SimpleModel < T >;

typedef SimpleSimulator < Tree, GlobalParameters, xpalm::ModelParameters > XPalmSimulator;
typedef SimpleContext XPalmContext;
typedef SimpleView View;
typedef SimpleObserver Observer;

//#ifdef WITH_TRACE
using Trace = SimpleTrace;
//using KernelInfo = SimpleKernelInfo;
template < class T > using TraceElement = SimpleTraceElement;
template < class T > using TraceElements = std::vector < TraceElement <T>>;
//#endif

#else

#include <utils/juliancalculator.h>
#include <ModelParameters.hpp>
#include <artis/kernel/Simulator.hpp>
#include <artis/kernel/AbstractAtomicModel.hpp>
#include <artis/utils/Trace.hpp>


namespace leaf {
enum leaf_state   { COUPE = 0,
                    NON_COUPE = 1,
                    LIG = 2,
                    DEAD = 3
                  };
}


namespace phytomer {
enum phytomer_state {  ACTIVE = 0,
                       INACTIVE = 1,
                       DEAD = 2
                    };
}


//namespace bunch {
//enum bunch_state {
//    OLEOSYNTHESE = 0,
//    AVANT_OLEOSYNTHESE = 1,
//    RECOLTE = 2,
//    ABORTED = 3,
//    ABLATED = 4
//};
//}



namespace inflo {
enum inflo_state {
    INITIATED = 1
    , NON_ABORTED = 2
    , ABORTED = 4
    , MALE = 8
    , FEMALE = 16
    , FLOWERING = 32
    , OLEOSYNTHESIS = 64
    , HARVEST = 128
    , DEAD = 256
    , SENESCENCE = 512
};


template <typename E, typename std::enable_if<std::is_enum<E>::value>::type* = nullptr>
class States {
public:
    int states;

    bool operator&(E state) {
        return (states & static_cast<int>(state)) != 0;
    }
    void operator<<(E state) {
        states = states | static_cast<int>(state);
    }
    void operator>>(E state) {
        states = states & ~static_cast<int>(state);
    }

    operator int() {
        return states;
    }

    bool operator !=(const States& s) const{
        return this->states != s.states;
    }

    bool is(E state) {
        return (states & static_cast<int>(state)) != 0;
    }
    void add(E state) {
        states = states | static_cast<int>(state);
    }
    void del(E state) {
        states = states & ~static_cast<int>(state);
    }

    void replace(E from, E to) {
        del(from);
        add(to);
    }
};

typedef States <inflo_state> inflo_states;

inline bool operator&(inflo_state a, inflo_state b) {
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}
inline void operator<<(inflo_state& a, inflo_state b) {
    a = static_cast<inflo_state>(static_cast<int>(a) | static_cast<int>(b));
}
inline void operator>>(inflo_state& a, inflo_state b) {
    a = static_cast<inflo_state>(static_cast<int>(a) & ~static_cast<int>(b));
}
}


using Model = artis::kernel::AbstractModel < artis::utils::DoubleTime, xpalm::ModelParameters >;

using Trace = artis::utils::Trace < artis::utils::DoubleTime >;

using TraceElement = artis::utils::TraceElement < artis::utils::DoubleTime >;

template < typename T >
using AtomicModel = artis::kernel::AbstractAtomicModel
<T, artis::utils::DoubleTime, xpalm::ModelParameters >;

template < typename T >
using CoupledModel = artis::kernel::AbstractCoupledModel
<T, artis::utils::DoubleTime, xpalm::ModelParameters, GlobalParameters >;

typedef artis::observer::Observer < artis::utils::DoubleTime, xpalm::ModelParameters > Observer;

typedef artis::observer::View < artis::utils::DoubleTime, xpalm::ModelParameters > View;

typedef artis::kernel::Simulator
< Tree, artis::utils::DoubleTime, xpalm::ModelParameters, GlobalParameters >
XPalmSimulator;

typedef artis::context::Context < artis::utils::DoubleTime > XPalmContext;
#endif

#endif // DEFINES_HPP
