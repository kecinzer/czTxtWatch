#pragma once
#include <pebble.h>

bool public_holiday(struct tm *t, int future);
char *get_nameday(struct tm *t, int future);