// ascii_codes.h
// Handy ANSI codes, Unicode symbols, and box drawing characters for terminal apps.

#ifndef ASCII_CODES_H
#define ASCII_CODES_H

#include <stdio.h>



// ----------------------------
// ANSI sequences
// ----------------------------
#define RESET              "\033[0m"
#define CLEAR_SCREEN       "\033[2J\033[H"
#define BOLD               "\033[1m"
#define SIZE               "\033[=18h"
#define SET_FONT_SIZE      "\033["
#define HIDE_CURSOR        "\033[?25l"
#define SHOW_CURSOR        "\033[?25h"
#define START_LINE_CLEAR   "\r\033[K"
#define STRIKE             "\033[9m"

// ----------------------------
// Colors
// ----------------------------
#define RED      "\x1b[38;2;255;0;0m"
#define ORANGE   "\x1b[38;2;230;76;0m"
#define YELLOW   "\x1b[38;2;230;226;0m"
#define GREEN    "\x1b[38;2;0;186;40m"
#define BLUE     "\x1b[38;2;0;72;255m"
#define INDIGO   "\x1b[38;2;84;0;230m"
#define VIOLET   "\x1b[38;2;176;0;230m"
#define ANSI_RESET "\x1b[0m"

// ----------------------------
// Fun strings / emoticons
// ----------------------------
#define SHRUG        "¯\\_(ツ)_/¯"
#define NICE         "(☞ﾟヮﾟ)☞"
#define NICE_OTHER   "☜(ﾟヮﾟ☜)"
#define TABLE_FLIP   "(╯°□°）╯︵ ┻━┻"
#define PUT_TABLE_BACK "┬─┬ ノ( ゜-゜ノ)"
#define WAVING       "(°▽°)/"

// ----------------------------
// Cursor control
// ----------------------------
static inline void set_cursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// ----------------------------
// 24-bit RGB foreground color
// ----------------------------
static inline void color_rgb(int r, int g, int b) {
    printf("\033[38;2;%d;%d;%dm", r, g, b);
}

// 8-bit foreground color
static inline void color_x(int x) {
    printf("\033[38;5;%dm", x);
}

// 24-bit RGB background color
static inline void bg_rgb(int r, int g, int b) {
    printf("\033[48;2;%d;%d;%dm", r, g, b);
}

// 8-bit background color
static inline void bg_x(int x) {
    printf("\033[48;5;%dm", x);
}


// ----------------------------
// ICONS
// ----------------------------
// Squares
#define WHITE_SQUARE                "□"
#define BLACK_SQUARE                "■"
#define WHITE_SQUARE_ROUNDED        "▢"

// Arrows
#define LEFT_ARROW                  "←"
#define RIGHT_ARROW                 "→"
#define UP_ARROW                    "↑"
#define DOWN_ARROW                  "↓"

#define LEFT_RIGHT_ARROW            "↔"
#define UP_DOWN_ARROW               "↕"

#define TOP_LEFT_ARROW              "↖"
#define TOP_RIGHT_ARROW             "↗"
#define BOTTOM_LEFT_ARROW           "↙"
#define BOTTOM_RIGHT_ARROW          "↘"

#define ENTER_ARROW                 "↵"
#define ENTER_ARROW_RIGHT           "↳"
#define ENTER                       "⏎"

#define DELETE_RIGHT                "⌦"
#define DELETE_LEFT                 "⌫"

// Misc keys and symbols
#define COMMAND                     "⌘"
#define CONTROL_KEY                 "⎈"
#define ESCAPE_SYMBOL               "⎋"

// Media controls
#define EJECT                       "⏏"
#define SKIP_FORWARD                "⏭"
#define FAST_FORWARD                "⏩"
#define REWIND                      "⏪"
#define SKIP_BACKWARD               "⏮"
#define PLAY_PAUSE                  "⏯"

#define SLOW_REWIND                 "⏴"
#define SLOW_FORWARD                "⏵"
#define UP_TRIANGLE                 "⏶"
#define DOWN_TRIANGLE               "⏷"

#define PLAY                        "⏵"
#define PAUSE                       "⏸"
#define STOP                        "⏹"
#define RECORD                      "⏺"

#define POWER                       "⏻"
#define SLEEP                       "⏼"

// Time symbols
#define WATCH                       "⌚"
#define HOURGLASS                   "⌛"
#define ALARM_CLOCK                 "⏰"
#define STOPWATCH                   "⏱"
#define TIMER_CLOCK                 "⏲"

// Stars and celestial
#define STAR_PIC                    "⭐"
#define STAR                        "⭑"
#define STAR_STAMP                  "⍟"
#define MOON                        "⏾"

// Brackets
#define BRACKET_LEFT                "〈"
#define BRACKET_RIGHT               "〉"

// Circles, diamonds, squares
#define WHITE_CIRCLE                "○"
#define BLACK_CIRCLE                "●"
#define BLACK_DIAMOND               "◆"
#define WHITE_DIAMOND               "◇"
#define WHITE_MEDIUM_SQUARE         "◻"
#define BLACK_MEDIUM_SQUARE         "◼"
#define WHITE_MEDIUM_SMALL_SQUARE   "◽"
#define BLACK_MEDIUM_SMALL_SQUARE   "◾"

// Circles
#define FISH_EYE                              "◉"
#define CIRCLE_WITH_LEFT_HALF_BLACK           "◐"
#define CIRCLE_WITH_RIGHT_HALF_BLACK          "◑"
#define CIRCLE_WITH_LOWER_HALF_BLACK          "◒"
#define CIRCLE_WITH_UPPER_HALF_BLACK          "◓"
#define CIRCLE_WITH_UPPER_RIGHT_QUADRANT_BLACK "◔"
#define CIRCLE_WITH_ALL_BUT_UPPER_LEFT_QUADRANT_BLACK "◕"

// Triangles
#define BLACK_LOWER_RIGHT_TRIANGLE            "◢"
#define BLACK_LOWER_LEFT_TRIANGLE             "◣"
#define BLACK_UPPER_LEFT_TRIANGLE             "◤"
#define BLACK_UPPER_RIGHT_TRIANGLE            "◥"

// Manicules / hands
#define MANICULE_LEFT_DARK                     "☚"
#define MANICULE_RIGHT_DARK                    "☛"
#define MANICULE_LEFT_LIGHT                    "☜"
#define MANICULE_RIGHT_LIGHT                   "☞"
#define MANICULE_UP_LIGHT                      "☝"
#define MANICULE_DOWN_LIGHT                    "☟"

// Danger / warning symbols
#define SKULL_AND_CROSSBONES                   "☠"
#define RADIO_ACTIVE                           "☢"
#define BIOHAZARD                              "☣"
#define WARNING                                "⚠"
#define CADUCEUS                               "☤"
#define CRESCENT_STAR                          "☪"
#define PEACE                                  "☮"
#define YIN_YANG                               "☯"
#define FROWN                                  "☹"
#define SMILE                                  "☺"
#define HEART                                  "♥"

// Gender symbols
#define MALE                                   "♂"
#define FEMALE                                 "♀"

// Chess pieces
#define WHITE_KING                             "♔"
#define WHITE_QUEEN                            "♕"
#define WHITE_ROOK                             "♖"
#define WHITE_BISHOP                           "♗"
#define WHITE_KNIGHT                           "♘"
#define WHITE_PAWN                             "♙"
#define BLACK_KING                             "♚"
#define BLACK_QUEEN                            "♛"
#define BLACK_ROOK                             "♜"
#define BLACK_BISHOP                           "♝"
#define BLACK_KNIGHT                           "♞"
#define BLACK_PAWN                             "♟"

// Card suits
#define SPADES                                 "♠"
#define SPADES_LIGHT                           "♤"
#define HEARTS                                 "♥"
#define HEARTS_LIGHT                           "♡"
#define DIAMONDS                               "♦"
#define DIAMONDS_LIGHT                         "♢"
#define CLUBS                                  "♣"
#define CLUBS_LIGHT                            "♧"

// Musical symbols
#define QUARTER_NOTE                           "♩"
#define EIGHTH_NOTE                            "♪"
#define BEAMED_EIGHTH_NOTES                     "♫"
#define BEAMED_SIXTEENTH_NOTES                  "♬"
#define FLAT                                   "♭"
#define NATURAL                                "♮"
#define SHARP                                  "♯"

// Recycle symbols
#define RECYCLE                                "♲"
#define RECYCLE_1                              "♳"
#define RECYCLE_2                              "♴"
#define RECYCLE_3                              "♵"
#define RECYCLE_4                              "♶"
#define RECYCLE_5                              "♷"
#define RECYCLE_6                              "♸"
#define RECYCLE_7                              "♹"
#define RECYCLE_TRIANGULAR                     "♺"
#define RECYCLE_DARK                           "♻"

// Dice
#define DICE_1                                 "⚀"
#define DICE_2                                 "⚁"
#define DICE_3                                 "⚂"
#define DICE_4                                 "⚃"
#define DICE_5                                 "⚄"
#define DICE_6                                 "⚅"

// Weather / celestial
#define SNOWFLAKE                              "❄"
#define SNOWMAN                                "☃"
#define SUN                                    "☀"
#define CLOUD                                  "☁"
#define RAIN                                   "⛆"
#define SNOW_CLOUD                             "⛈"
#define UMBRELLA                               "☂"

// Transport / objects
#define TRUCK                                  "⛟"
#define BOAT                                   "⛴"
#define ANCHOR                                 "⚓"
#define MOUNTAIN                               "⛰"
#define SKI                                    "⛷"
#define SKATE                                  "⛸"
#define TORII                                  "⛩"

// Tools / gear
#define CHAIN                                  "⛓"
#define GEAR                                   "⚙"
#define HAMMER                                 "⚒"
#define PICK                                   "⛏"
#define HAMMER_PICK                            "⚒"

// Weapons / flags / castles
#define SHIELD                                 "⛨"
#define SWORD                                  "⚔"
#define FLAG                                   "⚑"
#define CASTLE                                 "⛫"

// Asteroids / minor planets
#define CERES                                  "⚳"
#define PALLAS                                 "⚴"
#define JUNO                                   "⚵"
#define VESTA                                  "⚶"
#define CHIRON                                 "⚷"

// ----------------------------
// BOX DRAWING 
// ----------------------------

#define LIGHT_HORIZONTAL "─"
#define HEAVY_HORIZONTAL "━"
#define LIGHT_VERTICAL   "│"
#define HEAVY_VERTICAL   "┃"

#define LIGHT_TRIPE_DASH_HORIZONTAL "┄"
#define HEAVY_TRIPE_DASH_HORIZONTAL "┅"
#define LIGHT_TRIPE_DASH_VERTICAL   "┆"
#define HEAVY_TRIPE_DASH_VERTICAL   "┇"

#define LIGHT_QUADRUPLE_DASH_HORIZONTAL "┈"
#define HEAVY_QUADRUPLE_DASH_HORIZONTAL "┉"
#define LIGHT_QUADRUPLE_DASH_VERTICAL   "┊"
#define HEAVY_QUADRUPLE_DASH_VERTICAL   "┋"

#define LIGHT_DOWN_AND_RIGHT       "┌"
#define DOWN_LIGHT_AND_RIGHT_HEAVY "┍"
#define DOWN_HEAVY_AND_RIGHT_LIGHT "┎"
#define HEAVY_DOWN_AND_RIGHT       "┏"

#define LIGHT_DOWN_AND_LEFT        "┐"
#define DOWN_LIGHT_AND_LEFT_HEAVY  "┑"
#define DOWN_HEAVY_AND_LEFT_LIGHT  "┒"
#define HEAVY_DOWN_AND_LEFT        "┓"

#define LIGHT_UP_AND_RIGHT         "└"
#define UP_LIGHT_AND_RIGHT_HEAVY   "┕"
#define UP_HEAVY_AND_RIGHT_LIGHT   "┖"
#define HEAVY_UP_AND_RIGHT         "┗"

#define LIGHT_UP_AND_LEFT          "┘"
#define UP_LIGHT_AND_LEFT_HEAVY    "┙"
#define UP_HEAVY_AND_LEFT_LIGHT    "┚"
#define HEAVY_UP_AND_LEFT          "┛"

#define LIGHT_VERTICAL_AND_RIGHT   "├"
#define VERTICAL_LIGHT_AND_RIGHT_HEAVY "┝"
#define UP_HEAVY_AND_RIGHT_DOWN_LIGHT   "┞"
#define DOWN_HEAVY_AND_RIGHT_UP_LIGHT   "┟"
#define VERTICAL_HEAVY_AND_RIGHT_LIGHT  "┠"
#define DOWN_LIGHT_AND_RIGHT_UP_HEAVY   "┡"
#define UP_LIGHT_AND_RIGHT_DOWN_HEAVY   "┢"
#define HEAVY_VERTICAL_AND_RIGHT        "┣"

#define LIGHT_VERTICAL_AND_LEFT    "┤"
#define VERTICAL_LIGHT_AND_LEFT_HEAVY  "┥"
#define UP_HEAVY_AND_LEFT_DOWN_LIGHT   "┦"
#define DOWN_HEAVY_AND_LEFT_UP_LIGHT   "┧"
#define VERTICAL_HEAVY_AND_LEFT_LIGHT  "┨"
#define DOWN_LIGHT_AND_LEFT_UP_HEAVY   "┩"
#define UP_LIGHT_AND_LEFT_DOWN_HEAVY   "┪"
#define HEAVY_VERTICAL_AND_LEFT        "┫"

#define LIGHT_DOWN_AND_HORIZONTAL         "┬"
#define LEFT_HEAVY_AND_RIGHT_DOWN_LIGHT   "┭"
#define RIGHT_HEAVY_AND_LEFT_DOWN_LIGHT   "┮"
#define DOWN_LIGHT_AND_HORIZONTAL_HEAVY   "┯"
#define DOWN_HEAVY_AND_HORIZONTAL_LIGHT   "┰"
#define RIGHT_LIGHT_AND_LEFT_DOWN_HEAVY   "┱"
#define LEFT_LIGHT_AND_RIGHT_DOWN_HEAVY   "┲"
#define HEAVY_DOWN_AND_HORIZONTAL         "┳"

#define LIGHT_UP_AND_HORIZONTAL           "┴"
#define LEFT_HEAVY_AND_RIGHT_UP_LIGHT     "┵"
#define RIGHT_HEAVY_AND_LEFT_UP_LIGHT     "┶"
#define UP_LIGHT_AND_HORIZONTAL_HEAVY     "┷"
#define UP_HEAVY_AND_HORIZONTAL_LIGHT     "┸"
#define RIGHT_LIGHT_AND_LEFT_UP_HEAVY     "┹"
#define LEFT_LIGHT_AND_RIGHT_UP_HEAVY     "┺"
#define HEAVY_UP_AND_HORIZONTAL           "┻"

#define LIGHT_VERTICAL_AND_HORIZONTAL         "┼"
#define LEFT_HEAVY_AND_RIGHT_VERTICAL_LIGHT   "┽"
#define RIGHT_HEAVY_AND_LEFT_VERTICAL_LIGHT   "┾"
#define VERTICAL_LIGHT_AND_HORIZONTAL_HEAVY   "┿"
#define UP_HEAVY_AND_DOWN_HORIZONTAL_LIGHT    "╀"
#define DOWN_HEAVY_AND_UP_HORIZONTAL_LIGHT    "╁"
#define VERTICAL_HEAVY_AND_HORIZONTAL_LIGHT   "╂"
#define LEFT_UP_HEAVY_AND_RIGHT_DOWN_LIGHT    "╃"
#define RIGHT_UP_HEAVY_AND_LEFT_DOWN_LIGHT    "╄"
#define LEFT_DOWN_HEAVY_AND_RIGHT_UP_LIGHT    "╅"
#define RIGHT_DOWN_HEAVY_AND_LEFT_UP_LIGHT    "╆"
#define DOWN_LIGHT_AND_UP_HORIZONTAL_HEAVY    "╇"
#define UP_LIGHT_AND_DOWN_HORIZONTAL_HEAVY    "╈"
#define RIGHT_LIGHT_AND_LEFT_VERTICAL_HEAVY   "╉"
#define LEFT_LIGHT_AND_RIGHT_VERTICAL_HEAVY   "╊"
#define HEAVY_VERTICAL_AND_HORIZONTAL         "╋"

#define LIGHT_DOUBLE_DASH_HORIZONTAL "╌"
#define HEAVY_DOUBLE_DASH_HORIZONTAL "╍"
#define LIGHT_DOUBLE_DASH_VERTICAL   "╎"
#define HEAVY_DOUBLE_DASH_VERTICAL   "╏"

#define DOUBLE_HORIZONTAL "═"
#define DOUBLE_VERTICAL   "║"

#define DOWN_SINGLE_AND_RIGHT_DOUBLE   "╒"
#define DOWN_DOUBLE_AND_RIGHT_SINGLE   "╓"
#define DOUBLE_DOWN_AND_RIGHT          "╔"

#define DOWN_SINGLE_AND_LEFT_DOUBLE    "╕"
#define DOWN_DOUBLE_AND_LEFT_SINGLE    "╖"
#define DOUBLE_DOWN_AND_LEFT           "╗"

#define UP_SINGLE_AND_RIGHT_DOUBLE     "╘"
#define UP_DOUBLE_AND_RIGHT_SINGLE     "╙"
#define DOUBLE_UP_AND_RIGHT            "╚"

#define UP_SINGLE_AND_LEFT_DOUBLE      "╛"
#define UP_DOUBLE_AND_LEFT_SINGLE      "╜"
#define DOUBLE_UP_AND_LEFT             "╝"

#define VERTICAL_SINGLE_AND_RIGHT_DOUBLE  "╞"
#define VERTICAL_DOUBLE_AND_RIGHT_SINGLE  "╟"
#define DOUBLE_VERTICAL_AND_RIGHT         "╠"

#define VERTICAL_SINGLE_AND_LEFT_DOUBLE   "╡"
#define VERTICAL_DOUBLE_AND_LEFT_SINGLE   "╢"
#define DOUBLE_VERTICAL_AND_LEFT          "╣"

#define DOWN_SINGLE_AND_HORIZONTAL_DOUBLE "╤"
#define DOWN_DOUBLE_AND_HORIZONTAL_SINGLE "╥"
#define DOUBLE_DOWN_AND_HORIZONTAL        "╦"

#define UP_SINGLE_AND_HORIZONTAL_DOUBLE   "╧"
#define UP_DOUBLE_AND_HORIZONTAL_SINGLE   "╨"
#define DOUBLE_UP_AND_HORIZONTAL          "╩"

#define VERTICAL_SINGLE_AND_HORIZONTAL_DOUBLE "╪"
#define VERTICAL_DOUBLE_AND_HORIZONTAL_SINGLE "╫"
#define DOUBLE_VERTICAL_AND_HORIZONTAL        "╬"

#define LIGHT_ARC_DOWN_AND_RIGHT   "╭"
#define LIGHT_ARC_DOWN_AND_LEFT    "╮"
#define LIGHT_ARC_UP_AND_LEFT      "╯"
#define LIGHT_ARC_UP_AND_RIGHT     "╰"

#define LIGHT_DIAGONAL_UPPER_RIGHT_TO_LOWER_LEFT "╱"
#define LIGHT_DIAGONAL_UPPER_LEFT_TO_LOWER_RIGHT "╲"
#define LIGHT_DIAGONAL_CROSS                     "╳"

#define LIGHT_LEFT   "╴"
#define LIGHT_UP     "╵"
#define LIGHT_RIGHT  "╶"
#define LIGHT_DOWN   "╷"

#define HEAVY_LEFT   "╸"
#define HEAVY_UP     "╹"
#define HEAVY_RIGHT  "╺"
#define HEAVY_DOWN   "╻"

#define LIGHT_LEFT_AND_HEAVY_RIGHT "╼"
#define LIGHT_UP_AND_HEAVY_DOWN    "╽"
#define HEAVY_LEFT_AND_LIGHT_RIGHT "╾"
#define HEAVY_UP_AND_LIGHT_DOWN    "╿"



// ----------------------------
// BLOCK ELEMENTS
// ----------------------------


#define UPPER_HALF_BLOCK                         "▀"
#define LOWER_ONE_EIGHTH_BLOCK                   "▁"
#define LOWER_ONE_QUARTER_BLOCK                  "▂"
#define LOWER_THREE_EIGHTHS_BLOCK                "▃"
#define LOWER_HALF_BLOCK                          "▄"
#define LOWER_FIVE_EIGHTHS_BLOCK                 "▅"
#define LOWER_THREE_QUARTERS_BLOCK               "▆"
#define LOWER_SEVEN_EIGHTHS_BLOCK                "▇"
#define FULL_BLOCK                               "█"

#define LEFT_SEVEN_EIGHTHS_BLOCK                 "▉"
#define LEFT_THREE_QUARTERS_BLOCK                "▊"
#define LEFT_FIVE_EIGHTHS_BLOCK                  "▋"
#define LEFT_HALF_BLOCK                           "▌"
#define LEFT_THREE_EIGHTHS_BLOCK                 "▍"
#define LEFT_ONE_QUARTER_BLOCK                    "▎"
#define LEFT_ONE_EIGHTH_BLOCK                     "▏"

#define RIGHT_HALF_BLOCK                          "▐"

#define LIGHT_SHADE                               "░"
#define MEDIUM_SHADE                              "▒"
#define DARK_SHADE                                "▓"

#define UPPER_ONE_EIGHTH_BLOCK                    "▔"
#define RIGHT_ONE_EIGHTH_BLOCK                    "▕"

#define QUADRANT_LOWER_LEFT                        "▖"
#define QUADRANT_LOWER_RIGHT                       "▗"
#define QUADRANT_UPPER_LEFT                        "▘"
#define QUADRANT_UPPER_LEFT_AND_LOWER_LEFT_AND_LOWER_RIGHT "▙"
#define QUADRANT_UPPER_LEFT_AND_LOWER_RIGHT       "▚"
#define QUADRANT_UPPER_LEFT_AND_UPPER_RIGHT_AND_LOWER_LEFT "▛"
#define QUADRANT_UPPER_LEFT_AND_UPPER_RIGHT_AND_LOWER_RIGHT "▜"
#define QUADRANT_UPPER_RIGHT                       "▝"
#define QUADRANT_UPPER_RIGHT_AND_LOWER_LEFT       "▞"
#define QUADRANT_UPPER_RIGHT_AND_LOWER_LEFT_AND_LOWER_RIGHT "▟"

#endif // ASCII_CODES_H
