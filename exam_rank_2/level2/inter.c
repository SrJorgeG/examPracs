#include <unistd.h>
#include <stdbool.h>

void    inter(char *str1, char *str2)
{
    int i = 0;
    int j;
    bool    seen[256] = {false};

    while (str1[i])
    {
        j = 0;
        while (str2[j])
        {
            if (str2[j] == str1[i])
            {
                if (!seen[str1[i]])
                {
                    write(1, &str1[i], 1);
                    seen[str1[i]] = 1;
                }
            }
            j++;
        }
        i++;
    }
}
int main(int ac, char *av[])
{
    if (ac == 3)
        inter(av[1], av[2]);
	write(1, "\n", 1);
    return 0;
}

// int main(int ac, char *av[])
// {
//     int i = 0;
//     int j;
//     bool   seen[256] = {false};

//     if (ac == 3)
//     {
//         while (av[1][i])
//         {
//             j = 0;
//             while (av[2][j])
//             {
//                 if (av[1][i] == av[2][j])
//                 {
//                     if (!seen[(unsigned char)av[1][i]])
//                     {
//                         write(1, &av[1][i], 1);
//                         seen[(unsigned char)av[1][i]] = 1;
//                     }
//                 }
//                 j++;
//             }
//             i++;
//         }
//     }
//     write(1, "\n", 1);
//     return 0;
// }
