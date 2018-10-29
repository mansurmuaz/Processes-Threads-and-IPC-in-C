#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>

#define PAGESIZE 4096

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


int psearch1b(char* color_name, int input_file_count, char* input_filenames[], char* output_filename) {


    char* message_in_shared_memory = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //https://www.youtube.com/watch?v=rPV6b8BUwxM

    if (message_in_shared_memory == MAP_FAILED) {
        perror("mmap Failed!");
        exit(1);
    }

//    sem_t *semaphore = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//
//    if (semaphore == MAP_FAILED) {
//        perror("mmap Failed!");
//        exit(1);
//    }

    sem_t mutex;

    if(sem_init(&mutex, 1, 1) < 0) {
        perror("Semaphore initialization Failed!");
        exit(1);
    }

    for (int i = 0; i < input_file_count; ++i) {
        char *reading_file = input_filenames[i];

        pid_t pid = fork();

        if (pid < 0) {  // Fork Error
            fprintf(stderr, "Fork Failed!");
            return 1;
        } else if (pid == 0) {     // Child process

            sem_wait(&mutex);



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
            strcat(message_in_shared_memory, new_message);

            sem_post(&mutex);

            exit(0);
        }
    }

    // Parent process

    int status;     //https://www.youtube.com/watch?v=9seb8hddeK4&t=1316s

    for (int j = input_file_count; j > 0 ; j--) {  //Wait till all child processes end.
        wait(&status);
    }

    printf("%s", message_in_shared_memory);

    return 0;
}

int main() {

    char* input_files[] = {"input3.txt", "input4.txt", "input1.txt", "input2.txt"};
    int n = sizeof(input_files)/sizeof(input_files[0]);
    sort(input_files, n);
    psearch1b("Yellow", 4, input_files, "output.txt");

    return 0;
}