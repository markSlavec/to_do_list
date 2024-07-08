#include "main.h"

buffer * read_files() {
    buffer buffer_data;
    FILE* data_file;
    if ((data_file = fopen(DATA_TASKS, "r")) != NULL) {
        buffer_data.len_buffer = 50;
        buffer_data.buffer_tasks = malloc(sizeof(task) * buffer_data.len_buffer);
        if (buffer_data.buffer_tasks == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            fclose(data_file);
            return NULL;
            }

        size_t counter = 0;

        while (fscanf(data_file, "%d %d %[^\n]",
         &(buffer_data.buffer_tasks[counter].id),
          &(buffer_data.buffer_tasks[counter].done), 
          buffer_data.buffer_tasks[counter].name_task) == 3) {
            if (counter >= buffer_data.len_buffer - 1) {
                buffer_data.len_buffer *= 2;
                buffer_data.buffer_tasks = realloc(buffer_data.buffer_tasks, sizeof(task) * len_buffer);
                if (buffer_data.buffer_tasks == NULL) {
                    fprintf(stderr, "Ошибка выделения памяти\n");
                    fclose(data_file);
                    return NULL;
                    }
                }
            counter++;
            }
        fclose(data_file);
        buffer_data.task_count = counter;
        return &buffer_data;
        }
    }
}



int main(void) {
    
    return 0;
}