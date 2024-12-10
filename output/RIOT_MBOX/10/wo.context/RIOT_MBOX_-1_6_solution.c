#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)
#define THREAD_CREATE_STACKTEST (THREAD_CREATE_STACKTEST)

static char _stack[THREAD_STACKSIZE];

void *thread_handler(void *arg) {
    (void) arg;
    msg_t msg;
    while(1) {
        xtimer_sleep(1);
        msg.content.value = random_uint32();
        printf("Sending msg with value: %lu\n", msg.content.value);  // Use %lu for uint32_t
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received msg with value: %lu\n", msg.content.value);
    }

    return 0;
}

static uint32_t random_uint32(void) {
    return (uint32_t)rand();
}