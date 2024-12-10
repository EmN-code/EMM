
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <net/ipv4/addr.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <stdio.h>
#include <dht_params.h>
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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <architecture.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 60

static Network network;
static MQTTClient mqttClient;

void mqtt_app_start(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }

    printf("MQTT connected\n");

    while (1) {
        MQTTMessage message;
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.payload = (void*)MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        rc = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("MQTT publish failed\n");
        } else {
            printf("MQTT published\n");
        }

        xtimer_sleep(5);
    }
}

int main(void) {
    NetworkInit(&network);

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return -1;
    }

    printf("Network connected\n");

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    mqtt_app_start();

    return 0;
}
