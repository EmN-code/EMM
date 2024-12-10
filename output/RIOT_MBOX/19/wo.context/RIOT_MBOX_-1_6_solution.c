
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE_THREAD (THREAD_STACKSIZE_MAIN)
#define INTERVAL (1000000U)  // 1 second interval

static char sender_thread_stack[STACKSIZE_THREAD];

static void *sender_thread(void *arg) {
    msg_t msg;
    msg.content.value = 42;  // Example message content value
    while (1) {
        msg_send(&msg, *(kernel_pid_t *)arg);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];
    char stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t my_pid = thread_getpid();
    msg_init_queue(queue, 8);

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  SENDER_PRIO, THREAD_CREATE_STACKTEST,
                  sender_thread, &my_pid, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        /* Handle received message */
    }

    return 0;
}