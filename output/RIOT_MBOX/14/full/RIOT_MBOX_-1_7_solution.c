#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>

#define MBOX_SIZE 8
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)

static msg_t msg_queue[MBOX_SIZE];
static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

static mbox_t mbox;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0xABC;
    uint32_t counter = 0;
    
    while (1) {
        printf("Sender: Putting message with value %lu\n", (unsigned long)counter);
        mbox_post(&mbox, &msg, counter, 0); // corrected number of arguments
        printf("Sender: Message with value %lu put\n", (unsigned long)counter);
        counter++;
        xtimer_sleep(3);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    uint32_t value;

    while (1) {
        mbox_get(&mbox, &msg, &value);
        printf("Receiver: Got message with value %lu\n", (unsigned long)value); // correct format specifier
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, MBOX_SIZE);
    thread_create(receiver_stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    return 0;
}