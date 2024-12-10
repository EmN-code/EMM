#include "architecture.h"
#include "arpa/inet.h"
#include "atomic.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "ztimer.h"

/** Description of the solution:
 *  This program sets up a message box for an ESP32 using RIOT OS and sends events at certain intervals.
 *  The solution initializes a message queue, fills it with messages, and sends them through a message box using
 *  event timers.
 */

#define MBOX_SIZE 4

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[MBOX_SIZE];
static mbox_t mbox;

static void* sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // some arbitrary value
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Message received: %u\n", msg.content.value);
        }
        printf("Sending message...\n");
        mbox_send(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000); // sleep for 1 second
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, MBOX_SIZE);
    mbox_init(&mbox, queue, MBOX_SIZE);
    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                             THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    (void)sender_pid;

    printf("Message box setup complete.");

    while(1) {
        // main loop can be used to process other tasks
    }

    return 0;
}