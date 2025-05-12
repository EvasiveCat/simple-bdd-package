#ifndef OKFDD_H
#define OKFDD_H

#include "bddHash.h"
#include "ite.h"

/**
 * Global computed table for caching results of xor operations.
 * Prevents redundant computations by storing previously computed results.
 */
extern std::unordered_map<
    std::tuple<std::weak_ptr<bddNode>, std::weak_ptr<bddNode>>,
    std::weak_ptr<bddNode>, Tuple2WeakHash, Tuple2WeakEqual>
    computedTableXor;

std::shared_ptr<bddNode>
kfddExorKfdd(const std::shared_ptr<bddNode> F,
             const std::shared_ptr<bddNode> G) noexcept;

std::shared_ptr<bddNode>
terminalCases(const std::shared_ptr<bddNode> F,
              const std::shared_ptr<bddNode> G) noexcept;

void garbageCollectXorTable() noexcept;

#endif // OKFDD_H