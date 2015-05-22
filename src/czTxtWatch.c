#include <pebble.h>

static Window *s_main_window;
static TextLayer *first_line;
static TextLayer *second_line;
static TextLayer *third_line;
static TextLayer *nameday_line;
static TextLayer *fourth_line;
static TextLayer *fifth_line;

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

static void configureLayer(TextLayer *textlayer, bool bold) {
  text_layer_set_font(textlayer, fonts_get_system_font(bold ? FONT_KEY_GOTHIC_28_BOLD : FONT_KEY_GOTHIC_28));
  text_layer_set_text_color(textlayer, GColorWhite);
  text_layer_set_background_color(textlayer, GColorClear);
  text_layer_set_text_alignment(textlayer, GTextAlignmentLeft);
}

static void configureLayerSmaller(TextLayer *textlayer, bool bold) {
  text_layer_set_font(textlayer, fonts_get_system_font(bold ? FONT_KEY_GOTHIC_18_BOLD : FONT_KEY_GOTHIC_18));
  text_layer_set_text_color(textlayer, GColorWhite);
  text_layer_set_background_color(textlayer, GColorClear);
  text_layer_set_text_alignment(textlayer, GTextAlignmentRight);
}

static void configureLayerNameday(TextLayer *textlayer) {
  text_layer_set_font(textlayer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_color(textlayer, GColorWhite);
  text_layer_set_background_color(textlayer, GColorClear);
  text_layer_set_text_alignment(textlayer, GTextAlignmentCenter);
}

int leap_year(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int day_of_the_year(struct tm *t) {
  if (t->tm_yday+1 > 59 && !leap_year(t->tm_year+1900)) {
    return t->tm_yday+1;
  } else {
    return t->tm_yday;
  }
}

char *get_nameday(int day) {
  switch (day) {
    case 0:
      return "Den obnovy"; // svátek
    case 1:
      return "Karina";
    case 2:
      return "Radmila";
    case 3:
      return "Diana";
    case 4:
      return "Dalimil";
    case 5:
      return "Tři králové";
    case 6:
      return "Vilma";
    case 7:
      return "Čestmír";
    case 8:
      return "Vladan";
    case 9:
      return "Břetislav";
    case 10:
      return "Bohdana";
    case 11:
      return "Pravoslav";
    case 12:
      return "Edita";
    case 13:
      return "Radovan";
    case 14:
      return "Alice";
    case 15:
      return "Ctirad";
    case 16:
      return "Drahoslav";
    case 17:
      return "Vladislav";
    case 18:
      return "Doubravka";
    case 19:
     return "Ilona";
    case 20:
      return "Běla";
    case 21:
      return "Slavomír";
    case 22:
      return "Zdeněk";
    case 23:
      return "MIlena";
    case 24:
      return "Miloš";
    case 25:
      return "Zora";
    case 26:
      return "Ingrid";
    case 27:
      return "Otýlie";
    case 28:
      return "Zdislava";
    case 29:
      return "Robin";
    case 30:
      return "Marika";
    case 31:
      return "Hynek";
    case 32:
      return "Nela";
    case 33:
      return "Blažej";
    case 34:
      return "Jarmila";
    case 35:
      return "Dobromila";
    case 36:
      return "Vanda";
    case 37:
      return "Veronika";
    case 38:
      return "Milada";
    case 39:
      return "Apolena";
    case 40:
      return "Mojmír";
    case 41:
      return "Božena";
    case 42:
      return "Slavěna";
    case 43:
      return "Věnceslav";
    case 44:
      return "Valentýn";
    case 45:
      return "Jiřina";
    case 46:
      return "Ljuba";
    case 47:
      return "Miloslav";
    case 48:
      return "Gizela";
    case 49:
      return "Patrik";
    case 50:
      return "Oldřich";
    case 51:
      return "Lenka, Eleonora";
    case 52:
      return "Petr";
    case 53:
      return "Svatopluk";
    case 54:
      return "Matěj, Matyáš";
    case 55:
      return "Liliana";
    case 56:
      return "Dorota";
    case 57:
      return "Alexandr";
    case 58:
      return "Lumír";
    case 59:
      return "Horymír"; // 29.2. :))
    case 60:
      return "Bedřich";
    case 61:
      return "Anežka";
    case 62:
      return "Kamil";
    case 63:
      return "Stela";
    case 64:
      return "Kazimír";
    case 65:
      return "Miroslav";
    case 66:
      return "Tomáš";
    case 67:
      return "Gabriela";
    case 68:
      return "Františka";
    case 69:
      return "Viktorie";
    case 70:
      return "Anděla";
    case 71:
      return "Řehoř, Gregor";
    case 72:
      return "Růžena";
    case 73:
      return "Rút, Matylda";
    case 74:
      return "Ida";
    case 75:
      return "Elena, Herbert";
    case 76:
      return "Vlastimil";
    case 77:
      return "Eduard";
    case 78:
      return "Josef";
    case 79:
      return "Světlana";
    case 80:
      return "Radek";
    case 81:
      return "Leona";
    case 82:
      return "Ivona";
    case 83:
      return "Gabriel";
    case 84:
      return "Marián";
    case 85:
      return "Emanuel";
    case 86:
      return "Dita";
    case 87:
      return "Soňa";
    case 88:
      return "Taťána";
    case 89:
      return "Arnošt";
    case 90:
      return "Kvido";
    case 91:
      return "Hugo";
    case 92:
      return "Erika";
    case 93:
      return "Richard";
    case 94:
      return "Ivana";
    case 95:
      return "Miroslava";
    case 96:
      return "Vendula";
    case 97:
      return "Heřman";
    case 98:
      return "Ema";
    case 99:
      return "Dušan";
    case 100:
      return "Darja";
    case 101:
      return "Izabela";
    case 102:
      return "Julius";
    case 103:
      return "Aleš";
    case 104:
      return "Vincenc";
    case 105:
      return "Anastázie";
    case 106:
      return "Irena";
    case 107:
      return "Rudolf";
    case 108:
      return "Valerie";
    case 109:
      return "Rostislav";
    case 110:
      return "Marcela";
    case 111:
      return "Alexandra";
    case 112:
      return "Evžénie";
    case 113:
      return "Vojtěch";
    case 114:
      return "Jiří";
    case 115:
      return "Marek";
    case 116:
      return "Oto";  
    case 117:
      return "Jaroslav";
    case 118:
      return "Vlastislav";
    case 119:
      return "Robert";
    case 120:
      return "Blahoslav";
    case 121:
      return "Svátek práce"; // svátek
    case 122:
      return "Zikmund";
    case 123:
      return "Alexej";
    case 124:
      return "Květoslav";
    case 125:
      return "Klaudie";
    case 126:
      return "Radoslav";
    case 127:
      return "Stanislav";
    case 128:
      return "Den vítězství"; // svátek
    case 129:
      return "Ctibor";
    case 130:
      return "Blažena";
    case 131:
      return "Svatava";
    case 132:
      return "Pankrác";
    case 133:
      return "Servác";
    case 134:
      return "Bonifác";
    case 135:
      return "Žofie";
    case 136:
      return "Přemysl";
    case 137:
      return "Aneta";
    case 138:
      return "Nataša";
    case 139:
      return "Ivo, Ivoš";
    case 140:
      return "Zbyšek";
    case 141:
      return "Monika";
    case 142:
      return "Emil";
    case 143:
      return "Vladimír";
    case 144:
      return "Jana";
    case 145:
      return "Viola";
    case 146:
      return "Filip";
    case 147:
      return "Valdemar";
    case 148:
      return "Vilém";
    case 149:
      return "Maxmilián";
    case 150:
      return "Ferdinand";
    case 151:
      return "Kamila";
    case 152:
      return "Laura";
    case 153:
      return "Jarmil";
    case 154:
      return "Tamara";
    case 155:
      return "Dalibor";
    case 156:
      return "Dobroslav";
    case 157:
      return "Norbert";
    case 158:
      return "Iveta, Yveta";
    case 159:
      return "Medard";
    case 160:
      return "Stanislava";
    case 161:
      return "Otta";
    case 162:
      return "Bruno";
    case 163:
      return "Antonie";
    case 164:
      return "Antonín";
    case 165:
      return "Roland";
    case 166:
      return "Vít";
    case 167:
      return "Zbyněk";
    case 168:
      return "Adolf";
    case 169:
      return "Milan";
    case 170:
      return "Leoš";
    case 171:
      return "Květa";
    case 172:
      return "Alois";
    case 173:
      return "Pavla";
    case 174:
      return "Zdeňka";
    case 175:
      return "Jan";
    case 176:
      return "Ivan";
    case 177:
      return "Adriana";
    case 178:
      return "Ladislav";
    case 179:
      return "Lubomír";
    case 180:
      return "Petr a Pavel";
    case 181:
      return "Šárka";
    case 182:
      return "Jaroslava";
    case 183:
      return "Patricie";
    case 184:
      return "Radomír";
    case 185:
      return "Prokop";
    case 186:
      return "Cyril a Metoděj"; // svátek
    case 187:
      return "Upálení Jana Husa"; // svátek
    case 188:
      return "Bohuslava";
    case 189:
      return "Nora";
    case 190:
      return "Drahoslava";
    case 191:
      return "Libuše, Amálie";
    case 192:
      return "Olga";
    case 193:
      return "Bořek";
    case 194:
      return "Markéta";
    case 195:
      return "Karolína";
    case 196:
      return "Jindřich";
    case 197:
      return "Luboš";
    case 198:
      return "Martina";
    case 199:
      return "Drahomíra";
    case 200:
      return "Čeněk";
    case 201:
      return "Ilja";
    case 202:
      return "Vítězslav";
    case 203:
      return "Magdaléna";
    case 204:
      return "Libor";
    case 205:
      return "Kristýna";
    case 206:
      return "Jakub";
    case 207:
      return "Anna";
    case 208:
      return "Věroslav";
    case 209:
      return "Viktor";
    case 210:
      return "Marta";
    case 211:
      return "Bořivoj";
    case 212:
      return "Ignác";
    case 213:
      return "Oskar";
    case 214:
      return "Gustav";
    case 215:
      return "Miluše";
    case 216:
      return "Dominik";
    case 217:
      return "Kristián";
    case 218:
      return "Oldřiška";
    case 219:
      return "Lada";
    case 220:
      return "Soběslav";
    case 221:
      return "Roman";
    case 222:
      return "Vavřinec";
    case 223:
      return "Zuzana";
    case 224:
      return "Klára";
    case 225:
      return "Alena";
    case 226:
      return "Alan";
    case 227:
      return "Hana";
    case 228:
      return "Jáchym";
    case 229:
      return "Petra";
    case 230:
      return "Helena";
    case 231:
      return "Ludvík";
    case 232:
      return "Bernard";
    case 233:
      return "Johana";
    case 234:
      return "Bohuslav";
    case 235:
      return "Sandra";
    case 236:
      return "Bartoloměj";
    case 237:
      return "Radim";
    case 238:
      return "Luděk";
    case 239:
      return "Otakar";
    case 240:
      return "Augustýn";
    case 241:
      return "Evelína";
    case 242:
      return "Vladěna";
    case 243:
      return "Pavlína";
    case 244:
      return "Linda, Samuel";
    case 245:
      return "Adéla";
    case 246:
      return "Bronislav";
    case 247:
      return "Jindřiška";
    case 248:
      return "Boris";
    case 249:
      return "Boleslav";
    case 250:
      return "Regina";
    case 251:
      return "Mariana";
    case 252:
      return "Daniela";
    case 253:
      return "Irma";
    case 254:
      return "Denisa, Denis";
    case 255:
      return "Marie";
    case 256:
      return "Lubor";
    case 257:
      return "Radka";
    case 258:
      return "Jolana";
    case 259:
      return "Ludmila";
    case 260:
      return "Naděžda";
    case 261:
      return "Kryštof";
    case 262:
      return "Zita";
    case 263:
      return "Oleg";
    case 264:
      return "Matouš";
    case 265:
      return "Darina";
    case 266:
      return "Berta";
    case 267:
      return "Jaromír"; // 24.9.
    case 268:
      return "Zlata";
    case 269:
      return "Andrea";
    case 270:
      return "Jonáš";
    case 271:
      return "Václav - den české státnosti"; // svátek
    case 272:
      return "Michal";
    case 273:
      return "Jeroným";
    case 274:
      return "Igor";
    case 275:
      return "Olívie, Galina";
    case 276:
      return "Bohumil";
    case 277:
      return "František";
    case 278:
      return "Eliška";
    case 279:
      return "Hanuš";
    case 280:
      return "Justýna";
    case 281:
      return "Věra";
    case 282:
      return "Štefan";
    case 283:
      return "Marina";
    case 284:
      return "Andrej";
    case 285:
      return "Marcel";
    case 286:
      return "Renáta";
    case 287:
      return "Agáta";
    case 288:
      return "Tereza";
    case 289:
      return "Havel";
    case 290:
      return "Hedvika";
    case 291:
      return "Lukáš";
    case 292:
      return "Michaela";
    case 293:
      return "Vendelín";
    case 294:
      return "Brigita";
    case 295:
      return "Sabina";
    case 296:
      return "Teodor";
    case 297:
      return "Nina";
    case 298:
      return "Beáta";
    case 299:
      return "Erik";
    case 300:
      return "Šarlota, Zoe";
    case 301:
      return "Den vzniku sam. čs státu"; // svátek 28.10.
    case 302:
      return "Silvie";
    case 303:
      return "Tadeáš";
    case 304:
      return "Štepánka";
    case 305:
      return "Felix";
    case 306:
      return "";
    case 307:
      return "Hubert";
    case 308:
      return "Karel";
    case 309:
      return "Miriam";
    case 310:
      return "Liběna";
    case 311:
      return "Saskie";
    case 312:
      return "Bohumír";
    case 313:
      return "Bohdan";
    case 314:
      return "Evžen";
    case 315:
      return "Martin";
    case 316:
      return "Benedikt";
    case 317:
      return "Tibor";
    case 318:
      return "Sáva";
    case 319:
      return "Leopold";
    case 320:
      return "Otmar";
    case 321:
      return "Mahulena, den boje za sv. a dem."; // svátek - 17.11.
    case 322:
      return "Romana";
    case 323:
      return "Alžběta";
    case 324:
      return "Nikola";
    case 325:
      return "Albert";
    case 326:
      return "Cecílie";
    case 327:
      return "Klement";
    case 328:
      return "Emílie";
    case 329:
      return "Kateřina";
    case 330:
      return "Artur";
    case 331:
      return "Xenie";
    case 332:
      return "René";
    case 333:
      return "Zina";
    case 334:
      return "Ondřej";
    case 335:
      return "Iva";
    case 336:
      return "Blanka";
    case 337:
      return "Svatoslav";
    case 338:
      return "Barbora";
    case 339:
      return "Jitka";
    case 340:
      return "Mikuláš";
    case 341:
      return "Ambrož";
    case 342:
      return "Květoslava";
    case 343:
      return "Vratislav";
    case 344:
      return "Julie";
    case 345:
      return "Dana";
    case 346:
      return "Simona";
    case 347:
      return "Lucie";
    case 348:
      return "Lýdie";
    case 349:
      return "Radana";
    case 350:
      return "Albína";
    case 351:
      return "Daniel";
    case 352:
      return "Miloslav";
    case 353:
      return "Ester";
    case 354:
      return "Dagmar";
    case 355:
      return "Natálie";
    case 356:
      return "Šimon";
    case 357:
      return "Vlasta";
    case 358:
      return "Adam a Eva, štědrý den"; // svátek
    case 359:
      return "1. svátek vánoční"; // svátek
    case 360:
      return "2. svátek vánoční"; // svátek
    case 361:
      return "Žaneta";
    case 362:
      return "Bohumila";
    case 363:
      return "Judita";
    case 364:
      return "David";
    case 365:
      return "Silvestr";

  }
  return "Chyba";
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
  
  configureLayerNameday(nameday_line);
  configureLayerSmaller(fourth_line, true);
  configureLayerSmaller(fifth_line, false);

  text_layer_set_text(nameday_line, get_nameday(day_of_the_year(t)));
  text_layer_set_text(fourth_line, day_of_week[t->tm_wday]);

  char *day_with_month = malloc(strlen(day_of_month[t->tm_mday-1]) + strlen(month[t->tm_mon]) + 2);
  strcpy(day_with_month, day_of_month[t->tm_mday-1]);
  strcat(day_with_month, " ");
  strcat(day_with_month, month[t->tm_mon]);
  text_layer_set_text(fifth_line, day_with_month);
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

  configureLayer(first_line, first_bold ? true : false);
  configureLayer(second_line, second_bold ? true : false);
  configureLayer(third_line, third_bold ? true : false);

  text_layer_set_text(first_line, lines[0]);
  text_layer_set_text(second_line, lines[1]);
  text_layer_set_text(third_line, lines[2]);

  // aktualizace každý den o půlnoci
  if (hours == 0 && minutes == 0) {
    update_day();
  }
}

static void main_window_load(Window *window) {
  window_set_background_color(window, GColorBlack);

  // První řádek
  first_line = text_layer_create(GRect(0, 0, 144, 50));
  // Druhý řádek
  second_line = text_layer_create(GRect(0, 26, 144, 50));
  // Třetí řádek
  third_line = text_layer_create(GRect(0, 54, 144, 50));

  update_time();

  // řádek svátku
  nameday_line = text_layer_create(GRect(0, 134, 144, 50));
  // Čtvrtý řádek
  fourth_line = text_layer_create(GRect(0, 82, 144, 50));
  // Pátý řádek
  fifth_line = text_layer_create(GRect(0, 100, 144, 50));

  update_day();

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(first_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(second_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(third_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(nameday_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(fourth_line));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(fifth_line));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(first_line);
  text_layer_destroy(second_line);
  text_layer_destroy(third_line);
  text_layer_destroy(nameday_line);
  text_layer_destroy(fourth_line);
  text_layer_destroy(fifth_line);
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