#ifndef STDOUT_H_INCLUDED
#define STDOUT_H_INCLUDED

#include <stdarg.h>
#include <system.h>
#include <ipc/mutex.h>

#include <vesa.h>

#define COLUMNS 80
#define LINES 25

#define PAGES 8

#define TAB_SPACES 4

#define NUM_CHAR '0'
#define STR_CHAR ' '

#define PERM_R 0x1
#define PERM_W 0x2

#define TEXT_MODE 0x0

//Should be self-explanatory
void init_video(unsigned short video_mode, vesa_mode_info_t* v_mode_info);
void refresh_cursor();
void clear();
void putc(unsigned char ch);
int puts(const char* str);
int putd(int number, int base);
int putu(unsigned int number, int base);
void scroll_page();
void set_color(unsigned char backcolor, unsigned char forecolor);
void get_colors(unsigned char* backcolor, unsigned char* forecolor);

//very limited kernel's printf
int kprintf(char* arg, ...);

int get_x_cur();
int get_y_cur();
void set_cur_pos(int _x, int _y);

#endif // STDOUT_H_INCLUDED
