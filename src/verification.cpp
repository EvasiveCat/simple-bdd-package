#include "verification.h"

bool equivalent(std::vector<std::shared_ptr<bddNode>> outputVector1,
                std::vector<std::shared_ptr<bddNode>> outputVector2) {
    int size1 = outputVector1.size();
    int size2 = outputVector2.size();
    if (size1 != size2) {
        std::cout << "Not equal due to different output size\n";
        return false;
    }

    for (int i = 0; i < size1; i++) {
        if (outputVector1[i] != outputVector2[i]) {
            outputVector1[i]->dotPrint(false, false, "output 1");
            outputVector2[i]->dotPrint(false, false, "output 2");
            std::cout << "Difference in output " << i << "\n";
            return false;
        }
    }

    return true;
}

std::vector<bool> simulate(std::vector<bool> input,
                           std::vector<std::shared_ptr<bddNode>> outputVector) {
    std::vector<bool> result;
    for (auto &out : outputVector) {
        std::shared_ptr<bddNode> curr = out;
        while (!isTerminal(curr)) {
            if (input[curr->var - 1]) {
                curr = curr->high;
            } else {
                curr = curr->low;
            }
        }
        result.push_back(curr == terminal1);
    }

    return result;
}