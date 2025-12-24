#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Создание новой базы данных
VideoDatabase* create_database(void) {
    VideoDatabase* db = (VideoDatabase*)malloc(sizeof(VideoDatabase));
    if (!db) return NULL;
    
    db->head = NULL;
    db->tail = NULL;
    db->video_count = 0;
    db->next_video_id = 1;
    
    return db;
}

// Освобождение памяти базы данных
void free_database(VideoDatabase* db) {
    if (!db) return;
    
    Video* current = db->head;
    Video* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    free(db);
}

// Добавление нового видео в базу данных
bool add_video(VideoDatabase* db, const char* title, const char* channel,
               int duration, int views, int likes, const char* date) {
    
    if (!db  !channel || !date) return false;
    
    Video* new_video = (Video*)malloc(sizeof(Video));
    if (!new_video) return false;
    
    new_video->id = db->next_video_id++;
    strncpy(new_video->title, title, sizeof(new_video->title) - 1);
    new_video->title[sizeof(new_video->title) - 1] = '\0';
    
    strncpy(new_video->channel, channel, sizeof(new_video->channel) - 1);
    new_video->channel[sizeof(new_video->channel) - 1] = '\0';
    
    new_video->duration_seconds = duration;
    new_video->views = views;
    new_video->likes = likes;
    
    strncpy(new_video->upload_date, date, sizeof(new_video->upload_date) - 1);
    new_video->upload_date[sizeof(new_video->upload_date) - 1] = '\0';
    
    new_video->next = NULL;
    
    if (db->head == NULL) {
        db->head = new_video;
        db->tail = new_video;
    } else {
        db->tail->next = new_video;
        db->tail = new_video;
    }
    
    db->video_count++;
    return true;
}

// Удаление видео из базы данных по ID
bool remove_video(VideoDatabase* db, int video_id) {
    if (!db || !db->head) return false;
    
    Video* current = db->head;
    Video* prev = NULL;
    
    while (current != NULL && current->id != video_id) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) return false;
    
    if (prev == NULL) {
        db->head = current->next;
    } else {
        prev->next = current->next;
    }
    
    if (current == db->tail) {
        db->tail = prev;
    }
    
    free(current);
    db->video_count--;
    
    return true;
}

// Поиск видео по ID
Video* find_video_by_id(VideoDatabase* db, int video_id) {
    if (!db) return NULL;
    
    Video* current = db->head;
    while (current != NULL) {
        if (current->id == video_id) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Поиск видео по названию канала
Video** search_by_channel(VideoDatabase* db, const char* channel, int* result_count) {
    if (!db || !channel) {
        *result_count = 0;
        return NULL;
    }
    
    // Подсчет количества результатов
    int count = 0;
    Video* current = db->head;
    while (current != NULL) {
        if (strstr(current->channel, channel) != NULL) {
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) {
        *result_count = 0;
        return NULL;
    }
    
    // Сбор результатов
    Video results = (Video)malloc(count * sizeof(Video*));
    if (!results) {
        *result_count = 0;
        return NULL;
    }
    
    current = db->head;
    int index = 0;
    while (current != NULL) {
        if (strstr(current->channel, channel) != NULL) {
            results[index++] = current;
        }
        current = current->next;
    }
    
    *result_count = count;
    return results;
}

// Сохранение базы данных в текстовый файл
bool save_to_file(const char* filename, VideoDatabase* db) {
if (!db || !filename) return false;
    
    FILE* file = fopen(filename, "w");
    if (!file) return false;
    
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

// Загрузка базы данных из текстового файла
bool load_from_file(const char* filename, VideoDatabase* db) {
    if (!db || !filename) return false;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        return true; // Файл не существует - это нормально
    }
    
    char buffer[256];
    int max_id = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        char title[100], channel[50], date[11];
        int id, duration, views, likes;
        
        if (sscanf(buffer, "%d|%99[^|]|%49[^|]|%d|%d|%d|%10s",
                   &id, title, channel, &duration, &views, &likes, date) == 7) {
            
            Video* new_video = (Video*)malloc(sizeof(Video));
            if (!new_video) {
                fclose(file);
                return false;
            }
            
            new_video->id = id;
            strncpy(new_video->title, title, sizeof(new_video->title) - 1);
            new_video->title[sizeof(new_video->title) - 1] = '\0';
            
            strncpy(new_video->channel, channel, sizeof(new_video->channel) - 1);
            new_video->channel[sizeof(new_video->channel) - 1] = '\0';
            
            new_video->duration_seconds = duration;
            new_video->views = views;
            new_video->likes = likes;
            
            strncpy(new_video->upload_date, date, sizeof(new_video->upload_date) - 1);
            new_video->upload_date[sizeof(new_video->upload_date) - 1] = '\0';
            
            new_video->next = NULL;
            
            if (db->head == NULL) {
                db->head = new_video;
                db->tail = new_video;
            } else {
                db->tail->next = new_video;
                db->tail = new_video;
            }
            
            db->video_count++;
            
            if (id > max_id) {
                max_id = id;
            }
        }
    }
    
    fclose(file);
    
    db->next_video_id = max_id + 1;
    
    return true;
}
