
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time_units.h>
#include <ztimer.h>
#include <thread.h>
#include <msg.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex.h>
#include <timex.h>
#include <stddef.h>

#define STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Thread waiting to receive messages\n");
    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    char thread_stack[STACKSIZE_MAIN];
    kernel_pid_t thread_pid = thread_create(thread_stack, STACKSIZE_MAIN,
                                            THREAD_PRIORITY_MAIN - 1, 0,
                                            thread_handler, NULL, "recv_thread");

    if (thread_pid < 0) {
        printf("Error creating thread\n");
        return 1;
    }

    msg_t msg;
    msg.type = 1;

    while (1) {
        printf("Sending message to thread\n");
        msg_send(&msg, thread_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}