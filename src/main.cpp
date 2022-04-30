#include <tice.h>
#include <fontlibc.h>
#include <graphx.h>
#include <keypadc.h>
#include <debug.h>
#include "melgebra/melgebra.hpp"

int main() {
    // Start the graphics routines
    gfx_Begin();
    kb_EnableOnLatch();
    fontlib_font_t *oslfont;
    fontlib_font_t *ossfont;
    oslfont = fontlib_GetFontByIndex("OSLFONT", 0);
    if (!oslfont) {
        gfx_PrintStringXY(
                "font not found or invalid\n go to https://www.ticalc.org/archives/files/fileinfo/476/47627.html", 0,
                0);
        return 1;
    }
    ossfont = fontlib_GetFontByIndex("OSsfont", 0);
    if (!ossfont) {
        gfx_PrintStringXY(
                "font not found or invalid\n go to https://www.ticalc.org/archives/files/fileinfo/476/47627.html", 0,
                0);
        return 1;
    }
    /* Use font for whatever */
    fontlib_SetFont(oslfont, static_cast<fontlib_load_options_t>(0));


    fontlib_SetWindow(4, 4, LCD_WIDTH - 8, 13 * 18);
    fontlib_HomeUp();
    fontlib_SetLineSpacing(2, 2);
    fontlib_SetNewlineOptions(FONTLIB_ENABLE_AUTO_WRAP | FONTLIB_AUTO_SCROLL | FONTLIB_PRECLEAR_NEWLINE);
    do {
        kb_Scan();
        if (kb_IsDown(kb_KeyEnter)) {
            fontlib_DrawInt(rand(), 8);
            fontlib_Newline();
//            msleep(100);
        }
//        gfx_SwapDraw();
    } while (!kb_On);

    // End graphics drawing
    gfx_End();
    kb_ClearOnLatch();

    return 0;
}