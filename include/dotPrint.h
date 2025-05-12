#ifndef DOTPRINT_H
#define DOTPRINT_H

#include "bddNode.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_set>

/**
 * Writes a BDD structure to a DOT file for graph visualization.
 * @param out The output file stream to write to.
 * @param node The root node of the BDD.
 * @param visited A set to track visited nodes and avoid cycles.
 * @param showAddress If true, includes memory addresses in the node labels.
 */
void writeDotFile(std::ofstream &out, const std::shared_ptr<bddNode> &node,
                  std::unordered_set<std::shared_ptr<bddNode>> &visited,
                  bool showAddress);

#endif // DOTPRINT_H