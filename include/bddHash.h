#ifndef BDD_HASH_H
#define BDD_HASH_H

#include "bddNode.h"
#include <memory>
#include <tuple>

// Optimized hash combining
inline size_t hash_combine(size_t seed, size_t value) {
    return seed ^ (value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

// Hash for 3 elements computed table ite (F, G, H keys)
struct Tuple3WeakHash {
    using KeyType = std::tuple<std::weak_ptr<bddNode>, std::weak_ptr<bddNode>,
                               std::weak_ptr<bddNode>>;

    size_t operator()(const KeyType &key) const {
        size_t hash = 0;
        auto lock = [](const std::weak_ptr<bddNode> &wp) {
            return wp.lock().get();
        };
        if (auto p = lock(std::get<0>(key)))
            hash ^= std::hash<decltype(p)>{}(p);
        if (auto p = lock(std::get<1>(key)))
            hash = hash_combine(hash, std::hash<decltype(p)>{}(p));
        if (auto p = lock(std::get<2>(key)))
            hash = hash_combine(hash, std::hash<decltype(p)>{}(p));
        return hash;
    }
};

// Hash for 2-elements (kfddXOR)
struct Tuple2WeakHash {
    using KeyType = std::tuple<std::weak_ptr<bddNode>, std::weak_ptr<bddNode>>;

    size_t operator()(const KeyType &key) const {
        size_t hash = 0;
        auto lock = [](const std::weak_ptr<bddNode> &wp) {
            return wp.lock().get();
        };
        if (auto p = lock(std::get<0>(key)))
            hash ^= std::hash<decltype(p)>{}(p);
        if (auto p = lock(std::get<1>(key)))
            hash = hash_combine(hash, std::hash<decltype(p)>{}(p));
        return hash;
    }
};

// Hash for 1-elements (bddToKfdd)
struct Tuple1WeakHash {
    using KeyType = std::tuple<std::weak_ptr<bddNode>>;

    size_t operator()(const KeyType &key) const {
        auto lock = [](const std::weak_ptr<bddNode> &wp) {
            return wp.lock().get();
        };
        if (auto p = lock(std::get<0>(key)))
            return std::hash<decltype(p)>{}(p);
        return 0;
    }
};

// Hash for unique table (var, low, high keys)
struct UniqueTableHash {
    using KeyType = std::tuple<unsigned int, std::shared_ptr<bddNode>,
                               std::shared_ptr<bddNode>>;

    size_t operator()(const KeyType &key) const {
        const auto &var = std::get<0>(key);
        const auto &low = std::get<1>(key);
        const auto &high = std::get<2>(key);

        size_t hash = std::hash<unsigned int>{}(var);
        if (low)
            hash =
                hash_combine(hash, std::hash<decltype(low.get())>{}(low.get()));
        if (high)
            hash = hash_combine(hash,
                                std::hash<decltype(high.get())>{}(high.get()));
        return hash;
    }
};

// Equality for 3 element tuple
struct Tuple3WeakEqual {
    using KeyType = std::tuple<std::weak_ptr<bddNode>, std::weak_ptr<bddNode>,
                               std::weak_ptr<bddNode>>;

    bool operator()(const KeyType &a, const KeyType &b) const {
        auto lock = [](const std::weak_ptr<bddNode> &wp) { return wp.lock(); };
        return lock(std::get<0>(a)) == lock(std::get<0>(b)) &&
               lock(std::get<1>(a)) == lock(std::get<1>(b)) &&
               lock(std::get<2>(a)) == lock(std::get<2>(b));
    }
};

// Equality for 2 element tuple
struct Tuple2WeakEqual {
    using KeyType = std::tuple<std::weak_ptr<bddNode>, std::weak_ptr<bddNode>>;

    bool operator()(const KeyType &a, const KeyType &b) const {
        auto lock = [](const std::weak_ptr<bddNode> &wp) { return wp.lock(); };
        return lock(std::get<0>(a)) == lock(std::get<0>(b)) &&
               lock(std::get<1>(a)) == lock(std::get<1>(b));
    }
};

// Equality for 1 element tuple
struct Tuple1WeakEqual {
    using KeyType = std::tuple<std::weak_ptr<bddNode>>;

    bool operator()(const KeyType &a, const KeyType &b) const {
        auto lock = [](const std::weak_ptr<bddNode> &wp) { return wp.lock(); };
        return lock(std::get<0>(a)) == lock(std::get<0>(b));
    }
};

#endif // BDD_HASH_H