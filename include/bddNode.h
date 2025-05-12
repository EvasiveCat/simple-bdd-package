#ifndef BDDNODE_H
#define BDDNODE_H

#include "dtl.h"
#include <iostream>
#include <memory>

/**
 * Represents a node in a Binary Decision Diagram (BDD).
 * Each node has a variable index (`var`) and two child nodes (`low`, `high`).
 * Supports shared ownership via `std::shared_ptr`.
 */
class bddNode : public std::enable_shared_from_this<bddNode> {
  public:
    unsigned int var;              // Variable index associated with this node.
    std::shared_ptr<bddNode> low;  // Pointer to the low child (false branch).
    std::shared_ptr<bddNode> high; // Pointer to the high child (true branch).
    // bool negated; // If the edge pointing to this node is negated

    /**
     * Constructs a BDD node with a variable index and two child nodes.
     * @param l Low child node.
     * @param h High child node.
     * @param xi Variable index.
     */
    bddNode(std::shared_ptr<bddNode> l, std::shared_ptr<bddNode> h,
            unsigned int xi)          //, bool negated)
        : var(xi), low(l), high(h) {} //, negated(negated) {}

    /**
     * Prints the node's structure inline (e.g., for debugging).
     */
    void inlinePrint() const;

    /**
     * Prints the node's structure in DOT format for graph visualization.
     * @param showAddresses If true, includes memory addresses in the output.
     */
    void dotPrint(bool convertToPNG = false, bool showAddresses = false,
                  std::string heading = "") const;

    /**
     * Compares two BDD nodes for equality.
     * @param other The node to compare with.
     * @return True if the nodes are equal, false otherwise.
     */
    bool operator==(const bddNode &other) const;

    /**
     * Outputs the node's structure to an output stream.
     * @param os The output stream.
     * @param node The node to print.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const bddNode &node);
};

/**
 * Checks if a BDD node is a terminal node (leaf).
 * @param node The node to check.
 * @return True if the node is a terminal, false otherwise.
 */
bool isTerminal(const std::shared_ptr<bddNode> node);

/**
 * Performs a logical AND operation on two BDDs.
 * @param F The first BDD.
 * @param G The second BDD.
 * @return The resulting BDD.
 */
std::shared_ptr<bddNode> operator*(const std::shared_ptr<bddNode> F,
                                   const std::shared_ptr<bddNode> G);

/**
 * Performs a logical OR operation on two BDDs.
 * @param F The first BDD.
 * @param G The second BDD.
 * @return The resulting BDD.
 */
std::shared_ptr<bddNode> operator+(const std::shared_ptr<bddNode> F,
                                   const std::shared_ptr<bddNode> G);

/**
 * Performs a logical NOT operation on a BDD.
 * @param F The BDD to negate.
 * @return The resulting BDD.
 */
std::shared_ptr<bddNode> operator~(const std::shared_ptr<bddNode> F);

/**
 * Performs a logical XOR operation on two BDDs.
 * @param F The first BDD.
 * @param G The second BDD.
 * @return The resulting BDD.
 */
std::shared_ptr<bddNode> operator^(const std::shared_ptr<bddNode> F,
                                   const std::shared_ptr<bddNode> G);

// Terminal nodes representing constants 0 and 1.
extern const std::shared_ptr<bddNode> terminal0; // Terminal node for 0 (false).
extern const std::shared_ptr<bddNode> terminal1; // Terminal node for 1 (true).

#endif // BDDNODE_H