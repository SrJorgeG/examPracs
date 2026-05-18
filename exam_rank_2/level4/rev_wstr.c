#include <unistd.h>

void    rev_wstr1(char *str)
{
    int i = 0;
    int end = 0;
    int word = 0;

    while (str[i])
        i++;
    while (i >= 0)
    {
        while (str[i] == '\0' || str[i] == ' ')
            i--;
        end = i;
        while (str[i] && str[i] != ' ')
            i--;
        word = i + 1;
        while (word <= end)
        {
            write(1, &str[word], 1);
            word++;
        }
        if (word && str[i])
            write(1, " ", 1);
        i--;
    }
}

void    rev_wstr2(char *str)
{
    int i = 0;
    int end = 0;
    int word = 0;

    while (str[i])
        i++;
    while (i >= 0)
    {
        while (str[i] == '\0' || str[i] == ' ')
            i--;
        end = i;
        while (str[i] && str[i] != ' ')
            i--;
        word = i + 1;
        while (word <= end)
        {
            write(1, &str[word], 1);
            word++;
        }
        if (word && str[i])
            write(1, " ", 1);
        i--;
    }
}
void    rev_wstr3(char *str)
{
    int i = 0;
    int end = 0;
    int word = 0;

    while (str[i])
        i++;
    while (i >= 0)
    {
        while (str[i] == '\0' || str[i] == ' ')
            i--;
        end = i;
        while (str[i] && str[i] != ' ')
            i--;
        word = i + 1;
        while (word <= end)
        {
            write(1, &str[word], 1);
            word++;
        }
        if (word && str[i])
            write(1, " ", 1);
        i--;
    }
}
void    rev_wstr4(char *str)
{
    int i = 0;
    int end = 0;
    int word = 0;

    while (str[i])
        i++;
    while (i >= 0)
    {
        while (str[i] == '\0' || str[i] == ' ')
            i--;
        end = i;
        while (str[i] && str[i] != ' ')
            i--;
        word = i + 1;
        while (word <= end)
        {
            write(1, &str[word], 1);
            word++;
        }
        if (word && str[i])
            write(1, " ", 1);
        i--;
    }
}
// void    rev_wstr5(char *str)
// {
    
// }
// void    rev_wstr6(char *str)
// {
    
// }
// void    rev_wstr7(char *str)
// {
    
// }
// void    rev_wstr8(char *str)
// {
    
// }
// void    rev_wstr9(char *str)
// {
    
// }
// void    rev_wstr10(char *str)
// {
    
// }
// void    rev_wstr11(char *str)
// {
    
// }
// void    rev_wstr12(char *str)
// {
    
// }
// void    rev_wstr13(char *str)
// {
    
// }
// void    rev_wstr14(char *str)
// {
    
// }
// void    rev_wstr15(char *str)
// {
    
// }
// void    rev_wstr16(char *str)
// {
    
// }
// void    rev_wstr17(char *str)
// {
    
// }
// void    rev_wstr18(char *str)
// {
    
// }
// void    rev_wstr19(char *str)
// {
    
// }
// void    rev_wstr20(char *str)
// {
    
// }
int main(int ac, char *av[])
{
    if (ac == 2)
    {
        rev_wstr4(av[1]);
    }
    else
        write(1, "\n", 1);
    return 0;
}
