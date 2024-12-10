
#include "esp_system.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "unistd.h"
#include "stdatomic.h"

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1234;

    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_usleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char sender_stack[STACK_SIZE];
    char receiver_stack[STACK_SIZE];

    thread_create(sender_stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(receiver_stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    return 0;
}
