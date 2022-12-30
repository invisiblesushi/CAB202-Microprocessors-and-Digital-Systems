#include <avr/io.h>
#include <util/delay.h>
#include "graphics.h"
#include "cpu_speed.h"
#include "sprite.h"

//wirte the main file
int main(void) {

    set_clock_speed(CPU_8MHz);
    lcd_init(LCD_DEFAULT_CONTRAST);
    clear_screen();

    unsigned char bitmap[8] = {

      0b11111111, 0b10000001,
      0b10000001, 0b10000001,
      0b10000001, 0b10000001,
      0b10000001, 0b11111111

    };

    draw_line(83,1,83,48);
    draw_string(5,17,"Daniel Chen");
    draw_string(5,25,"N9879072");

    Sprite box;
    init_sprite(&box, 1, 1, 8, 8, bitmap);
    draw_sprite(&box);
    show_screen();


	return 0;
}
