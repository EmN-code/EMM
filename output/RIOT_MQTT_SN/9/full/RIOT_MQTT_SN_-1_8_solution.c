#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <timex.h>
#include <mutex.h>

#define EMCUTE_ID        "emcute"
#define EMCUTE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT      1883
#define MQTT_TOPIC       "state/topic"
#define MQTT_BROKER_IP   "47.102.103.1"
#define STACK_SIZE       (THREAD_STACKSIZE_MAIN)

static char stack[STACK_SIZE];
static msg_t queue[8];
static emcute_topic_t t;

void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; // This function will never be reached
}

int main(void) {
    int rc;
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse MQTT broker address");
        return 1;
    }

    // Establish connection to MQTT-SN broker
    while (1) {
        rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Connection failed: %d. Retrying...\n", rc);
            xtimer_sleep(1);
        } else {
            printf("Connected successfully\n");
            break;
        }
    }

    // Register a topic
    t.name = MQTT_TOPIC;
    rc = emcute_reg(&t);
    if (rc != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", rc);
        return 1;
    }

    while (1) {
        // Publish "work" state
        const char *message = "work";
        rc = emcute_pub(&t, message, strlen(message), 0);
        if (rc != EMCUTE_OK) {
            printf("Publication error: %d\n", rc);
        } else {
            printf("Published: %s\n", message);
        }
        xtimer_sleep(5);
    }
    return 0;
}