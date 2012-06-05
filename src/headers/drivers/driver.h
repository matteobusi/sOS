/* 
 * File:   driver.h
 * Author: caos
 *
 * That's the interface for drivers in SoS, the first step to a *real* hybrid kernel
 * 
 * A driver in order to be considered as such must register using this interface and provided system calls
 * 
 * When the specified interrupt will be fired the kernel will send a command "PROCESS_INT" to the driver waiting
 * for a response synchronously (if driver doesn't reply in DEFAULT_TRIES the request will be discharged)
 * 
 */

#ifndef DRIVER_H
#define	DRIVER_H

#include <utilities.h>

#define MAX_DRIVERS 256

#define ACK_INT "ACK_INTERRUPT"

struct task* drivers[MAX_DRIVERS];

unsigned int install_driver(void* mem_pos, char* name);
void drv_reg_handler(char* name, unsigned char int_no);
struct task* get_driver_int(unsigned char int_no);
void send_irq_ack(unsigned int pid);
void set_loaded();
int get_loaded();

#endif	/* DRIVER_H */

