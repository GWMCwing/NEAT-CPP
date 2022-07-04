#ifndef NEATCPP_CONNECTION_HEADER_H
#define NEATCPP_CONNECTION_HEADER_H
// #include <iostream>
#include <fstream>
namespace neatCpp {
    class Node;

    /**
     * @brief class for a connection between node
     *
     */
    class Connection {
    private:
        /** weight of the connection */
        long double weight;
        /** pointer to input node, !! do not delete it in destruction !!*/
        Node* fromNode;
        /** pointer to output node, !! do not delete it in destruction !!*/
        Node* toNode;
        /** is the connection enabled*/
        bool enabled;
        // 
        // methods
    public:
        /**
         * @brief Construct a new Connection object
         *
         * @param _fromNode input node
         * @param _toNode output node
         * @param _weight weight of the connection
         */
        Connection(Node* _fromNode, Node* _toNode, long double _weight);
        //
        /**
         * @brief Destroy the Connection object, called by the node object
         *
         */
        ~Connection();
        //
        //
        /**
         * @brief mutate the weight of the connection
         *
         */
        void mutateWeight();
        //
        /**
         * @brief return a deep copy of this connection
         *
         * @return Connection
         */
        Connection* clone() const;
        //
        void exportConnection(std::fstream& file) const;
        //
        /**
         * @brief Get the innovation number of this connection
         *
         * @return long int
         */
        long int getInnovationNumber() const;
        //
        // list of getter
        long double getWeight() const;
        Node* getFromNode() const;
        Node* getToNode() const;
        bool getEnabled() const;
        //
        // list of setter
        void setFromNode(Node* _fromNode);
        void setToNode(Node* _toNode);
        void setWeight(long int _weight);
        void setEnable(bool isEnabled);
        //
    };
}
#endif