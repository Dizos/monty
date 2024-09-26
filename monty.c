#include "monty.h"
#include <ctype.h>

#define BUFFER_SIZE 1024

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
    stack_t *stack = NULL;
    char *opcode;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line_number++;
        opcode = strtok(line, " \t\n");
        if (opcode == NULL || opcode[0] == '#')
            continue;

        if (strcmp(opcode, "push") == 0)
            push(&stack, line_number);
        else if (strcmp(opcode, "pall") == 0)
            pall(&stack, line_number);
        else
        {
            fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
            fclose(file);
            free_stack(stack);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    free_stack(stack);
    return EXIT_SUCCESS;
}

/**
 * push - pushes an element to the stack
 * @stack: double pointer to the head of the stack
 * @line_number: line number in the Monty byte code file
 */
void push(stack_t **stack, unsigned int line_number)
{
    char *arg = strtok(NULL, " \t\n");
    int n, i;
    stack_t *new_node;

    if (arg == NULL)
    {
        fprintf(stderr, "L%d: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    for (i = 0; arg[i] != '\0'; i++)
    {
        if (i == 0 && arg[i] == '-')
            continue;
        if (!isdigit(arg[i]))
        {
            fprintf(stderr, "L%d: usage: push integer\n", line_number);
            exit(EXIT_FAILURE);
        }
    }

    n = atoi(arg);
    new_node = malloc(sizeof(stack_t));
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = n;
    new_node->prev = NULL;
    new_node->next = *stack;
    if (*stack != NULL)
        (*stack)->prev = new_node;
    *stack = new_node;
}

/**
 * pall - prints all values on the stack
 * @stack: double pointer to the head of the stack
 * @line_number: line number in the Monty byte code file (unused)
 */
void pall(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;
    (void)line_number;

    while (current != NULL)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

/**
 * free_stack - frees a stack
 * @stack: pointer to the head of the stack
 */
void free_stack(stack_t *stack)
{
    stack_t *temp;

    while (stack != NULL)
    {
        temp = stack;
        stack = stack->next;
        free(temp);
    }
}
