#include "node.h"
Node::~Node() {
    // prevent double deallocation when calling from ~Genome
    // connection will be deleted in ~Genome instead of ~Node
    const size_t sizeOfConnection = outputConnections.size();
    for (int i = 0; i < sizeOfConnection; ++i) {
        outputConnections[i] = nullptr;
    }
}