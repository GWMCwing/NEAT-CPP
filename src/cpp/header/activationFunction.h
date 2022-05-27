#ifndef NEATCPP_ACTIVATIONF_HEADER_H
#define NEATCPP_ACTIVATIONF_HEADER_H

namespace neatCpp {
    const int numberOfActivation = 5;
    long double sigmoid(long double x);
    long double identity(long double x);
    long double step(long double x);
    long double tanh(long double x);
    long double ReLU(long double x);
    long double activationGetValue(const int activationIndex, const long double x);
}
#endif