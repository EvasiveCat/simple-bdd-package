#ifndef PARSER_H
#define PARSER_H

#include "bddNode.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * Parses the aiger file and constructs the bdd
 * @param path The path to the aiger file.
 * @return A vector of bdds of the circuit outputs
 */
std::vector<std::shared_ptr<bddNode>> parseAiger(const std::string path);

#endif // PARSER_H