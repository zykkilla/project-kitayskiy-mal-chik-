#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTENT_SIZE 4096

// 1.1 Функция открытия/создания файла системы
FILE* open_filesystem(const char* filename) {
    FILE* fs = fopen(filename, "a+"); // Открываем для чтения и добавления
    if (!fs) {
        perror("Failed to open filesystem");
        return NULL;
    }
    return fs;
}

// 1.2 Функция просмотра файла
char* view_file(FILE* fs, const char* filename) {
    fseek(fs, 0, SEEK_SET); // Переходим в начало файла
    
    char line[256];
    int found = 0;
    char* content = malloc(MAX_CONTENT_SIZE);
    content[0] = '\0'; // Инициализируем пустую строку
    
    while (fgets(line, sizeof(line), fs) {
        line[strcspn(line, "\n")] = 0; // Удаляем символ новой строки
        
        if (found) {
            if (line[0] == '/') { // Конец содержимого файла
                break;
            }
            strcat(content, line); // Добавляем строку к содержимому
            strcat(content, "\n"); // Добавляем перенос строки
        }
        
        if (strcmp(line, filename) == 0) { // Нашли имя файла
            found = 1;
        }
    }
    
    if (!found) {
        free(content);
        return NULL; // Файл не найден
    }
    
    return content;
}

// 1.3 Функция удаления файла
int delete_file(FILE* fs, const char* filename) {
    fseek(fs, 0, SEEK_SET);
    
    // Считываем весь файл в память
    fseek(fs, 0, SEEK_END);
    long length = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    
    char* temp = malloc(length + 1);
    fread(temp, 1, length, fs);
    temp[length] = '\0';
    
    // Обрабатываем содержимое
    char* new_content = malloc(length + 1);
    new_content[0] = '\0';
    
    char* line = strtok(temp, "\n");
    int in_target_file = 0;
    int found = 0;
    
    while (line != NULL) {
        if (strcmp(line, filename) == 0) {
            found = 1;
            in_target_file = 1;
            line = strtok(NULL, "\n");
            continue;
        }
        
        if (in_target_file && line[0] == '/') {
            in_target_file = 0;
        }
        
        if (!in_target_file) {
            strcat(new_content, line);
            strcat(new_content, "\n");
        }
        
        line = strtok(NULL, "\n");
    }
    
    free(temp);
    
    if (!found) {
        free(new_content);
        return 0; // Файл не найден
    }
    
    // Перезаписываем файл
    fseek(fs, 0, SEEK_SET);
    ftruncate(fileno(fs), 0); // Очищаем файл
    fputs(new_content, fs);
    fflush(fs);
    free(new_content);
    
    return 1; // Успешное удаление
}

// 2.1 Функция создания нового файла
int create_file(FILE* fs, const char* filename, const char* content) {
    fseek(fs, 0, SEEK_END); // Переходим в конец файла
    fprintf(fs, "%s\n", filename); // Записываем имя файла
    fprintf(fs, "%s\n", content); // Записываем содержимое
    fprintf(fs, "/\n"); // Маркер конца файла
    fflush(fs); // Сбрасываем буфер
    return 1;
}

// 2.2 Функция изменения файла
int modify_file(FILE* fs, const char* filename, const char* new_content) {
    // Сначала удаляем старый файл
    if (!delete_file(fs, filename)) {
        return 0; // Файл не найден
    }
    // Затем создаем новый с тем же именем
    return create_file(fs, filename, new_content);
}
