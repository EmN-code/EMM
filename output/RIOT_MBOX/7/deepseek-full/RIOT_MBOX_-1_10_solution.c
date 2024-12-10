
#include "net/gcoap.h"
#include "esp_system.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "esp_wifi.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "mma8x5x_params.h"

#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 512

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0x1234;

    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_usleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type %d and value %d\n", msg.type, msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(NULL, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    if (sender_pid == KERNEL_PID_UNDEF || receiver_pid == KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
