#ifndef NEATCPP_POPULATION_HEADER_H
#define NEATCPP_POPULATION_HEADER_H

#include <vector>

#include "player.h"
#include "./outputBuffer.h"

namespace neatCpp {

    // this class have to be implemented by the user
    class Population {
    public:
        // methods
        /**
         * @brief Construct a new Population object
         *
         * @param populationSize size of the population
         */
        Population(int populationSize, int inputSize, int outputSize, OutputBuffer* _outputBuffer);
        //
        /**
         * @brief Destroy the Population object
         *
         */
        ~Population();
        //
        /**
         * @brief generate the next generation
         *
         */
        void naturalSelection();
        //
        /**
         * @brief calculate the fitness of the population
         *
         */
        void calculateFitness();
        //
        /**
         * @brief fill the mating pool for natural selection
         *
         */
        void fillMatingPool();
        //
        /**
         * @brief randomly select a player from the population
         *
         * @return Player
         */
        Player* selectPlayer();
        //
        /**
         * @brief get the average score of the population
         *
         */
        long double getAverageScore() const;
        int getGeneration() const;
        int getPopulationSize() const;
        long double getBestPlayerScore() const;
        int getBestPlayerGeneration() const;
        //
        /**
         * @brief check if the population has ended
         *
         * @return true if the population has ended, false if not
         */
        bool populationDone();
        void deleteAllPlayer();
        bool exportPopulation(std::fstream& fileStream) const;
        Population* importPopulation(std::string path);
        //
        //  ------------- optional methods ---------
        //
        /**
         * @brief update the population
         *
         */
         // void updateAlive();

        //
    private:
        const int exportVersion = 1;
        std::vector<Player*> population; // population of the players
        Player* bestPlayer; // best player of the population
        long double bestFitness; // best fitness of the population
        OutputBuffer* outputBuffer;
        long int generation; // generation of the population
        std::vector<long int> matingPool; // mating pool of the population, for reproduction
        //
    };
}
#endif