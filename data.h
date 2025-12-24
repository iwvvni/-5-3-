#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

// Структура для хранения информации о YouTube видео
typedef struct Video {
    int id;                     // Уникальный идентификатор видео
    char title[100];            // Название видео
    char channel[50];           // Название канала
    int duration_seconds;       // Длительность в секундах
    int views;                  // Количество просмотров
    int likes;                  // Количество лайков
    char upload_date[11];       // Дата загрузки (YYYY-MM-DD)
    struct Video* next;         // Указатель на следующий элемент списка
} Video;

// Структура для управления базой данных
typedef struct {
    Video* head;                // Указатель на первый элемент
    Video* tail;                // Указатель на последний элемент
    int video_count;            // Количество видео в базе
    int next_video_id;          // ID для следующего видео
} VideoDatabase;

// Функции для работы с базой данных
VideoDatabase* create_database(void);
void free_database(VideoDatabase* db);
bool add_video(VideoDatabase* db, const char* title, const char* channel,
               int duration, int views, int likes, const char* date);
bool remove_video(VideoDatabase* db, int video_id);
Video* find_video_by_id(VideoDatabase* db, int video_id);
Video** search_by_channel(VideoDatabase* db, const char* channel, int* result_count);
bool save_to_file(const char* filename, VideoDatabase* db);
bool load_from_file(const char* filename, VideoDatabase* db);

#endif
