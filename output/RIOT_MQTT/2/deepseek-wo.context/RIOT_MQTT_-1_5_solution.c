
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static void connect_to_broker(sock_tcp_t *sock)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to broker");
        return;
    }
    puts("Connected to broker");
}

static void publish_message(sock_tcp_t *sock)
{
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    MQTTClientInit(&client, NULL, 1000, NULL, 0, NULL, 0);

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Error: failed to publish message");
    } else {
        puts("Message published");
    }

    MQTTDisconnect(&client);
    puts("Disconnected from MQTT broker");
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    connect_to_broker(&sock);

    while (1) {
        publish_message(&sock);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
