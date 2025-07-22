#include "minishell.h"

void free_token(t_token *head) 
{
    t_token *current;
    t_token *next;

    current = head;
    while (current != NULL) 
    {
        next = current->next;
        if (current->value) {
            free(current->value);
        }
        free(current);
        current = next;
    }
}