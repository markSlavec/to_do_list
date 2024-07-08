#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define DATA_TASKS "data_tasks.txt"

typedef struct {
    int id;
    char name_task [100];
    int done;
} task;

typedef struct {
    size_t task_count;
    task* buffer_tasks;
    size_t len_buffer;
} buffer;


void add_new_task(char* name_new_task);
void delete_task(int id);
void print_tasks(buffer* buffer_data);
buffer* read_file();
void close_task(int id);