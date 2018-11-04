#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <memory.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <bits/semaphore.h>
#include <semaphore.h>

static int myCompare (const void * a, const void * b) {    //https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
    return strcmp (*(const char **) a, *(const char **) b);
}

void sort(char *arr[], int n) {
    qsort (arr, n, sizeof (const char *), myCompare);
}

int psearch2b(char* color_name, int input_file_count, char* input_filenames[], char* output_filename){

    int file_d = shm_open("/shared_mem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
    if (file_d == -1) {
        perror("Shared memory Open Error!!");
        exit(EXIT_FAILURE);
    }

    int page_size = (int) sysconf(_SC_PAGESIZE);

    ftruncate(file_d, page_size);

    char *sh_mem = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_d, 0);
    if (sh_mem == -1) {
        perror("Map Failed while mapping shared mem!");
        exit(EXIT_FAILURE);
    }

    char *semaphore_name = "/semaphore";
    sem_t *semaphore = NULL;
    semaphore = sem_open(semaphore_name, O_CREAT, 0600, 1); // read+write
    if (semaphore == SEM_FAILED) {
        perror("Semaphore Failed!");
        exit(EXIT_FAILURE);
    }

    int status;

    for (int i = 0; i < input_file_count; ++i) {
        char *reading_file = input_filenames[i];
        pid_t pid = fork();

        if (pid < 0) {  // Fork Error
            fprintf(stderr, "Fork Failed!");
            return 1;
        } else if (pid == 0) {  // Child process
            execlp("./psearch2bslave", "psearch2bslave", color_name, reading_file, NULL);  //Execute Slave process
            exit(0);
        }
        pid = wait(&status);
    }

    //Parent Process

    FILE *final_output_file = fopen(output_filename , "w");

    if (final_output_file == NULL) {
        perror("Error occurred when writing file!");
        exit(0);
    }
    fprintf(final_output_file, "%s", final_output);
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

    psearch2b(color_name, count_of_inout_file, input_filenames, output_filename);

}