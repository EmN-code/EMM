
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <time_units.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <sys/socket.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void timer_callback(void *arg)
{
    (void)arg;
    atomic_fetch_add(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    evtimer_init_msg(&mbox);

    struct tm timer_event;
    timer_event.interval = 1000000; // 1 second
    timer_event.callback = timer_callback;

    evtimer_add_msg(&mbox, &timer_event, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);

        if (atomic_load(&interrupt_flag) > 0) {
            printf("Interrupt handled correctly.\n");
            atomic_store(&interrupt_flag, 0);
        } else {
            printf("Interrupt not handled correctly.\n");
        }
    }

    return 0;
}
