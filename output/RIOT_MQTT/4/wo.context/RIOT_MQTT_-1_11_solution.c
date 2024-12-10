// Import necessary libraries for MQTT and network functionalities
#include "net/sock/udp.h"
#include "msg.h"
#include "ztimer.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define UPDATE_INTERVAL 5

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];
static int is_running = 1;

void on_message_delivered(void *arg) {
    (void)arg;
    printf("Message delivered\n");
}

void prepare_and_connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    int result = NetworkConnect(&network, BROKER_IP, (int)BROKER_PORT);
    if (result != SUCCESS) {
        puts("Failed to connect to network");
        return;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = "RIOT_ESP32";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (MQTTConnect(&client, &data) != SUCCESS) {
        puts("Failed to connect to MQTT broker");
    }
}

void publish_state_loop(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work or not work";
    message.payloadlen = strlen(message.payload);

    while (is_running) {
        int rc = MQTTPublish(&client, "state", &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message: %d\n", rc);
        }

        ztimer_sleep(ZTIMER_MSEC, UPDATE_INTERVAL * 1000);
    }

    MQTTDisconnect(&client);
}

int main(void) {
    prepare_and_connect();
    publish_state_loop();
    return 0;
}