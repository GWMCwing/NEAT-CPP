#ifndef NEATCPP_GENOME_HEADER_H
#define NEATCPP_GENOME_HEADER_H

#include <vector>
#include <fstream>

#include "node.h"
#include "connection.h"
// #include "settings.h"
namespace neatCpp {
    class Genome {
    private:
        // members
        long int inputs; // number of inputs
        long int outputs; // number of outputs
        long int id; // id of the genome
        long int layers; // number of layers
        long int nextNode; // number of next node
        std::vector<Node*> nodes; // nodes of the genome
        std::vector<Connection*> connections; // connections of the genome
    public:
        //
        // methods
        /**
         * @brief Construct a new Genome object
         * @param _input number of inputs
         * @param _output number of outputs
         * @param _id id of the genome
         * @param _offSpring is this genome a offspring
         */
        Genome(long int _input, long int _output, long int _id, bool _offSpring);
        // 
        /**
         * @brief Destroy the Genome object, called by the player object
         * delete node then connection
         * fill node
         */
        ~Genome();
        // 
        /**
         * @brief generate network structure for feed forward
         *
         */
        void generateNetwork();
        // 
        /**
         * @brief feed forward the network
         *
         * @param input input vector
         * @return std::vector<long double> output vector
         */
        std::vector<long double> feedForward(const std::vector<long double>& input);
        //
        /**
         * @brief crossover two genomes
         *
         * @param partner partner genome
         * @return Genome offspring genome
         */
        Genome* crossover(const Genome* const partner);
        //
        /**
         * @brief mutate the genome
         *
         */
        void mutate();
        //
        /**
         * @brief add node to the network
         *
         */
        void addNode();
        //
        /**
         * @brief add connection to the network
         *
         */
        void addConnection();
        //
        /**
         * @brief
         *
         * @param innovationNumber innovation number of the connection
         * @param connections vector of connections to be checked
         * @return long int index of the connection, -1 if not found
         */
        long int commonConnection(long int innovationNumber, const std::vector<Connection*>& connections) const;
        //
        /**
         * @brief check if the nodes are connected
         *
         * @param node1 node 1
         * @param node2 node 2
         * @return true connected, false not connected
         */
        bool nodesConnected(Node* node1, Node* node2) const;
        //
        /**
         * @brief check if the genome is fully connected
         * @return true fully connected, false not fully connected
         */
        bool fullyConnected() const;
        //
        /**
         * @brief sort all nodes by layer
         *
         */
        void sortByLayer();
        //
        /**
         * @brief return a deep copy of this genome
         * @return Genome
         */
        Genome* clone() const;
        std::vector<Node*> cloneNodesVector() const;
        std::vector<Connection*> cloneConnectionVector(const std::vector<Node*>& nodeList) const;
        //
        /**
         * @brief Get the Node Index with the corresponding number
         *
         * @param number number of the node
         * @return long int index of the node, -1 if not found
        */
        long int getNode(long int number) const;
        //
        /**
         * @brief Get the computational weight of the network
         *
         * @return long double computational weight of the network
         */
        long double calculateWeight() const;
        void exportGenome(std::fstream& file);
        //
    };
}
#endif