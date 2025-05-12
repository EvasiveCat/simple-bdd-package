#!/bin/bash

# Exit immediately if any command fails
set -e

# Define the commands to run (as an array of arrays for clarity)
commands=(
  "./scripts/1-robdd_addierer.sh ./main_release benchmarks/Adders benchmarks/ref"
)

# Loop through and execute each command
for cmd in "${commands[@]}"; do
  echo "→ Executing: $cmd"
  eval "$cmd"
  
  # Check exit status
  if [ $? -eq 0 ]; then
    echo "✓ Success: $cmd"
  else
    echo "✗ Failed: $cmd (exit code: $?)"
    exit 1  # Terminate script on failure
  fi
  
  echo "--------------------------------------"
done

echo "All commands completed successfully!"
