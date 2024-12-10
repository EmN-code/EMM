#include "net/nanocoap/cache.h"
#include "hashes/sha2xx_common.h"
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH     (THREAD_PRIORITY_MAIN - 2)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

static void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) >= 0) {
            printf("High priority thread received: %ld\n", msg.content.value);
        }
    }

    return NULL;
}

static void *main_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t high_priority_pid;

    high_priority_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_HIGH, 0,
                                      high_priority_thread, NULL, "high_priority");

    for (unsigned long i = 0; i < 10; ++i) {
        msg.content.value = i;
        printf("Main thread sending: %ld\n", msg.content.value);
        msg_send(&msg, high_priority_pid);
    }

    return NULL;
}

int main(void) {
    msg_init_queue((msg_t [8]){}, 8);

    thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN, 0,
                  main_thread, NULL, "main");

    return 0;
}