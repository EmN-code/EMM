#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sched.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mqtt.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN  (GNRC_NETIF_PRIO - 1)

char sender_thread_stack[THREAD_STACKSIZE];
char receiver_thread_stack[THREAD_STACKSIZE];

static kernel_pid_t receiver_pid;

/* Sender Thread Function */
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return NULL;
}

/* Receiver Thread Function */
void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(&msg, 8);

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    /* Create receiver thread */
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    /* Create sender thread */
    thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}