#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>


static int myCompare (const void * a, const void * b) {    //https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
    return strcmp (*(const char **) a, *(const char **) b);
}

void sort(char *arr[], int n) {
    qsort (arr, n, sizeof (const char *), myCompare);
}


typedef struct node{
    char* content;
    struct node *previous;
    struct node *next;
} node;


sem_t* semaphores;
node* head;

typedef struct thread_job {
    int job_id;
    char* input_file_name;
    char* color;
} thread_job;


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


void *do_thread_job(void *k) {

    thread_job *current_job = ((thread_job*)k);

    char *file_content = read_file(current_job->input_file_name);
    int color_count = 0;
    int total_color_count = 0;

    char *color = strtok(file_content, " \n");

    while (color != NULL) {
        if (strcmp(color, current_job->color) == 0) {
            color_count++;
        }
        total_color_count++;
        color = strtok(NULL, " \n");
    }

    char *new_message = malloc(sizeof(char)*100);
    sprintf(new_message, "%s  %s  %d/%d\n",current_job->input_file_name, current_job->color,  color_count, total_color_count);
    usleep(5000000);
    int semaphore_id = current_job->job_id;
    sem_wait(&semaphores[semaphore_id]);

    node* current_node = head;

    while( current_node->next != NULL) {
        current_node = current_node->next;
    }

    if (current_node->content != NULL) {
        current_node->next = malloc(sizeof(node));
        current_node->next->content = new_message;
        current_node->next->next = NULL;
    } else {
        current_node->content = new_message;
    }

    sem_post(&semaphores[current_job->job_id + 1]);
    return NULL;
}


int psearch3(char* color_name, int input_file_count, char* input_filenames[], char* output_filename, pthread_t thread_id[]){


    for (int k = 0; k < input_file_count; ++k) {    // Children threads //

        thread_job* newJob = malloc(sizeof(thread_job));

        char *reading_file = input_filenames[k];
        newJob->job_id = k;
        newJob->input_file_name = reading_file;
        newJob->color = color_name;

        pthread_create(&thread_id[k], NULL, &do_thread_job, newJob);
    }


    sem_wait(&semaphores[input_file_count]);

    char *final_output = (char *) malloc(strlen(head->content));

    final_output = head->content;
    node *current_node = head;

    while(current_node->next != NULL){
        current_node = current_node->next;

        // Add next nodes content to final output
        char* temp_final_output = (char*) malloc(1 + strlen(final_output) + strlen(current_node->content));
        strcpy(temp_final_output, final_output);
        strcat(temp_final_output, current_node->content);
        final_output = temp_final_output;
    };

    //Main Thread

    FILE *final_output_file = fopen(output_filename , "w");

    if (final_output_file == NULL) {
        perror("Error occurred when writing file!");
        exit(0);
    }
    fprintf(final_output_file, "%s", final_output);
    fclose(final_output_file);

}

int main(int argc, char *argv[]) {

    char* color_name = "Yellow";
    int count_of_inout_file = 4;
    char* input_filenames[] = {"input1.txt", "input2.txt", "input3.txt", "input4.txt"};
    char* output_filename = "Output.txt";


//    char* color_name = argv[1];
//    int count_of_inout_file = atoi(argv[2]);
//    char* input_filenames[count_of_inout_file];
//    char* output_filename = argv[count_of_inout_file+3];

//    for (int i = 0; i < count_of_inout_file; ++i) {
//        input_filenames[i] = argv[i+3];
//    }
    int n = sizeof(input_filenames)/sizeof(input_filenames[0]);
    sort(input_filenames, n);

    head = (node*) malloc(sizeof(node));
    head->content = NULL;
    head->previous = NULL;
    head->next = NULL;


    pthread_t thread_id[count_of_inout_file];
    sem_t sem[count_of_inout_file];
    semaphores = sem;

    if (semaphores == MAP_FAILED) {
        perror("Semaphore mapping failed!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= count_of_inout_file; i++) {
        sem_init(&semaphores[i], count_of_inout_file, 0);
    }
    sem_post(&semaphores[0]);


    psearch3(color_name, count_of_inout_file, input_filenames, output_filename, thread_id);

    return 0;
}