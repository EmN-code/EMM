#include <ztimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <esp_system.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <msg.h>
#include <mutex.h>
#include <thread.h>

#define THREAD_STACKSIZE_ONE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_TWO (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_ONE (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_TWO (THREAD_PRIORITY_MAIN - 2)

static char stack_one[THREAD_STACKSIZE_ONE];
static char stack_two[THREAD_STACKSIZE_TWO];

static msg_t msg_queue[8]; 

void *thread_one(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    msg_send(&msg, *((kernel_pid_t *)arg));
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Thread Two received message with value: %lu\n", msg.content.value);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one),
                                         THREAD_PRIORITY_ONE, 0,
                                         thread_one, NULL, "thread_one");
    thread_create(stack_two, sizeof(stack_two),
                  THREAD_PRIORITY_TWO, 0,
                  thread_two, &pid_one, "thread_two");
    return 0;
}