#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "evtimer.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define THREAD_STACKSIZE_DEFAULT (1024 * 6)
#define THREAD_PRIORITY_MAIN    (1)
#define QUEUE_SIZE    (8)

static msg_t queue[QUEUE_SIZE];
static msg_t mbox_queue[QUEUE_SIZE];

static char stack[THREAD_STACKSIZE_DEFAULT];

static int message_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            // handle the message
            printf("Received message with type: %d\n", msg.type);
        }
    }
    return 0;
}

int main(void) {
    printf("Starting RIOT message box example\n");

    // Initialize the message queue
    msg_init_queue(queue, QUEUE_SIZE);

    // Initialize the mailbox
    mbox_t mbox;
    mbox_init(&mbox, mbox_queue, QUEUE_SIZE);

    // Create a thread to handle incoming messages
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack),
                                            THREAD_PRIORITY_MAIN, 0,
                                            message_handler, NULL, "receiver");

    // Send messages periodically
    for (unsigned i = 0; i < 10; ++i) {
        msg_t msg;
        msg.type = i;
        mbox_post(&mbox, &msg, 0);
        printf("Sent message with type: %d\n", msg.type);
        xtimer_sleep(1);
    }

    return 0;
}
