#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "thread.h"
#include "timex.h"
#include "mutex.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_system.h"
#include "kernel_defines.h"
#include "stdio.h"
#include "time_units.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)

static char main_stack[THREAD_STACKSIZE_MAIN];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static void *main_thread(void *arg) {
    (void)arg;
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    printf("RIOT application to send messages using mbox on ESP32\n");

    mbox_t mbox;
    msg_t mbox_buffer[4];
    mbox_init(&mbox, mbox_buffer, 4);

    // Simulate event sending at intervals
    while (1) {
        msg_t msg;
        msg.content.value = rand() % 100;
        mbox_send(&mbox, &msg);
        printf("Message sent: %d\n", msg.content.value);
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t pid = thread_create(main_stack, sizeof(main_stack), THREAD_PRIORITY_MAIN - 1, 0,
                                      main_thread, NULL, "main");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error creating main thread\n");
        return 1;
    }
    return 0;
}