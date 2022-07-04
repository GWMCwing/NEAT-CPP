#include "../util/randomFun.h"

#include "./node.h"
#include "./connection.h"
namespace neatCpp {
    //
    // definition of activation function
    // modify the numberOfActivation in settings.h if needed
    //
    extern const int numberOfActivation;
    Node::Node(long int _number, long int _layer, bool _isOutput) {
        number = _number;
        layer = _layer;
        output = _isOutput;
        activationFunctionIndex = randNum(0, numberOfActivation);

        inputSum = 0;
        activatedOutputValue = 0;
        outputConnections = std::vector<Connection*>();
        // std::cout << "created Node: " << _number << std::endl;
    }
    //
    Node::~Node() {
        // std::cout << "Deleted Node: " << number << std::endl;
    }
    //
    void Node::engage() {
        if (layer != 0) {
            activatedOutputValue = activationGetValue(activationFunctionIndex, inputSum + bias);
        }
        const long int connectionSize = outputConnections.size();
        for (int i = 0; i < connectionSize; ++i) {
            const Connection* const conn = outputConnections[i];
            if (conn->getEnabled()) {
                conn->getToNode()->inputSum += conn->getWeight() * activatedOutputValue;
            }
        }
    }
    void Node::mutateBias() {
        if (randNum(0, 1) < 0.05) {
            bias = randNum(-1, 1);
            return;
        }
        //? why 50
        bias += randGaussian(-1, 1, 1) / 50;
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
    void Node::exportNode(std::fstream& file) const {
        file << layer << " " << output << " " << activationFunctionIndex;
    }
    //
    // setter
    void Node::clearOutputConnections() { outputConnections.clear(); }
    void Node::pushBackConnections(Connection* connection) { outputConnections.push_back(connection); }
    void Node::setInputSum(long double value) { inputSum = value; }
    void Node::setActivatedOutputValue(long double value) { activatedOutputValue = value; }
    void Node::setActivationFunctionIndex(int index) { activationFunctionIndex = index; }
    void Node::setBias(int biasValue) { bias = biasValue; }
    void Node::increaseLayer(int value) { layer += value; }
    //
    // getter
    long int Node::getNumber() const { return number; }
    long int Node::getOutput() const { return output; }
    long int Node::getActivatedOutputValue() const { return activatedOutputValue; }
    int Node::getActivationFunctionIndex() const { return activationFunctionIndex; }
    long double Node::getBias() const { return bias; }
    long int Node::getLayer() const { return layer; }
    //
    /*
    fptrLDouble Node::getActivation(int index) const {
        switch (index) {
            case 0:
                return sigmoid; break;
            case 1:
                return identity; break;
            case 2:
                return step; break;
            case 3:
                return tanh; break;
            case 4:
                return ReLU; break;
            default:
                return sigmoid; break;
        }
    }
    */
}