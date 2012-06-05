/* 
 * File:   ipc_messages.h
 * Author: caos
 *
 * Created on May 7, 2012, 8:49 PM
 */

#ifndef IPC_MESSAGES_H
#define	IPC_MESSAGES_H

#include <structures/queue.h>
#include <multitasking.h>
#include <utilities.h>

#define MAX_MESSAGE 100
#define DEFAULT_TRIES 1000

struct message
{
    unsigned int ID;
    unsigned int sender_id;
    int reply_id;
    byte payload[MAX_MESSAGE];
};

/*
 * This sends a message to dest PID, it will be *copied* to ensure message availability
 * until the end of the message.
 * 
 * reply_to = 0, none
 * 
 * It returns the ID of the message.
 * 
 * TODO: reimplement using semaphores!
 */
int asyn_send(unsigned int dest, struct message* msg, unsigned int reply_to);

/*
 * req_id=-1 first in queue
 * TODO: reimplement using semaphores!
 */
struct message* asyn_receive(int req_id);

#endif	/* IPC_MESSAGES_H */

