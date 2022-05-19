#include <cmath>
#include <algorithm>
#include "./header/randomFun.h"
#include "./header/genome.h"
#include "./header/node.h"
#include "./header/connection.h"
namespace neatCpp {
    //
    bool nodeCompare(Node*, Node*);
    //
    Genome::Genome(long int _input, long int _output, long int _id, bool _offSpring) {
        inputs = _input;
        outputs = _output;
        id = _id;
        layers = 2;
        nextNode = 0;

        connections = std::vector<Connection*>();
        nodes = std::vector<Node*>();


        if (!_offSpring) {
            for (long int i = 0; i < _input; ++i) {
                nodes.push_back(new Node(nextNode, 0, false));
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
            nodes[i]->clearOutputConnections();
        }
        // add the connection to the nodes
        const long int connectionsLen = connections.size();
        for (long int i = 0; i < nodesLen;i++) {
            Connection* connection = connections[i];
            connection->getFromNode()->pushBackConnections(connection);
        }
        // prepare for feed forward
        sortByLayer();
    }

    std::vector<long double> Genome::feedForward(const std::vector<long double>& input) {
        const long int nodesLen = nodes.size();
        for (long int i = 0; i < nodesLen; ++i) {
            nodes[i]->setInputSum(0);
        }
        // assign new inputs
        const long int inputLen = input.size();
        for (long int i = 0; i < inputLen; ++i) {
            nodes[i]->setOutputValue(input[i]);
        }

        // engage all nodes and Extract the results from the output nodes
        std::vector<long double> result;
        for (long int i = 0; i < nodesLen; ++i) {
            Node* node = nodes[i];
            node->engage();
            if (node->getOutput()) {
                result.push_back(node->getOutputValue());
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
            if (node->getOutput()) {
                Node* partnerNode = partner->nodes[partner->getNode(node->getNumber())];
                if (randNum(0, 1) < 0.5) {
                    node->setActivationFunctionIndex(partnerNode->getActivationFunctionIndex());
                    node->setBias(partnerNode->getBias());
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
                long int fromNodeIndex = offSpring->getNode(connection->getFromNode()->getNumber());
                long int toNodeIndex = offSpring->getNode(connection->getToNode()->getNumber());
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

                long int fromNodeIndex = offSpring->getNode(connection->getFromNode()->getNumber());
                long int toNodeIndex = offSpring->getNode(connection->getToNode()->getNumber());

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
        Node* newNode = new Node(nextNode, pickedConnection->getFromNode()->getLayer() + 1, false);
        for (long int i = 0; i < nodesLen;++i) {
            if (nodes[i]->getLayer() > pickedConnection->getFromNode()->getLayer()) {
                nodes[i]->increaseLayer(1);
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
        while (nodes[node1Index]->getLayer() == nodes[node2Index]->getLayer() || nodesConnected(nodes[node1Index], nodes[node2Index])) {
            node1Index = floor(randNum(0, nodesLen));
            node2Index = floor(randNum(0, nodesLen));
        }
        // switch nodes based on their layers, ensure node1 is on the left
        if (nodes[node1Index]->getLayer() > nodes[node2Index]->getLayer()) {
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

    bool Genome::fullyConnected() const {
        long int maxConnections = 0;
        std::vector<long int> nodesPerLayer = {};
        for (int i = 0; i < nodes.size(); ++i) {
            const Node* const node = nodes[i];
            const long int nodeLayer = node->getLayer();
            // size start from 0 - n, layer start from 1 - n+1
            if (nodesPerLayer.size() <= nodeLayer) {
                nodesPerLayer.reserve(nodeLayer);
                nodesPerLayer[nodeLayer - 1] = 1;
            } else {
                ++nodesPerLayer[nodeLayer - 1];
            }
        }
        for (int i = 0; i < layers - 1; ++i) {
            for (int j = i + 1; i < layers; ++j) {
                maxConnections += nodesPerLayer[i] * nodesPerLayer[j];
            }
        }
        return maxConnections == connections.size();
    }

    void Genome::sortByLayer() {
        //!=============================================
        std::sort(nodes.begin(), nodes.end(), nodeCompare);
    }

    Genome* Genome::clone() const {
        Genome* genome = new Genome(inputs, outputs, id);
        genome->nodes = cloneNodesVector();
        genome->connections = cloneConnectionVector(genome->nodes);
        return genome;
    }
    std::vector<Node*> Genome::cloneNodesVector() const {
        std::vector<Node*> rV = {};
        rV.reserve(nodes.size());
        for (int i = 0;i < nodes.size();++i) {
            rV[i] = nodes[i]->clone();
        }
        return rV;
    }
    std::vector<Connection*> Genome::cloneConnectionVector(const std::vector<Node*>& nodeList) const {
        //! add connection from node to node, same as the original
        std::vector<Connection*> rV = {};
        rV.reserve(connections.size());
        for (int i = 0;i < connections.size();++i) {
            const Connection* const origConn = connections[i];
            const long int nFNumber = origConn->getFromNode()->getNumber();
            const long int nTNumber = origConn->getToNode()->getNumber();
            Node* nF = nodeList[getNode(nFNumber)];
            Node* nT = nodeList[getNode(nTNumber)];
            Connection* conn = new Connection(nF, nT, origConn->getWeight());
            conn->setEnable(origConn->getEnabled());
        }
        return rV;
    }

    long int Genome::getNode(long int number)const {
        for (int i = 0; i < nodes.size();++i) {
            if (nodes[i]->getNumber() == number) {
                return i;
            }
        }
        return -1;
    }

    long double Genome::calculateWeight() const {
        return connections.size() + nodes.size();
    }

    bool nodeCompare(Node* n1, Node* n2) {
        return n1->getNumber() < n2->getNumber();
    }


}

