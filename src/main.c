#include "main.h"

// Переписать позже меню используя массив указателей на функций и enum, как в учебнике.



void menu()
{
    while (1) {
        fputs("Выберите один из вариантов", stdout);
        fputs("1. Создать новую задачу\n2. Показать текущие.\n3. Удалить задачу по ID.\n4. Завершить задачу.");
        int user_input;
        fscnaf(stdin, "%d", &user_input);
        switch (user_input) {

            case 1:
                char [100] user_input_new_task;
                fputs("Введите новую задачу: ", stdout);
                fscanf(stdin, "%s", user_input_new_task);
                add_new_task(user_input_new_task);
                break;

            case 2:
                print_tasks();
                break;
            
            case 3:
                break;

            case 4:
                break;
        }
    }
}


void print_tasks() {
    FILE *data_tasks;
    if ((data_tasks = fopen(DATA_TASKS, "r")) != NULL) {
        char c;
        while((c = fgetc(data_tasks)) != EOF) {
            fputc(c);
        }

        fclose(data_tasks);
    }
}



// Переписать под новый интерфейс. Удаление через id задачи. Запись файла через массив структур; Или можно даже попробовать связный список
void delete_task (char * name_task) {
    FILE *data_tasks;

    int size_buffer_lines = 10;
    int sizebuffer_line = 50;
    char** buffer = malloc(sizeof(char *) * size_buffer_lines);     // Выделяем память под строки из файла
    int counter = 0;    // Для заполнения буффера
    for (int i = 0; i < size_buffer_lines; i++) {
        buffer[i] = malloc(sizeof(char) * sizebuffer_line);
        }
    
    if ((data_tasks = fopen(DATA_TASKS, "r")) != NULL) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

       while((read = getline(&line, &len, data_tasks)) != -1) {     // Копируем все данные файла в буффер
            if (read >= sizebuffer_line) 
                buffer[counter] = realloc(buffer[counter], sizeof(char) * (read + 1));
            strcpy(buffer[counter], line);
            if (counter >= size_buffer_lines - 1)  {
                size_buffer_lines += 10;
                buffer = realloc(buffer, sizeof(char*) * size_buffer_lines);
                for (int i = counter + 1; i < size_buffer_lines; i++) {
                    buffer[i] = malloc(sizeof(char) * size_buffer_lines);
                    }    
                }
            counter++;
        }
        free(line);
        fclose(data_tasks);


        // !!! ПОправить эту херню . getline копирует с \n а strcmp проверяет точную копию!!!!
       for (int i = 0; i < counter; i++) {      // Находим нужную строчку и чистим ее
        if (strcmp(buffer[i], name_task) == 0) 
            memset(buffer[i], '\0', strlen(buffer[i]));
       }


        // Перезаписываем в файл данные без удаленной строки
        if ((data_tasks = fopen(DATA_TASKS, "w")) != NULL) {      
            for (int i = 0; i < counter; i++) {
                if (strlen(buffer[i]) > 0) {
                    fprintf(data_tasks, "%s", buffer[i]);
                    }
            }
            fclose(data_tasks);
        }

        for (int i = 0; i < size_buffer_lines; i++)
        {
            free(buffer[i]);
        }

        free(buffer);
       
    }   
    else 
        fprintf(stderr, "%s", "Ошибка с открытием файла 'data_tasks'");
}


void add_new_task(char * name_task) {
    FILE *data_tasks;
    if ((data_tasks = fopen(DATA_TASKS, "a")) != NULL) {
        fprintf(data_tasks, "%s", name_task);
        fclose(data_tasks);
    }   
    else 
        fprintf(stderr, "%s", "Ошибка с открытием файла 'data_tasks'");
}



int main(void) {
    // char input_user [50];
    // fgets(input_user, 50, stdin);
    // add_new_task(input_user);
    delete_task("123");

    return 0;
}