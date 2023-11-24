#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include "functions.h"

// fileStat1 είναι το stat που έχει πληροφορίες για το πρώρο αρχείο
struct stat fileStat1, fileStat2;

int main(int argc, char *argv[])
{
    // ----------Variable-decleration----------
    int file1, file2;
    int using_standard_buffer = 1;
    int buffer_size;
    int chars_read, chars_write;
    clock_t start, elapsed, start_dU_count;
    double time_spent;
    double time_will_take;

    // dU είναι η ταχύτητα αντιγραφής μιας buffer
    double dU;
    long int bytes_copied = 0;
    long int bytes_copied_dU = 0;
    double bytes_copied_percentage;
    char hash_tag_array[40];
    char time_left[] = {"Time Left:"};
    for (int i = 0; i < 40; i++)
    {
        hash_tag_array[i] = '.';
    }

    // ----------Checking-the-arguments----------
    // If user haven't give any parameter
    if (argc <= 1)
    {
        print_usage();
        exit(1);
    }
    else if (argc > 4)
    {
        size_t argv_1_len = strlen(argv[1]);
        // If user puts space between -b and buffer size, ex. mycopy -b 1024 file1 file2
        if (argv_1_len == 2 && is_str_part_digit(argv, 2, 0, strlen(argv[2])))
        {
            printf("Incorrect form of buffer specification!\n");
            printf("example: [-b1024]\n\n");
            print_usage();
            exit(3);
        }
        else
        {
            printf("Too many parameters!\n");
            print_usage();
            exit(2);
        }
    }
    // if argv[1] has the format of '-bxxxxx'
    else if (argv[1][0] == '-' && argv[1][1] == 'b')
    {
        size_t argv_1_len = strlen(argv[1]);
        // If user doesn't specify the buffer size!
        if (argv_1_len == 2)
        {
            printf("Din't specify the buffer size!\n");
            printf("example: [-b1024]\n\n");
            print_usage();
            exit(4);
        }
        // The max buffer size can be -b99999
        else if (argv_1_len > 7)
        {
            printf("Maximum buffer size is 99999 !\n");
            printf("example: [-b99999]\n\n");
            print_usage();
            exit(5);
        }
        // Checks if every x of -bxxxxx is int
        else if (!is_str_part_digit(argv, 1, 2, argv_1_len))
        {
            printf("Buffer size is not valid! can be only integer!\n");
            print_usage();
            exit(6);
        }
        // User didn't specify the file1 and file2
        else if (argc == 2)
        {
            printf("Didn't specify any file!\n");
            print_usage();
            exit(7);
        }
        else
        {
            if (!fileExists(argv[2]))
            {
                printf("\"%s\" does not exist!\n", argv[2]);
                exit(9);
            }
            if (argc == 3)
            {
                printf("Didn't specify the destination file!\n");
                print_usage();
                exit(8);
            }

            // we take the integer part of '-b1024'
            char *temp_string = argv[1] + 2;
            buffer_size = atoi(temp_string);

            // The buffer size can't be 0
            if (buffer_size == 0)
            {
                printf("The buffer size can't be 0\n");
                printf("example: [-b1024]\n");
                print_usage();
                exit(10);
            }
            char buf[buffer_size];
            using_standard_buffer = 0;

            // ----------Openning-the-files----------
            if ((file1 = open(argv[2], O_RDONLY)) < 0)
            {
                perror("Open1: ");
                exit(13);
            }
            if (stat(argv[2], &fileStat1) != 0)
                exit(15);

            if ((file2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0)
            {
                perror("Open2: ");
                exit(13);
            }

            // time_start
            start = clock();
            chars_read = read(file1, buf, buffer_size);

            // Εδώ ξεκινάει η αντιγραφή
            start_dU_count = clock();
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
                else
                {
                    // how many bytes has been copied from the beggining of the program
                    bytes_copied += chars_write;
                    // how many bytes has been copied last 10000 ms
                    bytes_copied_dU += chars_write;
                }
                chars_read = read(file1, buf, sizeof(buf));
                // each 10 ms we refresh, update the frame!
                while (clock() - start_dU_count > 10000)
                {
                    // we are finding the percentage of the size has been copied!
                    bytes_copied_percentage = ((double)bytes_copied / fileStat1.st_size * 100);

                    // \r : move the cursor to the beginning of the line
                    // other option would be \b
                    printf("\r                                                                                  ");
                    printf("\r%3d%%", (int)bytes_copied_percentage);
                    printf("%2s", "[");
                    for (int i = 0; i < 40; i++)
                    {
                        // 0.4 because we have 40 . in hash_tag_array[40] and we multiply it with 0.01 to find the percentage
                        if (i < (int)bytes_copied_percentage * 0.4)
                        {
                            printf("#");
                        }
                        else
                        {
                            printf("%c", hash_tag_array[i]);
                        }
                    }
                    printf("%s", "]");

                    // dU in bytes/seconds, dU: μεταβολή της ταχύτητα μια χρονική στιγμή
                    dU =  1000 * bytes_copied_dU / ((clock() - start_dU_count) / 1000);

                    time_will_take = (fileStat1.st_size - bytes_copied) / dU;

                    printf("%20s%.0lf Seconds", time_left, time_will_take);
                    fflush(stdout);

                    start_dU_count = clock();
                    bytes_copied_dU = 0;
                }
            }
            // time_end
            elapsed = clock() - start;

            printf("\r                                                                                ");
            printf("\r100%% [########################################]          Copied Successfully!\n");
            printf("\n");

            if (chars_read < 0)
            {
                perror("reading error: ");
                exit(3);
            }

            // ----------closing-the-files----------
            if (close(file1) < 0)
            {
                perror("close1: ");
                exit(13);
            }

            if (close(file2) < 0)
            {
                perror("close2: ");
                exit(13);
            }
            if (stat(argv[3], &fileStat2) != 0)
                exit(15);
        }
    }
    // if user wants to use standard buffer can't insert more than 3 parameters
    else if (argc == 4)
    {
        printf("Too many parameters!\n");
        print_usage();
        exit(2);
    }
    else
    {
        if (!fileExists(argv[1]))
        {
            printf("\"%s\" does not exist!\n", argv[1]);
            exit(9);
        }
        else if (argc == 2)
        {
            printf("Didn't specify the destination file!\n");
            print_usage();
            exit(8);
        }
        else if (argc == 4)
        {
            printf("Too many parameters!\n");
            print_usage();
            exit(2);
        }
        // if argc == 3:
        else
        {
            // ----------opening-the-files-------------
            FILE *file1 = fopen(argv[1], "rb");
            if (file1 == NULL)
            {
                perror("Σφάλμα κατά το άνοιγμα του πρώτου αρχείου. \n");
                exit(13);
            }
            if (stat(argv[1], &fileStat1) != 0)
                exit(15);

            FILE *file2 = fopen(argv[2], "wb");
            if (file2 == NULL)
            {
                perror("Σφάλμα κατά το άνοιγμα του αρχείου 'test2.txt'");
                fclose(file1);
                exit(13);
            }

            char character;
            // time_start
            start = clock();

            while (1)
            {
                character = fgetc(file1);

                if (character == EOF)
                    break;

                fputc(character, file2);
            }
            // time_end
            elapsed = clock() - start;

            // --------------closing-the-files---------------
            fclose(file1);
            fclose(file2);
            if (stat(argv[2], &fileStat2) != 0)
                exit(15);
        }
    }

    // ---------------------------Reporting ...  ---------------------------|
    time_spent = (double)elapsed / CLOCKS_PER_SEC;

    off_t file_2_size = fileStat2.st_size;

    // velocity B/s  (Bytes / second)
    long double copy_velocity = file_2_size / time_spent;

    // Writing on report.out file
    // opening-the-file
    FILE *report_file;
    if ((report_file = fopen("report.out", "a+")) == NULL)
    {
        perror("Error while opening the report.out file!\n");
        exit(13);
    }

    const char *STANDARD_BUFFER_STRING = "STANDARD BUFFER";
    struct stat reportStat;
    stat("report.out", &reportStat);

    if (reportStat.st_size == 0)
    {
        fprintf(report_file, "File Size:          Buffer Size:        Copy Velocity:      \n");
        fprintf(report_file, "----------          ---------------     --------------      \n");
    }

    printf("File Size:          Buffer Size:        Copy Velocity:      \n");
    printf("----------          ---------------     --------------      \n");

    if (using_standard_buffer)
    {
        fprintf(report_file, "%-20ld%-20s%-10.0Lf B/s\n", file_2_size, STANDARD_BUFFER_STRING, copy_velocity);
        printf("%-20ld%-20s%-10.0Lf B/s\n\n", file_2_size, STANDARD_BUFFER_STRING, copy_velocity);
    }
    else
    {
        fprintf(report_file, "%-20ld%-20d%-10.0Lf B/s\n", file_2_size, buffer_size, copy_velocity);
        printf("%-20ld%-20d%-10.0Lf B/s\n\n", file_2_size, buffer_size, copy_velocity);
    }

    if (fclose(report_file) != 0)
    {
        perror("closing report.out: ");
        exit(13);
    }

    return 0;
}

// Άμα διαβάσετε μέχρι το τέλος, σας ευχαριστούμε  :)
// Μπορείτε να συμβάλλετε στο Github!
