#!/bin/bash

# --- Configuration ---
TEST_FILE="object_tests.c"
OUTPUT_NAME="test_objects"
CC="gcc"
CFLAGS="-Wall -Wextra -Werror -g"
INCLUDES="-I../includes -I../MLX/include/MLX42 -I../libft" 
LIBRARIES="-lm -L../libft -lft" 

# List all source files that the object intersection tests rely on.
CORE_SRCS="../src/vector.c ../src/vector_utils.c ../src/ray.c ../src/cylinder.c ../src/ray_utils.c ../src/lighting.c ../src/normal_vectors.c"

# --- Compilation Command ---
echo "Compiling $TEST_FILE..."

# CRITICAL FIX: The -lm flag MUST be placed AFTER all the source files.
COMMAND="$CC $CFLAGS $INCLUDES $TEST_FILE $CORE_SRCS -o $OUTPUT_NAME $LIBRARIES"

echo "Command: $COMMAND"

# Execute the compilation
$COMMAND

# --- Post-Compilation Check ---
if [ $? -eq 0 ]; then
    echo "Compilation successful! Executable created: ./$OUTPUT_NAME"
    echo "Running tests..."
    ./$OUTPUT_NAME
else
    echo "Compilation failed."
fi
