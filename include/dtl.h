#ifndef DTL_H
#define DTL_H

#include <cstdint>
#include <random>
#include <stdexcept>
#include <unordered_map>

/// @brief The decomposition Type Shannon, positive Davio or negative Davio
enum class decompositionType : uint8_t { S, pD, nD };

/// @brief The decomposition type list. Maps for each variable a decomposition
/// type (S, pD, nD)
extern std::unordered_map<unsigned int, decompositionType> dtl;

/// Decomposition selection of the user (ROBDD, RpFDD, RnFDD, ROKFDD)
extern uint8_t dtSelection;

/// Returns the decomposition type according to the selection of the user
decompositionType decomp(const unsigned int var);

#endif // DTL_H