#include "ite.h"
#include "kfddXOR.h"
#include "uniqueTable.h"
#include <cassert>

decltype(computedTable) computedTable;

std::shared_ptr<bddNode> iteKfdd(const std::shared_ptr<bddNode> F,
                                 const std::shared_ptr<bddNode> G,
                                 const std::shared_ptr<bddNode> H) noexcept {
    const std::shared_ptr<bddNode> terminalCases = checkTerminalCases(F, G, H);
    if (terminalCases) {
        return terminalCases;
    }

    // Check computed table
    auto key =
        std::make_tuple(std::weak_ptr(F), std::weak_ptr(G), std::weak_ptr(H));
    if (auto it = computedTable.find(key); it != computedTable.end()) {
        if (auto cached = it->second.lock())
            return cached;
        else
            computedTable.erase(it); // Clean expired entries on access
    }

    // Cofactor calculation
    const unsigned int x_i = topVariable(F, G, H);

    const auto F_cof_high = cofactor(x_i, F, true);
    const auto G_cof_high = cofactor(x_i, G, true);
    const auto H_cof_high = cofactor(x_i, H, true);
    const auto R_high =
        findOrAddNode(iteKfdd(F_cof_high, G_cof_high, H_cof_high));

    const auto F_cof_low = cofactor(x_i, F, false);
    const auto G_cof_low = cofactor(x_i, G, false);
    const auto H_cof_low = cofactor(x_i, H, false);
    const auto R_low = findOrAddNode(iteKfdd(F_cof_low, G_cof_low, H_cof_low));

    // Do stuff based on decomposition
    const decompositionType dt = dtl[x_i];
    std::shared_ptr<bddNode> new_low, new_high;

    switch (dt) {
    case decompositionType::S:
        new_low = R_low;
        new_high = R_high;
        break;
    case decompositionType::pD:
        new_low = R_low;
        new_high = R_low ^ R_high;
        break;
    case decompositionType::nD:
        new_low = R_high;
        new_high = R_high ^ R_low;
        break;
    }

    // Simplify if high child is zero (for Davio)
    if (dt != decompositionType::S && *new_high == *terminal0) {
        computedTable[key] = new_low;
        return new_low;
    }

    // Check if children are identical
    if (dt == decompositionType::S && *new_low == *new_high) {
        computedTable[key] = new_low;
        return new_low;
    }

    const std::shared_ptr<bddNode> result = findOrAdd(x_i, new_low, new_high);
    computedTable[key] = result; // Implicit conversion to weak ptr

    // Müllabfuhr
    static size_t lastTableSize = 100;
    if (computedTable.size() > 2 * lastTableSize) {
        garbageCollectComputedTable();
        lastTableSize = computedTable.size();
    }

    return result;
}

void garbageCollectComputedTable() noexcept {
    for (auto it = computedTable.begin(); it != computedTable.end();) {
        const auto &[f, g, h] = it->first;
        if (f.expired() || g.expired() || h.expired() || it->second.expired()) {
            it = computedTable.erase(it); // C++17 erase returns next iterator
        } else {
            ++it;
        }
    }
}