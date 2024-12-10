#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>

#define MAIN_QUEUE_SIZE (8)

msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example content
    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0, msg_sender, NULL, "msg_sender");
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", (unsigned)msg.content.value);
    }
    return 0;
}
