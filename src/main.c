#include "main.h"

// Переписать позже меню используя массив указателей на функций и enum, как в учебнике.
// Нужно узнать про типизированный файл data.
// ИЛИ просто сделать сканф в динамический массив структур для delete_task



// Добавить в menu буффер и закидывать его аргументом в функции, чтобы постоянно не открыть и закрывать файл и не чистить память. А делать это только один раз.
// И лучше всего буффер этот во что-то закинуть вместе с длиной массива. Общий размер массива и так далее. Чтобы никуда ничего не улетело;
void menu() {
    int flag = 1;
    buffer* buffer_data;

    while (flag) {
        fputs("Выберите один из вариантов\n", stdout);
        fputs("1. Создать новую задачу\n2. Показать текущие.\n3. Удалить задачу по ID.\n4. Выход \n5.Завершить задачу : ", stdout);
        int user_input;
        fscanf(stdin, "%d", &user_input);
        fflush(stdin);
        int user_input_id;
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
                
                fputs("Введите ID задачи для удаления: ", stdout); // Добавлено
                fscanf(stdin, "%d", &user_input_id);
                fflush(stdin);
                delete_task(user_input_id);
                break;
            }
            case 4:
                flag = 0;
                break;
            
            case 5:
                fputs("Введите ID задачи для завершения: ", stdout); // Добавлено
                fscanf(stdin, "%d", &user_input_id);
                fflush(stdin);
                close_task(user_input_id);

            default:
                fputs("Неверный ввод, попробуйте снова.\n", stdout); // Добавлено
                break;
        }
    }
}

// Нужен буффер в мейне с записью в файл после каждого действия. Иначе придется постоянно открывать файл и записать в него что-то а потом закрывать.
// Жопа крч! Нужно переделывать.
void close_task(int id) {
    size_t task_count;
    task *buffer_tasks = read_file(&task_count);
    for (size_t i = 0; i < task_count; i++) {
        if (buffer_tasks[i].id == id) {
            buffer_tasks[i].done = 1;
            break;
        }
    }
    free(buffer_tasks);
}



void add_new_task(char* name_new_task) {
    size_t task_count;
    task *buffer_tasks = read_file(&task_count);
    if (buffer_tasks != NULL) {
        FILE *new_data;
        if ((new_data = fopen(DATA_TASKS, "a")) != NULL) {
            buffer_tasks[task_count].done = 0;
            fprintf(new_data, "%d %d %s", (int)task_count + 1, buffer_tasks[task_count].done, name_new_task); 
            fclose(new_data);
        } else {
            fprintf(stderr, "Ошибка открытия файла %s для записи\n", DATA_TASKS); 
        }
        free(buffer_tasks); // Добавлено
    } else {
        fprintf(stderr, "Ошибка чтения файла %s\n", DATA_TASKS); 
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
            fprintf(stdout, "%d. %s: ", buffer_tasks[i].id, buffer_tasks[i].name_task);
            if (buffer_tasks[i].done)
                fprintf(stdout, "Выволнено\n");
            else
                fprintf(stdout, "В процессе\n");       
        }
        fputs("----------------------------------\n", stdout);
        free(buffer_tasks);
    } else {
        fprintf(stderr, "Ошибка чтения файла %s\n", DATA_TASKS);
    }
}

// Записываем данные в файл после каждого изменения в функции menu.
void write_in_file() {

}



// buffer * read_files() {
//     buffer * buffer_data;
//     FILE* data_file;
//     if ((data_file = fopen(DATA_TASKS, "r")) != NULL) {
//         size_t len_buffer = 50;
//         task *buffer_tasks = malloc(sizeof(task) * len_buffer);
//         if (buffer_tasks == NULL) {
//             fprintf(stderr, "Ошибка выделения памяти\n");
//             fclose(data_file);
//             return NULL;
//         }

//     }
// }


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

        size_t counter = 0;

        while (fscanf(data_file, "%d %d %[^\n]",
         &(buffer_tasks[counter].id),
          &(buffer_tasks[counter].done), 
          buffer_tasks[counter].name_task) == 3) {
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
        fprintf(stderr, "Ошибка открытия файла %s для чтения\n", DATA_TASKS);
        return NULL;
    }
}

int main(void) {
    menu();
    return 0;
}
