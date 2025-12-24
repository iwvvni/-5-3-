#ifndef MENU_H
#define MENU_H

#include "data.h"

// Функции меню
void print_menu(void);
void display_all_videos(VideoDatabase* db);
void add_video_menu(VideoDatabase* db);
void delete_video_menu(VideoDatabase* db);
void search_videos_menu(VideoDatabase* db);
void show_statistics(VideoDatabase* db);

#endif
