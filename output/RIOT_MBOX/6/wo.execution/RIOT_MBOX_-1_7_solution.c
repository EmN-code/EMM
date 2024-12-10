#include <msg.h>
#include <stdio.h>
#include <xtimer.h>

#define INTERVAL (1U * US_PER_SEC) // Send interval

static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];
static kernel_pid_t msg_thread_pid;

void *msg_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %ld\n", (long int)msg.content.value);
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    // Create a thread for the message box
    msg_thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, msg_thread, NULL, "msg_thread");

    msg_t msg;
    msg.content.value = 0;
    while (1) {
        xtimer_sleep(INTERVAL);
        msg_send_receive(&msg, NULL, msg_thread_pid);
        msg.content.value++;
    }

    return 0;
}