#include <ztimer.h>
#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define INTERVAL (1U * ZTIMER_MSEC)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char sender_thread_stack[STACK_SIZE];
static kernel_pid_t sender_thread_pid;
static msg_t msg_queue[4];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        msg_send(&msg, thread_getpid());
        printf("Message sent from sender thread\n");
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 4);
    sender_thread_pid = thread_create(sender_thread_stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    printf("Sender thread PID: %d\n", sender_thread_pid);
    return 0;
}