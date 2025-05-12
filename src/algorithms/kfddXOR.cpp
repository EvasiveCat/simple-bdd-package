#include "kfddXOR.h"
#include "uniqueTable.h"

std::shared_ptr<bddNode>
terminalCases(const std::shared_ptr<bddNode> F,
              const std::shared_ptr<bddNode> G) noexcept {
    if (*F == *terminal0) {
        return G;
    }

    if (*F == *terminal1) {
        return ~G;
    }

    if (*G == *terminal0) {
        return F;
    }

    if (*G == *terminal1) {
        return ~F;
    }

    return nullptr;
}

decltype(computedTableXor) computedTableXor;

std::shared_ptr<bddNode>
kfddExorKfdd(const std::shared_ptr<bddNode> F,
             const std::shared_ptr<bddNode> G) noexcept {
    const auto terminal = terminalCases(F, G);
    if (terminal) {
        return terminal;
    }

    auto wF = std::weak_ptr(F);
    auto wG = std::weak_ptr(G);
    const auto key = std::tie(wF, wG);

    // Single lookup with cleanup
    if (auto it = computedTableXor.find(key); it != computedTableXor.end()) {
        if (auto cached = it->second.lock())
            return cached;
        computedTableXor.erase(it);
    }

    // const std::shared_ptr<bddNode> topNode = topVariable(F, G, terminal0);
    const unsigned int x_i = topVariable(F, G, terminal0);

    std::shared_ptr<bddNode> R_high, R_low;

    const auto F_1 = cofactor(x_i, F, 1);
    const auto G_1 = cofactor(x_i, G, 1);
    const auto F_0 = cofactor(x_i, F, 0);
    const auto G_0 = cofactor(x_i, G, 0);

    if (dtl[x_i] == decompositionType::S) {
        R_high = kfddExorKfdd(F_1, G_1);
        R_low = kfddExorKfdd(F_0, G_0);

        if (*R_high == *R_low) {
            return R_high;
        }
    } else if (dtl[x_i] == decompositionType::pD) {
        const auto F_diff = kfddExorKfdd(F_0, F_1);
        const auto G_diff = kfddExorKfdd(G_0, G_1);
        R_high = kfddExorKfdd(F_diff, G_diff);
        R_low = kfddExorKfdd(F_0, G_0);

        if (*R_high == *terminal0) {
            return R_low;
        }
    } else {
        const auto F_diff = kfddExorKfdd(F_0, F_1);
        const auto G_diff = kfddExorKfdd(G_0, G_1);
        R_high = kfddExorKfdd(F_diff, G_diff);
        R_low = kfddExorKfdd(F_1, G_1);

        if (*R_high == *terminal0) {
            return R_low;
        }
    }

    const std::shared_ptr<bddNode> result = findOrAdd(x_i, R_low, R_high);

    computedTableXor[key] = result;

    // Müllabfuhr
    // static size_t lastTableSize = 100;
    // if (computedTable.size() > 2 * lastTableSize) {
    //     garbageCollectComputedTable();
    //     lastTableSize = computedTable.size();
    // }

    return result;
}

void garbageCollectXorTable() noexcept {
    auto it = computedTableXor.begin();
    while (it != computedTableXor.end()) {
        const auto &[f, g] = it->first;
        if (f.expired() || g.expired() || it->second.expired()) {
            it = computedTableXor.erase(it);
        } else {
            ++it;
        }
    }
}