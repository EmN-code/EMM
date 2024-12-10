
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <net/sock/util.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <msg.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <stddef.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <dht.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <timex.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <time_units.h>

#include "MQTTClient.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEANSESSION 1
#define MQTT_TIMEOUT 1000

static void _network_init(Network *n)
{
    n->socket = -1;
    n->mqttread = NULL;
    n->mqttwrite = NULL;
    n->disconnect = NULL;
}

static int _network_connect(Network *n, const char *ip, int port)
{
    struct sockaddr_in server_addr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return -1;
    }

    n->socket = sock;
    return 0;
}

static void _network_disconnect(Network *n)
{
    if (n->socket != -1) {
        close(n->socket);
        n->socket = -1;
    }
}

static int _mqtt_publish(MQTTClient *client, const char *topic, const char *payload)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    return MQTTPublish(client, topic, &message);
}

int main(void)
{
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

    _network_init(&network);
    if (_network_connect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTClientInit(&client, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    connect_data.keepAliveInterval = MQTT_KEEPALIVE;
    connect_data.cleansession = MQTT_CLEANSESSION;

    if (MQTTConnect(&client, &connect_data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        _network_disconnect(&network);
        return -1;
    }

    while (1) {
        if (_mqtt_publish(&client, MQTT_TOPIC, MQTT_PAYLOAD) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    _network_disconnect(&network);

    return 0;
}
