#include "main.h"

// Переписать позже меню используя массив указателей на функций и enum, как в
// учебнике.

void validation_user_input(value* user_input, typedata fuctuio_data) {
  int flag = 1;
  while (flag) {
    switch (fuctuio_data) {
      case INT:
        if (fscanf(stdin, "%d", &(user_input->id)) == 1) {
          flag = 0;
          fflush(stdin);  // fflush(stdin) не работает для ввода
          break;
        }
        fflush(stdin);  // fflush(stdin) не работает для ввода
        fputs("Вы ввели неверное значение: \n", stdout);
        break;

      case CHAR:
        if (fscanf(stdin, " %[^\n]", user_input->new_task) ==
            1) {  // Добавлен пробел перед %[^\n] для пропуска пробелов
          flag = 0;
          fflush(stdin);  // fflush(stdin) не работает для ввода
          break;
        }
        fflush(stdin);  // fflush(stdin) не работает для ввода
        fputs("Вы ввели неверное значение: \n", stdout);
        break;
    }
  }
}

void welcome_message() {
  fputs("--------------------------------------\n", stdout);
  fputs("| Добро пожаловать в TO DO LIST на С |\n", stdout);
  fputs("--------------------------------------\n", stdout);
}

void print_menu() {
  fputs("Выберите один из вариантов\n", stdout);
  fputs(
      "1. Создать новую задачу\n2. Показать текущие.\n3. Удалить задачу по "
      "ID.\n4. Выход\n5. Завершить задачу\n6. Удалить все задачи\n7. Удалить "
      "все выполненные задачи: ",
      stdout);
}

void menu() {
  int flag = 1;
  buffer* buffer_data =
      read_file();  // Здесь все данные файла, с ними и работаем.
  value user_input_option;
  value user_input;

  welcome_message();

  while (flag) {
    print_tasks(buffer_data);
    print_menu();
    validation_user_input(&user_input_option, INT);

    switch (user_input_option.id) {
      case 1: {
        fputs("Введите новую задачу: ", stdout);
        validation_user_input(
            &user_input, CHAR);  // Исправлено: user_input_option на user_input
        add_new_task(user_input.new_task, buffer_data);
        write_in_file(buffer_data);  // Запись в файл после добавления задачи
        break;
      }
      case 2:
        print_tasks(buffer_data);
        break;
      case 3: {
        fputs("Введите ID задачи для удаления: ", stdout);
        validation_user_input(&user_input,
                              INT);  // Исправлено: добавлена валидация ввода
        delete_task(user_input.id, buffer_data);
        write_in_file(buffer_data);  // Запись в файл после удаления задачи
        break;
      }
      case 4:
        flag = 0;
        break;
      case 5: {
        fputs("Введите ID задачи для завершения: ", stdout);
        validation_user_input(&user_input,
                              INT);  // Исправлено: добавлена валидация ввода
        close_task(user_input.id, buffer_data);
        write_in_file(buffer_data);  // Запись в файл после завершения задачи
        break;
      }
      case 6: {
        all_clean(buffer_data);
        write_in_file(buffer_data);  // Запись в файл после завершения задачи
        break;
      }
      case 7: {
        clean_done_tasks(buffer_data);
        write_in_file(buffer_data);  // Запись в файл после завершения задачи
        break;
      }
      default:
        fputs("Неверный ввод, попробуйте снова.\n", stdout);
        break;
    }

    system("clear");  // Чистим терминал
  }

  // Чистим буфер
  free(buffer_data->buffer_tasks);
  free(buffer_data);
}

void clean_done_tasks(buffer* bd) {
  int new_task_count = 0;

  for (size_t i = 0; i < bd->task_count; i++) {
    if (!bd->buffer_tasks[i].done)
      bd->buffer_tasks[new_task_count++] = bd->buffer_tasks[i];
  }
  bd->task_count = new_task_count;
}

void all_clean(buffer* bd) {
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

void print_tasks(buffer* buffer_data) {
  fputs("\n----------------------------------", stdout);
  fputs("\nСПИСОК ЗАДАЧ\n", stdout);
  fputs("----------------------------------\n", stdout);
  for (size_t i = 0; i < buffer_data->task_count; i++) {
    fprintf(stdout, "%d. %s: ", buffer_data->buffer_tasks[i].id,
            buffer_data->buffer_tasks[i].name_task);
    if (buffer_data->buffer_tasks[i].done)
      fprintf(stdout, "Выполнено\n");
    else
      fprintf(stdout, "В процессе\n");
  }
  fputs("----------------------------------\n\n", stdout);
}

buffer* read_file() {
  buffer* buffer_data = malloc(sizeof(buffer));
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
      free(buffer_data);
      return NULL;
    }

    size_t counter = 0;

    while (fscanf(data_file, "%d %d %[^\n]",
                  &(buffer_data->buffer_tasks[counter].id),
                  &(buffer_data->buffer_tasks[counter].done),
                  buffer_data->buffer_tasks[counter].name_task) == 3) {
      if (counter >= buffer_data->len_buffer - 1) {
        buffer_data->len_buffer *= 2;
        buffer_data->buffer_tasks = realloc(
            buffer_data->buffer_tasks, sizeof(task) * buffer_data->len_buffer);
        if (buffer_data->buffer_tasks == NULL) {
          fprintf(stderr, "Ошибка выделения памяти\n");
          fclose(data_file);
          free(buffer_data);
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
    free(buffer_data);
    return NULL;
  }
}

void add_new_task(char* name_new_task, buffer* buffer_data) {
  if (buffer_data->len_buffer <=
      buffer_data->task_count) {  // Исправлено условие
    buffer_data->len_buffer *= 2;
    buffer_data->buffer_tasks = realloc(buffer_data->buffer_tasks,
                                        sizeof(task) * buffer_data->len_buffer);
  }
  buffer_data->buffer_tasks[buffer_data->task_count].id =
      buffer_data->task_count + 1;
  strcpy(buffer_data->buffer_tasks[buffer_data->task_count].name_task,
         name_new_task);
  buffer_data->buffer_tasks[buffer_data->task_count].done = 0;

  buffer_data->task_count++;
}

void delete_task(int id, buffer* bd) {
  if (id > 0 && (size_t)id <= bd->task_count) {
    for (size_t i = id - 1; i < bd->task_count - 1;
         i++) {  // Исправлено условие цикла
      bd->buffer_tasks[i] = bd->buffer_tasks[i + 1];
      bd->buffer_tasks[i].id = i + 1;
    }
    bd->task_count--;
  }
}

void write_in_file(buffer* bd) {
  FILE* file;
  if ((file = fopen(DATA_TASKS, "w")) != NULL) {
    for (size_t i = 0; i < bd->task_count; i++) {
      fprintf(file, "%d %d %s\n", bd->buffer_tasks[i].id,
              bd->buffer_tasks[i].done, bd->buffer_tasks[i].name_task);
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
