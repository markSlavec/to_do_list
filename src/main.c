#include "main.h"

// Переписать позже меню используя массив указателей на функций и enum, как в учебнике.
// Нужно узнать про типизированный файл data.
// ИЛИ просто сделать сканф в динамический массив структур для delete_task



// Добавить в menu буффер и закидывать его аргументом в функции, чтобы постоянно не открыть и закрывать файл и не чистить память. А делать это только один раз.
// И лучше всего буффер этот во что-то закинуть вместе с длиной массива. Общий размер массива и так далее. Чтобы никуда ничего не улетело;
void menu() {
    int flag = 1;
    buffer* buffer_data = read_file();   // Здесь все данные файла с ними и игремся.

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


    // Чистим буффер
    free(buffer_data->buffer_tasks);
    free(buffer_data);
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


// Записываем данные в файл после каждого изменения в функции menu.
void write_in_file() {

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



int main(void) {
    menu();
    return 0;
}
