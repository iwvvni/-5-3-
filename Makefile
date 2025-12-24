# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = youtube_db

# Файлы
SOURCES = main.c data.c menu.c utils.c
HEADERS = data.h menu.h utils.h
OBJECTS = $(SOURCES:.c=.o)

# Правила сборки
all: $(TARGET)

$(TARGET): $(OBJECTS)
 $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
 $(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
 rm -f $(OBJECTS) $(TARGET) youtube_db.txt

# Запуск
run: $(TARGET)
 ./$(TARGET)

# Отладка
debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)

.PHONY: all clean run debug
