#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/wait.h>

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

int psearch1a(char* color_name, int input_file_count, char* input_filenames[], char* output_filename){

    for (int i = 0; i < input_file_count; ++i) {
        char *reading_file = input_filenames[i];
        pid_t pid = fork();

        if (pid < 0) {  // Fork Error
            fprintf(stderr, "Fork Failed!");
            return 1;
        } else if (pid == 0) {     // Child process

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

            char temp_output_filename[30];
            sprintf(temp_output_filename, "%s_%d.txt", "temp", i);

            FILE *temp_output_file = fopen(temp_output_filename , "w");
            if (temp_output_file == NULL) {
                perror("Error occurred when writing file!");
                exit(0);
            }
            fprintf(temp_output_file, "%s  %s  %d/%d\n",reading_file, color_name,  color_count, total_color_count);
            fclose(temp_output_file);
            exit(0);
        }
    }

    //Parent process

    int status;     //https://www.youtube.com/watch?v=9seb8hddeK4&t=1316s

    for (int j = input_file_count; j > 0 ; j--) {  //Wait till all child processes end.
        wait(&status);
    }

    char* final_output_content;
    final_output_content = realloc(NULL, sizeof(char*));

    for (int i = 0; i < input_file_count; ++i) {

        char temp_output_filename[30];
        sprintf(temp_output_filename, "%s_%d.txt", "temp", i);

        final_output_content = realloc(final_output_content, sizeof(char*) * strlen(read_file(temp_output_filename)));

        strcat(strcat(final_output_content, read_file(temp_output_filename)),"\n");
        remove(temp_output_filename);
    }

    FILE *final_output_file = fopen(output_filename , "w");

    if (final_output_file == NULL) {
        perror("Error occurred when writing file!");
        exit(0);
    }
    fprintf(final_output_file, "%s", final_output_content);
    fclose(final_output_file);

    return 1;
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

    psearch1a(color_name, count_of_inout_file, input_filenames, output_filename);

}


