#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

// ----------Checks-if-a-file-exists----------
int fileExists(const char *fileName)
{
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
int is_str_part_digit(char* arr[], int n, size_t len)
{
    for (int i = n; i < len; i++)
        {
            if (!isdigit(arr[i]))
            {
                print_usage();

                exit(11);
            }
        }
}

int main(int argc, char *argv[])
{

    // ----------Variable-decleration----------
    int file1, file2;
    int chars_read, chars_write;
    char buf[512];

    // ----------Checking-the-arguments----------
    if (argc <= 1)
    {
        print_usage();
        exit(1);
    }
    // if argv[1] has the format of '-bxxxxx'
    else if (argv[1][0] == '-' && argv[1][1] == 'b')
    {
        size_t argv_1_len = strlen(argv[1]);
        // If user puts space between -b and buffer size, ex. mycopy -b 1024 file1 file2
        // Or
        // If user doesn't specify the buffer size!
        if (argv_1_len = 2)
        {
            printf("Din't specify the buffer size!\n");
            printf("example: [-b1024]\n\n");
            print_usage();
            exit(12);
        }
        // The max buffer size can be -b99999
        else if (argv_1_len > 7)
        {
            printf("Maximum buffer size is 99999 !\n");
            printf("example: [-b99999]\n\n");
            print_usage();
            exit(13);
        }
        // Checks if every x of -bxxxxx is int
        else if(is_str_part_digit(argv[1], 2, argv_1_len))
        {
            printf("Buffer size is not valid!, it must be integer!");
            print_usage();
            exit(14);
        }
        else
        {
            printf("successes! 23");
        }

        
    }
    else
    {
        printf("not -b");
    }

    printf("\"%s\" not found!\n", argv[1]);
    exit(8);


// ----------Openning-the-files----------
if ((file1 = open("tmp.txt", O_RDONLY)) < 0)
{
    perror("Open1: ");
    exit(1);
}

if ((file2 = open("tmp2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600) < 0))
{
    perror("Open2: ");
    exit(5);
}

chars_read = read(file1, buf, sizeof(buf));

while (chars_read > 0)
{
    chars_read = read(file1, buf, sizeof(buf));
    if (chars_read < 0)
    {
        perror("read: ");
        exit(2);
    }

    chars_write = write(file2, buf, chars_read);
    if (chars_write < 0)
    {
        perror("write: ");
        exit(7);
    }
}
if (chars_read < 0)
{
    perror("reading error: ");
    exit(3);
}

if (close(file1) < 0)
{
    perror("close1: ");
    exit(4);
}

if (close(file2) < 0)
{
    perror("close2: ");
    exit(4);
}

return 0;
}
