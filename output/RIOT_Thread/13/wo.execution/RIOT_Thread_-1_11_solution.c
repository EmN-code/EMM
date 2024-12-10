// The code demonstrates sending and receiving messages between threads using RIOT OS on ESP32. 
// It creates two threads with different priorities and uses the msg_t struct for communication.

#include <mutex.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <msg.h>
#include <xtimer.h>
#include <dht_params.h>
#include <mbox.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <stdio.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <thread.h>
#include <time_units.h>
#include <net/emcute.h>
#include <dht.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <stdlib.h>
#include <evtimer_msg.h>

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN * 2)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 123456;
    while(1){
        msg_send(&msg, thread_getpid() + 1);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    while(1){
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(NULL, 8);
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, 0, sender, NULL, "sender");
    return 0;
}