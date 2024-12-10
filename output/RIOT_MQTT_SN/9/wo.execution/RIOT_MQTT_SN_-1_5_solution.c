#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mma8x5x_regs.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <thread.h>
#include <event/thread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define EMCUTE_ID           "mqtt-sn"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC          "state"
#define MQTT_QOS            0
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         1883

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *mqtt_pub_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Could not connect to MQTT-SN broker\n");
            continue;
        }

        topic.name = MQTT_TOPIC;
        if (emcute_reg(&topic) != EMCUTE_OK) {
            printf("Could not register topic\n");
            emcute_discon();
            continue;
        }

        char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), MQTT_QOS) != EMCUTE_OK) {
            printf("Could not publish message\n");
        }

        emcute_discon();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN Publisher for ESP32\n");

    emcute_run(EMCUTE_PRIO, stack, sizeof(stack), queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_pub_thread, NULL, "mqtt_pub");

    return 0;
}