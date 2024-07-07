#include "main.h"

// Переписать позже меню используя массив указателей на функций и enum, как в учебнике.
// Нужно узнать про типизированный файл data.
// ИЛИ просто сделать сканф в динамический массив структур для delete_task

void menu() {
    int flag = 1;
    while (flag) {
        fputs("Выберите один из вариантов\n", stdout);
        fputs("1. Создать новую задачу\n2. Показать текущие.\n3. Удалить задачу по ID.\n4. Завершить задачу.\n5. Изменить задачу: ", stdout);
        int user_input;
        fscanf(stdin, "%d", &user_input);
        fflush(stdin);
        switch (user_input) {
            case 1: {
                char user_input_new_task[100];
                fputs("Введите новую задачу: ", stdout);
                fgets(user_input_new_task, 100, stdin); // Исправлено
                fflush(stdin);
                add_new_task(user_input_new_task);
                break;
            }
            case 2:
                print_tasks();
                break;
            case 3: {
                int user_input_id;
                fputs("Введите ID задачи для удаления: ", stdout); // Добавлено
                fscanf(stdin, "%d", &user_input_id);
                fflush(stdin);
                delete_task(user_input_id);
                break;
            }
            case 4:
                flag = 0;
                break;
            default:
                fputs("Неверный ввод, попробуйте снова.\n", stdout); // Добавлено
                break;
        }
    }
}

void add_new_task(char* name_new_task) {
    size_t task_count;
    task *buffer_tasks = read_file(&task_count);
    if (buffer_tasks != NULL) {
        FILE *new_data;
        if ((new_data = fopen(DATA_TASKS, "a")) != NULL) {
            fprintf(new_data, "%d %s", (int)task_count + 1, name_new_task); // Исправлено
            fclose(new_data);
        } else {
            fprintf(stderr, "Ошибка открытия файла %s для записи\n", DATA_TASKS); // Исправлено
        }
        free(buffer_tasks); // Добавлено
    } else {
        fprintf(stderr, "Ошибка чтения файла %s\n", DATA_TASKS); // Исправлено
    }
}

void delete_task(int id) {
    print_tasks();
    size_t task_count;
    task *buffer_tasks = read_file(&task_count);
    if (buffer_tasks != NULL) {
        for (size_t i = 0; i < task_count; i++) {
            if (id == buffer_tasks[i].id) {
                buffer_tasks[i].id = -1; // Используем -1 для обозначения удаленного ID
                break;
            }
        }

        // Перезапись файла без удаленного ID
        FILE* new_data;
        if ((new_data = fopen(DATA_TASKS, "w")) != NULL) {
            int counter = 1;
            for (size_t i = 0; i < task_count; i++) {
                if (buffer_tasks[i].id != -1) {
                    fprintf(new_data, "%d %s\n", counter, buffer_tasks[i].name_task); // Исправлено
                    counter++;
                }
            }
            fclose(new_data);
        } else {
            fprintf(stderr, "Ошибка открытия файла %s для записи\n", DATA_TASKS); // Исправлено
        }
        free(buffer_tasks);
    } else {
        fprintf(stderr, "Ошибка чтения файла %s\n", DATA_TASKS); // Исправлено
    }
}

void print_tasks() { // Исправлено имя функции
    size_t task_count;
    task *buffer_tasks = read_file(&task_count);
    if (buffer_tasks != NULL) {
        fputs("----------------------------------\n", stdout);
        for (size_t i = 0; i < task_count; i++) {
            fprintf(stdout, "%d. %s\n", buffer_tasks[i].id, buffer_tasks[i].name_task);
        }
        fputs("----------------------------------\n", stdout);
        free(buffer_tasks);
    } else {
        fprintf(stderr, "Ошибка чтения файла %s\n", DATA_TASKS); // Исправлено
    }
}

task* read_file(size_t* task_count) {
    FILE* data_file;
    if ((data_file = fopen(DATA_TASKS, "r")) != NULL) {
        size_t len_buffer = 50;
        task *buffer_tasks = malloc(sizeof(task) * len_buffer);
        if (buffer_tasks == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            fclose(data_file);
            return NULL;
        }
        int counter = 0;

        while (fscanf(data_file, "%d %99s", &buffer_tasks[counter].id, buffer_tasks[counter].name_task) == 2) {
            if (counter >= len_buffer - 1) {
                len_buffer *= 2;
                buffer_tasks = realloc(buffer_tasks, sizeof(task) * len_buffer);
                if (buffer_tasks == NULL) {
                    fprintf(stderr, "Ошибка выделения памяти\n");
                    fclose(data_file);
                    return NULL;
                }
            }
            counter++;
        }
        fclose(data_file);
        *task_count = counter;
        return buffer_tasks;
    } else {
        fprintf(stderr, "Ошибка открытия файла %s для чтения\n", DATA_TASKS); // Исправлено
        return NULL;
    }
}

int main(void) {
    menu();
    return 0;
}
