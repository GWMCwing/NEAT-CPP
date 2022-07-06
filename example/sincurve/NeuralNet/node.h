#ifndef NEATCPP_NODE_HEADER_H
#define NEATCPP_NODE_HEADER_H

#include <vector>
#include <cmath>
#include <fstream>

#include "../util/activationFunction.h"
// #include "../header/settings.h"

namespace neatCpp {
    // forward declaration
    class Connection;
    class Node {
    public:
        //
        // methods
        /**
         * @brief Construct a new Node object
         *
         * @param _number number of the node
         * @param _layer layer of the node
         * @param _isOutput is the node an output node
         *
         */
        Node(long int _number, long int _layer, bool _isOutput);
        //
        /**
         * @brief Destroy the Node object, ONLY called by the genome object
         *
         */
        ~Node();
        //
        /**
         * @brief pass down the calculated output value
         *
         */
        void engage();
        // 
        /**
         * @brief randomly mutate the bias of this node
         *
         */
        void mutateBias();
        //
        /**
         * @brief randomly mutate a new activation function
         *
         */
        void mutateActivationFunction();
        //
        /**
         * @brief check this and node is connected
         * @param node node to check
         * @return true if connected, false if not connected
         *
         */
        bool isConnected(const Node* const node) const;
        //
        /**
         * @brief return a deep copy of this node
         * @return Node*
         *
         */
        Node* clone() const;
        /**
         * @brief export node data to given fstream
         *
         * @param fileStream fstream to be exported to
         */
        void exportNode(std::fstream& fileStream) const;
        /**
         * @brief clear all output connections in connection vector
         *
         */
        void clearOutputConnections();
        //
        // list of  getter

        long int getNumber() const;
        long int getOutput() const;
        long int getActivatedOutputValue() const;
        int getActivationFunctionIndex() const;
        long double getBias() const;
        long int getLayer() const;
        //
        // list of setter

        void removeConnection(Connection* connection);
        void pushBackConnections(Connection* connection);
        void setInputSum(long double value);
        void setActivatedOutputValue(long double value);
        void setActivationFunctionIndex(int index);
        void setBias(int biasValue);
        void increaseLayer(int value);
        //
    private:
        /** node number */
        long int number;
        /** layer of node */
        long int layer; // layer of the node
        /** index of activation function */
        int activationFunctionIndex; // index of the activation function
        /** bias of the node */
        long double bias; // bias of the node
        /** is the node an output node */
        bool output; // is the node an output node
        /** sum of inputs */
        long double inputSum; // sum of the input
        /** activated value of the output value */
        long double activatedOutputValue; // output value of the node
        /** vector of output connection from this node */
        std::vector<Connection*> outputConnections; // vector of output connections of the node, !! do not delete it in destruction !!
        //
        /**
         * @brief return activation function
         * @return auto activation function
         * ? https://stackoverflow.com/questions/31387238/c-function-returning-function
         */
         // fptrLDouble Node::getActivation(int index) const;
    };
}
#endif