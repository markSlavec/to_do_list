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




void print_tasks(buffer* buffer_data) { // Исправлено имя функции
    fputs("----------------------------------\n", stdout);
    for (size_t i = 0; i < buffer_data->task_count; i++) {
        fprintf(stdout, "%d. %s: ", buffer_data->buffer_tasks[i].id, buffer_data->buffer_tasks[i].name_task);
        if (buffer_data->buffer_tasks[i].done)
            fprintf(stdout, "Выполнено\n"); // Исправлено "Выволнено" на "Выполнено"
        else
            fprintf(stdout, "В процессе\n");
    }
    fputs("----------------------------------\n", stdout);
}

buffer* read_files() {
    buffer* buffer_data = malloc(sizeof(buffer)); // Изменено на указатель и выделение памяти
    if (buffer_data == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }

    FILE* data_file;
    if ((data_file = fopen(DATA_TASKS, "r")) != NULL) {
        buffer_data->len_buffer = 50;
        buffer_data->buffer_tasks = malloc(sizeof(task) * buffer_data->len_buffer);
        if (buffer_data->buffer_tasks == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            fclose(data_file);
            free(buffer_data); // Освобождение памяти
            return NULL;
        }

        size_t counter = 0;

        while (fscanf(data_file, "%d %d %[^\n]",
            &(buffer_data->buffer_tasks[counter].id),
            &(buffer_data->buffer_tasks[counter].done),
            buffer_data->buffer_tasks[counter].name_task) == 3) {
            if (counter >= buffer_data->len_buffer - 1) {
                buffer_data->len_buffer *= 2;
                buffer_data->buffer_tasks = realloc(buffer_data->buffer_tasks, sizeof(task) * buffer_data->len_buffer);
                if (buffer_data->buffer_tasks == NULL) {
                    fprintf(stderr, "Ошибка выделения памяти\n");
                    fclose(data_file);
                    free(buffer_data); // Освобождение памяти
                    return NULL;
                }
            }
            counter++;
        }
        fclose(data_file);
        buffer_data->task_count = counter;
        return buffer_data;
    } else {
        fprintf(stderr, "Ошибка открытия файла %s для чтения\n", DATA_TASKS);
        free(buffer_data); // Освобождение памяти
        return NULL;
    }
}


void add_new_task(char* name_new_task, buffer* buffer_data) {
    if (buffer_data->len_buffer <= buffer_data->task_count - 1) {
        buffer_data->len_buffer *= 2;
        buffer_data->buffer_tasks = realloc(buffer_data->buffer_tasks, sizeof(task) * buffer_data->len_buffer);
        }
    buffer_data->buffer_tasks[buffer_data->task_count].id = buffer_data->task_count + 1;
    strcpy(buffer_data->buffer_tasks[buffer_data->task_count].name_task, name_new_task);
    buffer_data->buffer_tasks[buffer_data->task_count].done = 0;

    buffer_data->task_count++;
}


void delete_task(int id, buffer* bd) {
     bd->task_count--;
    for(int i = 0; i < bd->task_count; i++) {
        if (bd->buffer_tasks[i].id == id) { // Смещение массива
            for(size_t j = id; j < bd->task_count; j++) {
                bd->buffer_tasks[j] = bd->buffer_tasks[j + 1];
                bd->buffer_tasks[j].id = j + 1;
            }
            break;
        }
    }
}


// Записываем данные в файл после каждого изменения в функции menu.
void write_in_file(buffer* bd) {
    FILE* file;
    if ((file = fopen(DATA_TASKS, "w")) != NULL) {
        for (size_t i = 0; i < bd->task_count; i++) {
            fprintf(file, "%d %d %s\n", 
                bd->buffer_tasks[i].id, 
                bd->buffer_tasks[i].done,
                bd->buffer_tasks[i].name_task);
        }

        fclose(file);
    }
    else 
        fprintf(stderr, "Ошибка открытия файла %s", DATA_TASKS);
}


int main(void) {
    buffer* buffer_data = read_files();
    if (buffer_data != NULL) {
        add_new_task("222222", buffer_data);
        write_in_file(buffer_data);
        print_tasks(buffer_data);
        delete_task(2, buffer_data);
        print_tasks(buffer_data);
        printf("%d", buffer_data->task_count);
        free(buffer_data->buffer_tasks); // Освобождение памяти
        free(buffer_data); // Освобождение памяти
    }
    return 0;
}