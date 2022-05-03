#include <tice.h>
//#include <fontlibc.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>
#include <cstring>
#include <cstdlib>
//#include "melgebra/melgebra.hpp"

uint8_t lrowmax = 14;
uint8_t lcolmax = 25;
uint8_t srowmax = 19;
uint8_t scolmax = 40;
struct curpos {
    uint8_t row = 0;
    uint8_t col = 0;
};
curpos largepos = {0, 0};
curpos smallpos = {0, 0};

enum stateenum {
    none,
    second,
    alphaupper,
    alphalower
};

stateenum calcstate = stateenum::none;
bool alock = false;

void init_terminal() {
    boot_ClearVRAM();
    os_FontSelect(reinterpret_cast<font_t *>(1));
    largepos = {0, 0};
    smallpos = {0, 0};
}

void draw_at_pos(const char *string, curpos p, bool large = true) {
    os_FontDrawText(string, large ? 2 + 12 * p.col : 10 * p.col + 1, large ? 2 + 16 * p.row : 12 * p.row);
}

void draw_text(const char *string, bool large = true) {
    // draw text using OS routines at the cursor position
    // WARNING: this routine assumes text is monospace but small text isn't soooooooooooooo
    curpos *posp;
    uint8_t colmax;
    if (large) {
        os_FontSelect(reinterpret_cast<font_t *>(1));
        posp = &largepos;
        colmax = lcolmax;
    } else {
        os_FontSelect(reinterpret_cast<font_t *>(0));
        posp = &smallpos;
        colmax = scolmax;
    }
    curpos *pos = posp;
    curpos origpos = *pos;
    size_t len = strlen(string) + 1;
    // assign area to do stuff with extra space for auto-newlines i think
    char *line = static_cast<char *>(malloc(colmax + 2));
    // continually add newlines where needed
    size_t orig_index, new_index;
    for (orig_index = 0, new_index = 0; orig_index < len; ++orig_index, ++new_index) {
        if (string[orig_index] == '\n' || string[orig_index] == '\0' || (*pos).col > colmax) { // newline
            line[new_index] = '\0'; // terminate
            draw_at_pos(line, origpos, large); // draw
            if (string[orig_index] == '\n') { // don't draw nelines
                ++orig_index;
                if (string[orig_index] == '\0') { // terminating with \n is a special case
                    (*pos).col = 0;
                    (*pos).row++;
                    return;
                }
            }
            if (string[orig_index] == '\0') { // return if terminated
                return;
            }
            // reset
            (*pos).col = 0;
            (*pos).row++;
            origpos = (*pos);
            new_index = 0;
        }
        line[new_index] = string[orig_index];
        (*pos).col++;
    }
}

void init_status_bar() {
    // set top to grey
    auto *p = (uint16_t *) gfx_vram;
    for (size_t i = 0; i < LCD_WIDTH * 2 * 17 / sizeof(*p); i++) {
        p[i] = 0x52AA;
    }
}

void printBits(size_t const size, void const *const ptr) {
    auto *b = (unsigned char *) ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            dbg_sprintf(dbgout, "%u", byte);
        }
    }
    dbg_sprintf(dbgout, "\r");
}

int main() {
    init_terminal();
    init_status_bar();
//    os_SetDrawFGColor(0xffff);
//    os_SetDrawBGColor(0x52AA);
//    draw_text("AMONGUS AARGH   A", false);
//    os_SetDrawBGColor(0xffff);
//    os_SetDrawFGColor(0x0);
    largepos = {1, 0};
    draw_text("some kind of generic text with spaces hehehaha grrr");
    draw_text("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    kb_EnableOnLatch();
    uint8_t kbprevstate[7];
    uint8_t risingedge[7];
    do {
        kb_Scan();
        for (uint8_t i = 0; i < 7; ++i) {
            auto rowtemp = kb_Data[i + 1];
            risingedge[i] = rowtemp & (~kbprevstate[i]);
            kbprevstate[i] = rowtemp;
        }
//        printBits(sizeof(uint8_t) * 7, risingedge);
    } while (!kb_On);
    kb_ClearOnLatch();
    kb_DisableOnLatch();
//    sleep(5);
    return 0;
}