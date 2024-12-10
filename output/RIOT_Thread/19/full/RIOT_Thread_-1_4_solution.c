#include <xtimer.h>
#include "msg.h"
#include "thread.h"

#define MESSAGE_QUEUE_SIZE    (8)
#define STACK_SIZE            THREAD_STACKSIZE_MAIN
#define MAIN_PRIORITY         (7)
#define SECONDARY_PRIORITY    (9)

static char main_stack[STACK_SIZE];
static char secondary_stack[STACK_SIZE];

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

static void *main_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    puts("Main thread started.");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread received message: %ld\n", (long)msg.content.value);
    }
    return NULL;
}

static void *secondary_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, thread_get(MAIN_PRIORITY));
        printf("Secondary thread sent message.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_main_thread = thread_create(main_stack, STACK_SIZE, MAIN_PRIORITY, THREAD_CREATE_STACKTEST, main_thread, NULL, "main_thread");

    kernel_pid_t pid_secondary_thread = thread_create(secondary_stack, STACK_SIZE, SECONDARY_PRIORITY, THREAD_CREATE_STACKTEST, secondary_thread, NULL, "secondary_thread");

    if (pid_main_thread <= KERNEL_PID_UNDEF || pid_secondary_thread <= KERNEL_PID_UNDEF) {
        printf("Error in creating thread\n");
        return 1;
    }

    return 0;
}