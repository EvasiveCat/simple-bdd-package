#ifndef VERIFICATION_H
#define VERIFICATION_H

#include "bddNode.h"
#include <bitset>
#include <vector>

/**
 * @brief Checks if two vectors of BDD nodes are equivalent.
 *
 * This function compares two vectors of shared pointers to `bddNode` objects.
 * The vectors are considered equivalent if they have the same size and all
 * corresponding elements in the vectors are bitwise equivalent (i.e., their
 * XOR result is equal to `terminal0`).
 *
 * @param outputVector1 The first vector of shared pointers to `bddNode`
 * objects.
 * @param outputVector2 The second vector of shared pointers to `bddNode`
 * objects.
 * @return `true` if the vectors are equivalent in size and all corresponding
 *         elements are bitwise equivalent; otherwise, `false`.
 */
bool equivalent(std::vector<std::shared_ptr<bddNode>> outputVector1,
                std::vector<std::shared_ptr<bddNode>> outputVector2);

/**
 * @brief Simulates the behavior of a Boolean circuit represented by a vector of
 * BDD nodes.
 *
 * This function takes a vector of input values and a vector of BDD nodes
 * representing the outputs of a Boolean circuit. It evaluates the outputs based
 * on the given inputs and returns the results as a vector of boolean values.
 *
 * @param input A vector of boolean values representing the input to the
 * circuit. The size of this vector should match the number of input variables
 * in the BDD.
 * @param outputVector A vector of `bddNode` objects representing the outputs of
 * the circuit. Each `bddNode` corresponds to a Boolean function defined over
 * the inputs.
 *
 * @return A vector of boolean values representing the simulation results for
 * each output in `outputVector`. The size of this vector will match the size of
 * `outputVector`.
 *
 * @example
 * // Input values for variables x1, x2, x3
 * std::vector<bool> input = {true, false, true};
 * // BDDs for outputs y1, y2
 * std::vector<bddNode> outputVector = {terminal0, x1};
 * // result will contain the evaluated values of y1 and y2 based on the input.
 * std::vector<bool> result = simulate(input, outputVector);
 * // In this case: result = {0, 1}
 */
std::vector<bool> simulate(std::vector<bool> input,
                           std::vector<std::shared_ptr<bddNode>> outputVector);

#endif // VERIFICATION_H