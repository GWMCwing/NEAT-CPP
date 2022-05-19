#ifndef POPULATION_HEADER_H
#define POPULATION_HEADER_H
#include "player.h"
#include <vector>
namespace neatCpp {
    // this class have to be implemented by the user
    class Population {
    private:
        std::vector<Player*> population; // population of the players
        Player* bestPlayer; // best player of the population
        long double bestFitness; // best fitness of the population

        long int generation; // generation of the population
        std::vector<Player*> matingPool; // mating pool of the population, for reproduction
        //
    public:
        // methods
        /**
         * @brief Construct a new Population object
         *
         * @param populationSize size of the population
         */
        Population(long int populationSize);
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
        long double getAverageScore();
        //
        //  ------------- optional methods ---------
        //
        /**
         * @brief update the population
         *
         */
        void updateAlive();
        //
        /**
         * @brief check if the population has ended
         *
         * @return true if the population has ended, false if not
         */
        bool populationDone();
        //
    };
}
#endif