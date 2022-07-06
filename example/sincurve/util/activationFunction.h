#ifndef NEATCPP_ACTIVATIONF_HEADER_H
#define NEATCPP_ACTIVATIONF_HEADER_H

// class storing all activation functions to be used
// modify numberOfActivation and activationGetValue accordingly
namespace neatCpp {
    /** number of activation function defined to be used*/
    const int numberOfActivation = 5;
    /**
     * @brief get the activated value of input x base on the activation function specified by activationIndex
     *
     * @param activationIndex index of the activation function
     * @param x input number to the activation function
     * @return long double activated value of the input
     */
    long double activationGetValue(const int activationIndex, const long double x);
    // list of activation function

    long double sigmoid(long double x);
    long double identity(long double x);
    long double step(long double x);
    long double tanh(long double x);
    long double ReLU(long double x);
}
#endif