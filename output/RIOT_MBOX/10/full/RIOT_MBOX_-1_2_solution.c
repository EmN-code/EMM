#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL            (1000000)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x100;
    msg.content.value = 42;
    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
        puts("Sending message...");
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t sender_pid = thread_create(stack,
                                            sizeof(stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            0,
                                            msg_sender,
                                            NULL,
                                            "msg_sender");
    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread");
    }

    msg_t msg, reply;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message of type: %