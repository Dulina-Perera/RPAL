/**
 * @file main.c
 * @brief Entry point of the program.
 *
 * This file contains the main function, which serves as the entry point of the program.
 *
 * It parses command line arguments, opens a file, reads its content, performs lexical analysis
 * and/or abstract syntax tree generation based on the command line arguments, and frees allocated memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/cse_machine.h"

#define MAX_FILE_SIZE 100000
#define SWITCH_COUNT 3
#define TEST_FILE "tests/1.rpal"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Main function of the program.
 *
 * This function is the entry point of the program. It takes command line arguments, parses them,
 * opens a file, reads its content, performs lexical analysis and/or abstract syntax tree generation
 * based on the command line arguments, and frees allocated memory.
 *
 * @param argc The number of command line arguments
 * @param argv An array of strings containing the command line arguments
 * @return The exit status of the program.
 */
int main(int argc, char *argv[])
{
    // Array of valid command line switches
    const char *const switches[] = {"-lex", "-ast", "-st"};

    // Array to keep track of found switches.
    size_t found_switch[SWITCH_COUNT] = {0};

    // Iterate through command line arguments.
    for (int i = 1; i < argc; ++i)
    {
        // Check if the argument matches any of the valid switches.
        for (int j = 0; j < SWITCH_COUNT; ++j)
        {
            if (strncmp(argv[i], switches[j], (size_t)sizeof(switches[j])) == 0)
            {
                // Check if the switch has already been found.
                if (found_switch[j] > 0)
                {
                    fprintf(stderr, "Duplicate switch: '%s'\n", argv[i]);
                    return EXIT_FAILURE;
                }

                found_switch[j] = i;
                break;
            }
        }
    }

    // Count the number of switches found.
    size_t switches_found = 0, temp = 0;
    for (int i = 0; i < sizeof(found_switch) / sizeof(found_switch[0]); i++)
    {
        if (found_switch[i] > 0)
        {
            switches_found++;
            temp += found_switch[i];
        }
    }
    // Check if there are no other arguments between switches.
    if (temp != switches_found * (switches_found + 1) / 2)
    {
        perror("Invalid switch order.\n");
        return EXIT_FAILURE;
    }

    // Check if there are too many arguments.
    if (argc > switches_found + 2)
    {
        perror("Too many arguments.\n");
        return EXIT_FAILURE;
    }

    // Get the file path as a command line argument or use a default file path.
    char *file_path = (argc == switches_found + 2) ? argv[switches_found + 1] : TEST_FILE;

    // Open the file for reading.
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return EXIT_FAILURE;
    }

    // Allocate memory to store the file content.
    char *content = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
    if (content == NULL)
    {
        fclose(file);

        fprintf(stderr, "Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    // Read the content of the file into the allocated memory.
    size_t bytesRead = fread(content, sizeof(char), MAX_FILE_SIZE, file);
    content[bytesRead] = '\0'; // Null-terminate the string

    // Close the file.
    fclose(file);

    // Perform lexical analysis and/or abstract syntax tree generation based on command line argument.
    TokenStream *stream = NULL;
    Vertex *ast = NULL, *st = NULL;
    if (found_switch[2] > 0)
    {
        stream = lex(content);
        ast = parse(stream);
        st = standardize(ast);

        ast = NULL;
        stream = NULL;
    } 
    else if (found_switch[1] > 0)
    {
        stream = lex(content);
        ast = parse(stream);

        stream = NULL;
    }
    else if (found_switch[0] > 0)
    {
        stream = lex(content);
    }

    if (stream != NULL)
    {
        display_list(stream);
    }
    if (ast != NULL)
    {
        AST(ast);
    }
    if (st != NULL)
    {
        ST(st);
    }

    stream = lex(content);
    ast = parse(stream);
    st = standardize(ast);
    // ST(st);
    init_cse_machine(st);
    eval_cse_machine();

    // Free the allocated memory.
    free(stream);
    stream = NULL;

    free(content);
    content = NULL;

    return EXIT_SUCCESS;
}
