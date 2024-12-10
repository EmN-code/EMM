
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "dht_params.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "MQTTClient.h"

#define MSG_QUEUE_SIZE 8
#define PERIODIC_INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t periodic_pid;

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

void periodic_thread(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        periodic_callback();
        ztimer_sleep(ZTIMER_USEC, PERIODIC_INTERVAL);
    }
}

int main(void)
{
    periodic_pid = thread_create(
        msg_queue, sizeof(msg_queue),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        periodic_thread,
        NULL, "periodic_thread"
    );

    if (periodic_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create periodic thread\n");
        return 1;
    }

    printf("Periodic thread started with PID %d\n", periodic_pid);

    return 0;
}
