#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

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


int main(int argc, char *argv[]) {

    char* color_name = argv[1];
    char* reading_file = argv[2];

    char* path = "../psearch2b/";

    char* reading_file_path = (char*) malloc(1 + strlen(reading_file) + strlen(path));
    strcpy(reading_file_path, path);
    strcat(reading_file_path, reading_file);

    char *file_content = read_file(reading_file_path);

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

    char new_output[100];
    sprintf(new_output, "%s  %s  %d/%d\n",reading_file, color_name,  color_count, total_color_count);



    return 0;
}