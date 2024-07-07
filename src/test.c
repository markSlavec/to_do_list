#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name_task [100];
} task;

#define DATA_TASKS "data_tasks.txt"


task* read_file(size_t* task_count) {
    FILE* data_file;
    if ((data_file = fopen(DATA_TASKS, "r")) != NULL) {
        size_t len_buffer = 50;
        task *buffer_tasks = malloc(sizeof(task) * len_buffer); // Исправлено
        int counter = 0;

        while (fscanf(data_file, "%d %99s", &buffer_tasks[counter].id, buffer_tasks[counter].name_task) == 2) { // Исправлено
            if (counter >= len_buffer - 1) {
                len_buffer *= 2; // Увеличиваем буфер в 2 раза
                buffer_tasks = realloc(buffer_tasks, sizeof(task) * len_buffer); // Исправлено
                if (buffer_tasks == NULL) {
                    fprintf(stderr, "Ошибка выделения памяти\n");
                    fclose(data_file);
                    return NULL;
                }
            }
            counter++;
        }
        fclose(data_file);
        *task_count = counter; // Возвращаем количество задач
        return buffer_tasks;
    } else {
        fprintf(stderr, "Ошибка чтения файла %s\n", DATA_TASKS);
        return NULL;
    }
}



int main(void) {
    size_t task_count;
    task* tasks = read_file(&task_count);
    if (tasks != NULL) {
        for (size_t i = 0; i < task_count; i++) {
            printf("Task ID: %d, Task Name: %s\n", tasks[i].id, tasks[i].name_task);
        }
        free(tasks);
    } else {
        return 1;
    }
    return 0;
}