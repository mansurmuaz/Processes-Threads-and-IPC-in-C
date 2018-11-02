#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

#define READ 0
#define WRITE 1

char* read_file(char* file_name) {     // https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c

    long lSize;
    char* buffer;
    FILE* file = fopen(file_name, "rb");

    if (!file) {
        fclose(file);
        perror("Occurred an error while reading file!");
        exit(1);
    }

    fseek( file , 0L , SEEK_END);
    lSize = ftell( file );
    rewind( file );

    /* Allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) {
        fclose(file);
        fputs("Memory alloc fails!", stderr);
        exit(1);
    }

    /* Copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , file) ) {
        fclose(file);
        free(buffer);
        fputs("Entire read fails!", stderr);
        exit(1);
    }

    fclose(file);
    return buffer;
}

static int myCompare (const void * a, const void * b) {    //https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
    return strcmp (*(const char **) a, *(const char **) b);
}

void sort(char *arr[], int n) {
    qsort (arr, n, sizeof (const char *), myCompare);
}

int psearch1c(char* color_name, int input_file_count, char* input_filenames[], char* output_filename) {

    int pfd[input_file_count][2];


    for (int i = 0; i < input_file_count; ++i) {

        if (pipe(pfd[i]) != 0) {
            perror("Pipe Failed!");
            exit(1);
        }

        char *reading_file = input_filenames[i];

        pid_t pid = fork();

        if (pid < 0) {  // Fork Error
            fprintf(stderr, "Fork Failed!");
            return 1;
        } else if (pid == 0) {     // Child process

            close(pfd[i][READ]);

            char *file_content = read_file(reading_file);
            int color_count = 0;
            int total_color_count = 0;

            char *color = strtok(file_content, " \n");

            while (color != NULL) {
                if (strcmp(color, color_name) == 0) {
                    color_count++;
                }
                total_color_count++;
                color = strtok(NULL, " \n");
            }

            char new_message[100];
            sprintf(new_message, "%s  %s  %d/%d\n",reading_file, color_name,  color_count, total_color_count);
            if (write(pfd[i][WRITE], new_message, 100) == -1) {
                perror("Write didn't return expected value!\n");
                exit(2);
            }

            close(pfd[i][WRITE]);
            exit(0);
        }
    }

    // Parent process
    int status;     //https://www.youtube.com/watch?v=9seb8hddeK4&t=1316s

    for (int j = input_file_count; j > 0 ; j--) {  //Wait till all child processes end.
        wait(&status);
    }

    char* final_output = (char*) malloc(sizeof(char)*100*input_file_count);

    for (int k = 0; k < input_file_count; ++k) {
        close(pfd[k][WRITE]);

        char* temp_output = (char*) malloc(sizeof(char)*100);
        if (read(pfd[k][READ], temp_output, 100) == -1) {
            perror("Read didn't return expected value!");
            exit(3);
        }
        close(pfd[k][READ]);
        strcat(final_output, temp_output);

    }


    FILE *final_output_file = fopen(output_filename , "w");

    if (final_output_file == NULL) {
        perror("Error occurred when writing file!");
        exit(0);
    }
    fprintf(final_output_file, "%s", final_output);
    fclose(final_output_file);

    free(final_output);
    return 0;
}


int main(int argc, char *argv[]) {


    char* color_name = argv[1];
    int count_of_inout_file = atoi(argv[2]);
    char* input_filenames[count_of_inout_file];
    char* output_filename = argv[count_of_inout_file+3];

    for (int i = 0; i < count_of_inout_file; ++i) {
        input_filenames[i] = argv[i+3];
    }
    int n = sizeof(input_filenames)/sizeof(input_filenames[0]);
    sort(input_filenames, n);

    psearch1c(color_name, count_of_inout_file, input_filenames, output_filename);

}