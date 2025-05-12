#ifndef BDDTOKFDD_H
#define BDDTOKFDD_H

#include "bddHash.h"
#include "bddNode.h"
#include <unordered_map>

/**
 * Global computed table for caching results of xor operations.
 * Prevents redundant computations by storing previously computed results.
 */
extern std::unordered_map<std::tuple<std::weak_ptr<bddNode>>,
                          std::weak_ptr<bddNode>, Tuple1WeakHash,
                          Tuple1WeakEqual>
    computedTableBddToKfdd;

/**
 * @brief Converts a Binary Decision Diagram (BDD) to a Kronecker Functional
 * Decision Diagram (KFDD).
 *
 * This function recursively transforms a BDD node `F` into its corresponding
 * KFDD representation. It handles terminal cases using the `terminalCases`
 * function and uses a computed table
 * (`computedTableBddToKfdd`) to avoid redundant computations. The function also
 * applies decomposition rules based on the decomposition type
 * (`decompositionType`) of the variable.
 *
 * @param F The BDD node to convert to a KFDD.
 * @return A shared pointer to the resulting KFDD node.
 *
 * @note The function assumes the existence of the following:
 *       - `isTerminal(F)`: A function to check if `F` is a terminal node.
 *       - `computedTableBddToKfdd`: A computed table to store and retrieve
 * intermediate results.
 *       - `topVariable(F, terminal0, terminal0)`: A function to determine the
 * top variable of `F`.
 *       - `dtl[x_i]`: A decomposition type lookup table for variable `x_i`.
 *       - `cofactor(x_i, F, value)`: A function to compute the cofactor of `F`
 * with respect to `x_i`.
 *       - `findOrAdd(x_i, R_low, R_high)`: A function to find or add a new KFDD
 * node.
 *       - `terminal0`: A shared pointer to the terminal node representing
 * logical false.
 */
std::shared_ptr<bddNode> bddToKfdd(const std::shared_ptr<bddNode> F) noexcept;

/**
 * @brief Handles terminal cases for a given BDD node.
 *
 * This function checks if the given BDD node `F` is a terminal node (e.g., a
 * constant). If `F` is a terminal node, it is returned directly. Otherwise, the
 * function returns `nullptr`.
 *
 * @param F The BDD node to check for terminal cases.
 * @return A shared pointer to the terminal node if `F` is a terminal;
 * otherwise, `nullptr`.
 *
 * @note This is a helper function in for the bddToKfdd algorithm
 *       to handle base cases.
 */
std::shared_ptr<bddNode>
terminalCases(const std::shared_ptr<bddNode> F) noexcept;

void garbageCollectBddToKfddTable() noexcept;

#endif // BDDTOKFDD_H