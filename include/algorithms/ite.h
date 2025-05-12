#ifndef ITE_H
#define ITE_H

#include "bddHash.h"
#include "bddNode.h"
#include "variableOrdering.h"
#include <algorithm>

/**
 * Performs the If-Then-Else (ITE) operation on three ROKFDDs.
 * ITE(F, G, H) = (F ∧ G) ∨ (¬F ∧ H).
 * @param F The condition ROKFDD.
 * @param G The then ROKFDD.
 * @param H The else ROKFDD.
 * @return The resulting ROKFDD.
 */
std::shared_ptr<bddNode> iteKfdd(const std::shared_ptr<bddNode> F,
                                 const std::shared_ptr<bddNode> G,
                                 const std::shared_ptr<bddNode> H) noexcept;

/**
 * Checks for terminal cases in the ITE operation to simplify computation.
 * @param F The condition BDD.
 * @param G The then BDD.
 * @param H The else BDD.
 * @return A simplified BDD if a terminal case is found, otherwise nullptr.
 */
inline std::shared_ptr<bddNode>
checkTerminalCases(const std::shared_ptr<bddNode> F,
                   const std::shared_ptr<bddNode> G,
                   const std::shared_ptr<bddNode> H) noexcept {
    if (*F == *terminal0)
        return H;
    if (*F == *terminal1 || *G == *H)
        return G;
    if (*G == *terminal1 && *H == *terminal0)
        return F;
    return nullptr;
}

/**
 * Computes the cofactor of a BDD with respect to a variable.
 * @param var The variable index.
 * @param node The BDD node.
 * @param polarity If true, computes the positive cofactor, else the negative.
 * @return The resulting cofactor BDD.
 */
inline std::shared_ptr<bddNode> cofactor(const unsigned int x_i,
                                         const std::shared_ptr<bddNode> node,
                                         const bool positive) noexcept {
    if (isTerminal(node) || node->var != x_i)
        return node;
    // Shannon: low = f_0, high = f_1
    if (dtl[x_i] == decompositionType::S)
        return positive ? node->high : node->low;
    // positive Davio: low = f_0, high = f_0 ^ f_1
    if (dtl[x_i] == decompositionType::pD)
        return positive ? node->high ^ node->low : node->low;
    // negative Davio: low = f_1, high = f_0 ^ f_1
    return positive ? node->low : node->low ^ node->high;
}

/**
 * Determines the top variable among three BDDs (the smallest variable index).
 * @param F The first BDD.
 * @param G The second BDD.
 * @param H The third BDD.
 * @return The top variable index.
 */
inline unsigned int topVariable(const std::shared_ptr<bddNode> F,
                                const std::shared_ptr<bddNode> G,
                                const std::shared_ptr<bddNode> H) noexcept {
    unsigned int f = isTerminal(F) ? std::numeric_limits<unsigned int>::max()
                                   : getVariablePriority(F->var);
    unsigned int g = isTerminal(G) ? std::numeric_limits<unsigned int>::max()
                                   : getVariablePriority(G->var);
    unsigned int h = isTerminal(H) ? std::numeric_limits<unsigned int>::max()
                                   : getVariablePriority(H->var);

    if (f <= g && f <= h) {
        return F->var;
    }

    if (g <= h && g <= f) {
        return G->var;
    }

    return H->var;
}

/**
 * Global computed table for caching results of ITE operations.
 * Prevents redundant computations by storing previously computed results.
 */
extern std::unordered_map<
    std::tuple<std::weak_ptr<bddNode>, std::weak_ptr<bddNode>,
               std::weak_ptr<bddNode>>,
    std::weak_ptr<bddNode>, Tuple3WeakHash, Tuple3WeakEqual>
    computedTable; // Cpp ist wunderschön

void garbageCollectComputedTable() noexcept;

#endif // ITE_H