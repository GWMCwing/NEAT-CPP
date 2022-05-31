#ifndef NEATCPP_PLAYER_HEADER_H
#define NEATCPP_PLAYER_HEADER_H

#include <fstream>
#include <vector>

#include "genome.h"

namespace neatCpp {
    // this class have to be implemented by the user
    class Player {
    private:
        // required members
        Genome* brain; // the brain of the player, the genome of the player, i.e. the neural network
        long double fitness; // fitness of the player
        long double score; // score of the player
        bool dead; // is the player dead
        std::vector<long double> decisions; // decisions of the player, the output of the network
        std::vector<long double> vision; // vision of the player, the input of the network

        // optional member
        long double lifespan;
        long double maxLifespan;
        //
        // methods
        // --------- required methods ---------------
        //
    public:
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
        Genome* getBrain() const;
        bool isDead() const;
        void setDead(bool isDead);
        long double getScore() const;
        void setScore(long double value);
        long double getFitness() const;
        void setFitness(long double value);
        //
        /**
         * @brief crossover two players
         *
         */
        Player* crossover(const Player* const partner);
        //
        /**
         * @brief receive input from the environment
         *
         * @param inputVision input of the player
         *
         */
        void look(const std::vector<long double>& inputVision);
        //
        /**
         * @brief decide what to do
         *
         */
        const std::vector<long double>& think();
        const std::vector<long double>& think(const std::vector<long double>& inputVision);
        //
        /**
         * @brief perform movement the player
         *
         */
        void move();
        //
        /**
         * @brief calculate fitness of the player
         *
         */
        void calculateFitness();
        void exportPlayer(std::fstream& file);
        //
        // ----- optional methods, depends on the implementation ----------
        //
        /**
         * @brief update the player meta data
         *
         */
        void update();

    };
}
#endif