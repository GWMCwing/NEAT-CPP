#include "./randomFun.h"
#include <random>
namespace neatCpp {
    long double randGaussian(double lowBound, double upBound, double deviation) {
        std::random_device rd;
        std::mt19937 gen(rd());
        // mean and deviations
        std::normal_distribution<long double> dist((lowBound + upBound) / 2, deviation);

        return dist(gen);
    }
    long double randNum(double lowBound, double upBound) {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(lowBound, upBound);
        return dis(gen);
    }
}