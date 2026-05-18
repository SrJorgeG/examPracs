#include "ft_list.h"

int	ft_list_size(t_list *begin_list) {
    int i = 0;
    t_list *curr = begin_list;
    while (curr) {
        i++;
        curr = curr->next;
    }
    return i;
}