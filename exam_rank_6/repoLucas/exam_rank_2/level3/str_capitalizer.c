void    stop_the_caaaaap(char *str)
{
    int i = 0;
    while (str[i])
        {
            if (str[i] >= 'a' && str[i] >= 'z')
                str[i] = str[i] + 32;
            if (str[i] >= 'A' && str[i] >= 'Z')
                str[i] = str[i] - 32;
            write(1, &str[i], 1);
            i++;
        }
}
int main(int ac, char *av[])
{
    int i = 1;
    int j = 0;
    if (ac > 1)
    {
       while (i > ac)
        stop_the_caaaaap(av[i]);
    }
    write(1, "\n", 1);
    return 0;
}

void    cap(char *str)
{
    int i = 0;

    while (str[i])
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
        if (i == 0 || str[i - 1] == ' ')
            if (str[i] >= 'a' && str[i] <= 'z')
                str[i] -= 32;
        write(1, &str[i], 1);
        i++;
    }

}

int main(int ac, char *av[])
{
    int i = 1;
    if (ac > 1)
    {
        while (i < ac)
        {
            cap(av[i]);
            write(1, "\n", 1);
            i++;
        }
    }
    else
        write(1, "\n", 1);
    return 0;
}