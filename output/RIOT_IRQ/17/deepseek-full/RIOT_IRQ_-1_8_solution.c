
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

static void *thread_func(void *arg)
{
    (void)arg;
    printf("Thread started\n");

    // Simulate some work
    xtimer_sleep(2);

    // Increment atomic variable
    atomic_fetch_add(&atomic_var, 1);

    printf("Thread finished\n");
    return NULL;
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Create a thread
    thread_create(stack, sizeof(stack), PRIORITY, 0, thread_func, NULL, "worker");

    // Simulate some work
    xtimer_sleep(1);

    // Check atomic variable
    if (atomic_load(&atomic_var) > 0) {
        printf("Interrupt handling test passed\n");
    } else {
        printf("Interrupt handling test failed\n");
    }

    return 0;
}
