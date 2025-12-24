> iwvvni:
#include "menu.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

// Вывод главного меню
void print_menu(void) {
    print_separator();
    printf("[  База данных YouTube  ]\n");
    print_separator();
    printf("1. Вывести все видео на экран\n");
    printf("2. Добавить видео\n");
    printf("3. Удалить видео\n");
    printf("4. Найти видео\n");
    printf("5. Показать статистику\n");
    printf("6. Сохранить изменения\n");
    printf("7. Загрузить из файла\n");
    printf("8. Загрузить другую базу данных\n");
    printf("9. Выход\n");
    print_separator();
    printf("\n");
}

// Отображение всех видео
void display_all_videos(VideoDatabase* db) {
    if (!db || db->video_count == 0) {
        printf("Нет видео для отображения!\n");
        return;
    }
    
    printf("\n[ Список видео ]\n");
    printf("Всего видео: %d\n\n", db->video_count);
    
    Video* current = db->head;
    int count = 1;
    while (current != NULL) {
        printf("%d. %s\n", count++, current->title);
        printf("   ID: %d | Канал: %s | Просмотры: %d\n", 
               current->id, current->channel, current->views);
        printf("   Длительность: %d сек | Лайки: %d\n", 
               current->duration_seconds, current->likes);
        printf("   Дата: %s\n", current->upload_date);
        print_separator();
        current = current->next;
    }
}

// Меню добавления видео
void add_video_menu(VideoDatabase* db) {
    if (!db) return;
    
    printf("[ Добавить новое видео ]\n");
    
    char title[100], channel[50], date[11];
    int duration, views, likes;
    
    get_string_input("Название видео: ", title, sizeof(title));
    get_string_input("Канал: ", channel, sizeof(channel));
    duration = get_int_input("Длительность (секунды): ", 1, 86400);
    views = get_int_input("Количество просмотров: ", 0, 1000000000);
    likes = get_int_input("Количество лайков: ", 0, views);
    
    // Проверка формата даты
    while (1) {
        get_string_input("Дата загрузки (ГГГГ-ММ-ДД): ", date, sizeof(date));
        if (validate_date_format(date)) {
            break;
        }
        printf("Неверный формат даты! Используйте ГГГГ-ММ-ДД (например: 2023-12-25)\n");
    }
    
    if (add_video(db, title, channel, duration, views, likes, date)) {
        printf("\n✅ Видео '%s' добавлено! ID: %d\n", title, db->next_video_id - 1);
    } else {
        printf("\n❌ Ошибка добавления видео!\n");
    }
}

// Меню удаления видео
void delete_video_menu(VideoDatabase* db) {
    if (!db || db->video_count == 0) {
        printf("Нет видео для удаления!\n");
        return;
    }
    
    printf("\n[ Удаление видео ]\n");
    display_all_videos(db);
    
    int video_id = get_int_input("\nВведите ID видео для удаления (0 для отмены): ", 
                                0, db->next_video_id - 1);
    
    if (video_id == 0) {
        printf("Удаление отменено.\n");
        return;
    }
    
    Video* video = find_video_by_id(db, video_id);
    if (!video) {
        printf("Видео с ID %d не найдено!\n", video_id);
        return;
    }
    
    printf("\nВы точно хотите удалить видео?\n");
    printf("Название: %s\n", video->title);
    printf("Канал: %s\n", video->channel);
    printf("ID: %d\n\n", video->id);
    printf("1. Да\n");
    printf("2. Нет\n");
    
    int confirm = get_int_input("Ваш выбор: ", 1, 2);
    if (confirm == 1) {
        if (remove_video(db, video_id)) {
            printf("\n✅ Видео удалено!\n");
        } else {
            printf("\n❌ Ошибка удаления!\n");
        }
    } else {
        printf("Удаление отменено.\n");
    }
}

// Меню поиска видео
void search_videos_menu(VideoDatabase* db) {
    if (!db || db->video_count == 0) {
        printf("База данных пуста!\n");
        return;
    }
    
    printf("\n[ Поиск видео ]\n");

> iwvvni:
printf("1. По каналу\n");
    printf("2. По количеству просмотров\n");
    
    int choice = get_int_input("Ваш выбор: ", 1, 2);
    
    if (choice == 1) {
        char search_term[50];
        get_string_input("Введите название канала: ", search_term, 50);
        
        int result_count;
        Video** results = search_by_channel(db, search_term, &result_count);
        
        if (result_count == 0) {
            printf("\nВидео с канала '%s' не найдены.\n", search_term);
        } else {
            printf("\nРезультаты поиска '%s':\n", search_term);
            printf("Найдено видео: %d\n\n", result_count);
            
            for (int i = 0; i < result_count; i++) {
                print_video_detail(results[i]);
                if (i < result_count - 1) {
                    printf("\n");
                }
            }
            
            free(results); // Освобождаем память
        }
        
    } else if (choice == 2) {
        int min_views = get_int_input("Минимальное количество просмотров: ", 0, 1000000000);
        
        Video* current = db->head;
        int found = 0;
        
        printf("\nВидео с %d+ просмотрами:\n", min_views);
        
        while (current != NULL) {
            if (current->views >= min_views) {
                printf("\nID: %d | %s\n", current->id, current->title);
                printf("   Канал: %s, Просмотры: %d, Лайков: %d\n",
                       current->channel, current->views, current->likes);
                printf("   Длительность: %d сек, Дата: %s\n",
                       current->duration_seconds, current->upload_date);
                found++;
            }
            current = current->next;
        }
        
        if (found == 0) {
            printf("Видео с %d+ просмотрами не найдены.\n", min_views);
        } else {
            printf("\nВсего найдено: %d видео\n", found);
        }
    }
}

// Показать статистику базы данных
void show_statistics(VideoDatabase* db) {
    if (!db || db->video_count == 0) {
        printf("База данных пуста!\n");
        return;
    }
    
    Video* current = db->head;
    int total_views = 0, total_likes = 0, total_duration = 0;
    int max_views = 0, max_likes = 0;
    Video* most_viewed = NULL;
    Video* most_liked = NULL;
    
    while (current != NULL) {
        total_views += current->views;
        total_likes += current->likes;
        total_duration += current->duration_seconds;
        
        if (current->views > max_views) {
            max_views = current->views;
            most_viewed = current;
        }
        
        if (current->likes > max_likes) {
            max_likes = current->likes;
            most_liked = current;
        }
        
        current = current->next;
    }
    
    printf("\n[ Статистика базы данных ]\n");
    printf("Всего видео: %d\n", db->video_count);
    printf("Всего просмотров: %d\n", total_views);
    printf("Всего лайков: %d\n", total_likes);
    
    if (db->video_count > 0) {
        printf("Среднее просмотров на видео: %.0f\n", (float)total_views / db->video_count);
        printf("Среднее лайков на видео: %.0f\n", (float)total_likes / db->video_count);
        printf("Общее время просмотра: %d часов %d минут\n", 
               total_duration / 3600, (total_duration % 3600) / 60);
    }
    
    if (most_viewed != NULL) {
        printf("\nСамое просматриваемое видео:\n");
        printf("  %s (ID: %d)\n", most_viewed->title, most_viewed->id);
        printf("  Просмотры: %d\n", most_viewed->views);
    }
    
    if (most_liked != NULL) {
        printf("\nСамое любимое видео:\n");
        printf("  %s (ID: %d)\n", most_liked->title, most_liked->id);
        printf("  Лайки: %d\n", most_liked->likes);
    }
}
