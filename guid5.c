#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
// #include <stdio.h>
// #include <conio.h>
// #include <graphics.h>


#define bgColor BLACK
#define fgColor GREEN

int main(void)
{
    
    // char *str = "TEAM D";
    int value = 1;
    rectangle rect1 = {90,100,40,180};
    // rectangle line1 = {10,250,30,31};

    init_lcd();
    set_orientation(South);
    
    while (1)
    {
        init_screen();
        display.x = 110;
        display.y = 20;
        build_empty_box(0,237,0,317,TURQUOISE,2);
        build_battery(180,215,280,297, WHITE);
        write_string("TEAM D", WHITE, 110, 10);
        write_string("BB_V", BLUE2, 40 ,40);
        write_string("BB_C", BLUE2, 90, 40);
        build_empty_box(10,150,33,150,WHITE,1);

        // fill_rectangle(rect1, WHITE);
        // fill_rectangle(rect1, WHITE);
        // build_box (20 ,120 , 40 , 180, GREEN, 0, 0);
        // fill_rectangle(rect1, WHITE);
        // build_box(90 ,62 , 40 , 180, WHITE, 0, 0);
        // write_string("TRY", GREEN, 50, 200);
    }
}

void init_screen(){
    display.background = bgColor;
    display.foreground = fgColor;
}

void build_box(int p1, int p2, int p3, int p4 , uint16_t col, int x, int y){
    rectangle rect = {p1,p2,p3,p4};
    display.x = x;
    display.y = y;
    fill_rectangle(rect,col);
}

void write_string(char a[], uint16_t col, int x, int y){
    display.x = x;
    display.y = y;
    display.foreground = col;
    display_string(a);

}

void draw_hor_line(int x1, int x2, int y, int w, uint16_t col){
    rectangle rect = {x1,x2,y,y+w};
    fill_rectangle(rect, col);
}
void draw_vert_line(int y1, int y2, int x, int w, uint16_t col){
    rectangle rect = {x,x+w,y1,y2};
    fill_rectangle(rect, col);
}

void build_empty_box(int x1, int x2, int y1, int y2, uint16_t col, int w){
    draw_vert_line(y1,y2,x1,w,col);
    draw_vert_line(y1,y2+2,x2,w,col);
    draw_hor_line(x1,x2,y1,w,col);
    draw_hor_line(x1,x2+2,y2,w,col);
}

void build_battery(int x1,int x2, int y1, int y2, uint16_t col){
    draw_vert_line(y1,y2,x1,2,col);
    draw_vert_line(y1,y2,x2,2,col);
    draw_hor_line(x1,x2,y1,2,col);
    draw_hor_line(x1,x2,y2,2,col);
    rectangle r1 = {x2,x2+5,y1+4,y2-4};
    fill_rectangle(r1, WHITE);
}

void leaf(int x1, int x2, int y1, int y2){
    uint16_t x;
    for(x=0; x<=15; x++)
    {
    build_box()
    }
}
