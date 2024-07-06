#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name_task;
} task;

void delete_task(char *name_task) {
    FILE *data_tasks;
    int size_buffer_lines = 10;
    int sizebuffer_line = 50;
    char **buffer = malloc(sizeof(char *) * size_buffer_lines); // Выделяем память под строки из файла
    int counter = 0; // Для заполнения буфера
    for (int i = 0; i < size_buffer_lines; i++) {
        buffer[i] = malloc(sizeof(char) * sizebuffer_line);
    }

    if ((data_tasks = fopen("data_tasks.txt", "r")) != NULL) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, data_tasks)) != -1) { // Копируем все данные файла в буфер
            if (read >= sizebuffer_line) {
                buffer[counter] = realloc(buffer[counter], sizeof(char) * (read + 1));
            }
            strcpy(buffer[counter], line);
            if (counter >= size_buffer_lines - 1) {
                size_buffer_lines += 10;
                buffer = realloc(buffer, sizeof(char *) * size_buffer_lines);
                // Выделяем дополнительную память для новых строк
                for (int i = counter + 1; i < size_buffer_lines; i++) {
                    buffer[i] = malloc(sizeof(char) * sizebuffer_line);
                }
            }
            counter++;
        }

        free(line);
        fclose(data_tasks);

        // Находим нужную строчку и чистим ее
        for (int i = 0; i < counter; i++) {
            if (strstr(buffer[i], name_task)) {
                memset(buffer[i], '\0', strlen(buffer[i]));
            }
        }

        // Перезаписываем файл данными без удаленной строки
        if ((data_tasks = fopen("data_tasks.txt", "w")) != NULL) {
            for (int i = 0; i < counter; i++) {
                if (strlen(buffer[i]) > 0) {
                    fprintf(data_tasks, "%s", buffer[i]);
                }
            }
            fclose(data_tasks);
        }

        // Освобождаем память
        for (int i = 0; i < size_buffer_lines; i++) {
            free(buffer[i]);
        }
        free(buffer);
    } else {
        fprintf(stderr, "Ошибка с открытием файла 'data_tasks'\n");
    }
}

void add_new_task(char *name_task) {
    FILE *data_tasks;
    if ((data_tasks = fopen("data_tasks.txt", "a")) != NULL) {
        fprintf(data_tasks, "%s\n", name_task); // Добавляем новую задачу с новой строки
        fclose(data_tasks);
    } else {
        fprintf(stderr, "Ошибка с открытием файла 'data_tasks'\n");
    }
}

int main(void) {
    // char input_user[50];
    // fgets(input_user, 50, stdin);
    // add_new_task(input_user);
    delete_task("123");

    return 0;
}
