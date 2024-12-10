#include <xtimer.h>
#include <stdio.h>
#include <thread.h>

#define THREAD_PRIORITY_RECEIVER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER    (THREAD_PRIORITY_MAIN - 2)

#define SENDER_STACKSIZE          (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

static msg_t queue[8];
static msg_t msg;

static kernel_pid_t receiver_pid;

void *receiver(void *arg) {
    (void)arg;
    printf("Receiver thread started\n");
    msg_init_queue(queue, 8);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    printf("Sender thread started\n");
    xtimer_sleep(2);
    msg_t msg;
    msg.content.value = 42;
    printf("Sending message with value: %" PRIu32 "\n", msg.content.value);
    msg_send(&msg, receiver_pid);
    return NULL;
}

int main(void) {
    puts("RIOT ESP32 Thread Communication Example\n");
    receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE,
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver, NULL, "receiver");
    thread_create(sender_stack, SENDER_STACKSIZE,
                  THREAD_PRIORITY_SENDER, 0,
                  sender, NULL, "sender");
    return 0;
}