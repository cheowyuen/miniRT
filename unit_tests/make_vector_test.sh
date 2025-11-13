#!/bin/bash

# --- Configuration ---
TEST_FILE="vector_math_test.c"
OUTPUT_NAME="test_math"
CC="gcc"
CFLAGS="-Wall -Wextra -Werror -g"
INCLUDES="-I../includes -I../MLX/include/MLX42 -I../libft"
LIBRARIES="-lm" # Links the standard math library (for sqrt, fabs, etc.)

# List all source files that the math tests rely on.
# We must include the C files that implement the functions being tested.
CORE_MATH_SRCS="../src/vector.c ../src/vector_utils.c"

# --- Compilation Command ---

echo "Compiling $TEST_FILE..."
echo "Command: $CC $CFLAGS $INCLUDES $TEST_FILE $CORE_MATH_SRCS $LIBRARIES -o $OUTPUT_NAME"

# Execute the compilation
$CC $CFLAGS $INCLUDES $TEST_FILE $CORE_MATH_SRCS $LIBRARIES -o $OUTPUT_NAME

# --- Post-Compilation Check ---
if [ $? -eq 0 ]; then
    echo "Compilation successful! Executable created: ./$OUTPUT_NAME"
    echo "Running tests..."
    ./$OUTPUT_NAME
else
    echo "Compilation failed."
fi
