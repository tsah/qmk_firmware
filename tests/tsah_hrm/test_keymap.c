#include "quantum.h"
// Left half = cols 0-4, right half = cols 5-9, row 3 = thumbs ('*').
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    {'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R'},
    {'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R'},
    {'L', 'L', 'L', 'L', 'L', 'R', 'R', 'R', 'R', 'R'},
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
};
#include "../../users/tsah/tsah.c"
