
#include <string.h>
#include <net/sock.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <ztimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&mqttClient, TOPIC, &message);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;

    // Initialize the network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up the remote endpoint for the MQTT broker
    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    MQTTConnect(&mqttClient, &options);

    // Create a UDP socket for communication
    sock_udp_create(&sock, &local, &remote, 0);

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}
