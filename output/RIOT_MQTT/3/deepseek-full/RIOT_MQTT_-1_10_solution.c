
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <kernel_defines.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <mma8x5x.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAINED false
#define MQTT_DUP false
#define MQTT_PAYLOADLEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 10000

static MQTTClient mqttClient;
static Network mqttNetwork;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAINED;
    message.dup = MQTT_DUP;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = MQTT_PAYLOADLEN;

    MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }

    printf("MQTT connected\n");
}

void network_connect(void) {
    int rc = NetworkConnect(&mqttNetwork, EMQX_SERVER_IP, EMQX_SERVER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return;
    }

    printf("Network connected\n");
}

void mqtt_init(void) {
    NetworkInit(&mqttNetwork);
    MQTTClientInit(&mqttClient, &mqttNetwork, MQTT_COMMAND_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

int main(void) {
    mqtt_init();
    network_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
