#include "variableOrdering.h"

std::vector<unsigned int> ordering; // Maps variable to priority
uint8_t orderingChoice;

unsigned int getVariablePriority(unsigned int var) {
    if (var >= 1 && var < ordering.size()) {
        return ordering[var];
    }
    return var; // Fallback if var is out of bounds or invalid
}

void additionOrdering(unsigned int inputs) {
    unsigned int n = inputs / 2;
    ordering.clear();
    ordering.resize(inputs + 1);

    for (unsigned int i = 0; i < n; i++) {
        ordering[i + 1] = 2 * i + 1;     // Assign priority to variable i+1
        ordering[n + i + 1] = 2 * i + 2; // Assign priority to variable n+i+1
    }
}

void randomizeVariableOrder(unsigned int inputs) {
    ordering.clear();
    ordering.resize(inputs + 1);

    // Create a vector of variable indices (starting at 1)
    std::vector<unsigned int> variables(inputs);
    for (unsigned int i = 0; i < inputs; ++i) {
        variables[i] = i + 1; // Variables start at 1
    }

    // Shuffle the variables
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(variables.begin(), variables.end(), g);

    // Assign priorities based on the shuffled order
    for (unsigned int i = 0; i < inputs; ++i) {
        ordering[variables[i]] = i + 1; // Priority starts from 1
    }

    // std::cout << "Using the random variable order:" << std::endl;
    // for (unsigned int i = 1; i <= inputs; ++i) {
    //     std::cout << "Variable " << i << " -> Priority " << ordering[i]
    //               << std::endl;
    // }
}