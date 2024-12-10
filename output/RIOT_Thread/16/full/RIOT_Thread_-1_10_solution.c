#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <inttypes.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT (1024)
#define PRIORITY_HIGH            (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW             (THREAD_PRIORITY_MAIN - 2)
#define SLEEP_DURATION           (1U)

static char high_priority_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char low_priority_thread_stack[THREAD_STACKSIZE_DEFAULT];

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread received message: %