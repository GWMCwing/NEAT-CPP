#ifndef NEATCPP_POPULATION_HEADER_H
#define NEATCPP_POPULATION_HEADER_H

#include <vector>

#include "./player.h"
#include "../CLI/outputBuffer.h"

namespace neatCpp {

    // this class have to be implemented by the user
    class Population {
    public:
        // methods
        /**
         * @brief Construct a new Population object
         *
         * @param populationSize size of population
         * @param inputSize input size of neural network
         * @param outputSize output size of neural network
         * @param _outputBuffer pointer to OutputBuffer, for log output handling
         */
        Population(int populationSize, int inputSize, int outputSize, OutputBuffer* _outputBuffer);
        //
        /**
         * @brief Destroy the Population object, and it's allocated Player objects
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
         * @return Player player of a randomly selected player
         */
        Player* selectPlayer();
        /**
         * @brief check if the population has ended
         *
         * @return true if the population has ended, false otherwise
         */
        bool populationDone();
        /**
         * @brief deallocate all player in the population
         *
         */
        void removeAllPlayer();
        /**
         * @brief export population data to a fstream
         *
         * @param fileStream reference of a fstream, data to be exported to
         * @return true the export is successful or not
         */
        bool exportPopulation(std::fstream& fileStream) const;
        bool exportBestPlayer(std::fstream& fileStream) const;
        /**
         * @brief import population from a fstream, must be formatted according to the given standard
         *
         * @param fileStream file stream containing the population data
         * @return Population* pointer to a population
         */
        static Population* importPopulation(std::fstream& fileStream);
        // 
        // list of getter
        long double getAverageScore() const;
        int getGeneration() const;
        int getPopulationSize() const;
        std::vector<Player*> getPopulationPlayers() const;
        long double getBestPlayerScore() const;
        int getBestPlayerGeneration() const;
        //
    private:
        /** version of the export format */
        const int exportVersion = 1;
        /** vector of current players */
        std::vector<Player*> population;
        /** pointer to the best player in all generations*/
        Player* bestPlayer;
        /** best fitness value across all generations */
        long double bestFitness;
        /** pointer to a OutputBuffer object */
        OutputBuffer* outputBuffer;
        /* generation of the current population */
        long int generation;
        /** vector of relative fitness of current generation, used in reproducing next generation */
        std::vector<long int> matingPool;
        //
        int inputSize;
        int outputSize;
        //
    };
}
#endif