#include "dtl.h"
#include "fileFinder.h"
#include "interface.h"
#include "ite.h"
#include "parser.h"
#include "uniqueTable.h"
#include "variableOrdering.h"
#include <cassert>

int main() {
    unsigned short bddChoice, orderChoice, comparisionChoice;

    logo();

    while (true) {
        ordering.clear();
        dtl.clear();
        uniqueTable.clear();
        computedTable.clear();
        displayMainMenu();
        std::cin >> bddChoice;
        if (std::cin.fail() || bddChoice < 1 || bddChoice > 4) {
            handleInvalidInput();
            if (checkExit())
                break;
            continue;
        }

        dtSelection = bddChoice;

        displayOrderingMenu();
        std::cin >> orderChoice;
        if (std::cin.fail() || orderChoice < 1 || orderChoice > 3) {
            handleInvalidInput();
            if (checkExit())
                break;
            continue;
        }

        orderingChoice = orderChoice;

        displayCheckingMenu();

        std::cin >> comparisionChoice;
        if (std::cin.fail() || orderChoice < 1 || orderChoice > 3) {
            handleInvalidInput();
            if (checkExit())
                break;
            continue;
        }

        if (comparisionChoice == 1) {
            std::cout << "Gib den Pfad zur Aiger Datei an: ";
            std::string path1;

            std::cin >> path1;
            if (std::cin.fail()) {
                handleInvalidInput();
                if (checkExit())
                    break;
                continue;
            }

            std::cout << "\n=" << path1 << "=" << "\n ";
            auto vec1 = parseAiger(path1);

            std::cout << "Gib den Pfad zur Aiger Datei an mit der der "
                         "Schaltkreis verglichen werden soll\n ";
            std::string path2;
            std::cin >> path2;
            if (std::cin.fail()) {
                handleInvalidInput();
                if (checkExit())
                    break;
                continue;
            }

            std::cout << "=" << path2 << "=" << "\n ";
            auto vec2 = parseAiger(path2);

            printResult(vec1, vec2);
        } else if (comparisionChoice == 2) {
            // Compare file with folder
            std::cout << "Gib den Pfad zur Aiger Datei an: ";
            std::string path1;

            std::cin >> path1;
            if (std::cin.fail()) {
                handleInvalidInput();
                if (checkExit())
                    break;
                continue;
            }

            auto vec1 = parseAiger(path1);

            std::cout << "Gib den Pfad zum Directory an: ";
            std::string directoryPath;

            std::cin >> directoryPath;
            if (std::cin.fail()) {
                handleInvalidInput();
                if (checkExit())
                    break;
                continue;
            }

            std::vector<std::string> v = getAagFilesInFolder(directoryPath);

            for (std::string &file : v) {
                std::cout << "Comparing with: " << file << std::endl;
                auto vec2 = parseAiger(file);
                printResult(vec1, vec2);
            }
        } else {
            // Compare folder with folder
            std::cout << "Gib den Pfad zum Ordner mit den zu"
                         "überprüfenden Aiger Dateien an : ";
            std::string path1;

            std::cin >> path1;
            if (std::cin.fail()) {
                handleInvalidInput();
                if (checkExit())
                    break;
                continue;
            }

            std::vector<std::string> v1 = getAagFilesInFolder(path1, false);

            std::cout << "Gib den Pfad zum Referenzordner an: ";
            std::string path2;

            std::cin >> path2;
            if (std::cin.fail()) {
                handleInvalidInput();
                if (checkExit())
                    break;
                continue;
            }

            std::vector<std::string> v2 = getAagFilesInFolder(path2, false);

            for (std::string &file : v1) {
                std::cout << "Überprüfen: " << file << std::endl;

                if (std::find(v2.begin(), v2.end(), file) != v2.end()) {
                    auto vec1 = parseAiger(constructFilePath(path1, file));
                    auto vec2 = parseAiger(constructFilePath(path1, file));
                    printResult(vec1, vec2);
                } else {
                    std::cout
                        << "\033[31m "
                        << "Datei kann nicht im Referenzordner gefunden werden!"
                        << "\033[0m\n";
                    continue;
                }
            }
        }

        if (checkExit())
            break;
    }

    return 0;
}