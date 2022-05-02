#include <tice.h>
//#include <fontlibc.h>
//#include <graphx.h>
//#include <keypadc.h>
#include <debug.h>
#include <cstring>
#include <cstdlib>
//#include "melgebra/melgebra.hpp"

uint8_t rowmax = 14;
uint8_t colmax = 25;
struct curpos {
    uint8_t row = 0; // 0-14
    uint8_t col = 0; // 0-25
};
curpos pos = {0, 0};

void init_terminal() {
    boot_ClearVRAM();
    os_FontSelect(reinterpret_cast<font_t *>(1));
    pos = {0, 0};
}

void draw_at_pos(const char *string, curpos p) {
    os_FontDrawText(string, 2 + 12 * p.col, 2 + 16 * p.row);
}

void draw_text(const char *string) {
    curpos origpos = pos;
    size_t len = strlen(string) + 1;
    // assign area to do stuff with extra space for auto-newlines i think
    char *line = static_cast<char *>(malloc(colmax + 2));
    // continually add newlines where needed
    size_t orig_index, new_index;
    for (orig_index = 0, new_index = 0; orig_index < len; ++orig_index, ++new_index) {
        if (string[orig_index] == '\n' || string[orig_index] == '\0' || pos.col > colmax) { // newline
            line[new_index] = '\0'; // terminate
            draw_at_pos(line, origpos); // draw
            if (string[orig_index] == '\0') {
                return;
            }
            // reset
            pos.col = 0;
            pos.row++;
            origpos = pos;
            new_index = 0;
        }
        line[new_index] = string[orig_index];
        pos.col++;
    }
}

int main() {
    init_terminal();
    draw_text("123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    draw_text("123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    draw_text("123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    sleep(5);
    return 0;
}