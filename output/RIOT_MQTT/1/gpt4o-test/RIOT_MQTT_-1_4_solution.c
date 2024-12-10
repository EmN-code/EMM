#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

#include "MQTTClient.h" // Ensure this is the last header included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "mqtt_client"
#define PUB_TOPIC "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)

MQTTClient client;
static unsigned char sendbuf[200], readbuf[200];

void MQTT_publish_state(MQTTClient *client) {
    MQTTMessage message;
    char payload[30];
    snprintf(payload, sizeof(payload), "work");
    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;
    MQTTPublish(client, PUB_TOPIC, &message);
}

int main(void) {
    int res;
    sock_udp_ep_t broker_endpoint;
    broker_endpoint.family = AF_INET;
    broker_endpoint.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_endpoint.addr.ipv4);
    broker_endpoint.port = MQTT_BROKER_PORT;

    Network network;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    res = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (res != 0) {
        return res;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = CLIENT_ID;

    if ((res = MQTTConnect(&client, &options)) != 0) {
        return res;
    }

    while (1) {
        MQTT_publish_state(&client);
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}