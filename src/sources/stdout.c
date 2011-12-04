#include <stdout.h>
#include <stdarg.h>
#include <system.h>

unsigned short attrib = 0x07;
unsigned short* videomem;

//cursor's position
int x, y, page;
int initialized=0;

void init_video()
{
   videomem = (unsigned short*) 0xb8000;
   initialized=1;
   x = y=page=0;
}

void refresh_cursor()
{
    int tmp = COLUMNS*y + x;
    outportb(0x3D4, 14);
    outportb(0x3D5, tmp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, tmp);
}

void set_color(unsigned char backcolor, unsigned char forecolor)
{
    attrib = (backcolor << 4) | forecolor;
}

void get_colors(unsigned char* backcolor, unsigned char* forecolor)
{
    *backcolor = (attrib>>4)&0xFF;
    *forecolor = ((attrib) - ((*backcolor)<<4))&0xFF;
}

void scroll_page(int n_LINES)
{
    if(y >= LINES)
    {
       unsigned short blank = ' ' | (attrib<<8);
       int i;
       for(i=0; i < (LINES - n_LINES)*COLUMNS; i++)
        videomem[i] = videomem[i+COLUMNS*n_LINES];
        
       for (i = 24*80; i < 25*80; i++)
           videomem[i] = blank;
        y -= n_LINES;
    }
}

void putc(unsigned char ch)
{
    if(!initialized)
        init_video();
    if(ch == '\b')
    {
        if(x <=0)
        {
            x=COLUMNS;
            y--;
        }
        x--;
        putc(' ');
         if(x <=0)
        {
            x=COLUMNS;
            y--;
        }
        x--;
    }
    else if(ch=='\n')
    {
        x=0;
        y++;
    }
    else if(ch == '\0')
    {
    }
    else
    {
        videomem[x+y*COLUMNS] = ch | (attrib<<8);
        x++;
        if(x >= COLUMNS)
        {
           x=0;
           y++;
        }
    }
    scroll_page(1);
    refresh_cursor();
}

void clear()
{
    int i,j;
    x=y=0;
    unsigned short blank = ' ';
    for(i=0; i < LINES; i++)
        for(j=0; j<COLUMNS; j++)
            putc(blank);
    x=y=0;
    refresh_cursor();
}


int puts(const char* str)
{
    while(*str != '\0')
        putc(*str++);
    return strlen(str);
}

int putu(unsigned int passed, int base)
{
    unsigned int number = passed;
    static char* digits = "0123456789ABCDEF";
    char tmp[12];
    int i=0;
    do
    {
        tmp[i++] = digits[number%base];
    }while((number /= base) >0);

    tmp[i++] = '\0';

    strrev(tmp);
    return puts(tmp);
}

int putd(int number, int base)
{
    static char* digits = "0123456789ABCDEF";
    char tmp[12];
    int i=0, negative=0;
    if(number < 0)
    {
        number = -number;
        negative = 1;
    }
    do
    {
        tmp[i++] = digits[number%base];
    }while((number /= base) >0);

    if(negative)
        tmp[i++] = '-';

    tmp[i++] = '\0';

    strrev(tmp);
    return puts(tmp);
}

int kprintf(char* arg, ...)
{
    va_list argp;
    va_start(argp, arg);
    char* s;
    int d;
    char c;
    int sum=0;

    while(*arg != '\0')
    {
        //if format specified prints it
        if(*arg == '%')
        {
            ++arg;
            switch(*arg)
            {
                case 's':
                    s=va_arg(argp, char*);
                    sum+= puts(s);
                break;

                case 'd':
                    d=va_arg(argp, signed int);
                    sum+=putd(d, 10);
                break;

                case 'u':
                    d=va_arg(argp, unsigned int);
                    sum+=putu(d, 10);
                break;

                case 'h':
                    d=va_arg(argp, unsigned int);
                    sum+=putu(d, 16);
                break;

                case 'b':
                    d=va_arg(argp, signed int);
                    sum+=putd(d, 2);
                break;

                case 'c':
                    c=va_arg(argp, char);
                    putc(c);
                    sum++;
                break;

                default:
                    putc('%');
                    putc(*arg);
                    sum +=2;
            }
        }
        else
        {
            putc(*arg);
            sum++;
        }
        arg++;
    }
    va_end(arguments);
    return sum;
}