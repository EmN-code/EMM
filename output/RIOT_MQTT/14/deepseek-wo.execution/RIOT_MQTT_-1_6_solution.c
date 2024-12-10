
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    return recv(n->sock, buffer, len, 0);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    return send(n->sock, buffer, len, 0);
}

void network_init(void) {
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;
}

void connect_to_broker(void) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    network.sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(network.sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    network_init();
    connect_to_broker();

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "ESP32Client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    MQTTConnect(&client, &options);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
