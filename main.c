#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "menu.h"
#include "utils.h"

#define DEF_FILENAME "youtube_db.txt"

// Получение имени файла базы данных
void get_database_name(char* buffer, int buffer_size) {
    printf("\n[ Выбор базы данных ]\n");
    printf("Введите имя файла для базы данных\n");
    printf("(или нажмите Enter для использования '%s'): ", DEF_FILENAME);
    
    if (fgets(buffer, buffer_size, stdin) == NULL) {
        strcpy(buffer, DEF_FILENAME);
        return;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        strcpy(buffer, DEF_FILENAME);
    }
    
    if (strstr(buffer, ".txt") == NULL) {
        strcat(buffer, ".txt");
    }
    
    printf("Используется файл: %s\n", buffer);
}

// Главная функция
int main(void) {
    VideoDatabase* db = create_database();
    if (!db) {
        printf("Ошибка создания базы данных.\n");
        return 1;
    }
    
    char cur_filename[500];
    strcpy(cur_filename, DEF_FILENAME);
    get_database_name(cur_filename, sizeof(cur_filename));
    
    printf("\nЗагружаем '%s'...\n", cur_filename);
    if (load_from_file(cur_filename, db)) {
        printf("Данные загружены из файла!\n");
        printf("Видео в базе: %d\n", db->video_count);
    } else {
        printf("Файл не найден. Создана новая база данных.\n");
    }
    
    int choice;
    int running = 1;
    
    while (running) {
        print_menu();
        choice = get_int_input("Введите номер: ", 1, 9);
        
        switch (choice) {
            case 1:
                display_all_videos(db);
                press_enter_to_continue();
                break;
                
            case 2:
                add_video_menu(db);
                press_enter_to_continue();
                break;
                
            case 3:
                delete_video_menu(db);
                press_enter_to_continue();
                break;
                
            case 4:
                search_videos_menu(db);
                press_enter_to_continue();
                break;
                
            case 5:
                show_statistics(db);
                press_enter_to_continue();
                break;
                
            case 6:
                if (save_to_file(cur_filename, db)) {
                    printf("✅ Успешно сохранено!\n");
                } else {
                    printf("❌ Ошибка сохранения данных!\n");
                }
                press_enter_to_continue();
                break;
                
            case 7:
                if (load_from_file(cur_filename, db)) {
                    printf("✅ Успешно загружено!\n");
                } else {
                    printf("❌ Ошибка загрузки данных!\n");
                }
                press_enter_to_continue();
                break;
                
            case 8: {
                char new_filename[500];
                printf("\n[ Загрузка другой базы данных ]\n");
                printf("Текущий файл: %s\n", cur_filename);
                printf("Текущие данные будут утеряны!\n");
                printf("Сохранить текущие данные?\n");
                printf("1. Да\n");
                printf("2. Нет\n");
                printf("3. Отмена\n");
                
                int load_choice = get_int_input("Ваш выбор: ", 1, 3);
                if (load_choice == 1) {
                    if (save_to_file(cur_filename, db)) {
                        printf("Данные сохранены в '%s'\n", cur_filename);
                    }
                } else if (load_choice == 3) {
                    printf("Загрузка отменена.\n");
                    press_enter_to_continue();
break;
                }
                
                get_string_input("Введите имя файла для загрузки: ", 
                               new_filename, sizeof(new_filename));
                
                VideoDatabase* temp_db = create_database();
                if (!temp_db) {
                    printf("❌ Ошибка создания базы!\n");
                    press_enter_to_continue();
                    break;
                }
                
                if (load_from_file(new_filename, temp_db)) {
                    free_database(db);
                    db = temp_db;
                    strcpy(cur_filename, new_filename);
                    printf("✅ Данные загружены из '%s'!\n", cur_filename);
                    printf("Видео в базе: %d\n", db->video_count);
                } else {
                    free_database(temp_db);
                    printf("❌ Ошибка загрузки файла '%s'!\n", new_filename);
                    printf("Файл не найден.\n");
                }
                press_enter_to_continue();
                break;
            }
                
            case 9:
                printf("Идёт сохранение данных...\n");
                if (save_to_file(cur_filename, db)) {
                    printf("✅ Успешно сохранено! Выход из программы.\n");
                } else {
                    printf("❌ Ошибка сохранения данных!\n");
                }
                running = 0;
                break;
        }
    }
    
    free_database(db);
    return 0;
}
