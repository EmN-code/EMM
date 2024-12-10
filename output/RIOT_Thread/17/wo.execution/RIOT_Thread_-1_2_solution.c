#include <thread.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_msg.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_SENDER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    for (uint32_t i = 0; i < 5; i++) {
        msg.content.value = i;
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            THREAD_PRIORITY_SENDER, 0,
                                            sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_RECEIVER, 0,
                                              receiver, NULL, "receiver");
    
    return 0;
}