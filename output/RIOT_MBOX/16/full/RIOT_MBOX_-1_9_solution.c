#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "MQTTClient.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "thread.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "time_units.h"
#include "timex.h"
#include "ztimer.h"
#include "xtimer.h"
#include "evtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "stdio.h"
#include "unistd.h"

void *thread_handler(void *arg) {
    msg_t msg;
    mbox_t mbox;
    msg_t queue[4];
    mbox_init(&mbox, queue, 4);

    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            // Process received message
            printf("Received message of type: %u\n", msg.type);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_handler, NULL, "receiver");
    
    evtimer_t evtimer;
    evtimer_mbox_event_t evt;

    evtimer_init_mbox(&evtimer);
    evtimer_add_mbox(&evtimer, &evt, &mbox);

    while (1) {
        msg_t msg;
        msg.type = 0x1234;
        evtimer_set_msg(&evtimer, &msg);
        xtimer_sleep(5);
    }

    return 0;
}