#ifndef NEATCPP_PLAYER_HEADER_H
#define NEATCPP_PLAYER_HEADER_H

#include <fstream>
#include <vector>

#include "./genome.h"

namespace neatCpp {
    // this class have to be implemented by the user
    class Player {
        //
    public:
        // methods
        // --------- required methods ---------------
        Player(long int id, int inputSize, int outputSize);
        Player(const Player* player);
        Player(Genome* _brain);
        ~Player();
        /**
         * @brief return a deep copy of this player
         *
         * @return Player
         */
        Player* clone() const;

        /**
         * @brief crossover two players
         *
         */
        Player* crossover(const Player* const partner);
        /**
         * @brief set input value of the player
         *
         * @param inputVision input of the player
         *
         */
        void look(const std::vector<long double>& inputVision);
        /**
         * @brief generate a output vector base on input vector, using genome saved in brain
         *
         */
        const std::vector<long double> think();
        const std::vector<long double> think(const std::vector<long double>& inputVision);
        /**
         * @brief perform movement the player
         *
         */
        void move();
        /**
         * @brief calculate fitness value of the player
         *
         */
        void calculateFitness();
        /**
         * @brief export player data to fileStream
         *
         * @param fileStream
         */
        void exportPlayer(std::fstream& fileStream);
        //
        // list of getter

        Genome* getBrain() const;
        bool isDead() const;
        long double getScore() const;
        long double getFitness() const;
        //
        // list of setter

        void setDead(bool isDead);
        void setScore(long double value);
        void setFitness(long double value);
        // 
        // optional members
    private:
        // required members
        /** brain of the player, i.e. genome and the neural network */
        Genome* brain;
        /** fitness value of the player */
        long double fitness;
        /** score of the player, not involved in reproducing next generation */
        long double score;
        /** is the player dead or no need to update*/
        bool dead;
        /** vector of output of the brain, neural network */
        std::vector<long double> decisions;
        /** vector of input of the brain, neural network */
        std::vector<long double> vision;
        //
        // optional members
    };
}
#endif