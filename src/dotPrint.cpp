#include "dotPrint.h"

unsigned int count = 0;

// Helper function to write the DOT file
void writeDotFile(std::ofstream &out, const std::shared_ptr<bddNode> &node,
                  std::unordered_set<std::shared_ptr<bddNode>> &visited,
                  bool showAddress) {
    if (node == nullptr || visited.count(node)) {
        return;
    }

    visited.insert(node);

    std::ostringstream nodeName;
    nodeName << "\"" << node.get() << "\"";

    if (node == terminal0 || node == terminal1) {
        // Terminal nodes
        out << "    " << nodeName.str() << " [shape=box, label=\""
            << (node->var == 0 ? "0" : "1") << "\"];\n";
    } else {
        // Non-terminal nodes
        std::ostringstream label;
        if (showAddress) {
            label << node.get();
        } else {
            label << "x" << node->var;
            // label << (node->negated ? "~" : "") << "x" << node->var;
        }
        out << "    " << nodeName.str() << " [label=\"" << label.str()
            << "\"];\n";

        if (node->low) {
            std::ostringstream lowName;
            lowName << "\"" << node->low.get() << "\"";
            out << "    " << nodeName.str() << " -> " << lowName.str()
                << " [style=dotted];\n";
            writeDotFile(out, node->low, visited, showAddress);
        }
        if (node->high) {
            std::ostringstream highName;
            highName << "\"" << node->high.get() << "\"";
            out << "    " << nodeName.str() << " -> " << highName.str()
                << " [style=solid];\n";
            writeDotFile(out, node->high, visited, showAddress);
        }
    }
}

// Member function to print the BDD graph as a DOT file
void bddNode::dotPrint(bool convertToPNG, bool showAddress,
                       std::string heading) const {
    auto now = std::time(nullptr);
    auto localtime = std::localtime(&now);

    std::ostringstream filename;
    filename << "log/bdd_graph_"
             << std::put_time(localtime, "%Y-%m-%d_%H-%M-%S") << "-" << count
             << ".dot";
    count++;

    std::ofstream out(filename.str());
    if (!out.is_open()) {
        std::cerr << "Error opening file for writing DOT graph\n";
        return;
    }

    out << "digraph BDD {\n";
    out << "\tlabel = \"" << heading << "\";\n";
    out << "\tnode [shape=circle];\n";

    std::unordered_set<std::shared_ptr<bddNode>> visited;
    writeDotFile(out, std::const_pointer_cast<bddNode>(shared_from_this()),
                 visited, showAddress);

    out << "}\n";
    out.close();

    if (!convertToPNG) {
        return;
    }

    // converting dot to png
    std::ostringstream command;
    command << "dot -Tpng " << filename.str() << " -o "
            << filename.str().substr(0, filename.str().size() - 4) << ".png";

    int result = std::system(command.str().c_str());
    if (result != 0) {
        std::cerr << "Error converting DOT to PNG\n";
    } else {
        std::cout << "BDD graph written and converted to PNG: "
                  << filename.str().substr(0, filename.str().size() - 4)
                  << ".png\n";
    }
}
