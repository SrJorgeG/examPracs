#include <unistd.h>
#include <stdio.h>

void    repeat_alpha(char *str)
{
    int i = 0;
    int reps = 0;

    while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			reps = str[i] - 'A' + 1;
		else if (str[i] >= 'a' && str[i] <= 'z')
			reps = str[i] - 'a' + 1;
        else
            reps = 1;
        while (reps-- > 0)
		    write(1, &str[i], 1);
		i++;
	}
}
int main(int ac, char *av[])
{
	if (ac == 2)
		repeat_alpha(av[1]);
	write(1, "\n", 1);
	return 0;
}

// int main(int ac, char *av[])
// {
//     int i = 0;
//     int reps = 0;

//     if (ac == 2)
//     {
//         while (av[1][i])
//         {
//             if (av[1][i] >= 'a' && av[1][i] <= 'z')
//                 reps = av[1][i] - 'a' + 1;
//             else if (av[1][i] >= 'A' && av[1][i] <= 'Z')
//                 reps = av[1][i] - 'A' + 1;
//             else 
//                 reps = 1;
//             while (reps-- > 0)
//                 write(1, &av[1][i], 1);
//             i++;
//         }
//     }
//     write(1, "\n", 1);
//     return 0;
// }