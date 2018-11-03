#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

static int myCompare (const void * a, const void * b) {    //https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
    return strcmp (*(const char **) a, *(const char **) b);
}

void sort(char *arr[], int n) {
    qsort (arr, n, sizeof (const char *), myCompare);
}

int psearch2a(char* color_name, int input_file_count, char* input_filenames[], char* output_filename){

    const char* shared_file_path= "../shared_file.txt";

    int status;

    for (int i = 0; i < input_file_count; ++i) {
        char *reading_file = input_filenames[i];
        pid_t pid = fork();

        if (pid < 0) {  // Fork Error
            fprintf(stderr, "Fork Failed!");
            return 1;
        } else if (pid == 0) {  // Child process
            execlp("../psearch2aslave/psearch2aslave", "psearch2aslave", color_name, reading_file, NULL);  //Execute Slave process
            exit(0);
        }
        pid = wait(&status);
    }

    //Parent Process

    int file_d = open(shared_file_path, O_RDWR);
    if (file_d == -1) {
        perror("File Open Error!!");
        exit(EXIT_FAILURE);
    }

    // http://man7.org/linux/man-pages/man2/stat.2.html
    struct stat f_status;
    fstat(file_d, &f_status);
    off_t fstatus_size = f_status.st_size;

    char *final_output = (char *) mmap(0, fstatus_size , PROT_READ, MAP_SHARED, file_d, 0);

    if (final_output == MAP_FAILED) {
        perror("Map Failed while mapping final output!");
    }


    FILE *final_output_file = fopen(output_filename , "w");

    if (final_output_file == NULL) {
        perror("Error occurred when writing file!");
        exit(0);
    }
    fprintf(final_output_file, "%s", final_output);
    fclose(final_output_file);

    if (munmap(final_output, strlen(final_output)) == -1) {
        perror("Error while removing mapping the file!");
    }
    close(file_d);

    remove(shared_file_path);

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

    psearch2a(color_name, count_of_inout_file, input_filenames, output_filename);

}