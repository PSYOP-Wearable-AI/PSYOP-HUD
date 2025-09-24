#include "lvgl.h"

static lv_style_t style_card;
static lv_style_t style_label;

static void make_card(lv_obj_t* parent, const char* title) {
  lv_obj_t* card = lv_obj_create(parent);
  lv_obj_add_style(card, &style_card, 0);
  lv_obj_set_size(card, 300, 220);

  lv_obj_t* lbl = lv_label_create(card);
  lv_obj_add_style(lbl, &style_label, 0);
  lv_label_set_text(lbl, title);
  lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 12, 8);
}

void ui_create(void) {
  lv_style_init(&style_card);
  lv_style_set_bg_color(&style_card, lv_color_hex(0x0a1e33));
  lv_style_set_bg_opa(&style_card, LV_OPA_90);
  lv_style_set_border_color(&style_card, lv_color_hex(0x00ffff));
  lv_style_set_border_width(&style_card, 2);
  lv_style_set_radius(&style_card, 14);
  lv_style_set_pad_all(&style_card, 10);

  lv_style_init(&style_label);
  lv_style_set_text_color(&style_label, lv_color_hex(0x00ffff));
  lv_style_set_text_font(&style_label, &lv_font_montserrat_14);

  lv_obj_t* scr = lv_scr_act();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x001122), 0);

  // Grid layout 2x2
  static int col_dsc[] = {310, 310, 310, LV_GRID_TEMPLATE_LAST};
  static int row_dsc[] = {230, 230, LV_GRID_TEMPLATE_LAST};

  lv_obj_t* grid = lv_obj_create(scr);
  lv_obj_set_size(grid, 950, 500);
  lv_obj_center(grid);
  lv_obj_set_style_bg_opa(grid, LV_OPA_0, 0);
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);

  lv_obj_t* c1 = lv_obj_create(grid);
  lv_obj_remove_style_all(c1);
  lv_obj_set_grid_cell(c1, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 0, 1);
  make_card(c1, "Voice Activation");

  lv_obj_t* c2 = lv_obj_create(grid);
  lv_obj_remove_style_all(c2);
  lv_obj_set_grid_cell(c2, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 0, 1);
  make_card(c2, "Translation");

  lv_obj_t* c3 = lv_obj_create(grid);
  lv_obj_remove_style_all(c3);
  lv_obj_set_grid_cell(c3, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 1, 1);
  make_card(c3, "System Monitor");

  lv_obj_t* c4 = lv_obj_create(grid);
  lv_obj_remove_style_all(c4);
  lv_obj_set_grid_cell(c4, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_START, 1, 1);
  make_card(c4, "Notifications");
}
