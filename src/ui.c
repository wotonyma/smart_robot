#include "ui.h"
#include "lcd.h"


void show_home(void)
{
	lcd_draw_jpg(0, 0, "../image/home.jpg", NULL, 0, 0);
}
