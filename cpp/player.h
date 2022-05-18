#ifndef PLAYER_HEADER_H
#define PLAYER_HEADER_H
#include "genome.h"
#include <vector>
// this class have to be implemented by the user
class Player {
public:
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
    /**
     * @brief return a deep copy of this player
     *
     * @return Player
     */
    Player* clone() const;
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
    void look();
    //
    /**
     * @brief decide what to do
     *
     */
    void think() {
        decisions = brain->feedForward(vision);
    }
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
    //
    // ----- optional methods, depends on the implementation ----------
    //
    /**
     * @brief update the player meta data
     *
     */
    void update();

};
#endif