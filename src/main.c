#include "main.h"

// Переписать позже меню используя массив указателей на функций и enum, как в учебнике.
// Нужно узнать про типизированный файл data.
// ИЛИ просто сделать сканф в динамический массив структур для delete_task


void menu() {
    int flag = 1;
    buffer* buffer_data = read_file();   // Здесь все данные файла, с ними и работаем.

    while (flag) {
        fputs("Выберите один из вариантов\n", stdout);
        fputs("1. Создать новую задачу\n2. Показать текущие.\n3. Удалить задачу по ID.\n4. Выход\n5. Завершить задачу\n6. Удалить все задачи\n7. Удалить все выполненные задачи: ", stdout);
        int user_input;
        fscanf(stdin, "%d", &user_input);
        fflush(stdin);
        int user_input_id;
        switch (user_input) {
            case 1: {
                char user_input_new_task[100];
                fputs("Введите новую задачу: ", stdout);
                // fgets(user_input_new_task, 100, stdin); // Исправлено
                fscanf(stdin, "%99[^\n]", user_input_new_task);
                fflush(stdin);
                add_new_task(user_input_new_task, buffer_data);
                write_in_file(buffer_data); // Запись в файл после добавления задачи
                break;
            }
            case 2:
                print_tasks(buffer_data);
                break;
            case 3: {
                fputs("Введите ID задачи для удаления: ", stdout); // Добавлено
                fscanf(stdin, "%d", &user_input_id);
                fflush(stdin);
                delete_task(user_input_id, buffer_data);
                write_in_file(buffer_data); // Запись в файл после удаления задачи
                break;
            }
            case 4:
                flag = 0;
                break;
            case 5: {
                fputs("Введите ID задачи для завершения: ", stdout); // Добавлено
                fscanf(stdin, "%d", &user_input_id);
                fflush(stdin);
                close_task(user_input_id, buffer_data);
                write_in_file(buffer_data); // Запись в файл после завершения задачи
                break;
            }
            case 6: {
                all_clean(buffer_data);
                write_in_file(buffer_data); // Запись в файл после завершения задачи
                break;
            }
            case 7 : {
                clean_done_tasks(buffer_data);
                write_in_file(buffer_data); // Запись в файл после завершения задачи
                break;
            }
            default:
                fputs("Неверный ввод, попробуйте снова.\n", stdout); // Добавлено
                break;
        }
    }

    // Чистим буфер
    free(buffer_data->buffer_tasks);
    free(buffer_data);
}


void clean_done_tasks(buffer* bd) {
    int new_task_count = 0;

    for (size_t i = 0; i < bd->task_count; i++) {
        if ( !bd->buffer_tasks[i].done )
            bd->buffer_tasks[new_task_count++] = bd->buffer_tasks[i];
    }
    bd->task_count = new_task_count;
}


void all_clean(buffer * bd) {
    free(bd->buffer_tasks);
    bd->task_count = 0;
    bd->len_buffer = 50;
    bd->buffer_tasks = malloc(sizeof(task) * bd->len_buffer);
}


void close_task(int id, buffer* bd) {
    for (size_t i = 0; i < bd->task_count; i++) {
        if (bd->buffer_tasks[i].id == id) {
            bd->buffer_tasks[i].done = 1;
            break;
        }
    }
}


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


buffer* read_file() {
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

    for (size_t i = id - 1; i < bd->task_count; i++)
    {
        bd->buffer_tasks[i] = bd->buffer_tasks[i + 1];
        bd->buffer_tasks[i].id = i + 1;
    }
    
}

// Нужно открыть только один раз и перередавать стрим. Но это будет менее стабильно. 
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
    } else {
        fprintf(stderr, "Ошибка открытия файла %s", DATA_TASKS);
    }
}

int main(void) {
    menu();
    return 0;
}
