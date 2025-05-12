#include "dtl.h"

uint8_t dtSelection;

decompositionType decomp(const unsigned int var) {
    if (dtSelection == 1)
        return decompositionType::S;
    if (dtSelection == 2)
        return decompositionType::pD;
    if (dtSelection == 3)
        return decompositionType::nD;

    // Check if we already have a decomposition type for this node
    const auto it = dtl.find(var);
    if (it != dtl.end()) {
        return it->second;
    }

    // Create a random number generator
    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister random number engine
    std::uniform_int_distribution<> dis(
        0, 2); // Uniform distribution between 0 and 2

    // Generate a random number between 0 and 2
    int randomValue = dis(gen);

    // Map the random number to a decompositionType
    switch (randomValue) {
    case 0:
        return decompositionType::S;
    case 1:
        return decompositionType::pD;
    case 2:
        return decompositionType::nD;
    default:
        throw std::runtime_error("Invalid random value generated");
    }
}