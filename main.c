#include "filesystem.h"
#include <stdio.h>
#include <string.h>

#define MAX_CMD_LEN 1024

void print_help() {
    puts("\nДоступные команды:");
    puts("  создать <имя> <текст>  - Создать новый файл");
    puts("  прочитать <имя>        - Показать содержимое файла");
    puts("  обновить <имя> <текст> - Изменить содержимое файла");
    puts("  удалить <имя>          - Удалить файл");
    puts("  помощь                 - Показать эту справку");
    puts("  выход                  - Завершить программу\n");
}

void process_command(const char* cmd, const char* arg1, const char* arg2) {
    if (strcmp(cmd, "создать") == 0 && arg1 && arg2) {
        if (fs_create(arg1, arg2)) {
            printf("Файл '%s' создан\n", arg1);
        } else {
            puts("Ошибка: не удалось создать файл");
        }
    }
    else if (strcmp(cmd, "прочитать") == 0 && arg1) {
        char* content = fs_read(arg1);
        if (content) {
            printf("Содержимое '%s':\n%s\n", arg1, content);
            free(content);
        } else {
            puts("Файл не найден");
        }
    }
    else if (strcmp(cmd, "обновить") == 0 && arg1 && arg2) {
        if (fs_update(arg1, arg2)) {
            printf("Файл '%s' обновлен\n", arg1);
        } else {
            puts("Ошибка обновления файла");
        }
    }
    else if (strcmp(cmd, "удалить") == 0 && arg1) {
        if (fs_delete(arg1)) {
            printf("Файл '%s' удален\n", arg1);
        } else {
            puts("Ошибка удаления файла");
        }
    }
    else if (strcmp(cmd, "помощь") == 0) {
        print_help();
    }
    else {
        puts("Неизвестная команда. Введите 'помощь' для справки.");
    }
}

int main() {
    if (!fs_init("data.fs")) {
        puts("Ошибка инициализации файловой системы!");
        return 1;
    }

    puts("Файловая система готова к работе. Введите 'помощь' для списка команд.");

    char input[MAX_CMD_LEN];
    while (1) {
        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // Парсинг команды
        char cmd[32] = {0};
        char arg1[MAX_FILENAME_LEN] = {0};
        char arg2[MAX_CONTENT_LEN] = {0};
        
        int count = sscanf(input, "%31s %255s %4095[^\n]", cmd, arg1, arg2);
        
        if (strcmp(cmd, "выход") == 0) break;
        
        if (count >= 1) {
            process_command(cmd, 
                          count >= 2 ? arg1 : NULL, 
                          count >= 3 ? arg2 : NULL);
        }
    }

    fs_shutdown();
    puts("Работа файловой системы завершена.");
    return 0;
}
