
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "msg.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "MQTTClient.h"

#define STACK_SIZE 1024
#define NUM_MESSAGES 8

static char stack_sender[STACK_SIZE];
static char stack_receiver[STACK_SIZE];

static msg_t msg_array[NUM_MESSAGES];

void *sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 12345;

    while (1) {
        printf("Sender: Sending message with value %lu\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %lu\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_array, NUM_MESSAGES);

    pthread_t sender, receiver;

    pthread_create(&sender, NULL, sender_thread, NULL);
    pthread_create(&receiver, NULL, receiver_thread, NULL);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
