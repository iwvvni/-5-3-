#ifndef UTILS_H
#define UTILS_H
#include "data.h"

void clear_input_buffer(void);
int get_int_input(const char* prompt, int min, int max);
void get_string_input(const char* prompt, char* buffer, int max_len);
void print_separator(void);
void print_video_detail(const Video* video);
void press_enter_to_continue(void);
void convert_duration(int seconds, int* minutes, int* secs);
bool validate_date_format(const char* date); // НОВАЯ ФУНКЦИЯ

#endif
