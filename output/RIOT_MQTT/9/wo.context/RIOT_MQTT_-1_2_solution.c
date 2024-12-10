#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static const char *topic = "device/state";

void publish_state(MQTTClient *client, bool working) {
    char *message = working ? "work" : "not work";
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);
    MQTTPublish(client, topic, &mqtt_message);
}

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    char server_ip[] = SERVER_IP;
    ip_addr_t remote_addr;
    inet_pton(AF_INET6, server_ip, &remote_addr);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    memcpy(&gw.addr.ipv6, &remote_addr.u8, sizeof(gw.addr.ipv6));

    sock_udp_create(&network.sock, NULL, &gw, 0);
    MQTTConnect(&client, &connectData);

    bool working = true;
    while (1) {
        publish_state(&client, working);
        working = !working;  // Toggle state
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    return 0;
}