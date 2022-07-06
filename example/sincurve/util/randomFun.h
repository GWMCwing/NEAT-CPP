#ifndef NEATCPP_RANDOMFUNCTION_HEADER_H
#define NEATCPP_RANDOMFUNCTION_HEADER_H

namespace neatCpp {
    /**
     * @brief generate a random gaussian number
     *
     * @param lowBound lower bound of the random gaussian number
     * @param upBound upper bound of the random gaussian number
     * @param deviation deviation of the gaussian number generator
     * @return long double gaussian number
     */
    long double randGaussian(double lowBound, double upBound, double deviation);
    /**
     * @brief return a random number within range [min,max)
     *
     * @param lowBound lower bound of the output range
     * @param upBound upper bound of the output range
     * @return long double random number generated
     */
    long double randNum(double lowBound, double upBound);
}
#endif