cmake_minimum_required(VERSION 3.16)
project(visor_lvgl_hud C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Options
option(ENABLE_WEBSOCKET "Enable websocket client (libwebsockets)" OFF)

# Point LVGL to our config header
add_definitions(-DLV_CONF_INCLUDE_SIMPLE=1)
add_definitions(-DLV_CONF_PATH=${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h)

include(FetchContent)

# Fetch LVGL
FetchContent_Declare(
  lvgl
  GIT_REPOSITORY https://github.com/lvgl/lvgl.git
  GIT_TAG v9.2.0
)
FetchContent_MakeAvailable(lvgl)

# SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Sources
file(GLOB SRC
  src/main.c
  src/ui.c
)

add_executable(visor_lvgl_hud ${SRC})

# Include dirs
target_include_directories(visor_lvgl_hud PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${lvgl_SOURCE_DIR}
  ${lvgl_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Also ensure lvgl itself sees our config
target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE=1 LV_CONF_PATH=${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h)

# Minimal lv_conf via compile definitions (fallbacks)
add_compile_definitions(
  LV_COLOR_DEPTH=32
  LV_USE_LOG=1
  LV_USE_GPU=0
  LV_FONT_MONTSERRAT_14=1
)

# Link
target_link_libraries(visor_lvgl_hud PRIVATE
  lvgl
  ${SDL2_LIBRARIES}
)

# Compiler flags
target_compile_options(visor_lvgl_hud PRIVATE ${SDL2_CFLAGS_OTHER})

# macOS framework links if needed
if(APPLE)
  target_link_options(visor_lvgl_hud PRIVATE -framework Cocoa -framework IOKit -framework CoreVideo)
endif()

# Install
install(TARGETS visor_lvgl_hud RUNTIME DESTINATION bin)
