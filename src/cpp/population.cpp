#include <iostream>
#include "./header/population.h"
#include "./header/randomFun.h"
namespace neatCpp {
    Population::Population(int populationSize) {
        bestFitness = 0;
        generation = 0;
        bestPlayer = nullptr;
        for (int i = 0;i < populationSize;++i) {
            Player* p = new Player(i);
            p->getBrain()->generateNetwork();
            p->getBrain()->mutate();
            population.push_back(p);
        }
    }
    Population::~Population() {
        if (bestPlayer)
            delete bestPlayer;
    }
    void Population::updateAlive() {
        for (int i = 0; i < population.size();++i) {
            Player* p = population[i];
            if (!p->isDead()) {
                p->look();
                p->think();
                p->move();
                p->update();
            }
        }
        //* display is not implemented
    }
    bool Population::populationDone() {
        for (int i = 0; i < population.size();++i) {
            if (!population[i]->isDead()) {
                return false;
            }
        }
        return true;
    }
    void Population::naturalSelection() {
        calculateFitness();
        long double averageSum = getAverageScore();
        std::cout << "Average Sum of Generation " << generation << ": " << averageSum << std::endl;
        std::vector<Player*> children;
        fillMatingPool();
        for (int i = 0; i < population.size(); ++i) {
            Player* parent1 = selectPlayer();
            Player* parent2 = selectPlayer();
            if (parent1->getFitness() > parent2->getFitness()) {
                children.push_back(parent1->crossover(parent2));
            } else {
                children.push_back(parent2->crossover(parent1));
            }
        }
        deleteAllPlayer();
        population = children;
        ++generation;
        //
        for (int i = 0;i < population.size();++i) {
            population[i]->getBrain()->generateNetwork();
            children[i] = nullptr;
        }
        std::cout << "Generation: " << generation << " created" << std::endl;
    }
    void Population::calculateFitness() {
        long double currentMax = 0;
        for (int i = 0; i < population.size();++i) {
            Player* player = population[i];
            player->calculateFitness();
            long double fitness = player->getFitness();
            if (fitness > bestFitness) {
                bestFitness = fitness;
                if (bestPlayer) {
                    delete bestPlayer;
                    bestPlayer = nullptr;
                }
                bestPlayer = player->clone();
                // bestPlayer->getBrain()
            }

            if (fitness > currentMax) {
                currentMax = fitness;
            }
        }
        // normalize
        for (int i = 0; i < population.size(); ++i) {
            population[i]->setFitness(population[i]->getFitness() / currentMax);
        }
    }
    void Population::fillMatingPool() {
        matingPool.clear();
        for (int i = 0; i < population.size();++i) {
            const long int n = population[i]->getFitness() * 100;
            for (int j = 0; j < n; ++j) {
                matingPool.push_back(i);
            }
        }
    }
    Player* Population::selectPlayer() {
        return population[matingPool[floor(randNum(0, matingPool.size()))]];
    }
    long double Population::getAverageScore() {
        long int averageSum = 0;
        for (int i = 0; i < population.size();++i) {
            averageSum += population[i]->getScore();
        }
        return averageSum / population.size();
    }
    void Population::deleteAllPlayer() {
        for (int i = 0; i < population.size(); ++i) {
            delete population[i];
            population[i] = nullptr;
        }
    }
}