/**
 * @file lv_theme.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "etx_lv_theme.h"
#include "../colors.h"
#include "font.h"

#include "libopenui_config.h"

extern lv_color_t makeLvColor(uint32_t colorFlags);

/*********************
 *      DEFINES
 *********************/
#define MODE_DARK 1
#define RADIUS_DEFAULT (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 12) : lv_disp_dpx(theme.disp, 8))

/*SCREEN*/
#define LIGHT_COLOR_SCR        makeLvColor(COLOR_THEME_SECONDARY3)
#define LIGHT_COLOR_GREY       makeLvColor(COLOR_THEME_SECONDARY2)
#define DARK_COLOR_SCR         lv_color_hex(0x15171A)
#define DARK_COLOR_GREY        lv_color_hex(0x2f3237)

#define BORDER_WIDTH            lv_disp_dpx(theme.disp, 2)

#define PAD_SMALL   (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 14) : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 12) : lv_disp_dpx(theme.disp, 10))
#define PAD_TINY   (disp_size == DISP_LARGE ? lv_disp_dpx(theme.disp, 8) : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 6) : lv_disp_dpx(theme.disp, 2))

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
  lv_style_t keyboard;
  lv_style_t scrollbar;
  lv_style_t scrollbar_scrolled;
  lv_style_t btn;
  lv_style_t line_btn;
  lv_style_t field;

  /*Utility*/
  lv_style_t bg_color_grey;
  lv_style_t bg_color_white;
  lv_style_t bg_color_active;
  lv_style_t bg_color_focus;
  lv_style_t bg_color_edit;
  lv_style_t pressed;
  lv_style_t disabled;
  lv_style_t focus_border;
  lv_style_t pad_zero;
  lv_style_t pad_tiny;
  lv_style_t pad_small;
  lv_style_t circle;
  lv_style_t rounded;
  lv_style_t border;
  lv_style_t anim_fast;

  // Slider parts
  lv_style_t slider_main;
  lv_style_t slider_knob;

  // Choice
  lv_style_t choice_main;

#if LV_USE_SWITCH
  lv_style_t switch_main;
  lv_style_t switch_knob;
#endif

#if LV_USE_TABLE
  lv_style_t table_cell;
#endif

#if LV_USE_TEXTAREA
  lv_style_t field_cursor, edit_cursor;
#endif

#if LV_USE_KEYBOARD
  lv_style_t keyboard_btn_bg;
#endif
} my_theme_styles_t;

typedef enum {
    DISP_SMALL = 3,
    DISP_MEDIUM = 2,
    DISP_LARGE = 1,
} disp_size_t;

/**********************
 *  STATIC VARIABLES
 **********************/
static my_theme_styles_t styles;
static lv_theme_t theme;
static disp_size_t disp_size;
static lv_color_t color_grey;
static bool inited = false;

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init_reset(lv_style_t * style)
{
  if(inited) {
    lv_style_reset(style);
  }
  else {
    lv_style_init(style);
  }
}

static lv_color_t dark_color_filter_cb(const lv_color_filter_dsc_t * f, lv_color_t c, lv_opa_t opa)
{
  LV_UNUSED(f);
  return lv_color_darken(c, opa);
}

static lv_color_t grey_filter_cb(const lv_color_filter_dsc_t * f, lv_color_t color, lv_opa_t opa)
{
  LV_UNUSED(f);
  if(theme.flags & MODE_DARK) return lv_color_mix(lv_palette_darken(LV_PALETTE_GREY, 2), color, opa);
  else return lv_color_mix(lv_palette_lighten(LV_PALETTE_GREY, 2), color, opa);
}

static void style_init(void)
{
  color_grey = theme.flags & MODE_DARK ? DARK_COLOR_GREY : LIGHT_COLOR_GREY;

  style_init_reset(&styles.scrollbar);
  lv_style_set_bg_color(&styles.scrollbar,
                        (theme.flags & MODE_DARK) ? lv_palette_darken(LV_PALETTE_GREY, 2) : lv_palette_main(LV_PALETTE_GREY));
  lv_style_set_pad_all(&styles.scrollbar, lv_disp_dpx(theme.disp, 7));
  lv_style_set_width(&styles.scrollbar,  lv_disp_dpx(theme.disp, 5));
  lv_style_set_bg_opa(&styles.scrollbar,  LV_OPA_40);

  style_init_reset(&styles.scrollbar_scrolled);
  lv_style_set_bg_opa(&styles.scrollbar_scrolled,  LV_OPA_COVER);

  style_init_reset(&styles.keyboard);
  lv_style_set_bg_opa(&styles.keyboard, LV_OPA_COVER);
  lv_style_set_bg_color(&styles.keyboard, theme.flags & MODE_DARK ? DARK_COLOR_SCR : LIGHT_COLOR_SCR);
  lv_style_set_pad_row(&styles.keyboard, PAD_SMALL);
  lv_style_set_pad_column(&styles.keyboard, PAD_SMALL);

  style_init_reset(&styles.border);
  lv_style_set_border_opa(&styles.border, LV_OPA_100);
  lv_style_set_border_width(&styles.border, 1);
  lv_style_set_border_color(&styles.border, makeLvColor(COLOR_THEME_SECONDARY2));

  style_init_reset(&styles.btn);
  lv_style_set_bg_opa(&styles.btn, LV_OPA_COVER);
  lv_style_set_bg_color(&styles.btn, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_border_opa(&styles.btn, LV_OPA_0);
  lv_style_set_border_width(&styles.btn, BORDER_WIDTH);
  lv_style_set_text_color(&styles.btn, makeLvColor(COLOR_THEME_SECONDARY1));
  lv_style_set_pad_hor(&styles.btn, PAD_SMALL);
  lv_style_set_pad_ver(&styles.btn, PAD_SMALL);
  lv_style_set_pad_column(&styles.btn, lv_disp_dpx(theme.disp, 5));
  lv_style_set_pad_row(&styles.btn, lv_disp_dpx(theme.disp, 5));

  style_init_reset(&styles.line_btn);
  lv_style_set_bg_opa(&styles.line_btn, LV_OPA_COVER);
  lv_style_set_bg_color(&styles.line_btn, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_border_opa(&styles.line_btn, LV_OPA_COVER);
  lv_style_set_border_width(&styles.line_btn, BORDER_WIDTH);
  lv_style_set_border_color(&styles.line_btn, makeLvColor(COLOR_THEME_SECONDARY2));

  style_init_reset(&styles.field);
  lv_style_set_border_width(&styles.field, lv_dpx(1));
  lv_style_set_border_color(&styles.field, makeLvColor(COLOR_THEME_SECONDARY2));
  lv_style_set_bg_color(&styles.field, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_bg_opa(&styles.field, LV_OPA_COVER);
  lv_style_set_text_color(&styles.field, makeLvColor(COLOR_THEME_SECONDARY1));
  lv_style_set_pad_top(&styles.field, 4);
  lv_style_set_pad_bottom(&styles.field, 5);
  lv_style_set_pad_left(&styles.field, 5);

  static lv_color_filter_dsc_t dark_filter;
  lv_color_filter_dsc_init(&dark_filter, dark_color_filter_cb);

  static lv_color_filter_dsc_t grey_filter;
  lv_color_filter_dsc_init(&grey_filter, grey_filter_cb);

  style_init_reset(&styles.pressed);
  lv_style_set_color_filter_dsc(&styles.pressed, &dark_filter);
  lv_style_set_color_filter_opa(&styles.pressed, 35);

  style_init_reset(&styles.disabled);
  lv_style_set_color_filter_dsc(&styles.disabled, &grey_filter);
  lv_style_set_color_filter_opa(&styles.disabled, LV_OPA_50);

  style_init_reset(&styles.focus_border);
  lv_style_set_border_color(&styles.focus_border, makeLvColor(COLOR_THEME_FOCUS));
  lv_style_set_border_opa(&styles.focus_border, LV_OPA_100);
  lv_style_set_border_width(&styles.focus_border, BORDER_WIDTH);

  style_init_reset(&styles.pad_small);
  lv_style_set_pad_all(&styles.pad_small, PAD_SMALL);
  lv_style_set_pad_gap(&styles.pad_small, PAD_SMALL);

  style_init_reset(&styles.pad_zero);
  lv_style_set_pad_all(&styles.pad_zero, 0);
  lv_style_set_pad_row(&styles.pad_zero, 0);
  lv_style_set_pad_column(&styles.pad_zero, 0);

  style_init_reset(&styles.pad_tiny);
  lv_style_set_pad_all(&styles.pad_tiny, PAD_TINY);
  lv_style_set_pad_row(&styles.pad_tiny, PAD_TINY);
  lv_style_set_pad_column(&styles.pad_tiny, PAD_TINY);

  style_init_reset(&styles.bg_color_grey);
  lv_style_set_bg_color(&styles.bg_color_grey, color_grey);
  lv_style_set_bg_opa(&styles.bg_color_grey, LV_OPA_COVER);

  style_init_reset(&styles.bg_color_white);
  lv_style_set_bg_color(&styles.bg_color_white, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_bg_opa(&styles.bg_color_white, LV_OPA_COVER);
  lv_style_set_text_color(&styles.bg_color_white, makeLvColor(COLOR_THEME_PRIMARY1));

  style_init_reset(&styles.bg_color_active);
  lv_style_set_bg_color(&styles.bg_color_active, makeLvColor(COLOR_THEME_ACTIVE));
  lv_style_set_bg_opa(&styles.bg_color_active, LV_OPA_COVER);
  lv_style_set_text_color(&styles.bg_color_active, makeLvColor(COLOR_THEME_PRIMARY1));

  style_init_reset(&styles.bg_color_focus);
  lv_style_set_bg_color(&styles.bg_color_focus, makeLvColor(COLOR_THEME_FOCUS));
  lv_style_set_bg_opa(&styles.bg_color_focus, LV_OPA_COVER);
  lv_style_set_text_color(&styles.bg_color_focus, makeLvColor(COLOR_THEME_PRIMARY2));

  style_init_reset(&styles.bg_color_edit);
  lv_style_set_bg_color(&styles.bg_color_edit, makeLvColor(COLOR_THEME_EDIT));
  lv_style_set_bg_opa(&styles.bg_color_edit, LV_OPA_COVER);
  lv_style_set_text_color(&styles.bg_color_edit, makeLvColor(COLOR_THEME_PRIMARY2));

  style_init_reset(&styles.circle);
  lv_style_set_radius(&styles.circle, LV_RADIUS_CIRCLE);

  style_init_reset(&styles.rounded);
  lv_style_set_radius(&styles.rounded,
        (disp_size == DISP_LARGE    ? lv_disp_dpx(theme.disp, 8)
         : disp_size == DISP_MEDIUM ? lv_disp_dpx(theme.disp, 6)
                                    : lv_disp_dpx(theme.disp, 4)));

  // Slider
  style_init_reset(&styles.slider_main);
  lv_style_set_bg_opa(&styles.slider_main, LV_OPA_100);
  lv_style_set_bg_color(&styles.slider_main, makeLvColor(COLOR_THEME_SECONDARY1));
  style_init_reset(&styles.slider_knob);
  lv_style_set_bg_color(&styles.slider_knob, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_bg_opa(&styles.slider_knob, LV_OPA_COVER);
  lv_style_set_border_color(&styles.slider_knob, makeLvColor(COLOR_THEME_SECONDARY1));
  lv_style_set_border_width(&styles.slider_knob, 1);
  lv_style_set_pad_all(&styles.slider_knob, lv_disp_dpx(theme.disp, 6));

  // Choice
  style_init_reset(&styles.choice_main);
  lv_style_set_pad_top(&styles.choice_main, 3);
  lv_style_set_pad_bottom(&styles.choice_main, 4);
  lv_style_set_pad_right(&styles.choice_main, 5);
  lv_style_set_bg_opa(&styles.choice_main, LV_OPA_100);
  lv_style_set_bg_color(&styles.choice_main, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_border_color(&styles.choice_main, makeLvColor(COLOR_THEME_SECONDARY2));
  lv_style_set_border_width(&styles.choice_main, 1);
  lv_style_set_text_color(&styles.choice_main, makeLvColor(COLOR_THEME_SECONDARY1));

  style_init_reset(&styles.anim_fast);
  lv_style_set_anim_time(&styles.anim_fast, 120);

#if LV_USE_SWITCH
  style_init_reset(&styles.switch_main);
  lv_style_set_pad_all(&styles.switch_main, 1);
  style_init_reset(&styles.switch_knob);
  lv_style_set_pad_all(&styles.switch_knob, -3);
  lv_style_set_bg_opa(&styles.switch_knob, LV_OPA_100);
  lv_style_set_bg_color(&styles.switch_knob, makeLvColor(COLOR_THEME_PRIMARY2));
  lv_style_set_border_color(&styles.switch_knob, makeLvColor(COLOR_THEME_SECONDARY1));
  lv_style_set_border_width(&styles.switch_knob, 2);
#endif

#if LV_USE_TABLE
  style_init_reset(&styles.table_cell);
  lv_style_set_border_width(&styles.table_cell, lv_disp_dpx(theme.disp, 1));
  lv_style_set_border_color(&styles.table_cell, color_grey);
  lv_style_set_border_side(&styles.table_cell, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM);
#endif

#if LV_USE_TEXTAREA
  style_init_reset(&styles.field_cursor);
  lv_style_set_opa(&styles.field_cursor, LV_OPA_0);

  style_init_reset(&styles.edit_cursor);
  lv_style_set_opa(&styles.edit_cursor, LV_OPA_COVER);
  lv_style_set_bg_opa(&styles.edit_cursor, LV_OPA_50);
#endif

#if LV_USE_KEYBOARD
  style_init_reset(&styles.keyboard_btn_bg);
  lv_style_set_shadow_width(&styles.keyboard_btn_bg, 0);
  lv_style_set_radius(&styles.keyboard_btn_bg, disp_size == DISP_SMALL ? RADIUS_DEFAULT / 2 : RADIUS_DEFAULT);
#endif
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_theme_t *etx_lv_theme_init(lv_disp_t *disp, lv_color_t color_primary,
                              lv_color_t color_secondary, bool dark,
                              const lv_font_t *font)
{
  if (LV_HOR_RES <= 320)
    disp_size = DISP_SMALL;
  else if (LV_HOR_RES < 720)
    disp_size = DISP_MEDIUM;
  else
    disp_size = DISP_LARGE;

  theme.disp = disp;
  theme.color_primary = color_primary;
  theme.color_secondary = color_secondary;
  theme.font_small = font;
  theme.font_normal = font;
  theme.font_large = font;
  theme.flags = dark ? MODE_DARK : 0;

  style_init();

  if (disp == NULL || lv_disp_get_theme(disp) == &theme)
    lv_obj_report_style_change(NULL);

  inited = true;

  return (lv_theme_t *)&theme;
}

lv_theme_t * etx_lv_theme_get(void)
{
  if(!etx_lv_theme_is_inited()) {
    return NULL;
  }

  return (lv_theme_t *)&theme;
}

bool etx_lv_theme_is_inited(void)
{
  return inited;
}

/**********************
 *   Custom object creation
 **********************/

const lv_obj_class_t input_mix_line_class = {
    .base_class = &lv_btn_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size = sizeof(lv_btn_t),
};

lv_obj_t* input_mix_line_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&input_mix_line_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.line_btn, 0);
  lv_obj_add_style(obj, &styles.rounded, 0);
  lv_obj_add_style(obj, &styles.pad_tiny, 0);
  lv_obj_set_style_bg_color(obj, makeLvColor(COLOR_THEME_SECONDARY3), LV_STATE_CHECKED);
  lv_obj_add_style(obj, &styles.focus_border, LV_STATE_FOCUSED);
  lv_obj_set_style_text_color(obj, makeLvColor(COLOR_THEME_SECONDARY1), 0);
  lv_obj_set_style_text_font(obj, getFont(FONT(STD)), 0);

  return obj;
}

const lv_obj_class_t input_mix_group_class = {
    .base_class = &lv_obj_class,
    .width_def = LV_PCT(100),
    .height_def = LV_SIZE_CONTENT,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_FALSE,
    .instance_size = sizeof(lv_obj_t),
};

lv_obj_t* input_mix_group_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&input_mix_group_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.line_btn, 0);
  lv_obj_add_style(obj, &styles.rounded, 0);
  lv_obj_add_style(obj, &styles.pad_tiny, 0);
  lv_obj_set_style_text_color(obj, makeLvColor(COLOR_THEME_SECONDARY1), 0);
  lv_obj_set_style_text_font(obj, getFont(FONT(BOLD)), 0);

  return obj;
}

static void field_edit_event(const lv_obj_class_t* class_p, lv_event_t* e);

const lv_obj_class_t field_edit_class = {
    .base_class = &lv_textarea_class,
    .event_cb = field_edit_event,
    .width_def = LV_DPI_DEF / 2,
    .height_def = LV_SIZE_CONTENT,
    .editable = LV_OBJ_CLASS_EDITABLE_TRUE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size = sizeof(lv_textarea_t)
};

static void field_edit_event(const lv_obj_class_t* class_p, lv_event_t* e)
{
  /* Call the ancestor's event handler */
  lv_obj_event_base(&field_edit_class, e);
}

lv_obj_t* etx_text_edit_create(lv_obj_t* parent)
{
  lv_obj_t* obj = lv_obj_class_create_obj(&field_edit_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.field, 0);
  lv_obj_add_style(obj, &styles.rounded, 0);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_STATE_FOCUSED);
  lv_obj_add_style(obj, &styles.bg_color_edit, LV_STATE_EDITED);

  lv_obj_add_style(obj, &styles.field_cursor, LV_PART_CURSOR);
  lv_obj_add_style(obj, &styles.edit_cursor, LV_PART_CURSOR | LV_STATE_EDITED);

  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  lv_textarea_set_password_mode(obj, false);
  lv_textarea_set_one_line(obj, true);

  return obj;
}

lv_obj_t* etx_number_edit_create(lv_obj_t* parent)
{
  lv_obj_t* obj = etx_text_edit_create(parent);

  lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, 0);

  return obj;
}

extern "C" void window_event_cb(lv_event_t * e);
static void window_base_event(const lv_obj_class_t* class_p, lv_event_t* e);

const lv_obj_class_t window_base_class = {
    .base_class = &lv_obj_class,
    .event_cb = window_base_event,
    .width_def = LV_DPI_DEF,
    .height_def = LV_DPI_DEF,
    .editable = LV_OBJ_CLASS_EDITABLE_FALSE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_FALSE,
    .instance_size = sizeof(lv_obj_t)
};

static void window_base_event(const lv_obj_class_t* class_p, lv_event_t* e)
{
  /* Call the ancestor's event handler */
  lv_res_t res = lv_obj_event_base(&window_base_class, e);
  if (res != LV_RES_OK) return;

  window_event_cb(e);
}

lv_obj_t* window_create(lv_obj_t* parent)
{
  lv_obj_t* obj = lv_obj_class_create_obj(&window_base_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.scrollbar, LV_PART_SCROLLBAR);
  lv_obj_add_style(obj, &styles.scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);

  return obj;
}

static void table_event(const lv_obj_class_t * class_p, lv_event_t * e);

const lv_obj_class_t table_class  = {
    .base_class = &lv_table_class,
    .constructor_cb = NULL,
    .destructor_cb = NULL,
    .event_cb = table_event,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .editable = LV_OBJ_CLASS_EDITABLE_TRUE,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size = 0,
};

static void table_event(const lv_obj_class_t * class_p, lv_event_t * e)
{
  lv_res_t res;
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_RELEASED) {
    /*Call the ancestor's event handler*/
    res = lv_obj_event_base(&table_class, e);
    if(res != LV_RES_OK) return;
  } else {
    lv_obj_t * obj = lv_event_get_target(e);
    lv_table_t * table = (lv_table_t *)obj;

    /*From lv_table.c: handler for LV_EVENT_RELEASED*/
    lv_obj_invalidate(obj);
    lv_indev_t* indev = lv_indev_get_act();
    lv_obj_t* scroll_obj = lv_indev_get_scroll_obj(indev);
    if (table->col_act != LV_TABLE_CELL_NONE &&
        table->row_act != LV_TABLE_CELL_NONE && scroll_obj == NULL) {
      res = lv_event_send(obj, LV_EVENT_VALUE_CHANGED, NULL);
      if (res != LV_RES_OK) return;
    }
  }
}

lv_obj_t * etx_table_create(lv_obj_t * parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&table_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.pad_zero, 0);
  lv_obj_add_style(obj, &styles.scrollbar, LV_PART_SCROLLBAR);
  lv_obj_add_style(obj, &styles.scrollbar_scrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
  lv_obj_add_style(obj, &styles.bg_color_white, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.table_cell, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.pad_small, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_PART_ITEMS | LV_STATE_EDITED);

  return obj;
}

lv_obj_t* etx_keyboard_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&lv_keyboard_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.keyboard, 0);
  lv_obj_add_style(obj, disp_size == DISP_LARGE ? &styles.pad_small : &styles.pad_tiny, 0);
  lv_obj_add_style(obj, &styles.btn, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.rounded, 0);
  lv_obj_add_style(obj, &styles.disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
  lv_obj_add_style(obj, &styles.bg_color_white, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.keyboard_btn_bg, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
  lv_obj_add_style(obj, &styles.bg_color_active, LV_PART_ITEMS | LV_STATE_CHECKED);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_PART_ITEMS | LV_STATE_EDITED);

  return obj;
}

const lv_obj_class_t etx_switch_class = {
    .base_class = &lv_switch_class,
    .height_def = 32
};

lv_obj_t* etx_switch_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&etx_switch_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.switch_main, 0);
  lv_obj_add_style(obj, &styles.bg_color_grey, 0);
  lv_obj_add_style(obj, &styles.border, 0);
  lv_obj_add_style(obj, &styles.circle, 0);
  lv_obj_add_style(obj, &styles.circle, LV_PART_INDICATOR);
  lv_obj_add_style(obj, &styles.circle, LV_PART_KNOB);
  lv_obj_add_style(obj, &styles.anim_fast, 0);
  lv_obj_add_style(obj, &styles.disabled, LV_STATE_DISABLED);
  lv_obj_add_style(obj, &styles.disabled, LV_PART_INDICATOR | LV_STATE_DISABLED);
  lv_obj_add_style(obj, &styles.disabled, LV_PART_KNOB | LV_STATE_DISABLED);
  lv_obj_add_style(obj, &styles.focus_border, LV_STATE_FOCUSED);
  lv_obj_add_style(obj, &styles.focus_border, LV_PART_INDICATOR | LV_STATE_FOCUSED);
  lv_obj_add_style(obj, &styles.bg_color_active, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_add_style(obj, &styles.switch_knob, LV_PART_KNOB);

  return obj;
}

lv_obj_t* etx_slider_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&lv_slider_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.slider_main, LV_PART_MAIN);
  lv_obj_add_style(obj, &styles.slider_knob, LV_PART_KNOB);
  lv_obj_add_style(obj, &styles.circle, LV_PART_MAIN);
  lv_obj_add_style(obj, &styles.circle, LV_PART_KNOB);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_PART_MAIN | LV_STATE_FOCUSED | LV_STATE_EDITED);

  return obj;
}

lv_obj_t* etx_btnmatrix_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&lv_btnmatrix_class, parent);
  lv_obj_class_init_obj(obj);

  // main
  lv_obj_add_style(obj, &styles.rounded, 0);
  // items
  lv_obj_add_style(obj, &styles.btn, LV_PART_ITEMS);
  lv_obj_add_style(obj, &styles.rounded, 0);
  lv_obj_add_style(obj, &styles.disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
  lv_obj_add_style(obj, &styles.pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
  lv_obj_add_style(obj, &styles.bg_color_active, LV_PART_ITEMS | LV_STATE_CHECKED);
  lv_obj_add_style(obj, &styles.focus_border, LV_PART_ITEMS | LV_STATE_EDITED);

  return obj;
}

lv_obj_t* etx_button_create(lv_obj_t* parent)
{
  lv_obj_t * obj = lv_obj_class_create_obj(&lv_btn_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.btn, 0);
  lv_obj_add_style(obj, &styles.border, 0);
  lv_obj_add_style(obj, &styles.rounded, 0);
  lv_obj_add_style(obj, &styles.bg_color_active, LV_STATE_CHECKED);
  lv_obj_add_style(obj, &styles.focus_border, LV_STATE_FOCUSED);
  lv_obj_add_style(obj, &styles.disabled, LV_STATE_DISABLED);

  return obj;
}

lv_obj_t* etx_choice_create(lv_obj_t* parent)
{
  lv_obj_t* obj = lv_obj_class_create_obj(&lv_obj_class, parent);
  lv_obj_class_init_obj(obj);

  lv_obj_add_style(obj, &styles.choice_main, 0);
  lv_obj_add_style(obj, &styles.bg_color_focus, LV_PART_MAIN | LV_STATE_FOCUSED);
  lv_obj_add_style(obj, &styles.rounded, 0);

  return obj;
}
