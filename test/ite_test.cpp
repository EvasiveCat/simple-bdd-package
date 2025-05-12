#include "dotPrint.h"
#include <bddNode.h>
#include <bddToKfdd.h>
#include <gtest/gtest.h>
#include <ite.h>
#include <uniqueTable.h>
#include <verification.h>

class BDDTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Initialize variables with mixed decomposition types
        x1 = newVariable(1, decompositionType::S);
        x2 = newVariable(2, decompositionType::pD);
        x3 = newVariable(3, decompositionType::nD);
        x4 = newVariable(4, decompositionType::S);
        x5 = newVariable(5, decompositionType::pD);
        x6 = newVariable(6, decompositionType::nD);
    }

    std::shared_ptr<bddNode> x1, x2, x3, x4, x5, x6;
};

// --- Cache Behavior Verification ---
TEST_F(BDDTest, UniqueTableConsistency) {
    size_t initial_cache_size = 6;

    auto expr1 = (x1 * x2);

    EXPECT_EQ(uniqueTable.size(), initial_cache_size + 1)
        << "Unique table should create new cache entry";

    auto expr2 = (x2 * x1); // Duplicate

    EXPECT_EQ(uniqueTable.size(), initial_cache_size + 1)
        << "Unique table should not restore duplicates";

    auto expr3 = x1 * x3; // Different

    EXPECT_EQ(uniqueTable.size(), initial_cache_size + 2)
        << "Unique table should create new cache entry";
}

// // --- Basic Logic Tests ---
TEST_F(BDDTest, BasicOperationsTest) {
    auto shannonAnd = x1 * x4;
    EXPECT_EQ(shannonAnd, findOrAdd(1, terminal0, x4));
    auto shannonOr = x1 + x4;
    EXPECT_EQ(shannonOr, findOrAdd(1, x4, terminal1));
    auto shannonNot = ~x1;
    EXPECT_EQ(shannonNot, findOrAdd(1, terminal1, terminal0));
    auto shannonXOR = x1 ^ x4;
    EXPECT_EQ(shannonXOR, findOrAdd(1, x4, ~x4));

    auto pDAnd = x2 * x5;
    EXPECT_EQ(pDAnd, findOrAdd(2, terminal0, x5));
    auto pDOr = x2 + x5;
    EXPECT_EQ(pDOr, findOrAdd(2, x5, findOrAdd(5, terminal1, terminal1)));
    auto pDNot = ~x2;
    EXPECT_EQ(pDNot, findOrAdd(2, terminal1, terminal1));
    auto pDXOR = x2 ^ x5;
    EXPECT_EQ(pDXOR, findOrAdd(2, x5, terminal1));

    auto nDAnd = x3 * x6;
    EXPECT_EQ(nDAnd, findOrAdd(3, x6, x6));
    auto nDOr = x3 + x6;
    EXPECT_EQ(nDOr,
              findOrAdd(3, terminal1, findOrAdd(6, terminal0, terminal1)));
    auto nDNot = ~x3;
    EXPECT_EQ(nDNot, findOrAdd(3, terminal0, terminal1));
    auto nDXOR = x3 ^ x6;
    EXPECT_EQ(nDXOR, findOrAdd(3, ~x6, terminal1));
}

TEST_F(BDDTest, AndOrDeMorganEquivalence) {
    auto ref = (x1 * x2) + (x3 * x4);
    auto opt = ~(~(x1 * x2) * ~(x3 * x4));
    EXPECT_EQ(ref, opt);
}

TEST_F(BDDTest, XorEquivalence) {
    auto ref = (x1 * ~x2) + (~x1 * x2);
    auto opt = x1 ^ x2;
    EXPECT_EQ(*ref, *opt);
}

// --- Decomposition-Sensitive Tests ---
TEST_F(BDDTest, DavioExpansionConsistency) {
    // pD: f = f_0 ⊕ (x ∧ (f_0 ⊕ f_1))
    dtl[1] = decompositionType::pD;
    auto pd_expr = x1 * x2;

    // Manually compute pD expansion
    auto f0 = cofactor(1, pd_expr, false);
    auto f1 = cofactor(1, pd_expr, true);
    auto manual_pd = f0 ^ (x1 * (f0 ^ f1));
    EXPECT_EQ(*pd_expr, *manual_pd);
}

TEST_F(BDDTest, NegativeDavioConsistency) {
    // nD: f = f_1 ⊕ (¬x ∧ (f_0 ⊕ f_1))
    auto nd_expr = x3 * x4;

    auto f0 = cofactor(3, nd_expr, false);
    auto f1 = cofactor(3, nd_expr, true);
    auto manual_nd = f1 ^ (~x3 * (f0 ^ f1));
    EXPECT_EQ(*nd_expr, *manual_nd);
}

// -- -Terminal Edge Cases-- -
TEST_F(BDDTest, TerminalPropagation) {
    auto expr1 = (x1 * terminal0) + (x2 * terminal1);
    auto expr2 = x2;
    EXPECT_EQ(*expr1, *expr2);

    auto expr3 = (x3 + terminal1) * (x4 + terminal0);
    auto expr4 = x4;
    EXPECT_EQ(*expr3, *expr4);
}

// ---Complex Structure Tests---
TEST_F(BDDTest, NestedOperations) {
    auto complex_ref = (x1 * (x2 + x3)) + (x4 * ~(x5 + x6));
    auto complex_opt = ~(~(x1 * (x2 + x3)) * ~(x4 * ~(x5 + x6)));

    EXPECT_EQ(complex_ref, complex_opt);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}