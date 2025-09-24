#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "lvgl.h"

// Forward
void ui_create(void);

static void sdl_tick(void* data) {
  (void)data;
  lv_tick_inc(5);
}

int main(int argc, char** argv) {
  (void)argc; (void)argv;

  // Init SDL
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
    fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
    return 1;
  }

  // Init LVGL
  lv_init();

  // Create SDL window and renderer
  SDL_Window* window = SDL_CreateWindow(
    "VISOR LVGL HUD",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 800,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );

  if(!window) {
    fprintf(stderr, "SDL window failed: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!renderer) {
    fprintf(stderr, "SDL renderer failed: %s\n", SDL_GetError());
    return 1;
  }

  // Create an LVGL display buffer and display driver that renders into an SDL texture
  static lv_color_t buf1[1280 * 50];
  static lv_draw_buf_t draw_buf;
  lv_draw_buf_init(&draw_buf, buf1, NULL, 1280 * 50);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 1280;
  disp_drv.ver_res = 800;

  // Simple render callback using SDL
  static SDL_Texture* texture = NULL;
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, disp_drv.hor_res, disp_drv.ver_res);

  disp_drv.draw_buf = &draw_buf;
  disp_drv.flush_cb = [](lv_disp_drv_t* d, const lv_area_t* area, lv_color_t* color_p) {
    int32_t w = (area->x2 - area->x1 + 1);
    int32_t h = (area->y2 - area->y1 + 1);

    // Update SDL texture
    SDL_UpdateTexture(texture, NULL, color_p, d->hor_res * sizeof(lv_color_t));

    // Render
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    lv_disp_flush_ready(d);
  };

  lv_disp_t* disp = lv_disp_drv_register(&disp_drv);
  (void)disp;

  // Create UI
  ui_create();

  // Timer for LVGL tick
  SDL_AddTimer(5, (SDL_TimerCallback)sdl_tick, NULL);

  bool running = true;
  while(running) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) running = false;
    }

    lv_timer_handler();
    SDL_Delay(5);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
