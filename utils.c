#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Очистка буфера ввода
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Получение целого числа с проверкой диапазона
int get_int_input(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Ошибка! Введите число от %d до %d\n", min, max);
    }
}

// Получение строки от пользователя
void get_string_input(const char* prompt, char* buffer, int max_len) {
    if (!buffer || max_len <= 0) return;
    
    printf("%s", prompt);
    if (fgets(buffer, max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    } else {
        clear_input_buffer();
    }
}

// Вывод разделительной линии
void print_separator(void) {
    printf("---\n");
}

// Подробный вывод информации о видео
void print_video_detail(const Video* video) {
    if (!video) return;
    
    int minutes, seconds;
    convert_duration(video->duration_seconds, &minutes, &seconds);
    
    print_separator();
    printf("ID: %d\n", video->id);
    printf("Название: %s\n", video->title);
    printf("Канал: %s\n", video->channel);
    printf("Длительность: %02d:%02d\n", minutes, seconds);
    printf("Просмотры: %d\n", video->views);
    printf("Лайки: %d\n", video->likes);
    printf("Дата загрузки: %s\n", video->upload_date);
    
    if (video->views > 0) {
        printf("Процент лайков: %.1f%%\n", 
               (float)video->likes / video->views * 100);
    }
}

// Ожидание нажатия Enter
void press_enter_to_continue(void) {
    printf("\nНажмите Enter, чтобы продолжить...");
    clear_input_buffer();
    getchar();
}

// Конвертация секунд в минуты и секунды
void convert_duration(int seconds, int* minutes, int* secs) {
    if (minutes && secs) {
        *minutes = seconds / 60;
        *secs = seconds % 60;
    }
}

// Проверка формата даты (ГГГГ-ММ-ДД)
bool validate_date_format(const char* date) {
    if (!date) return false;
    
    // Проверяем длину
    if (strlen(date) != 10) return false;
    
    // Проверяем формат ГГГГ-ММ-ДД
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        } else {
            if (!isdigit(date[i])) return false;
        }
    }
    
    // Проверяем значения
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return false;
    
    // Простые проверки
    if (year < 2005 || year > 2025) return false; // YouTube запущен в 2005
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    return true;
}
