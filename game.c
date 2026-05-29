#include "raylib.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define SCALE 4
#define WIDTH (WINDOW_WIDTH / SCALE)
#define HEIGHT (WINDOW_HEIGHT / SCALE)
#define SCREEN_SIZE (WIDTH * HEIGHT)

void draw_screen(int *buf, Texture2D texture, Color *pixels);
void tick(int **screenBuffer, int **updateBuffer, int size);
void prepare_buffers(int *screenBuffer, int *updateBuffer, int size);
int count_neighbors(int i, int *buf, int size);

int main(void) {
  srand(time(NULL));

  int screenBuffer[SCREEN_SIZE];
  int updateBuffer[SCREEN_SIZE];

  int *ptrScreenBuf = screenBuffer;
  int *ptrUpdateBuf = updateBuffer;

  prepare_buffers(screenBuffer, updateBuffer, SCREEN_SIZE);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "game of life");
  SetTargetFPS(60);

  Image image = GenImageColor(WIDTH, HEIGHT, BLACK);
  Texture2D texture = LoadTextureFromImage(image);
  UnloadImage(image);

  Color pixels[SCREEN_SIZE];

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    tick(&ptrScreenBuf, &ptrUpdateBuf, SCREEN_SIZE);
    draw_screen(screenBuffer, texture, pixels);
    EndDrawing();
  }

  CloseWindow();
}

// Rules
// 1. Live cell with fewer than 2 live neighbors dies
// 2. Live cell with 2 or 3 neighbors lives on
// 3. Live cell with more than 3 live neighbors dies
// 4. Dead cell with exactly 3 live neighbors becomes alive
void tick(int **screenBuffer, int **updateBuffer, int size) {
  int *src = *screenBuffer;
  int *dst = *updateBuffer;

  for (int i = 0; i < size; i++) {
    int aliveNeighbors = count_neighbors(i, src, size);
    int alive = src[i];

    if (alive && aliveNeighbors < 2) {
      dst[i] = 0;
    } else if (alive && aliveNeighbors <= 3) {
      dst[i] = 1;
    } else if (alive && aliveNeighbors > 3) {
      dst[i] = 0;
    } else if (!alive && aliveNeighbors == 3) {
      dst[i] = 1;
    } else {
      dst[i] = 0;
    }
  }

  int *temp = *screenBuffer;
  *screenBuffer = *updateBuffer;
  *updateBuffer = temp;
}

void draw_screen(int *buf, Texture2D texture, Color *pixels) {
  for (int i = 0; i < SCREEN_SIZE; i++) {
    pixels[i] = buf[i] ? RAYWHITE : BLACK;
  }

  UpdateTexture(texture, pixels);
  DrawTextureEx(texture, (Vector2){0, 0}, 0.0f, (float)SCALE, WHITE);
}

// [0 1 2]    [i-1-w i-w i+1-w]
// [3 4 5] -> [i-1   i   i+1  ]
// [6 7 8]    [i-1+w i+w i+1+w]
int count_neighbors(int i, int *buf, int size) {
  int col = i % WIDTH;
  int row = i / WIDTH;
  int count = 0;

  if (col > 0 && buf[i - 1])
    count++;
  if (col < WIDTH - 1 && buf[i + 1])
    count++;
  if (row > 0 && buf[i - WIDTH])
    count++;
  if (row < HEIGHT - 1 && buf[i + WIDTH])
    count++;
  if (row > 0 && col > 0 && buf[i - WIDTH - 1])
    count++;
  if (row > 0 && col < WIDTH - 1 && buf[i - WIDTH + 1])
    count++;
  if (row < HEIGHT - 1 && col > 0 && buf[i + WIDTH - 1])
    count++;
  if (row < HEIGHT - 1 && col < WIDTH - 1 && buf[i + WIDTH + 1])
    count++;

  return count;
}

void prepare_buffers(int *screenBuffer, int *updateBuffer, int size) {
  for (int i = 0; i < size; i++) {
    int cell = rand() % 2;
    screenBuffer[i] = cell;
  }
}
