#include <ztimer.h>
#include <net/sock/udp.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL_MS 5000

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];
static ztimer_t timer;

void publish_message(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen("work");
    MQTTPublish(&client, "status/update", &message);

    ztimer_set(ZTIMER_MSEC, &timer, PUBLISH_INTERVAL_MS);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, MQTT_BROKER_IP);

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        return -1;
    }

    ztimer_set_cb(ZTIMER_MSEC, &timer, publish_message, NULL);
    publish_message();

    return 0;
}
