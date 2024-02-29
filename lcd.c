/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 */
 
#include "ili934x.h"
#include "font.h"
#include "lcd.h"
#include <stdint.h>

lcd display = {LCDWIDTH, LCDHEIGHT, North, 0, 0, WHITE, BLACK};

typedef struct {
    int16_t x;
    int16_t y;
} point;

void init_lcd()
{
	/* Disable JTAG in software, so that it does not interfere with Port C  */
	/* It will be re-enabled after a power cycle if the JTAGEN fuse is set. */
	MCUCR |= (1<<JTD);
	MCUCR |= (1<<JTD);
	
	/* Configure ports */
	CTRL_DDR = 0x7F;
	DATA_DDR = 0xFF;
	
	init_display_controller();
}

void set_orientation(orientation o)
{
	display.orient = o;
	write_cmd(MEMORY_ACCESS_CONTROL);
	if (o==North) { 
		display.width = LCDWIDTH;
		display.height = LCDHEIGHT;
		write_data(0x48);
	}
	else if (o==West) {
		display.width = LCDHEIGHT;
		display.height = LCDWIDTH;
		write_data(0xE8);
	}
	else if (o==South) {
		display.width = LCDWIDTH;
		display.height = LCDHEIGHT;
		write_data(0x88);
	}
	else if (o==East) {
		display.width = LCDHEIGHT;
		display.height = LCDWIDTH;
		write_data(0x28);
	}
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(0);
	write_data16(display.width-1);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(0);
	write_data16(display.height-1);
}

void fill_rectangle(rectangle r, uint16_t col)
{
	uint16_t x, y;
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(r.left);
	write_data16(r.right);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(r.top);
	write_data16(r.bottom);
	write_cmd(MEMORY_WRITE);
	for(x=r.left; x<=r.right; x++)
		for(y=r.top; y<=r.bottom; y++)
			write_data16(col);
}

void fill_rectangle_indexed(rectangle r, uint16_t* col)
{
	uint16_t x, y;
	write_cmd(COLUMN_ADDRESS_SET);
	write_data16(r.left);
	write_data16(r.right);
	write_cmd(PAGE_ADDRESS_SET);
	write_data16(r.top);
	write_data16(r.bottom);
	write_cmd(MEMORY_WRITE);
	for(x=r.left; x<=r.right; x++)
		for(y=r.top; y<=r.bottom; y++)
			write_data16(*col++);
}


// Function to draw a single pixel
// void draw_pixel(uint16_t x, uint16_t y, uint16_t col)
// {
//     // Call the graphics library function to set the pixel color
//     set_pixel(x, y, col);
// }

// void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t col)
// {
//     int16_t dx = abs(x1 - x0);
//     int16_t dy = abs(y1 - y0);
//     int16_t sx = x0 < x1 ? 1 : -1;
//     int16_t sy = y0 < y1 ? 1 : -1;
//     int16_t err = dx - dy;

//     while (x0 != x1 || y0 != y1)
//     {
//         draw_pixel(x0, y0, col);
//         int16_t e2 = 2 * err;
//         if (e2 > -dy)
//         {
//             err -= dy;
//             x0 += sx;
//         }
//         if (e2 < dx)
//         {
//             err += dx;
//             y0 += sy;
//         }
//     }
//     draw_pixel(x0, y0, col); // Ensure the last pixel is drawn
// }

// void fill_circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t col)
// {
//     int16_t x = radius;
//     int16_t y = 0;
//     int16_t err = 0;

//     while (x >= y)
//     {
//         // Draw lines from center to the eight octants
//         draw_line(x0 + x, y0 + y, x0 - x, y0 + y, col);
//         draw_line(x0 + y, y0 + x, x0 - y, y0 + x, col);
//         draw_line(x0 - x, y0 - y, x0 + x, y0 - y, col);
//         draw_line(x0 - y, y0 - x, x0 + y, y0 - x, col);

//         y++;

//         // Mid-point is inside or on the perimeter
//         if (err <= 0)
//         {
//             err += 2*y + 1;
//         }
//         // Mid-point is outside the perimeter
//         if (err > 0)
//         {
//             x--;
//             err -= 2*x + 1;
//         }
//     }
// }

void clear_screen()
{
	display.x = 0;
	display.y = 0;
	rectangle r = {0, display.width-1, 0, display.height-1};
	fill_rectangle(r, display.background);
}

//  void display_char(char c)
// {
// 	uint16_t x, y;
// 	PGM_P fdata; 
// 	uint8_t bits, mask
// 	uint16_t sc=display.x, ec=display.x + 4, sp=display.y, ep=display.y + 7;
// 	if (c < 32 || c > 126) return;
// 	fdata = (c - ' ')*5 + font5x7;
// 	write_cmd(PAGE_ADDRESS_SET);
// 	write_data16(sp);
// 	write_data16(ep);
// 	for(x=sc; x<=ec; x++) {
// 		write_cmd(COLUMN_ADDRESS_SET);
// 		write_data16(x);
// 		write_data16(x);
// 		write_cmd(MEMORY_WRITE);
// 		bits = pgm_read_byte(fdata++);
// 		for(y=sp, mask=0x01; y<=ep; y++, mask<<=1)
// 			write_data16((bits & mask) ? display.foreground : display.background);
// 	}
// 	write_cmd(COLUMN_ADDRESS_SET);
// 	write_data16(x);
// 	write_data16(x);
// 	write_cmd(MEMORY_WRITE);
// 	for(y=sp; y<=ep; y++)
// 		write_data16(display.background);

// 	display.x += 6;
// 	if (display.x >= display.width) { display.x=0; display.y+=8; }
// }

void display_char(char c)
{
    uint16_t x, y;
    PGM_P fdata;
    uint8_t bits, mask;
    uint16_t sc = display.x, ec = display.x + 19, sp = display.y, ep = display.y + 15; // Adjust for 20x16 character size
    if (c < 32 || c > 126)
        return;
    fdata = (c - ' ') * 5 + font5x7; // Assuming font5x7 is your font data
    write_cmd(PAGE_ADDRESS_SET);
    write_data16(sp);
    write_data16(ep);
    for (y = sp; y <= ep; y++) {
        write_cmd(COLUMN_ADDRESS_SET);
        write_data16(sc);
        write_data16(ec);
        write_cmd(MEMORY_WRITE);
        for (x = sc; x <= ec; x++) {
            bits = pgm_read_byte(fdata++);
            mask = 0x01;
            for (uint8_t i = 0; i < 4; i++) { // Display each column 4 times to increase width
                write_data16((bits & mask) ? display.foreground : display.background);
                write_data16((bits & mask) ? display.foreground : display.background);
                mask <<= 1;
            }
        }
    }
    display.x += 20; // Increase character width by 4 times
    if (display.x >= display.width) {
        display.x = 0;
        display.y += 16; // Increase character height by 1 row
    }
}

void display_string(char *str)
{
	uint8_t i;
	for(i=0; str[i]; i++) 
		display_char(str[i]);
}

