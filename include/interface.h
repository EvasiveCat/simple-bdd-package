#ifndef INTERFACE_H
#define INTERFACE_H

#include <bddNode.h>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

/**
 * @brief Displays a green ASCII art logo for "sbdd".
 */
void logo();

/**
 * @brief Displays a menu for selecting the variable ordering method.
 */
void displayOrderingMenu();

/**
 * @brief Displays the main menu for selecting the variable decomposition
 * method.
 */
void displayMainMenu();

/**
 * @brief Displays a menu for selecting the type of comparison to perform.
 */
void displayCheckingMenu();

/**
 * @brief Displays a message indicating that two circuits are equivalent.
 */
void equalText();

/**
 * @brief Displays a message indicating that two circuits are not equivalent.
 */
void unequalText();

/**
 * @brief Displays a message indicating that two circuits are empty.
 */
void emptyText();

/**
 * @brief Prints the result of comparing two sets of BDD nodes.
 *
 * @param a The first set of BDD nodes.
 * @param b The second set of BDD nodes.
 */
void printResult(const std::vector<std::shared_ptr<bddNode>> a,
                 const std::vector<std::shared_ptr<bddNode>> b);

/**
 * @brief Handles invalid input by clearing the input buffer and displaying an
 * error message.
 */
void handleInvalidInput();

/**
 * @brief Prompts the user to decide whether to continue or exit the program.
 *
 * @return true if the user chooses to exit, false otherwise.
 */
bool checkExit();

#endif // INTERFACE_H