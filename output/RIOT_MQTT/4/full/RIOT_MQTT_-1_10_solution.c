#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/paho_mqtt.h>
#include <msg.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <net/gcoap.h>
#include <stdatomic.h>
#include <net/emcute.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "device/state"

static char mqtt_send_buf[128];
static char mqtt_read_buf[128];

static MQTTClient client;
static Network network;

void *mqtt_publisher(void *arg) {
    (void)arg;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.username.cstring = NULL;
    options.password.cstring = NULL;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    // Initialize network
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Initialize and connect MQTT client
    MQTTClientInit(&client, &network, 1000, mqtt_send_buf, sizeof(mqtt_send_buf), mqtt_read_buf, sizeof(mqtt_read_buf));
    MQTTConnect(&client, &options);

    while (1) {
        static const char *state = "work";

        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)state;
        message.payloadlen = strlen(state);

        MQTTPublish(&client, MQTT_TOPIC, &message);
        ztimer_sleep(ZTIMER_USEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_publisher, NULL, "mqtt_pub_thread");
    return 0;
}