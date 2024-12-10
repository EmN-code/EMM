#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <msg.h>
#include <thread.h>

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_SECONDS    (5)

static kernel_pid_t my_pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *event_generator(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t counter = 0;
    while (1) {
        msg.content.value = counter;
        msg_send(&msg, my_pid);
        printf("Sent message with count: %lu\n", counter);
        counter++;
        xtimer_sleep(INTERVAL_SECONDS);
    }
    return NULL;
}

int main(void) {
    my_pid = thread_getpid();
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    printf("Starting event generator thread...\n");
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_generator, NULL, "event_gen");
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with count: %lu\n", msg.content.value);
    }
    return 0;
}