
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
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
#include <string.h>
#include <net/gcoap.h>

#define STACKSIZE 512
#define NUM_MESSAGES 16

static msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, (kernel_pid_t)arg, false);
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sender_thread,
                                            (void *)thread_getpid(), "sender");

    kernel_pid_t receiver_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                                              THREAD_CREATE_STACKTEST, receiver_thread,
                                              NULL, "receiver");

    (void)sender_pid; // Suppress unused variable warning

    return 0;
}
