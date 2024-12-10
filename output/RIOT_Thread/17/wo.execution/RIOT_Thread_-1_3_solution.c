#include <ztimer.h>
#include <timex.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>
#include <event/thread.h>

#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN)
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_MAIN)

static char receiver_stack[THREAD_STACKSIZE_MAIN];
static char sender_stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 4);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary value
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sent message with value: %d\n", msg.content.value);
        ztimer_sleep(ZTIMER_SECOND, 2);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[4];
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 2, 0, sender_thread, NULL, "sender");
    
    while (1) {
        thread_yield();
    }
    return 0;
}