#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// ... остальные функции ...

bool validate_date_format(const char* date) {
    // Проверяем формат ГГГГ-ММ-ДД
    if (strlen(date) != 10) return false;
    
    // Проверяем цифры и разделители
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
