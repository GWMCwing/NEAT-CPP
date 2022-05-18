#include "genome.h"
#include "node.h"
#include "connection.h"
#include "randomFun.h"
#include <cmath>

Genome::Genome(long int _input, long int _output, long int _id, bool _offSpring = false) {
    inputs = _input;
    outputs = _output;
    id = _id;
    layers = 2;
    nextNode = 0;

    connections = std::vector<Connection*>();
    nodes = std::vector<Node*>();


    if (!_offSpring) {
        for (long int i = 0; i < _input; ++i) {
            nodes.push_back(new Node(nextNode, 0));
            nextNode++;
        }

        for (long int i = 0; i < _output; ++i) {
            nodes.push_back(new Node(nextNode, 1, true));
            nextNode++;
        }

        for (long int i = 0; i < _input; ++i) {
            for (long int j = 0; j < _output + _input; ++j) {
                const long double weight = randNum(0, 1) * _input * sqrt(2.0 / (double)_input);
                connections.push_back(new Connection(nodes[i], nodes[j], weight));
            }
        }
    }
}

Genome::~Genome() {
    // delete node first to prevent double deallocation;
    const size_t sizeOfNodes = nodes.size();
    for (int i = 0; i < sizeOfNodes; ++i) {
        delete nodes[i];
        nodes[i] = nullptr;
    }
    const size_t sizeOfConnection = connections.size();
    for (int i = 0; i < sizeOfConnection; ++i) {
        delete connections[i];
        connections[i] = nullptr;
    }
}

void Genome::generateNetwork() {
    // clear all output connection in the nodes
    const long int nodesLen = nodes.size();
    for (long int i = 0; i < nodesLen;++i) {
        nodes[i]->outputConnections.clear();
    }
    // add the connection to the nodes
    const long int connectionsLen = connections.size();
    for (long int i = 0; i < nodesLen;i++) {
        Connection* connection = connections[i];
        connection->getFromNode()->outputConnections.push_back(connection);
    }
    // prepare for feed forward
    sortByLayer();
}

std::vector<long double> Genome::feedForward(const std::vector<long double>& input) {
    const long int nodesLen = nodes.size();
    for (long int i = 0; i < nodesLen; ++i) {
        nodes[i]->inputSum = 0;
    }
    // assign new inputs
    const long int inputLen = input.size();
    for (long int i = 0; i < inputLen; ++i) {
        nodes[i]->outputValue = input[i];
    }

    // engage all nodes and Extract the results from the output nodes
    std::vector<long double> result;
    for (long int i = 0; i < nodesLen; ++i) {
        Node* node = nodes[i];
        node->engage();
        if (node->output) {
            result.push_back(node->outputValue);
        }
    }

    return result;
}

Genome* Genome::crossover(const Genome* const partner) {

    Genome* offSpring = new Genome(inputs, outputs, 0, true);
    offSpring->nextNode = nextNode;

    // take all nodes from this parent - output node activation 50% - 50%
    const long int nodesLen = nodes.size();
    for (long int i = 0; i < nodesLen; ++i) {
        Node* node = nodes[i]->clone();
        if (node->output) {
            Node* partnerNode = partner->nodes[partner->getNode(node->number)];
            if (randNum(0, 1) < 0.5) {
                node->activationFunctionIndex = partnerNode->activationFunctionIndex;
                node->bias = partnerNode->bias;
            }
        }
        offSpring->nodes.push_back(node);
    }

    // Randomly take connections form this or the partner network
    //? never used
    const long int maxLayer = 0;
    const long int connectionLen = connections.size();
    for (long int i = 0; i < connectionLen; ++i) {
        const long int index = commonConnection(connections[i]->getInnovationNumber(), partner->connections);

        if (index != -1) {
            // have common connections

            Connection* connection = randNum(0, 1) < 0.5 ? connections[i]->clone() : partner->connections[index]->clone();

            // reassign nodes
            long int fromNodeIndex = offSpring->getNode(connection->getFromNode()->number);
            long int toNodeIndex = offSpring->getNode(connection->getToNode()->number);
            if (fromNodeIndex != -1 && toNodeIndex != -1) {

                Node* fromNode = offSpring->nodes[fromNodeIndex];
                Node* toNode = offSpring->nodes[toNodeIndex];

                connection->setFromNode(fromNode);
                connection->setToNode(toNode);

                offSpring->connections.push_back(connection);
            }

        } else {
            // no common connection => take from this
            Connection* connection = connections[i]->clone();

            long int fromNodeIndex = offSpring->getNode(connection->getFromNode()->number);
            long int toNodeIndex = offSpring->getNode(connection->getToNode()->number);

            if (fromNodeIndex != -1 && toNodeIndex != -1) {

                Node* fromNode = offSpring->nodes[fromNodeIndex];
                Node* toNode = offSpring->nodes[toNodeIndex];

                connection->setFromNode(fromNode);
                connection->setToNode(toNode);

                offSpring->connections.push_back(connection);
            }
        }
    }

    offSpring->layers = layers;
    return offSpring;
}

void Genome::mutate() {
    const long int connectionLen = connections.size();
    const long int nodesLen = nodes.size();
    if (randNum(0, 1) < 0.8) {
        for (long int i = 0; i < connectionLen; ++i) {
            connections[i]->mutateWeight();
        }
    }
    if (randNum(0, 1) < 0.5) {
        for (long int i = 0; i < nodesLen; ++i) {
            nodes[i]->mutateBias();
        }
    }
    if (randNum(0, 1) < 0.1) {
        const long int i = floor(randNum(0, nodesLen));
        nodes[i]->mutateActivationFunction();
    }
    if (randNum(0, 1) < 0.01) {
        addNode();
    }
}

void Genome::addNode() {
    // add node to network
    // get a random connection to replace with a node
    const long int connectionLen = connections.size();
    const long int connectionIndex = floor(randNum(0, connectionLen));
    const long int nodesLen = nodes.size();
    Connection* pickedConnection = connections[connectionIndex];
    // delete connection
    connections.erase(connections.begin() + connectionIndex);

    // create new node
    Node* newNode = new Node(nextNode, pickedConnection->getFromNode()->layer + 1);
    for (long int i = 0; i < nodesLen;++i) {
        if (nodes[i]->layer > pickedConnection->getFromNode()->layer) {
            ++nodes[i]->layer;
        }
    }

    // new connections
    Connection* newConnection1 = new Connection(pickedConnection->getFromNode(), newNode, 1);
    Connection* newConnection2 = new Connection(newNode, pickedConnection->getToNode(), pickedConnection->getWeight());

    ++layers;
    connections.push_back(newConnection1);
    connections.push_back(newConnection2);
    nodes.push_back(newNode);
    ++nextNode;
}

void Genome::addConnection() {
    if (fullyConnected()) return;
    const long int nodesLen = nodes.size();
    // choose nodes to connect
    long int node1Index = floor(randNum(0, nodesLen));
    long int node2Index = floor(randNum(0, nodesLen));
    //TODO factorize
    while (nodes[node1Index]->layer == nodes[node2Index]->layer || nodesConnected(nodes[node1Index], nodes[node2Index])) {
        node1Index = floor(randNum(0, nodesLen));
        node2Index = floor(randNum(0, nodesLen));
    }
    // switch nodes based on their layers, ensure node1 is on the left
    if (nodes[node1Index]->layer > nodes[node2Index]->layer) {
        long int tmp = node1Index;
        node1Index = node2Index;
        node2Index = tmp;
    }

    // add connection
    Connection* newConnection = new Connection(nodes[node1Index], nodes[node2Index], randNum(0, 1) * inputs * sqrt(2 / inputs));
    connections.push_back(newConnection);
}

long int Genome::commonConnection(long int innovationNumber, const std::vector<Connection* >& connections) const {

    const long int connectionLen = connections.size();
    for (long int i = 0; i < connectionLen; ++i) {
        if (connections[i]->getInnovationNumber() == innovationNumber) {
            return i;
        }
    }
    return -1;
}

bool Genome::nodesConnected(Node* node1, Node* node2) const {
    const long int connectionLen = connections.size();
    for (long int i = 0; i < connectionLen; ++i) {
        const Connection* conn = connections[i];
        if (
            ((conn->getFromNode()) == node1 && (conn->getToNode()) == node2) ||
            ((conn->getFromNode()) == node2 && (conn->getToNode()) == node1)
            ) {
            return true;
        }
    }
    return false;
}

bool Genome::fullyConnected() const { }

void Genome::sortByLayer() { }

Genome* Genome::clone() const { }

long int getNode(long int index) { }

long double Genome::calculateWeight() const { }






