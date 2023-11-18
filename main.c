#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>

// ----------Checks-if-a-file-exists----------
int fileExists(const char *fileName)
{
    struct stat file;
    if(stat(fileName, &file) == 0)
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

int main(int argc, char *argv[])
{

    // ----------Variable-decleration----------
    int file1, file2;
    int chars_read, chars_write;


    size_t argv_1_len = strlen(argv[1]);

    // ----------Checking-the-arguments----------
    // If user haven't give any parameter
    if (argc <= 1)
    {
        print_usage();
        exit(1);
    }
    else if (argc > 4)
    {
        // If user puts space between -b and buffer size, ex. mycopy -b 1024 file1 file2
        if (argv_1_len == 2 && is_str_part_digit(argv, 2, 0, strlen(argv[2])))
        {
            printf("Incorrect form of buffer specification!\n");
            printf("example: [-b1024]\n\n");
            print_usage();
            exit(17);
        }
        else
        {
            printf("Too many parameters!\n");
            print_usage();
            exit(18);
        }
    }
    // if argv[1] has the format of '-bxxxxx'
    else if (argv[1][0] == '-' && argv[1][1] == 'b')
    {

        // If user doesn't specify the buffer size!
        if (argv_1_len == 2)
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
        else if (!is_str_part_digit(argv, 1, 2, argv_1_len))
        {
            printf("Buffer size is not valid!, can be only integer!\n");
            print_usage();
            exit(14);
        }
        else
        {
            if (!fileExists(argv[2]))
            {
                printf("\"%s\" does not exist!\n", argv[2]);
                exit(19);
            }

            // i take the integer part of '-b1024'
            char* temp_string = argv[1] + 2;
            char buf[atoi(temp_string)];

            // ----------Openning-the-files----------
            if ((file1 = open(argv[2], O_RDONLY)) < 0)
            {
                perror("Open1: ");
                exit(1);
            }

            if ((file2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0)
            {
                perror("Open2: ");
                exit(5);
            }

            // time_start
            chars_read = read(file1, buf, sizeof(buf));

            while (chars_read > 0)
            {
                if (chars_read < 0)
                {
                    perror("read: ");
                    exit(2);
                }

                chars_write = write(file2, buf, chars_read);
                
                if (chars_write < 0 || chars_write != chars_read)
                {
                    perror("write: ");
                    exit(7);
                }

                chars_read = read(file1, buf, sizeof(buf));
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
        }
    }
    else
    {
        if (!fileExists(argv[1]))
        {
            printf("\"%s\" does not exist!", argv[1]);
            exit(19);
        }
        else{
            clock_t start,elapsed;
            double time_spent;
            
            FILE* file1 = fopen(argv[1],"r");
            if (file1 == NULL) {
              perror("Σφάλμα κατά το άνοιγμα του πρώτου αρχείου. \n");
              return 1;
            }

            FILE *file2 = fopen(argv[2], "w");
            if (file2 == NULL) {
                perror("Σφάλμα κατά το άνοιγμα του αρχείου 'test2.txt'");
                fclose(file1);
                return 1;
            }

            char c;
            start = clock();

            while (1) {
              c = fgetc(file1);

              if (c == EOF) {
                break;
              }

              fputc(c, file2);

            }
            elapsed = clock() - start;
            time_spent = (double)elapsed *1000.0/CLOCKS_PER_SEC;

            fclose(file1);
            fclose(file2);

        }
    }

    return 0;
}
