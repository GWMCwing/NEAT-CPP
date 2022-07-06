#include <iostream>
#include <fstream>
#include "./population.h"
#include "../util/randomFun.h"
namespace neatCpp {
    Population::Population(int populationSize, int _inputSize, int _outputSize, OutputBuffer* _outputBuffer) {
        bestFitness = 0;
        generation = 0;
        bestPlayer = nullptr;
        outputBuffer = _outputBuffer;
        inputSize = _inputSize;
        outputSize = _outputSize;
        for (int i = 0;i < populationSize;++i) {
            // std::cout << "generating Player: " << i << std::endl;
            Player* p = new Player(i, _inputSize, _outputSize);
            p->getBrain()->generateNetwork();
            p->getBrain()->mutate();
            population.push_back(p);
        }
    }
    Population::~Population() {
        if (bestPlayer)
            delete bestPlayer;
        for (int i = 0; i < population.size();++i) {
            delete population[i];
        }
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

        // std::cout << "Average Sum of Generation " << generation << ": " << averageSum << std::endl;
        if (outputBuffer)
            outputBuffer->printLine(
                "Average Sum of Generation " + std::to_string(generation) + ": " + std::to_string(averageSum) + "\n"
                , OutputType::INFO
            );
        std::vector<Player*> children;
        fillMatingPool();
        for (int i = 0; i < population.size(); ++i) {
            Player* parent1 = selectPlayer();
            Player* parent2 = selectPlayer();
            // removed compare, already in crossover
            children.push_back(parent1->crossover(parent2));

        }
        removeAllPlayer();
        population = children;
        ++generation;
        //
        for (int i = 0;i < population.size();++i) {
            population[i]->getBrain()->generateNetwork();
        }
        // std::cout << "Generation: " << generation << " created" << std::endl;
        if (outputBuffer)
            outputBuffer->printLine(
                "Generation: " + std::to_string(generation) + " created\n"
                , OutputType::INFO
            );
    }
    //
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
        return population[matingPool[(int)(randNum(0, matingPool.size()))]];
    }
    long double Population::getAverageScore() const {
        long int averageSum = 0;
        for (int i = 0; i < population.size();++i) {
            averageSum += population[i]->getScore();
        }
        return averageSum / population.size();
    }
    int Population::getGeneration() const {
        return generation;
    }
    int Population::getPopulationSize() const { return population.size(); }
    std::vector<Player*> Population::getPopulationPlayers() const { return population; }
    long double Population::getBestPlayerScore() const {
        return bestPlayer ? bestPlayer->getScore() : -1;
    }
    void Population::removeAllPlayer() {
        for (int i = 0; i < population.size(); ++i) {
            delete population[i];
            population[i] = nullptr;
        }
    }
    //
    //
    //! ask for user to create or not before passing
    bool Population::exportPopulation(std::fstream& fileStream) const {
        /*
            exportVersion
            populationSize inputSize outputSize generation bestFitness
            (best player) (NULL if no best player)
            (all genome including best player)
                (genome)
                    numberOfNode numberOfConnection layers nextNode
                    (node) (single line)
                        layer isOutput activationFunctionIndex
                    (connection) (single line)
                        fromNode_number toNode_number weight enabled
        */
        fileStream << exportVersion << "\n";
        fileStream << population.size() << " " << inputSize << " " << outputSize << " "
            << generation << " " << bestFitness << "\n";
        //
        if (bestPlayer) {
            // fileStream << "BestPlayer: " << "\n";
            bestPlayer->exportPlayer(fileStream);
            fileStream << "\n";
        } else {
            fileStream << "NULL\n";
        }
        //
        for (int i = 0; i < population.size();++i) {
            // fileStream << "Population: " << i << "\n";
            population[i]->exportPlayer(fileStream);
            fileStream << "\n";
            // file << "Population: " << i << "End" << "\n";
        }
        return true;
    }
    bool Population::exportBestPlayer(std::fstream& fileStream) const {
        bestPlayer->exportPlayer(fileStream);
    }
    Population* Population::importPopulation(std::fstream& fileStream) {
        //TODO
    }
}