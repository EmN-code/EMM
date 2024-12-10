
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "event/thread.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "unistd.h"
#include "stddef.h"
#include "mbox.h"
#include "mutex.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "timex.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, (thread_task_func_t)periodic_callback, NULL, "periodic_callback");

    xtimer_t timer;
    xtimer_init(&timer, CLOCK_CORECLOCK, (xtimer_callback_t)periodic_callback, NULL);
    xtimer_set(&timer, 1000000); // 1 second interval

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
