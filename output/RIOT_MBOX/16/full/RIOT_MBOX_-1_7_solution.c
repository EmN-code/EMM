#include "netinet/in.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "mqtt/MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "evtimer.h"
#include "dht_params.h"
#include "dht.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "mutex.h"
#include "arpa/inet.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char main_thread_stack[THREAD_STACKSIZE_MAIN];

static void *main_thread(void *arg) {
    (void)arg;
    
    msg_t msg_queue[4];
    msg_init_queue(msg_queue, 4);

    evtimer_t my_eventtimer;
    evtimer_init_mbox(&my_eventtimer);

    evtimer_event_t my_event = {
        .msg = { .type = 0 },
    };
    
    kernel_pid_t main_pid = thread_getpid();
    evtimer_add_msg(&my_eventtimer, &my_event, main_pid);

    while (1) {
        msg_receive(&my_event.msg);
        puts("Event triggered!");
    }
    return NULL;
}

int main(void) {
    thread_create(main_thread_stack, sizeof(main_thread_stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, main_thread, NULL, "main");
    return 0;
}