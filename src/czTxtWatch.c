#include <pebble.h>
#include "namedays-cs.h"

static Window *s_main_window;
static TextLayer *first_time_line;
static TextLayer *second_time_line;
static TextLayer *third_time_line;
static TextLayer *day_line;
static TextLayer *date_line;
static TextLayer *nameday_line;

static void configureLayer(TextLayer *textlayer, const char *font, int alignment) {
  text_layer_set_font(textlayer, fonts_get_system_font(font));
  text_layer_set_text_color(textlayer, GColorWhite);
  text_layer_set_background_color(textlayer, GColorClear);
  text_layer_set_text_alignment(textlayer, alignment);
}

void spell_hours_right(char *lines[3], int minutes, char *format1, char *format2, char *format3, char *format4) {
  if (minutes < 6) {
    if (minutes > 0 && strcmp(format1, "půlnoc") == 0) {
      lines[1] = "půlnoci";
    } else if (minutes > 0 && strcmp(format1, "poledne") == 0) {
      lines[1] = "poledni";
    } else {
      lines[0] = format1;
    }
  } else if (minutes > 5 && minutes < 15) {
    lines[2] = format2;
  } else if (minutes > 14 && minutes < 21){
    lines[1] = format2;
  } else if (minutes > 20 && minutes < 30) {
    lines[2] = format3;
  } else if (minutes > 29 && minutes < 36) {
    lines[1] = format3;
  } else if (minutes > 35 && minutes < 45) {
    lines[2] = format2;
  } else if (minutes > 44 && minutes < 51) {
    lines[1] = format2;
  } else if (minutes > 50) {
    lines[1] = format4;
  }
}

static void update_day() {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char *day_of_week[7] = {"neděle", "pondělí", "úterý", "středa", "čtvrtek", "pátek", "sobota"};
  char *day_of_month[31] = {"prvního", "druhého", "třetího", "čtvrtého", "pátého", "šestého", "sedmého", "osmého",
      "devátého", "desátoho", "jedenáctého", "dvanáctého", "třináctého", "čtrnáctého", "patnáctého", "šestnáctého",
      "sedmnáctého", "osmnáctého", "devatenáctého", "dvacátého", "dvacátého prvního", "dvacátého druhého",
      "dvacátého třetího", "dvacátého čtvrtého", "dvacátého pátého", "dvacátého šestého", "dvacátého sedmého",
      "dvacátého osmého", "dvacátého devátého", "třicátého", "třicátého prvního"};
  char *month[12] = {"ledna", "února", "března", "dubna", "května", "června", "července", "srpna", "září", "října",
      "listopadu", "prosince"};
  
  configureLayer(day_line, FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentRight);
  configureLayer(date_line, FONT_KEY_GOTHIC_18, GTextAlignmentRight);
  configureLayer(nameday_line, FONT_KEY_GOTHIC_24, GTextAlignmentCenter);

  text_layer_set_text(day_line, day_of_week[t->tm_wday]);

  int date_size = strlen(day_of_month[t->tm_mday-1]) + strlen(month[t->tm_mon]) + 2;
  char *day_with_month = malloc(date_size);
  snprintf(day_with_month, date_size, "%s %s", day_of_month[t->tm_mday-1], month[t->tm_mon]);

  text_layer_set_text(date_line, day_with_month);
  text_layer_set_text(nameday_line, get_nameday(t, 0));
}

static void update_time() {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  int hours = t->tm_hour;
  int minutes = t->tm_min;
  char *lines[3] = {"EMPTY", "EMPTY", "EMPTY"};
  bool first_bold = false;
  bool second_bold = false;
  bool third_bold = false;

  switch (hours) {
    case 0:
      spell_hours_right(lines, minutes, "půlnoc", "jednu", "jedné", "jedna hodina");
      break;
    case 1:
    case 13:
      spell_hours_right(lines, minutes, "jedna hodina", "dvě", "druhé", "dvě hodiny");
      break;
    case 2:
    case 14:
      spell_hours_right(lines, minutes, "dvě hodiny", "tři", "třetí", "tři hodiny");
      break;
    case 3:
    case 15:
      spell_hours_right(lines, minutes, "tři hodiny", "čtyři", "čtvrté", "čtyři hodiny");
      break;
    case 4:
    case 16:
      spell_hours_right(lines, minutes, "čtyři hodiny", "pět", "páté", "pět hodin");
      break;
    case 5:
    case 17:
      spell_hours_right(lines, minutes, "pět hodin", "šest", "šesté", "šest hodin");
      break;
    case 6:
    case 18:
      spell_hours_right(lines, minutes, "šest hodin", "sedm", "sedmé", "sedm hodin");
      break;
    case 7:
    case 19:
      spell_hours_right(lines, minutes, "sedm hodin", "osm", "osmé", "osm hodin");
      break;
    case 8:
    case 20:
      spell_hours_right(lines, minutes, "osm hodin", "devět", "deváté", "devět hodin");
      break;
    case 9:
    case 21:
      spell_hours_right(lines, minutes, "devět hodin", "deset", "desáté", "deset hodin");
      break;
    case 10:
    case 22:
      spell_hours_right(lines, minutes, "deset hodin", "jedenáct", "jedenácté", "jedenáct hodin");
      break;
    case 11:
      spell_hours_right(lines, minutes, "jedenáct hodin", "dvanáct", "dvanácté", "poledne");
      break;
    case 12:
      spell_hours_right(lines, minutes, "poledne", "jednu", "jedné", "jedna hodina");
      break;
    case 23:
      spell_hours_right(lines, minutes, "jedenáct hodin", "dvanáct", "dvanácté", "půlnoc");
      break;
  }

  if (strcmp(lines[0], "EMPTY") != 0) {
    first_bold = true;
    lines[1] = "";
    lines[2] = "";
  } else if (strcmp(lines[1], "EMPTY") != 0) {
    second_bold = true;
    lines[2] = "";
  } else if (strcmp(lines[2], "EMPTY") != 0) {
    third_bold = true;
  }

  switch (minutes) {
    case 1:
      if (hours == 12 || hours == 0) {
        lines[0] = "minuta po";
      } else {
        lines[1] = "a minuta";
      }
      break;
    case 2:
      if (hours == 12 || hours == 0) {
        lines[0] = "dvě minuty po";
      } else {
        lines[1] = "a dvě minuty";
      }
      break;
    case 3:
      if (hours == 12 || hours == 0) {
        lines[0] = "tři minuty po";
      } else {
        lines[1] = "a tři minuty";
      }
      break;
    case 4:
      if (hours == 12 || hours == 0) {
        lines[0] = "čtyři minuty po";
      } else {
        lines[1] = "a čtyři minuty";
      }
      break;
    case 5:
      if (hours == 12 || hours == 0) {
        lines[0] = "pět minut po";
      } else {
        lines[1] = "a pět minut";
      }
      break;
    case 6:
    case 21:
    case 36:
    case 51:
      lines[0] = "za devět minut";
      break;
    case 7:
    case 22:
    case 37:
    case 52:
      lines[0] = "za osm minut";
      break;
    case 8:
    case 23:
    case 38:
    case 53:
      lines[0] = "za sedm minut";
      break;
    case 9:
    case 24:
    case 39:
    case 54:
      lines[0] = "za šest minut";
      break;
    case 10:
    case 25:
    case 40:
    case 55:
      lines[0] = "za pět minut";
      break;
    case 11:
    case 26:
    case 41:
    case 56:
      lines[0] = "za čtyři minuty";
      break;
    case 12:
    case 27:
    case 42:
    case 57:
      lines[0] = "za tři minuty";
      break;
    case 13:
    case 28:
    case 43:
    case 58:
      lines[0] = "za dvě minuty";
      break;
    case 14:
    case 29:
    case 44:
    case 59:
      lines[0] = "za minutu";
      break;
    case 16:
    case 31:
    case 46:
      lines[2] = "a minuta";
      break;
    case 17:
    case 32:
    case 47:
      lines[2] = "a dvě minuty";
      break;
    case 18:
    case 33:
    case 48:
      lines[2] = "a tři minuty";
      break;
    case 19:
    case 34:
    case 49:
      lines[2] = "a čtyři minuty";
      break;
    case 20:
    case 35:
    case 50:
      lines[2] = "a pět minut";
      break;
  }

  if (minutes > 5 && minutes < 15) {
    lines[1] = "čtvrt na";
  } else if (minutes > 14 && minutes < 21) {
    lines[0] = "čtvrt na";
  } else if (minutes > 20 && minutes < 30) {
    lines[1] = "půl";
  } else if (minutes > 29 && minutes < 36) {
    lines[0] = "půl";
  } else if (minutes > 35 && minutes < 45) {
    lines[1] = "tři čtvrtě na";
  } else if (minutes > 44 && minutes < 51) {
    lines[0] = "tři čtvrtě na";
  }

  configureLayer(first_time_line, first_bold ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_28, GTextAlignmentLeft);
  configureLayer(second_time_line, second_bold ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_28, GTextAlignmentLeft);
  configureLayer(third_time_line, third_bold ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_28, GTextAlignmentLeft);

  text_layer_set_text(first_time_line, lines[0]);
  text_layer_set_text(second_time_line, lines[1]);
  text_layer_set_text(third_time_line, lines[2]);

  // aktualizace každý den o půlnoci
  if (hours == 0 && minutes == 0) {
    update_day();
  }
}

static void main_window_load(Window *window) {
  window_set_background_color(window, GColorBlack);

  first_time_line = text_layer_create(GRect(0, 0, 144, 50));
  second_time_line = text_layer_create(GRect(0, 26, 144, 50));
  third_time_line = text_layer_create(GRect(0, 54, 144, 50));

  update_time();

  day_line = text_layer_create(GRect(0, 82, 144, 50));
  date_line = text_layer_create(GRect(0, 100, 144, 50));
  nameday_line = text_layer_create(GRect(0, 134, 144, 50));

  update_day();

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(first_time_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(second_time_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(third_time_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(nameday_line));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(first_time_line);
  text_layer_destroy(second_time_line);
  text_layer_destroy(third_time_line);
  text_layer_destroy(day_line);
  text_layer_destroy(date_line);
  text_layer_destroy(nameday_line);
}

static void refresh_every_minute(struct tm *tick_time, TimeUnits units_changed) {
  // Aktualizace času
  update_time();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Registrace minutového sledování
  tick_timer_service_subscribe(MINUTE_UNIT, refresh_every_minute);
}

static void deinit() {
    tick_timer_service_unsubscribe();
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}