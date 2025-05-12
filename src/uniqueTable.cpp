#include "uniqueTable.h"
#include "bddHash.h"
#include "dtl.h"

decltype(uniqueTable) uniqueTable;

std::shared_ptr<bddNode> newVariable(unsigned int var,
                                     decompositionType d) noexcept {
    dtl.emplace(var, d);
    // return findOrAdd(var, terminal0, terminal1);
    return (d == decompositionType::nD) ? findOrAdd(var, terminal1, terminal1)
                                        : findOrAdd(var, terminal0, terminal1);
}

std::shared_ptr<bddNode> findOrAdd(unsigned int var,
                                   std::shared_ptr<bddNode> low,
                                   std::shared_ptr<bddNode> high) noexcept {
    // Terminale
    if (!low || !high)
        return handleTerminals(var);

    // Key für die hashmap
    const auto key = std::make_tuple(var, low, high);

    // Single lookup with cleanup
    if (auto it = uniqueTable.find(key); it != uniqueTable.end()) {
        if (auto node = it->second.lock())
            return node;
        uniqueTable.erase(it); // Clean expired entries immediately
    }

    // Create and store new node
    auto node = std::make_shared<bddNode>(low, high, var);
    uniqueTable[key] = node;
    return node;
}

std::shared_ptr<bddNode> findOrAddNode(std::shared_ptr<bddNode> node) noexcept {
    return findOrAdd(node->var, node->low, node->high);
}

void printUniqueTable() noexcept {
    std::cout << "Unique Table:\n";
    for (const auto &[key, node] : uniqueTable) {
        auto [var, low, high] = key;
        std::cout << "var: " << var << ", Low: " << low.get()
                  << ", High: " << high.get()
                  << " => Node: " << node.lock().get() << "\n";
    }
    std::cout << "Done printing unique table\n";
}
