
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <timex.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <dht_params.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_udp_t sock;
static struct Network network;
static MQTTClient client;

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data, data->message->payloadlen, data->message->payload);
}

int connect_to_broker(void) {
    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &broker_addr.sin_addr);

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = htons(BROKER_PORT), .addr = { .ipv4 = broker_addr.sin_addr } };

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return -1;
    }

    network.sock = sock;
    network.mqttread = sock_udp_read;
    network.mqttwrite = sock_udp_send;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return -1;
    }

    puts("Connected to MQTT broker");
    return 0;
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    if (connect_to_broker() != 0) {
        return -1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    MQTTDisconnect(&client);
    sock_udp_close(&sock);

    return 0;
}
