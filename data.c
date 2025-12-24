#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ... остальные функции остаются как были ...

bool save_to_file(const char* filename, VideoDatabase* db) {
    if (!db || !filename) return false;
    
    FILE* file = fopen(filename, "w");
    if (!file) return false;
    
    // ТЕКСТОВЫЙ ФОРМАТ: записываем каждое поле с разделителем |
    Video* current = db->head;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%d|%d|%d|%s\n",
                current->id,
                current->title,
                current->channel,
                current->duration_seconds,
                current->views,
                current->likes,
                current->upload_date);
        current = current->next;
    }
    
    fclose(file);
    return true;
}

bool load_from_file(const char* filename, VideoDatabase* db) {
    if (!db || !filename) return false;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        // Файл не существует - это нормально при первом запуске
        return true; // Возвращаем true, а не false!
    }
    
    char buffer[256];
    
    // Читаем построчно - ТЕКСТОВЫЙ ФОРМАТ
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Убираем символ новой строки
        buffer[strcspn(buffer, "\n")] = '\0';
        
        char title[100], channel[50], date[11];
        int id, duration, views, likes;
        
        // Парсим строку с разделителем |
        if (sscanf(buffer, "%d|%99[^|]|%49[^|]|%d|%d|%d|%10s",
                   &id, title, channel, &duration, &views, &likes, date) == 7) {
            
            // Добавляем видео в список
            add_video(db, title, channel, duration, views, likes, date);
            
            // Восстанавливаем правильный next_video_id
            if (id >= db->next_video_id) {
                db->next_video_id = id + 1;
            }
        }
    }
    
    fclose(file);
    return true;
}
