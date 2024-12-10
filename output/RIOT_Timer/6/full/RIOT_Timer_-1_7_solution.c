#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"
#include "stdatomic.h"
#include "unistd.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "sys/socket.h"

#define MSG_QUEUE_SIZE     (8)
#define TIMER_INTERVAL_SEC (1)

// Global message queue
static msg_t _msg_queue[MSG_QUEUE_SIZE];

void *_timer_thread(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_t timer;

    msg.type = EVTIMER_MSG_TYPE_PERIODIC;
    evtimer_msg_init(&evqueue_ptr, EVT_MSG_TYPE_PERIODIC, &_msg_queue, MSG_QUEUE_SIZE);
    evtimer_set(&timer, TIMER_INTERVAL_SEC, &evqueue_ptr);
    evtimer_start(&timer, &msg);

    while (1) {
        msg_receive(&msg);
        // Simulate LED toggle without using gpio APIs, just a placeholder
        printf("LED toggled\n");
    }
    return NULL;
}

int main(void) {
    puts("RIOT Timer with Network Retry Example");

    // Initialize message queue
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    // Create and start timer thread
    pthread_t timer_pid;
    if (pthread_create(&timer_pid, NULL, _timer_thread, NULL) != 0) {
        printf("Error creating timer thread\n");
        return 1;
    }

    // Simulate network connections with retry logic
    int rc;
    while (1) {
        rc = mock_network_connect();
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            sleep(1);
            continue;
        }
        printf("Network connected successfully\n");
        break;
    }

    pthread_join(timer_pid, NULL);
    return 0;
}

int mock_network_connect() {
    // Mock function to simulate network connection, randomly fails
    return rand() % 2;
}