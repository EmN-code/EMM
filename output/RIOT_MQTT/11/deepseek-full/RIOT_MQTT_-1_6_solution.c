
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/af.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <architecture.h>
#include <time_units.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <msg.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_udp_t sock;
static sock_udp_ep_t broker;
static MQTTClient client;
static Network network;

void connect_to_broker(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    broker.family = AF_INET;
    broker.port = htons(BROKER_PORT);
    inet_aton(BROKER_IP, &broker.addr.ipv4);

    if (sock_udp_create(&sock, &local, &broker, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        puts("Error connecting to network");
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
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
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
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
