#include "bddToKfdd.h"
#include "kfddXOR.h"
#include "uniqueTable.h"

decltype(computedTableBddToKfdd) computedTableBddToKfdd;

std::shared_ptr<bddNode>
terminalCases(const std::shared_ptr<bddNode> F) noexcept {
    if (isTerminal(F)) {
        return F;
    }

    return nullptr;
}

std::shared_ptr<bddNode> bddToKfdd(const std::shared_ptr<bddNode> F) noexcept {
    const auto terminal = terminalCases(F);
    if (terminal) {
        return terminal;
    }

    // Check cache
    auto wF = std::weak_ptr(F);
    const auto key = std::make_tuple(wF);

    if (auto it = computedTableBddToKfdd.find(key);
        it != computedTableBddToKfdd.end()) {
        if (auto cached = it->second.lock())
            return cached;
        computedTableBddToKfdd.erase(it);
    }

    // const std::shared_ptr<bddNode> topNode =
    //     topVariable(F, terminal0, terminal0);
    const unsigned int x_i = topVariable(F, terminal0, terminal0);

    std::shared_ptr<bddNode> R_high, R_low;
    auto F_1 = cofactor(x_i, F, 1);
    auto F_0 = cofactor(x_i, F, 0);

    if (dtl[x_i] == decompositionType::S) {
        R_high = bddToKfdd(F_1);
        R_low = bddToKfdd(F_0);

        if (*R_high == *R_low) {
            return R_high;
        }
    } else if (dtl[x_i] == decompositionType::pD) {
        R_high = bddToKfdd(F_0 ^ F_1);
        R_low = bddToKfdd(F_0);

        if (*R_high == *terminal0) {
            return R_low;
        }
    } else {
        R_high = bddToKfdd(F_0 ^ F_1);
        R_low = bddToKfdd(F_1);

        if (*R_high == *terminal0) {
            return R_low;
        }
    }

    const std::shared_ptr<bddNode> result = findOrAdd(x_i, R_low, R_high);

    computedTableBddToKfdd.try_emplace(key, result);

    // // Müllabfuhr
    // static size_t lastTableSize = 100;
    // if (computedTable.size() > 2 * lastTableSize) {
    //     garbageCollectComputedTable();
    //     lastTableSize = computedTable.size();
    // }

    return result;
}

void garbageCollectBddToKfddTable() noexcept {
    auto it = computedTableBddToKfdd.begin();
    while (it != computedTableBddToKfdd.end()) {
        auto &[key, val] = *it;
        if (std::get<0>(key).expired() || val.expired()) {
            it = computedTableBddToKfdd.erase(it);
        } else {
            ++it;
        }
    }
}