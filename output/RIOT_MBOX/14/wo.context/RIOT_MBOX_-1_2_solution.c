#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: type=%d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Application: Starting message box example");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN-1, THREAD_CREATE_STACKTEST, thread_handler, NULL, "receiver");

    msg_t msg = { .type = 1, .content.value = 42 };
    while (1) {
        msg_send(&msg, thread_handler);
        xtimer_usleep(1000000);
    }

    return 0;
}