
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <esp_system.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <dht_params.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <event/thread.h>
#include <thread.h>
#include <irq.h>

#define MSG_QUEUE_SIZE 10
#define TIMER_INTERVAL (1 * US_PER_SEC)

static atomic_int atomic_var = ATOMIC_VAR_INIT(0);
static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_msg_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, TIMER_INTERVAL, timer_callback, NULL);

    unsigned state = irq_disable();
    xtimer_sleep(2);
    irq_restore(state);

    if (atomic_load(&atomic_var) > 0) {
        puts("Interrupts handled correctly.");
    } else {
        puts("Interrupts not handled correctly.");
    }

    return 0;
}
