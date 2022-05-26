#include <cmath>

namespace neatCpp {
    inline long double sigmoid(long double x) { return 1 / (1 + exp(-4.9 * x)); }
    inline long double identity(long double x) { return x; }
    inline long double step(long double x) { return x > 0 ? 1 : 0; }
    inline long double tanh(long double x) { return tanh(x); }
    inline long double ReLU(long double x) { return x > 0 ? x : 0; }
    //
    long double activationGetValue(const int activationIndex, const long double x) {
        switch (activationIndex) {
            case 0:
                return sigmoid(x);break;
            case 1:
                return identity(x);break;
            case 2:
                return step(x);break;
            case 3:
                return tanh(x);break;
            case 4:
                return ReLU(x); break;
            default:
                return sigmoid(x); break;
        }
    }
}