#include "interface.h"
#include <chrono>
#include <verification.h>

void logo() {
    // Green ASCII art for "sbdd"
    const char *asciiArt = R"(
 ░▒▓███████▓▒░▒▓███████▓▒░░▒▓███████▓▒░░▒▓███████▓▒░  
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
 ░▒▓██████▓▒░░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
       ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
       ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓███████▓▒░░▒▓███████▓▒░░▒▓███████▓▒░░▒▓███████▓▒░                                                                                                                                                                                              
                  )";

    std::cout << "\033[32m" << asciiArt << "\033[0m\n" << "\n";
}

void displayOrderingMenu() {
    std::cout << "\n====================================\n";
    std::cout << "Was für eine Variablenordnung hättest du gerne?\n";
    std::cout << "====================================\n";
    std::cout << "1.Interleaved (für Addierer)\n";
    std::cout << "2.Zufällig\n";
    std::cout << "3.Nach Variablennamen\n";
    std::cout << "====================================\n";
    std::cout << "Wähle 1, 2 oder 3: ";
}

void displayMainMenu() {
    std::cout << "\n====================================\n";
    std::cout << "Welche Variablenzerlegung soll benutzt werden?\n";
    std::cout << "====================================\n";
    std::cout << "1. Nur Shannon (ROBDD)\n";
    std::cout << "2. Nur positiv Davio (OpFDD)\n";
    std::cout << "3. Nur negativ Davio (OnFDD)\n";
    std::cout << "4. Zufällig für jede Variable (ROKFDD)\n";
    std::cout << "====================================\n";
    std::cout << "Wähle 1, 2, 3 oder 4: ";
}

void displayCheckingMenu() {
    std::cout << "====================================\n";
    std::cout << "Was soll miteinander verglichen werden?\n";
    std::cout << "====================================\n";
    std::cout << "1.Einzelne Datei mit einzelner Datei\n";
    std::cout << "2.Einzelne Datei mit Ordner von Aiger Dateien\n";
    std::cout << "3.Ordner von Aiger Dateien mit Ordner von Aiger Dateien "
                 "(Vergleich von Dateien mit gleichem Namen)\n";
    std::cout << "====================================\n";
    std::cout << "Wähle 1, 2 oder 3: ";
}

void equalText() {
    std::cout << "\033[32m"
              << "================================="
                 "\033[0m\n"
              << "\n";
    std::cout << "\033[32m"
              << "=The two circuits are equivalent=" << "\033[0m\n"
              << "\n";
    std::cout << "\033[32m"
              << "=================================" << "\033[0m\n"
              << "\n";
}

void unequalText() {
    std::cout << "\033[31m"
              << "====================================="
                 "\033[0m\n"
              << "\n";
    std::cout << "\033[31m"
              << "=The two circuits are NOT equivalent=" << "\033[0m\n"
              << "\n";
    std::cout << "\033[31m"
              << "=====================================" << "\033[0m\n"
              << "\n";
}

void emptyText() {
    std::cout << "\033[33m"
              << "============================"
                 "\033[0m\n"
              << "\n";
    std::cout << "\033[33m"
              << "=The two circuits are empty=" << "\033[0m\n"
              << "\n";
    std::cout << "\033[33m"
              << "============================" << "\033[0m\n"
              << "\n";
}

void printResult(const std::vector<std::shared_ptr<bddNode>> a,
                 const std::vector<std::shared_ptr<bddNode>> b) {
    auto start = std::chrono::high_resolution_clock::now();

    if (a.size() == 0 && a.size() == 0) {
        emptyText();
    } else if (equivalent(a, b)) {
        equalText();
    } else {
        unequalText();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "======================\n";
    std::cout << "Time for equivalence checking: "
              << ((double)duration.count()) / 1000000 << " seconds\n";
    std::cout << "======================\n";
}

void handleInvalidInput() {
    std::cout << "Invalid input! Bitte eine Option auswählen.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool checkExit() {
    std::cin.clear();
    std::cout << "Do you want to continue? (y/n): ";
    char continueChoice;
    std::cin >> continueChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (continueChoice != 'y' && continueChoice != 'Y') {
        std::cout << "Exiting the program. Goodbye!\n";
        return true;
    }

    return false;
}