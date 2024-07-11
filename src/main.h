#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_TASKS "data_tasks.txt"

typedef enum { INT, CHAR } typedata;

typedef union {
  int id;
  char new_task[100];
} value;

typedef struct {
  int id;
  char name_task[100];
  int done;
} task;

typedef struct {
  size_t task_count;
  task* buffer_tasks;
  size_t len_buffer;
} buffer;

void menu();
void print_tasks(buffer* buffer_data);
buffer* read_file();
void add_new_task(char* name_new_task, buffer* buffer_data);
void delete_task(int id, buffer* bd);
void write_in_file(buffer* bd);
void close_task(int id, buffer* bd);
void all_clean(buffer* bd);
void clean_done_tasks(buffer* bd);
void print_menu();
void welcome_message();