
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <timex.h>
#include <msg.h>
#include <event/thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <architecture.h>
#include <stddef.h>
#include <mbox.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <stdatomic.h>
#include <time_units.h>

#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return recv(n->sock, buffer, len, 0);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    (void)timeout;
    return send(n->sock, buffer, len, 0);
}

void connect_to_broker(void) {
    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &broker_addr.sin_addr);

    network.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (network.sock < 0) {
        puts("Failed to create socket");
        return;
    }

    if (connect(network.sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        puts("Failed to connect to broker");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Failed to publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
