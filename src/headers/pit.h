#ifndef PIT_H
#define PIT_H

//PIC datas
#define FREQ 1193180 //the pic's clock frequecy in hertz
#define CHAN0 0x40  //the first channel..
#define CMDCHAN 0x43

//Timer IST handler
void init_timer(unsigned int frequency);

#endif // PIT_H
