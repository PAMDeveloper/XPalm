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


//static double age_relative_var(double age, double age_ini, double age_fin, double val_ini, double val_fin) {
//    if ( age > age_fin ) {
//        return val_fin;
//    } else if (age < age_ini ) {
//        return val_ini;
//    } else {
//        double age_relative = age - age_ini;
//        double inc = (val_fin - val_ini) / (age_fin - age_ini);
//        return val_ini + age_relative * inc;
//    }
//}

//coeffIC(ics, rank, IC_spikelet_RANG_DEBUT, IC_spikelet_RANG_FIN);


#ifdef UNSAFE_RUN

namespace inflo {
typedef int inflo_status;
static const unsigned int APPARITION_FLORAISON = 0;
static const unsigned int FLORAISON_RECOLTE = 1;
}

namespace phytomer {
typedef int phytomer_state;
static const unsigned int ACTIVE = 0;
static const unsigned int INACTIVE = 1;
static const unsigned int DEAD = 2;
}

namespace peduncle {
typedef int peduncle_phase;
static const unsigned int INITIAL = 0;
static const unsigned int TRANSITION = 1;
static const unsigned int REALIZATION = 2;
static const unsigned int FLO = 3;
static const unsigned int END_FILLING = 4;
static const unsigned int MATURITY = 5;
static const unsigned int DEAD = 6;
}

namespace internode {
typedef int internode_phase;
static const unsigned int INITIAL = 0;
static const unsigned int VEGETATIVE = 1;
static const unsigned int REALIZATION = 2;
static const unsigned int MATURITY = 3;
static const unsigned int DEAD = 4;
}

namespace leaf {
typedef int leaf_phase;
static const unsigned int INITIAL = 0;
static const unsigned int VEGETATIVE = 1;
static const unsigned int LIG = 2;
static const unsigned int DEAD = 3;
}

namespace culm {
typedef int culm_phase;
static const unsigned int INITIAL = 0;
static const unsigned int VEGETATIVE = 1;
static const unsigned int ELONG = 2;
static const unsigned int PRE_PI = 3;
static const unsigned int PI = 4;
static const unsigned int PRE_FLO = 5;
static const unsigned int FLO = 6;
static const unsigned int END_FILLING = 7;
static const unsigned int MATURITY = 8;
static const unsigned int DEAD = 9;
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
namespace plant {
typedef int plant_phase;
static const unsigned int INITIAL = 0;
static const unsigned int VEGETATIVE = 1;
static const unsigned int ELONG = 2;
static const unsigned int PI = 3;
static const unsigned int PRE_FLO = 4;
static const unsigned int FLO = 5;
static const unsigned int END_FILLING = 6;
static const unsigned int MATURITY = 7;
static const unsigned int DEAD = 8;

typedef flag plant_state;
static const unsigned int NO_STATE = 0;
static const unsigned int NOGROWTH = 1;
static const unsigned int NEW_PHYTOMER_AVAILABLE = 2;
static const unsigned int LIG = 4;
static const unsigned int KILL = 8;

}

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

typedef SimpleSimulator < PlantModel, GlobalParameters, xpalm::ModelParameters > XPalmSimulator;
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




//namespace peduncle {
//enum peduncle_phase {   INITIAL = 0,
//                        TRANSITION = 1,
//                        REALIZATION = 2,
//                        FLO = 3,
//                        END_FILLING = 4,
//                        MATURITY = 5,
//                        DEAD = 6
//                    };

//}

//namespace internode {
//enum internode_phase {  INITIAL = 0,
//                        VEGETATIVE = 1,
//                        REALIZATION = 2,
//                        MATURITY = 3,
//                        DEAD = 4
//                     };
//}


//namespace culm {
//enum culm_phase {   INITIAL = 0,
//                    VEGETATIVE = 1,
//                    ELONG = 2,
//                    PRE_PI = 3,
//                    PI = 4,
//                    PRE_FLO = 5,
//                    FLO = 6,
//                    END_FILLING = 7,
//                    MATURITY = 8,
//                    DEAD = 9
//                };

//}



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



//namespace oil {
//enum oil_state   { PAS_DE_FRUITS = 0,
//                   APPARITION_FLORAISON = 1,
//                   RECOLTE = 2,
//                   OLEOSYNTHESE = 3,
//                   AVANT_OLEOSYNTHESE = 4,
//                   UNKNOWN = 5
//                 };
//}


namespace bunch {
enum bunch_state {
    OLEOSYNTHESE = 0,
    AVANT_OLEOSYNTHESE = 1,
    RECOLTE = 2,
    ABORTED = 3,
    ABLATED = 4
};
}



namespace inflo {

//enum inflo_sex {
//    FEMALE = 0,
//    MALE = 1,
//    ABORTED = 2,
//    UNKNOWN = 3
//};


//enum inflo_state {      APPARITION_FLORAISON = 1
//                      , FLORAISON_RECOLTE = 2
//                      , PAS_DE_FRUITS = 4
//                      , UNKNOWN = 8
//                      , NON_ABLATED = 16
//                      , INITIE = 32
//                      , RECOLTE = 64
//                 };

enum inflo_state {
      ABORTED = 1
    , INITIATED = 2
    , MALE = 4
    , FEMALE = 8
    , FLOWERING = 16
    , OLEOSYNTHESIS = 32
    , HARVEST = 64
    , DEAD = 128
    , SENESCENCE = 256
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
        states = states & !static_cast<int>(state);
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
        states = states & !static_cast<int>(state);
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
    a = static_cast<inflo_state>(static_cast<int>(a) & !static_cast<int>(b));
}
}


//// Plant enums
//namespace plant {
//enum plant_state { NO_STATE = 0,
//                   NOGROWTH = 1,
//                   NEW_PHYTOMER_AVAILABLE = 2,
//                   LIG = 4,
//                   KILL = 8
//                 };

//enum plant_phase {  INITIAL = 0,
//                    VEGETATIVE = 1,
//                    ELONG = 2,
//                    PI = 3,
//                    PRE_FLO = 4,
//                    FLO = 5,
//                    END_FILLING = 6,
//                    MATURITY = 7,
//                    DEAD = 8
//                 };

//typedef States <plant_state> plant_states;

//inline bool operator&(plant_state a, plant_state b) {
//    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
//}
//inline void operator<<(plant_state& a, plant_state b) {
//    a = static_cast<plant_state>(static_cast<int>(a) | static_cast<int>(b));
//}
//inline void operator>>(plant_state& a, plant_state b) {
//    a = static_cast<plant_state>(static_cast<int>(a) & !static_cast<int>(b));
//}
//}

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
