#ifndef NEATCPP_GENOME_HEADER_H
#define NEATCPP_GENOME_HEADER_H

#include <vector>
#include <fstream>

#include "./node.h"
#include "./connection.h"
// #include "settings.h"
namespace neatCpp {
    class Genome {
    public:
        // methods
        /**
         * @brief Construct a new Genome object
         * @param _input number of inputs
         * @param _output number of outputs
         * @param _id id of the genome
         * @param _offSpring is this genome a offspring
         */
        Genome(long int _input, long int _output, long int _id, bool _offSpring);
        /**
         * @brief Destroy the Genome object, called by the player object
         * delete node then connection
         * fill node
         */
        ~Genome();
        /**
         * @brief generate network structure for feed forward
         *
         */
        void generateNetwork();
        /**
         * @brief feed forward the network
         *
         * @param input input vector
         * @return std::vector<long double> output vector
         */
        std::vector<long double> feedForward(const std::vector<long double>& input);
        /**
         * @brief crossover two genomes and return a pointer to an offspring
         *
         * @param partner partner genome
         * @return Genome offspring genome
         */
        Genome* crossover(const Genome* const partner);
        /**
         * @brief mutate the genome
         *
         */
        void mutate();
        /**
         * @brief randomly add a node to the network
         *
         */
        void addNode();
        /**
         * @brief randomly add a connection to the network
         *
         */
        void addConnection();
        /**
         * @brief check if the innovationNumber is correspond to a connection in this genome, and return it's index in the conenction vector
         *
         * @param innovationNumber innovation number of the connection
         * @param connections vector of connections to be checked
         * @return long int index of the connection, -1 if not found
         */
        long int commonConnection(const long int& innovationNumber, const std::vector<Connection*>& connections) const;
        /**
         * @brief check if the nodes are connected, does not detect if the connection is enabled or not
         *
         * @param node1 node 1
         * @param node2 node 2
         * @return true connected, false not connected
         */
        bool nodesConnected(Node* node1, Node* node2) const;
        /**
         * @brief check if the genome is fully connected
         * @return true fully connected, false not fully connected
         */
        bool fullyConnected() const;
        /**
         * @brief sort all nodes by layer
         *
         */
        void sortByLayer();
        /**
         * @brief return a deep copy of this genome
         * @return Genome pointer to the copy of the genome
         */
        Genome* clone() const;
        /**
         * @brief deep copy of the Nodes vector, return vector of the cloned Node pointer.
         *
         * @return std::vector<Node*>
         */
        std::vector<Node*> cloneNodesVector() const;
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
        /**
         * @brief export genome data to fileStream
         *
         * @param fileStream fstream data to be exported to
         */
        void exportGenome(std::fstream& fileStream);
        //
    private:
        // members
        /** number of inputs */
        long int inputs;
        /** number of outputs */
        long int outputs;
        /** id of the genome */
        long int id;
        /** number of layers the genome have */
        long int layers;
        /** the identifier number of next node is going to be*/
        long int nextNode;
        /** vector of storing all nodes */
        std::vector<Node*> nodes;
        /** vector of storing all connections within the genome */
        std::vector<Connection*> connections;
        /**
         * @brief deep copy of the Connection vector with reference of the input nodeList , return vector of the cloned Connection pointer
         *
         * @param nodeList vector of Node pointer that connections in connection vector should be pointed to
         * @return std::vector<Connection*>
         */
        std::vector<Connection*> cloneConnectionVector(const std::vector<Node*>& nodeList) const;
        /**
         * @brief compare the layer of two nodes
         *
         * @param node1 first node
         * @param node2 second node
         * @return true if the first node is in a lower layer number, false otherwise
         */
        static bool nodeCompare(Node* node1, Node* node2);
    };
}
#endif