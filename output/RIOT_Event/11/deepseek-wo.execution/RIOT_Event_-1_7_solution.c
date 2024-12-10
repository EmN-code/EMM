
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <xtimer.h>
#include <event/thread.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <timex.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define STACKSIZE 512
#define PRIORITY 5
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
    xtimer_periodic_wakeup((uint32_t *)arg, 1000000); // 1 second
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char stack[STACKSIZE];
    kernel_pid_t pid = thread_create(stack, STACKSIZE, PRIORITY, 0, (thread_task_func_t)periodic_callback, NULL, "periodic_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    xtimer_sleep(1); // Initial delay to start the periodic callback

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
