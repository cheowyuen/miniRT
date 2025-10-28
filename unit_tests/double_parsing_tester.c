#include <stdio.h>
#include <stdlib.h>
#include "minirt.h"
#include "libft.h"

double ft_atod(char *line, int *read, char delimiter);


/*
Some expected behaviour: correct double, but wrong delimiter: value is correct, 
but characters read set to -1 so can react:

./tester -55.0,10.0,0. ' '
Parsed double: 55.000000
Characters read: -1

correct double and delimiter: doube returned, and characters read allows you to
 move to the start of the next number or space after a number:
./tester -55.0,10.0,0. ','
Parsed double: -55.000000
Characters read: 6

*/
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s \"string\" 'delimiter'\n", argv[0]);
        return 1;
    }

    char *line = argv[1];
    char delimiter = argv[2][0]; // Only use the first character
    int read = 0;

    double result = ft_atod(line, &read, delimiter);

    printf("Parsed double: %f\n", result);
    printf("Characters read: %d\n", read);

    return 0;
}
