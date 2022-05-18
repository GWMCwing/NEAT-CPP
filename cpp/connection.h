#ifndef CONNECTION_HEADER_H
#define CONNECTION_HEADER_H
class Node;

/**
 * @brief class for a connection between node
 *
 */
class Connection {
private:
    long double weight; // weight of the connection
    Node* fromNode; // input node, !! do not delete it in destruction !!
    Node* toNode; // output node, !! do not delete it in destruction !!
    bool enabled; // is the connection enabled
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
    //getter
    long double getWeight() const;
    Node* getFromNode() const;
    Node* getToNode() const;
    bool getEnabled() const;
    // setter
    void setFromNode(const Node* const fromNode);
    void setToNode(const Node* const toNode);
    void setWeight(const long int weight);
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
    /**
     * @brief Get the innovation number of this connection
     *
     * @return long int
     */
    long int getInnovationNumber() const;
};
#endif