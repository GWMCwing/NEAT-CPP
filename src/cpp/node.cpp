#include "node.h"
#include "connection.h"
#include "randomFun.h"
namespace neatCpp {
    Node::Node(long int _number, long int _layer, bool _isOutput = false) {
        number = _number;
        layer = _layer;
        output = _isOutput;
        activationFunctionIndex = floor(randNum(0, numberOfActivation));

        inputSum = 0;
        outputValue = 0;
        outputConnections = std::vector<Connection*>();
    }
    //
    Node::~Node() {
        // prevent double deallocation when calling from ~Genome
        // connection will be deleted in ~Genome instead of ~Node
        const size_t sizeOfConnection = outputConnections.size();
        for (int i = 0; i < sizeOfConnection; ++i) {
            outputConnections[i] = nullptr;
        }
    }
    //
    void Node::engage() {
        if (layer != 0) {
            outputValue = getActivation(activationFunctionIndex)(inputSum + bias);
        }
        const long int connectionSize = outputConnections.size();
        for (int i = 0; i < connectionSize; ++i) {
            const Connection* const conn = outputConnections[i];
            if (conn->getEnabled()) {
                conn->getToNode()->inputSum += conn->getWeight() * outputValue;
            }
        }
    }
    void Node::mutateBias() {
        if (randNum(0, 1) < 0.05) {
            bias = randNum(-1, 1);
            return;
        }
        bias += randGaussian(-1, 1) / 50;
    }
    void Node::mutateActivationFunction() {
        activationFunctionIndex = randNum(0, 1) * numberOfActivation;
    }
    bool Node::isConnected(const Node* const node) const {
        if (node->getLayer() == layer) { return false; }
        if (node->getLayer() < layer) {
            for (int i = 0; i < node->outputConnections.size(); ++i) {
                if (node->outputConnections[i]->getToNode() == this) {
                    return true;
                }
            }
            return false;
        }
        for (int i = 0; i < node->outputConnections.size(); ++i) {
            if (outputConnections[i]->getToNode() == node) {
                return true;
            }
        }
        return false;
    }
    Node* Node::clone()const {
        Node* node = new Node(number, layer, output);
        node->bias = bias;
        node->activationFunctionIndex = activationFunctionIndex;
        return node;
    }
    //
    // setter
    void Node::clearOutputConnections() { outputConnections.clear(); }
    void Node::pushBackConnections(Connection* connection) { outputConnections.push_back(connection); }
    void Node::setInputSum(long double value) { inputSum = value; }
    void Node::setOutputValue(long double value) { outputValue = value; }
    void Node::setActivationFunctionIndex(int index) { activationFunctionIndex = index; }
    void Node::setBias(int biasValue) { bias = biasValue; }
    void Node::increaseLayer(int value) { layer += value; }
    //
    // getter
    long int Node::getNumber() const { return number; }
    long int Node::getOutput() const { return output; }
    long int Node::getOutputValue() const { return outputValue; }
    int Node::getActivationFunctionIndex() const { return activationFunctionIndex; }
    long double Node::getBias() const { return bias; }
    long int Node::getLayer() const { return layer; }

}