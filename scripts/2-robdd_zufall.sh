#!/bin/bash

# Check for correct number of arguments
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <executable_path> <folder1_path> <folder2_path>"
    exit 1
fi

executable="$1"
folder1="$2"
folder2="$3"

# Verify the executable exists and is executable
if [ ! -x "$executable" ]; then
    echo "Error: Executable '$executable' not found or not executable"
    exit 1
fi

# Verify the folders exist
if [ ! -d "$folder1" ]; then
    echo "Error: Folder '$folder1' does not exist"
    exit 1
fi

if [ ! -d "$folder2" ]; then
    echo "Error: Folder '$folder2' does not exist"
    exit 1
fi

# Process each file in folder1
for file1 in "$folder1"/*; do
    # Skip if it's a directory
    if [ -d "$file1" ]; then
        continue
    fi
    
    # Get the filename without path
    filename=$(basename "$file1")
    
    # Construct the corresponding file path in folder2
    file2="$folder2/$filename"
    
    # Check if the corresponding file exists in folder2
    if [ ! -f "$file2" ]; then
        echo "Warning: Corresponding file '$file2' not found, skipping"
        continue
    fi
    
    echo "Processing $file1 with $file2"
    
    # Execute the command with the specified input sequence
    echo -e "1\n2\n1\n$file1\n$file2" | "$executable"
    
    # Check the exit status of the executable
    if [ $? -ne 0 ]; then
        echo "Error: Execution failed for $file1"
    fi
done

echo "Processing complete"
