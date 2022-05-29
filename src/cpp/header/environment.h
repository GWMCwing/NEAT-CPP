#ifndef NEATCPP_ENVIRONMENT_HEADER_H
#define NEATCPP_ENVIRONMENT_HEADER_H
#include "./population.h"
namespace neatCpp {
    // negative meaning environment is exiting
    // 0 is running without thread blocking
    // positive is thread blocking action ongoing
    enum EnvironmentState {
        EXITED = -2,
        PAUSE = -1,
        RUNNING = 0,
        INITIATING = 1,
        UPDATING = 2,
    };
    enum CommandId {
        EXIT = -2,
        PAUSE = -1,
        RESUME = 0,
        EXPORT = 1,
    };
    struct EnvData {
        long int lastGeneration;
        std::vector<long double> averageScore;
        std::vector<long double> averageFitness;
        std::vector<Player*> bestPlayerList;
    };
    class Environment {
    public:
        Environment(int populationSize, int inputSize, int outputSize);
        ~Environment();
        // cli required functions
        int updateEnvironment();
        void setEnvironmentLock(bool lock);
        bool getEnvironemntLock() const;
        void setEnvironmentUpdateState(int state);
        int getEnvironmentUpdateState() const;
        void AddToEvent(CommandId eventId);
        // Required event handler

        // user defined functions
    private:
        void HandleEvent();
        void updateObject();
        void updatePlayer();
        bool exitEnvironment();
        Population* population;
        EnvironmentState state;
        EnvData envData;
        std::vector<CommandId> commandBuffer;
    };
}
#endif