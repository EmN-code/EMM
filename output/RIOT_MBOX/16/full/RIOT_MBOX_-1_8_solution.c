/* Include necessary RIOT and ESP32-specific headers in accordance with alphabetical order */

#include "xtimer.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "esp_system.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#include "mbox.h"
#include "evtimer_mbox.h"
#include "message.h"

#define STACKSIZE       (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACKSIZE];
static kernel_pid_t thread_pid;

static void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        mbox_post(&msg, &mbox, 5);
        xtimer_sleep(2);
    }
    return NULL;
}

static void msg_receiver(void) {
    msg_t msg;
    mbox_get(&msg, &mbox);
    printf("Received value: %lu\n", msg.content.value);
}

int main(void) {
    /* Initialize a message box with pre-allocated message queue */
    msg_t msg_queue[8];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, 8);

    /* Start the sender thread */
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN 
                              - 1, 0, msg_sender, NULL, "msg_sender");

    /* Continuously receive messages in the main thread */
    while (1) {
        msg_receiver();
    }

    /* Clean the compiled objects */
    return 0;
}