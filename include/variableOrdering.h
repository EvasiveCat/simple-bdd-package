#ifndef VARIABLEORDERING_H
#define VARIABLEORDERING_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>

/**
 * @brief A vector that maps each variable to its priority in the ordering.
 */
extern std::vector<unsigned int> ordering;

/**
 * @brief A variable to store the user's choice of ordering method.
 */
extern uint8_t orderingChoice;

/**
 * @brief Retrieves the priority of a given variable.
 *
 * @param var The variable index.
 * @return unsigned int The priority of the variable. If the variable is out of
 * bounds, the variable index itself is returned as a fallback.
 */
unsigned int getVariablePriority(unsigned int var);

/**
 * @brief Generates an interleaved variable ordering for addition circuits.
 *
 * This function assigns priorities to variables in an interleaved manner, which
 * is suitable for addition circuits. The ordering alternates between the first
 * half and the second half of the input variables.
 *
 * @param inputs The total number of input variables.
 */
void additionOrdering(unsigned int inputs);

/**
 * @brief Randomizes the priority order of variables.
 *
 * This function shuffles the variables and assigns them random priorities.
 *
 * @param inputs The total number of input variables.
 */
void randomizeVariableOrder(unsigned int inputs);

#endif // VARIABLEORDERING_H