#include "parser.h"
#include "uniqueTable.h"
#include "variableOrdering.h"

int nodeCount = 0;

std::vector<std::shared_ptr<bddNode>> parseAiger(const std::string path) {
    std::ifstream file(path);
    // Tracking output variables
    std::vector<std::shared_ptr<bddNode>> outputVector;
    if (!file.is_open()) {
        std::cerr << "Invalid path! Got path: " << path << "\n";
        return outputVector;
    }

    std::string line;
    std::getline(file, line);

    std::string format;
    unsigned short max_var_index, num_inputs, num_latches, num_outputs,
        num_ands;

    std::istringstream iss(line);
    iss >> format;

    if (format != "aag") {
        std::cerr << "parser.cpp: Does not have the format of an Aiger File. "
                     "Expected 'aag' in the header but got: "
                  << format << std::endl;
        return outputVector;
    }

    iss >> max_var_index >> num_inputs >> num_latches >> num_outputs >>
        num_ands;

    if (iss.fail()) {
        std::cerr
            << "parser.cpp: File doesnt have the expected aiger header with 5 "
               "non negative integers 'M', 'I', 'L', 'O', 'A' separated "
               "by spaces.\n";
        return outputVector;
    }

    // For execution time of this function
    auto start = std::chrono::high_resolution_clock::now();

    // std::cout << max_var_index << " " << num_inputs << " " << num_latches <<
    // " " << num_outputs << " " << num_ands << "\n";

    // Create the variable order
    if (orderingChoice == 1) {
        additionOrdering(num_inputs);
    } else if (orderingChoice == 2) {
        if (ordering.empty())
            randomizeVariableOrder(num_inputs);
    } else {
        ordering.clear();
    }

    // Circuit without outputs is obviously empty
    if (num_outputs == 0) {
        return outputVector;
    }

    // Tracking all variables
    std::shared_ptr<bddNode> vars[max_var_index + 1];

    unsigned short lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;
        std::stringstream iss(line);
        // Handle input variables
        if (lineCount <= num_inputs) {
            unsigned short inp;
            iss >> inp;
            // std::cout << "Input variable: " << inp / 2 << std::endl;
            vars[inp / 2] = newVariable(inp / 2, decomp(inp / 2));
            continue;
        }

        // Handle output variables
        if (lineCount <= num_inputs + num_outputs) {
            unsigned short out;
            iss >> out;
            // std::cout << "Output variable: " << out / 2 << std::endl;
            // Handle constant output (1 or 0)
            auto o = out < 2 ? (out == 0 ? terminal0 : terminal1)
                             : newVariable(out / 2, decomp(out / 2));
            // outputVector is at first used keep track of the labels, not
            // actual result. Will be overwritten with correct result
            outputVector.push_back(o);
            continue;
        }

        // Handle AND gates
        unsigned short variable, and1, and2;
        iss >> variable >> and1 >> and2;

        // File end indicated by comment header (c) where not number is read
        if (variable == 0) {
            break;
        }

        // std::cout << "Operation: " << variable / 2 << " = "
        //           << (and1 % 2 == 0 ? "" : "~") << and1 / 2 << " AND "
        //           << (and2 % 2 == 0 ? "" : "~") << and2 / 2 << "\n";

        // 0 and 1 in aiger stand for terminals
        // the rest are variables
        auto a1 =
            and1 < 2 ? (and1 == 0 ? terminal0 : terminal1)
                     : (and1 % 2 == 0 ? vars[and1 / 2] : ~vars[(and1 - 1) / 2]);

        auto a2 =
            and2 < 2 ? (and2 == 0 ? terminal0 : terminal1)
                     : (and2 % 2 == 0 ? vars[and2 / 2] : ~vars[(and2 - 1) / 2]);

        vars[variable / 2] = a1 * a2;
    }

    // Handle file ending
    for (int i = 0; i < num_outputs; i++) {
        // terminals are handled directly at the output variable declaration
        if (isTerminal(outputVector[i])) {
            continue;
        }

        outputVector[i] = vars[outputVector[i]->var];
    }

    file.close();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "======================\n";
    std::cout << "BDD construction time: "
              << ((double)duration.count()) / 1000000 << " seconds\n";
    std::cout << "Amount of new nodes created for this circuit: "
              << uniqueTable.size() - nodeCount << "\n";
    std::cout << "Amount of BDD nodes created so far: " << uniqueTable.size()
              << "\n";
    std::cout << "======================\n";

    nodeCount = uniqueTable.size();

    return outputVector;
}