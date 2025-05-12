#include "ite.h"
#include "kfddXOR.h"
#include "uniqueTable.h"

const std::shared_ptr<bddNode> terminal0 =
    std::make_shared<bddNode>(nullptr, nullptr, 0); //, false);
const std::shared_ptr<bddNode> terminal1 =
    std::make_shared<bddNode>(nullptr, nullptr, 1); // , false);

std::unordered_map<unsigned int, decompositionType> dtl;

bool bddNode::operator==(const bddNode &other) const {
    return (this->low == other.low) && (this->high == other.high) &&
           this->var == other.var; //&& this->negated == other.negated;
}

std::ostream &operator<<(std::ostream &os, const bddNode &node) {
    if (!node.low || !node.high) {
        os << "[" << node.var << "]";
    } else {
        os << "bddNode(var: " << node.var;
        if (node.low) {
            os << ", low: " << *node.low;
            if (node.high) {
                os << ", high: " << *node.high;
            }
        }
        os << ")";
    }
    return os;
}

std::shared_ptr<bddNode> operator*(const std::shared_ptr<bddNode> F,
                                   const std::shared_ptr<bddNode> G) {
    // std::cout << "Calling x" << F->var << " * " << "x" << G->var <<
    // std::endl;
    return iteKfdd(F, G, terminal0);
}

std::shared_ptr<bddNode> operator+(const std::shared_ptr<bddNode> F,
                                   const std::shared_ptr<bddNode> G) {
    return iteKfdd(F, terminal1, G);
}

std::shared_ptr<bddNode> operator~(const std::shared_ptr<bddNode> F) {
    if (F == terminal0)
        return terminal1;
    if (F == terminal1)
        return terminal0;

    // Recursive ite negate
    return iteKfdd(F, terminal0, terminal1);

    // Negated edges dont destroy canonicity if
    // the low edge is never negated
    // Canonicity is ensured in the findOrAdd function

    // std::cout << "Calling not x" << F->var << std::endl;

    // return findOrAdd(F->var, F->low, F->high, !(F->negated));
}

std::shared_ptr<bddNode> operator^(const std::shared_ptr<bddNode> F,
                                   const std::shared_ptr<bddNode> G) {
    // return ite(F, ~G, G);
    // Making sure F is smaller address for computed
    if (&F <= &G)
        return kfddExorKfdd(F, G);

    return kfddExorKfdd(G, F);
}

bool isTerminal(const std::shared_ptr<bddNode> node) {
    return node == terminal0 || node == terminal1;
}

// Für die Haskell-vibes
void bddNode::inlinePrint() const {
    if (this == terminal0.get()) {
        std::cout << "0";
    } else if (this == terminal1.get()) {
        std::cout << "1";
    } else {
        std::cout << "Low child: [";
        this->low->inlinePrint();
        std::cout << "], High child: [";
        this->high->inlinePrint();
        std::cout << "], var: " << var;
    }
}