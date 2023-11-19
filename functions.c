#include "functions.h"
#include <stdio.h>
#include <ctype.h>

// ----------Checks-if-a-file-exists----------
int fileExists(const char *fileName)
{
    struct stat file;
    if (stat(fileName, &file) == 0)
    {
        return 1;
    }
    return 0;
}

void print_usage()
{
    printf("-----------------------------------------------\n");
    printf("|   usage: mycopy [-bnumber] file1 file2      |\n");
    printf("|   Χρήση: mycopy [-bnumber] αρχείο1 αρχείο2  |\n");
    printf("-----------------------------------------------\n");
}

// -bxxxxx : checks if every x is digit!
// @param j : it is the index of the first dimention of => argv[j][]
// @param n : από ποιο index του πίνακα να ξεκινήσει η αναζήτηση
// @param end : μέχρι ποιο index να γίνει η αναζήτηση
int is_str_part_digit(char *arr[], int j, int n, size_t end)
{
    for (int i = n; i < end; i++)
    {
        if (!isdigit(arr[j][i]))
        {
            return 0;
        }
    }
    return 1;
}