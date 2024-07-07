#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define DATA_TASKS "data_tasks.txt"

typedef struct {
    int id;
    char name_task [100];
} task;


void add_new_task(char* name_new_task);
void delete_task(int id);
void print_tasks();
task* read_file(size_t* task_count);
