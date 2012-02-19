#include <interrupts/pit.h>

unsigned int tick=0;

extern stack_t schedule(stack_t context);

void init_timer(unsigned int frequency)
{
    unsigned int div = FREQ / frequency;

    //sends the command to the pit
    outportb(CMDCHAN, 0x36);

    //divides the div in H and L
    unsigned char l = (unsigned char)(div&0xFF);
    unsigned char h = (unsigned char)((div>>8)&0xFF);

    //Sends the divider to CHAN0 of the PIT
    outportb(CHAN0, l);
    outportb(CHAN0, h);
}
