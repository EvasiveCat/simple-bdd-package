#include "fileFinder.h"

// Function to get the paths of all .aag files in a folder
std::vector<std::string> getAagFilesInFolder(const std::string &folderPath,
                                             bool returnFullPath) {
    std::vector<std::string> aagFiles;

    // Check if the folder exists
    if (!std::filesystem::exists(folderPath)) {
        std::cerr << "Error: Folder does not exist: " << folderPath << "\n";
        return aagFiles;
    }

    if (!std::filesystem::is_directory(folderPath)) {
        std::cerr << "Error: Path is not a directory: " << folderPath << "\n";
        return aagFiles;
    }

    // Iterate over all files in the folder
    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".aag") {
            // Add the full path or just the file name to the list
            if (returnFullPath) {
                aagFiles.push_back(entry.path().string()); // Full path
            } else {
                aagFiles.push_back(
                    entry.path().filename().string()); // File name only
            }
        }
    }

    return aagFiles;
}

std::string constructFilePath(const std::string &folderPath,
                              const std::string &fileName) {
    // Create a path object for the folder
    std::filesystem::path folder(folderPath);

    // Append the file name to the folder path
    folder /= fileName;

    // Return the constructed file path as a string
    return folder.string();
}