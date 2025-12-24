void search_videos_menu(VideoDatabase* db) {
    if (!db || db->video_count == 0) {
        printf("База данных пуста!\n");
        return;
    }
    
    printf("\n[ Поиск видео ]\n");
    printf("1. По каналу\n");
    printf("2. По количеству просмотров (минимальное)\n");
    printf("3. По длительности (диапазон)\n");
    
    int choice = get_int_input("Ваш выбор: ", 1, 3);
    
    if (choice == 1) {
        // Поиск по каналу
        char search_term[50];
        get_string_input("Введите название канала: ", search_term, 50);
        
        Video* current = db->head;
        int found = 0;
        
        printf("\nРезультаты поиска по каналу '%s':\n", search_term);
        
        while (current != NULL) {
            // Используем strstr для частичного совпадения
            if (strstr(current->channel, search_term) != NULL) {
                printf("\n");
                print_video_detail(current);
                found++;
            }
            current = current->next;
        }
        
        if (found == 0) {
            printf("Видео с канала '%s' не найдены.\n", search_term);
        } else {
            printf("\nНайдено видео: %d\n", found);
        }
        
    } else if (choice == 2) {
        // Поиск по просмотрам
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
        
    } else if (choice == 3) {
        // Поиск по длительности (новый параметр поиска)
        printf("Поиск по длительности видео\n");
        int min_duration = get_int_input("Минимальная длительность (секунды): ", 0, 86400);
        int max_duration = get_int_input("Максимальная длительность (секунды): ", min_duration, 86400);
        
        Video* current = db->head;
        int found = 0;
        
        printf("\nВидео длительностью %d-%d секунд:\n", min_duration, max_duration);
        
        while (current != NULL) {
            if (current->duration_seconds >= min_duration && 
                current->duration_seconds <= max_duration) {
                printf("\nID: %d | %s\n", current->id, current->title);
                printf("   Канал: %s, Длительность: %d сек\n",
                       current->channel, current->duration_seconds);
                printf("   Просмотры: %d, Дата: %s\n",
                       current->views, current->upload_date);
                found++;
            }
            current = current->next;
        }
        
        if (found == 0) {
            printf("Видео длительностью %d-%d секунд не найдены.\n", 
                   min_duration, max_duration);
        } else {
            printf("\nВсего найдено: %d видео\n", found);
        }
    }
}
