#ifndef UNIQUETABLE_H
#define UNIQUETABLE_H

#include "bddHash.h"
#include "bddNode.h"
#include <cassert>
#include <tuple>
#include <unordered_map>

/**
 * Global hash table (unique table) for storing BDD nodes.
 * Ensures that each BDD node is unique by storing nodes based on their
 * variable index and child pointers.
 *
 * @param key The tuple containing the variable, low node, and high node.
 * @return size_t The combined hash value.
 */
extern std::unordered_map<std::tuple<unsigned int, std::shared_ptr<bddNode>,
                                     std::shared_ptr<bddNode>>,
                          std::weak_ptr<bddNode>, UniqueTableHash>
    uniqueTable;

extern size_t maxNodes;

/**
 * @brief Creates a new BDD variable node with the specified variable and
 * decomposition type.
 *
 * @param var The variable index.
 * @param d The decomposition type (Shannon, positive Davio, negative Davio).
 * @return std::shared_ptr<bddNode> The newly created BDD node.
 */
std::shared_ptr<bddNode>
newVariable(const unsigned int var,
            const decompositionType d = decompositionType::S) noexcept;

// Terminale müssen nicht in die unique table
inline std::shared_ptr<bddNode> handleTerminals(unsigned int var) noexcept {
    assert(var == 0 || var == 1);
    return var == 0 ? terminal0 : terminal1;
}

/**
 * Finds or adds a BDD node to the unique table.
 * @param var The variable index of the node.
 * @param low The low child node.
 * @param high The high child node.
 * @return A shared pointer to the existing or newly created node.
 */
std::shared_ptr<bddNode>
findOrAdd(const unsigned int var, const std::shared_ptr<bddNode> low,
          const std::shared_ptr<bddNode> high) noexcept;

/**
 * Finds or adds a BDD node to the unique table using an existing node.
 * @param node The node to find or add.
 * @return A shared pointer to the existing or newly created node.
 */
std::shared_ptr<bddNode>
findOrAddNode(const std::shared_ptr<bddNode> node) noexcept;

/**
 * Prints the contents of the unique table for debugging purposes.
 */
void printUniqueTable() noexcept;

#endif // UNIQUETABLE_H