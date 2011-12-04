#ifndef STDOUT_H_INCLUDED
#define STDOUT_H_INCLUDED

#define COLUMNS 80
#define LINES 25
#define PAGES 8

//Should be self-explanatory
void init_video();
void refresh_cursor();
void clear();
void putc(unsigned char ch);
int puts(const char* str);
int putd(int number, int base);
void scroll_page();
void set_color(unsigned char backcolor, unsigned char forecolor);
void get_colors(unsigned char* backcolor, unsigned char* forecolor);

//very limited kernel's printf
int kprintf(char* arg, ...);
#endif // STDOUT_H_INCLUDED
