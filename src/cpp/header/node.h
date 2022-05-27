#ifndef NEATCPP_NODE_HEADER_H
#define NEATCPP_NODE_HEADER_H

#include <vector>
#include <cmath>

#include "activationFunction.h"
#include "settings.h"

namespace neatCpp {
    // activation function
    //
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
        //
        // getter
        long int getNumber() const;
        long int getOutput() const;
        long int getOutputValue() const;
        int getActivationFunctionIndex() const;
        long double getBias() const;
        long int getLayer() const;
        // setter
        void clearOutputConnections();
        void pushBackConnections(Connection* connection);
        void setInputSum(long double value);
        void setOutputValue(long double value);
        void setActivationFunctionIndex(int index);
        void setBias(int biasValue);
        void increaseLayer(int value);
        //
    private:
        long int number; // number of the node
        long int layer; // layer of the node
        int activationFunctionIndex; // index of the activation function
        long double bias; // bias of the node
        bool output; // is the node an output node
        long double inputSum; // sum of the input
        long double outputValue; // output value of the node
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