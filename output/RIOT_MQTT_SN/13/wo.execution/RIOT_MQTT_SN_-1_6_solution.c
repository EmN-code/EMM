#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <net/emcute.h>

#define EMCUTE_ID           "device"
#define STATE_MSG           "work or not work"
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         (1883)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

// Function to connect to the MQTT broker
int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

// Task function to publish messages
void *publish_task(void *arg) {
    (void)arg;
    emcute_topic_t t;
    t.name = "state/topic";
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&t, STATE_MSG, strlen(STATE_MSG), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish.");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));

    // Connect to broker
    if (connect_to_broker() != EMCUTE_OK) {
        puts("Error: could not connect to broker.");
        return 1;
    }

    // Create a thread to handle publishing messages
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_task, NULL, "publish thread");

    return 0;
}