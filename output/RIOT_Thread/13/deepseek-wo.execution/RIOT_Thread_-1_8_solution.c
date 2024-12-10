
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <inttypes.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 16

static char stack_low_prio[STACKSIZE];
static char stack_high_prio[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *low_prio_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

void *high_prio_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                              THREAD_PRIORITY_MAIN - 1, 0, low_prio_thread, NULL, "low_prio");

    kernel_pid_t high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                               THREAD_PRIORITY_MAIN - 2, 0, high_prio_thread, NULL, "high_prio");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 1234;

    msg_send(&msg, low_prio_pid);
    msg_send(&msg, high_prio_pid);

    return 0;
}
