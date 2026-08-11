#include <unistd.h>

void    putstr(char *start, char *end)
{
    while (start < end)
    {
        write(1, start, 1);
        start++;
    }
}
int main(int ac, char *av[])
{
    int first_word = 0;
    char    *str;
    int     start;
    int     end;
    int     i = 0;

    if (ac > 1)
    {
        str = av[1];
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        start = str[i];
        while (str[i] && (str[i] != ' ' && str[i] != '\t'))
            i++;
        end = str[i];
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        while (str[i])
        {
            if (str[i] == ' ' || str[i] == '\t')
            {
                while (str[i] == ' ' || str[i] == '\t')
                    i++;
                if (str[i])
                {
                    if (first_word)
                        write(1, " ", 1);
                }
            }
            else
            {   
                write(1, str++, 1);
                first_word = 1;
            }
        }
        if (start < end)
        {
            if (first_word)
                write(1, " ", 1);
            while (start <= end)
                write(1, &str[start++], 1);
        }
    }
    write(1, "\n", 1);
    return 0;
}

// #include <unistd.h>

// #include <unistd.h>

// void	ft_putstr(char *start, char *end)
// {
// 	while (start < end)
// 		write(1, start++, 1);
// }

// void	rostring(char *str)
// {
//     int i = 0;
// 	int	start;
// 	int	end;
// 	int		first_word_printed = 0;

// 	// Skip leading spaces/tabs
// 	while (*str == ' ' || *str == '\t')
// 		str++;

// 	// Mark the start of the first word
// 	start = str;

// 	// Find the end of the first word
// 	while (*str && *str != ' ' && *str != '\t')
// 		str++;

// 	end = str;

// 	// Skip whitespace after the first word
// 	while (*str == ' ' || *str == '\t')
// 		str++;

// 	// Print remaining words
// 	while (*str)
// 	{
// 		if (*str != ' ' && *str != '\t')
// 		{
// 			if (first_word_printed)
// 				write(1, " ", 1);

// 			// Print the next word
// 			while (*str && *str != ' ' && *str != '\t')
// 				write(1, str++, 1);

// 			first_word_printed = 1;
// 		}
// 		else
// 			str++; // Skip spaces/tabs
// 	}

// 	// Print first word at the end
// 	if (start < end)
// 	{
// 		if (first_word_printed)
// 			write(1, " ", 1);
// 		ft_putstr(start, end);
// 	}

// 	write(1, "\n", 1);
// }


// int main(int ac, char *av[])
// {
//     if (ac > 1)
//         rostring(av[1]);
//     else
//         write(1, "\n", 1);
//     return 0;
// }