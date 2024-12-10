#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define PRIO_SENDER       (THREAD_PRIORITY_MAIN - 1)
#define PRIO_RECEIVER     (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE         (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE    (8)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    
    while (1) {
        msg.content.value++;
        msg_send(&msg, thread_get(PRIO_RECEIVER));
        printf("[Sender] Sent message with value: %u\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    printf("Starting message passing example on RIOT OS on ESP32\n");

    thread_create(sender_stack, sizeof(sender_stack),
                  PRIO_SENDER, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack),
                  PRIO_RECEIVER, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");

    return 0;
}