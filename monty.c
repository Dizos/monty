#include "monty.h"
#include <ctype.h>

#define BUFFER_SIZE 1024
stack_t *stack = NULL;

instruction_t instructions[] = {
    {"push", push},
    {"pall", pall},
    {"pint", pint},
    {"pop", pop},
    {"swap", swap},
    {NULL, NULL}
};

/**
 * main - Monty bytecode interpreter
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int main(int argc, char *argv[])
{
    FILE *file;
    char line[BUFFER_SIZE];
    unsigned int line_number = 0;
    char *opcode;
    int i;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        return EXIT_FAILURE;
    }

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_number++;
        opcode = strtok(line, " \t\n");
        if (opcode == NULL || opcode[0] == '#')
            continue;

        for (i = 0; instructions[i].opcode; i++)
        {
            if (strcmp(opcode, instructions[i].opcode) == 0)
            {
                instructions[i].f(&stack, line_number);
                break;
            }
        }

        if (instructions[i].opcode == NULL)
        {
            fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
            fclose(file);
            free_stack(stack);
            return EXIT_FAILURE;
        }
    }

    fclose(file);
    free_stack(stack);
    return EXIT_SUCCESS;
}

/**
 * is_number - checks if a string is a valid number
 * @str: string to check
 * Return: 1 if valid number, 0 otherwise
 */
int is_number(char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    if (*str == '-')
        str++;

    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }

    return 1;
}
