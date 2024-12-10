#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <unistd.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <evtimer.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>

static evtimer_msg_t event_timer;
static msg_t msg_queue[4];

void callback_function(void *arg){
    printf("Periodic callback executed.\n");
}

int main(void){
    /* Initialize message queue with size 4 (must be power of two) */
    msg_init_queue(msg_queue, 4);

    /* Initialize the event timer */
    evtimer_init_msg(&event_timer);

    /* Create a periodic event with 1 second intervals */
    evtimer_msg_event_t event;
    event.event.interval = 1000000U; // 1 second in microseconds
    event.event.callback = callback_function;
    event.event.arg = NULL;

    /* Add event to the event timer targeting current thread (kernel_pid_t 0) */
    evtimer_add_msg(&event_timer, &event, 0);

    /* Enter an idle loop where we wait for IPC messages to trigger events */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}