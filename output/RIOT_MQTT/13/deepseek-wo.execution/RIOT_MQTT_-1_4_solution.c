
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <net/ipv4/addr.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <stdio.h>
#include <ztimer.h>
#include <net/sock/tcp.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <MQTTClient.h>  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static Network network;
static MQTTClient client;

void mqtt_publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)"work";
    message.payloadlen = 4;

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void mqtt_init(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return;
    }

    mqtt_connect();
}

int main(void) {
    mqtt_init();

    while (1) {
        mqtt_publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
