#include <filesystem>
#include <iostream>
#include <vector>

/**
 * @brief Retrieves a list of `.aag` files in the specified folder.
 *
 * This function scans the given folder for files with the `.aag` extension and
 * returns them as a vector of strings. The user can choose whether to include
 * the full path or just the file name for each entry.
 *
 * @param folderPath The path to the folder to search for `.aag` files.
 * @param returnFullPath If `true`, the returned file paths include the full
 * path. If `false`, only the file names are returned.
 * @return std::vector<std::string> A list of `.aag` files found in the folder.
 *                                  Returns an empty vector if the folder does
 *                                  not exist or is not a directory.
 */
std::vector<std::string> getAagFilesInFolder(const std::string &folderPath,
                                             bool returnFullPath = true);

/**
 * @brief Constructs a full file path by combining a folder path and a file
 * name.
 *
 * This function takes a folder path and a file name, concatenates them using
 * the appropriate filesystem path separator, and returns the resulting full
 * file path.
 *
 * @param folderPath The path to the folder.
 * @param fileName The name of the file.
 * @return std::string The full file path as a string.
 */
std::string constructFilePath(const std::string &folderPath,
                              const std::string &fileName);