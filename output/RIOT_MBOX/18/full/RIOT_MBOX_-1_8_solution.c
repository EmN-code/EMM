#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <evtimer.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <MQTTClient.h>

#define MBOX_SIZE           (8U)
#define MSG_SEND_INTERVAL   (5U * US_PER_SEC)

static msg_t mbox_queue[MBOX_SIZE];
static mbox_t my_mbox;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    while (1) {
        mbox_send(&my_mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, MSG_SEND_INTERVAL);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_receive(&my_mbox, &msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    mbox_init(&my_mbox, mbox_queue, MBOX_SIZE);
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    return 0;
}