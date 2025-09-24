#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "lvgl.h"
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

// Forward
void ui_create(void);
void ui_set_status(const char* text);

static void sdl_tick(void* data) {
  (void)data;
  lv_tick_inc(5);
}

static void ui_set_status_async(void* msg) {
  const char* text = (const char*)msg;
  if(text) {
    ui_set_status(text);
    free((void*)text);
  }
}

static void* udp_listener_thread(void* arg) {
  (void)arg;
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("udp socket");
    return NULL;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
  addr.sin_port = htons(9876);

  if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("udp bind");
    close(sock);
    return NULL;
  }

  char buffer[1024];
  for(;;) {
    ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if(n > 0) {
      buffer[n] = '\0';
      // Copy text for async call; LVGL will update on main thread
      char* text = strdup(buffer);
      if(text) lv_async_call(ui_set_status_async, text);
    } else if(n < 0) {
      // Sleep briefly to avoid busy loop on error
      usleep(1000 * 10);
    }
  }
  close(sock);
  return NULL;
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

  // Start UDP listener thread (updates status label)
  pthread_t tid;
  pthread_create(&tid, NULL, udp_listener_thread, NULL);
  pthread_detach(tid);

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
