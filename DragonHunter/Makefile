CC = gcc

CFLAGS = -Wall -Wextra -Iinclude -IC:/msys64/mingw64/include/SDL2

LIBS = -LC:/msys64/mingw64/lib \
       -lmingw32 \
       -lSDL2main \
       -lSDL2 \
       -lSDL2_ttf \
       -lSDL2_mixer \
       -lm

SRC = src/main.c \
      src/draw.c \
      src/player.c \
      src/dragon.c \
      src/platform.c \
      src/pickup.c \
      src/hud.c \
      src/menu.c \
      src/particle.c \
      src/audio.c \
      src/game.c \
      src/level.c

OBJ = $(SRC:.c=.o)

TARGET = DragonHunter.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o $(TARGET)

run: all
	./$(TARGET)