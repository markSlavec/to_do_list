#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"



// typedef enum {
//     INT,
//     CHAR
// } typedata;

// typedef union {
//     int id;
//     char new_task [100];
// } value;



void validation_user_input(value * user_input, typedata fuctuio_data) {
    int flag = 1;
    while(flag) {
        switch (fuctuio_data) {
            case INT:
                if (fscanf(stdin, "%d", &(user_input->id)) == 1) {
                    flag = 0;
                    fflush(stdin);
                    break;
                    }
                fflush(stdin);
                fputs("Вы ввели неверное значение: \n", stdout);
                break;
                
                
            case CHAR:
                if (fscanf(stdin, "%[^\n]", user_input->new_task) == 1) {
                    flag = 0;
                    fflush(stdin);
                    break;
                    }   
                fflush(stdin);
                fputs("Вы ввели неверное значение: \n", stdout);
                break;
        }
    }
}


int main(void) {
    value user_input;
    validation_user_input(&user_input, INT);
    printf("%d", user_input.id);
    validation_user_input(&user_input, CHAR);
    printf("%s", user_input.new_task);
    return 0;
}