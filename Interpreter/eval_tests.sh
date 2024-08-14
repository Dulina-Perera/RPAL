#!/bin/bash

# Test the RPAL interpreter with the provided test cases.
# The test cases are in the form of a list of RPAL programs and their expected output.

# Function to display usage information.
usage() {
	echo "Usage: $0 [-d|--diff-only]"
	echo "  -d, --diff-only: Keep only the .diff files and remove the generated ._lex, ._ast and ._st files."
	exit 1
}

# Check if the 'myrpal' executable is present.
if [[ ! -f myrpal ]]; then
	echo "'myrpal' executable not found. Please compile the RPAL interpreter first."
	exit 1
fi

# Check if the 'tests' directory exists.
if [[ ! -d tests ]]; then
	echo "Test directory 'tests' not found."
	exit 1
fi

# Check if the 'tests/expected' directory exists.
if [[ ! -d tests/expected ]]; then
	echo "Expected output directory 'tests/expected' not found."
	exit 1
fi

# Parse command-line options.
diff_only=false
while [[ $# -gt 0 ]]; do
	case "$1" in
	-d | --diff-only)
		diff_only=true
		shift
		;;
	*)
		usage
		;;
	esac
done

# Create a directory to store generated files for each test case.
mkdir -p tests/generated

# Count the number of files in the 'tests/' directory.
file_cnt=$(ls --indicator-style=slash tests/ | grep --invert-match /$ | wc --lines)

# Initialize a counter to keep track of the number of test cases that passed.
passed_count=0

# Loop through each test case.
for i in $(seq 1 $file_cnt); do
	# Check if the test case file exists.
	if [[ ! -f "tests/$i.rpal" ]]; then
		echo "Test case 'tests/$i.rpal' does not exist."
		exit 1
	fi

	# Generate ._lex, ._ast and ._st files for each test case using the 'myrpal' executable.
	./myrpal -lex "tests/$i.rpal" >"tests/generated/$i._lex" || {
		echo "Failed to generate lexical output file for 'tests/$i.rpal'."
		exit 1
	}
	./myrpal -ast "tests/$i.rpal" >"tests/generated/$i._ast" || {
		echo "Failed to generate AST output file for 'tests/$i.rpal'."
		exit 1
	}
	./myrpal -st "tests/$i.rpal" >"tests/generated/$i._st" || {
		echo "Failed to generate ST output file for 'tests/$i.rpal'."
		exit 1
	}

	# Check if the expected files exist.
	if [[ ! -f "tests/expected/$i._lex" ]]; then
		echo "Expected lexical output file 'tests/expected/$i._lex' does not exist."
		exit 1
	fi
	if [[ ! -f "tests/expected/$i._ast" ]]; then
		echo "Expected AST output file 'tests/expected/$i._ast' does not exist."
		exit 1
	fi
	if [[ ! -f "tests/expected/$i._st" ]]; then
		echo "Expected ST output file 'tests/expected/$i._st' does not exist."
		exit 1
	fi

	# Compare the generated files with the expected files and write differences to .diff files if there is a difference.
	lex_diff=$(diff "tests/generated/$i._lex" "tests/expected/$i._lex")
	ast_diff=$(diff "tests/generated/$i._ast" "tests/expected/$i._ast")
	st_diff=$(diff "tests/generated/$i._st" "tests/expected/$i._st")

	# Output differences to .diff files if any.
	if [[ -n "$lex_diff" ]]; then
		echo "$lex_diff" >"tests/generated/$i._lex.diff"
	fi
	if [[ -n "$ast_diff" ]]; then
		echo "$ast_diff" >"tests/generated/$i._ast.diff"
	fi
	if [[ -n "$st_diff" ]]; then
		echo "$st_diff" >"tests/generated/$i._st.diff"
	fi

	# Determine test case pass/fail based on the presence of .diff files.
	if [[ -f "tests/generated/$i._lex.diff" || -f "tests/generated/$i._ast.diff" || -f "tests/generated/$i._st.diff" ]]; then
		echo "Test case 'tests/$i.rpal' failed."
	else
		echo "Test case 'tests/$i.rpal' passed."
		((passed_count++))
	fi

	# Remove generated ._lex, ._ast and ._st files if diff-only option is specified.
	if "$diff_only" == true; then
		rm "tests/generated/$i._lex"
		rm "tests/generated/$i._ast"
		rm "tests/generated/$i._st"
	fi
done

# Summarize the test results.
printf "\n%d out of %d test cases passed.\n" $passed_count $file_cnt
