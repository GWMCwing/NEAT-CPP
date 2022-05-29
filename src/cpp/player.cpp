#include "./header/player.h"
namespace neatCpp {
    // modify according to your needs
    void Player::calculateFitness() {
        fitness = score;
        fitness /= brain->calculateWeight();
    }
    // default implementation
    Player::Player(long int id, int inputSize, int outputSize) {
        brain = new Genome(inputSize, outputSize, id, false);
        fitness = 0;
        score = 1;
        lifespan = 0;
        dead = false;
        decisions.reserve(outputSize);
        vision.reserve(inputSize);
    }
    Player::Player(const Player* player) {
        brain = player->brain->clone();
        fitness = player->fitness;
        score = player->score;
        lifespan = player->lifespan;
        dead = player->dead;
        decisions = player->decisions;
        vision = player->vision;
    }
    Player::Player(Genome* _brain) {
        brain = _brain;
        fitness = 0;
        score = 1;
        lifespan = 0;
        dead = false;
        decisions.reserve(outputSize);
        vision.reserve(inputSize);
    }
    Player::~Player() {
        delete brain;
    }
    Player* Player::clone() const {
        return new Player(this);
    }
    Player* Player::crossover(const Player* const parent) {
        Genome* _brain;
        if (parent->fitness < fitness) {
            _brain = brain->crossover(parent->brain);
        } else {
            _brain = parent->brain->crossover(brain);
        }
        Player* child = new Player(_brain);
        child->brain->mutate();
        return child;
    }
    Genome* Player::getBrain() const { return brain; }
    long double Player::getFitness() const { return fitness; }
    void Player::setFitness(long double value) { fitness = value; }
    bool Player::isDead() const { return dead; }
    void Player::setDead(bool isDead) { dead = isDead; }
    long double Player::getScore() const { return score; }
    void Player::setScore(long double value) { score = value; }
    //
    void Player::look(const std::vector<long double>& inputVision) {
        vision = inputVision;
    }
    const std::vector<long double>& Player::think() {
        decisions = brain->feedForward(vision);
        return decisions;
    }
    const std::vector<long double>& Player::think(const std::vector<long double>& inputVision) {
        decisions = brain->feedForward(inputVision);
        return decisions;
    }

}