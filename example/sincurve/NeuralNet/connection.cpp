#include "../util/randomFun.h"

#include "./connection.h"
#include "./node.h"

namespace neatCpp {
    // extern long double randGaussian(double, double, double);
    // extern long double randNum(double, double);
    //
    Connection::Connection(Node* _fromNode, Node* _toNode, long double _weight) {
        fromNode = _fromNode;
        toNode = _toNode;
        weight = _weight;
        enabled = true;
        // std::cout << "Created Connection From: " << toNode->getNumber() << std::endl;
    }
    Connection::~Connection() {
        // std::cout << "Deleted Connection From: " << toNode->getNumber() << std::endl;
    }
    void Connection::mutateWeight() {
        const int randomNumber = randNum(0, 100);
        if (randomNumber < 5) {
            weight = randGaussian(-1, 1, 1);
        } else if (randomNumber < 10) {
            //TODO why 50
            weight += randGaussian(-1, 1, 1) / 50;
        }
    }
    Connection* Connection::clone() const {
        Connection* newConnection = new Connection(fromNode, toNode, weight);
        newConnection->enabled = enabled;
        return newConnection;
    }
    void Connection::exportConnection(std::fstream& file) const {
        file << fromNode->getNumber() << " " << toNode->getNumber()
            << " " << weight << " " << enabled;
    }
    long int Connection::getInnovationNumber() const {
        return (
            (1 / 2) * (fromNode->getNumber() + toNode->getNumber())
            * (fromNode->getNumber() + toNode->getNumber() + 1)
            + toNode->getNumber()
            );
    }
    //getter
    long double Connection::getWeight() const {
        return weight;
    }
    Node* Connection::getFromNode() const {
        return fromNode;
    }
    Node* Connection::getToNode() const {
        return toNode;
    }
    bool Connection::getEnabled() const {
        return enabled;
    }
    // setter
    void Connection::setFromNode(Node* _fromNode) {
        fromNode = _fromNode;
    }
    void Connection::setToNode(Node* _toNode) {
        toNode = _toNode;
    }
    void Connection::setWeight(long int _weight) {
        weight = _weight;
    }
    void Connection::setEnable(bool isEnabled) {
        enabled = isEnabled;
    }
}