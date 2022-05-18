#include "connection.h"
#include "randomFun.h"
#include "node.h"

Connection::Connection(Node* _fromNode, Node* _toNode, long double _weight) {
    fromNode = _fromNode;
    toNode = _toNode;
    weight = _weight;
    enabled = true;
}
Connection::~Connection() { }
void Connection::mutateWeight() {
    const int randomNumber = randNum(0, 100);
    if (randomNumber < 5) {
        weight = randGaussian(-1, 1);
    } else if (randomNumber < 10) {
        //TODO why 50
        weight += randGaussian(-1, 1) / 50;
    }
}
Connection* Connection::clone() const {
    Connection* newConnection = new Connection(fromNode, toNode, weight);
    newConnection->enabled = enabled;
    return newConnection;
}
long int Connection::getInnovationNumber() const {
    return (
        (1 / 2) * (fromNode->number + toNode->number) * (fromNode->number + toNode->number + 1) + toNode->number
        );
}