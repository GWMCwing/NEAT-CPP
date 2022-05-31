#include "./header/environment.h"
#include "./header/population.h"
#include <iostream>
namespace neatCpp {
    Environment::Environment(int populationSize, int inputSize, int outputSize) {
        state = EnvironmentState::INITIATING;
        population = new Population(populationSize, inputSize, outputSize);
        state = EnvironmentState::RUNNING;
    }
    //! use exit Env
    Environment::~Environment() {
        if (!population) {
            std::cerr
                << "Population was not deleted when deallocating Environment\nDeleting Population..."
                << std::endl;
            delete population;
            population = nullptr;
        }
    }
    void Environment::addToEvent(CommandId commandId) {
        commandBuffer.push_back(commandId);
    }
    int Environment::updateEnvironment() {
        state = EnvironmentState::UPDATING;
        handleEvent();
        if (state == EnvironmentState::EXITED || state == EnvironmentState::PAUSE) return state;
        // TODO
        updateObject();
        updatePlayer();
        state = EnvironmentState::RUNNING;
        return 0;
    }
    bool Environment::exitEnvironment() {
        state = EnvironmentState::PAUSE;
        delete population;
        population = nullptr;
        state = EnvironmentState::EXITED;
    }
    void Environment::handleEvent() {

    }
    // edit base on needs
    void Environment::updateObject() {

    }
    void Environment::updatePlayer() {

    }

}