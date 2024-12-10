
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <mma8x5x_params.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    return recv(n->sock, buffer, len, 0);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    return send(n->sock, buffer, len, 0);
}

void network_init(Network* n) {
    n->mqttread = mqtt_read;
    n->mqttwrite = mqtt_write;
}

void connect_to_broker(Network* n) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    n->sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(n->sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
}

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    network_init(&network);
    connect_to_broker(&network);

    unsigned char sendbuf[2048];
    unsigned char readbuf[2048];
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    close(network.sock);

    return 0;
}
