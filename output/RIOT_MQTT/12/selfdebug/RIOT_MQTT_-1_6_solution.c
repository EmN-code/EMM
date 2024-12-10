#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static mqtt_client_t client;
static sock_udp_ep_t broker_endpoint = {
    .family = AF_INET,
    .port = BROKER_PORT
};

static const char *pub_topic = "device/state";
static char message[] = "work or not work";

static int publish_data(mqtt_client_t *client, const char *pub_topic, const char *msg)
{
    mqtt_message_t message;
    message.payload = (void *)msg;
    message.payloadlen = strlen(msg);
    message.qos = MQTT_QOS_0;
    message.retained = 0;
    message.dup = 0;
    int res = mqtt_publish(client, NULL, pub_topic, &message);
    if (res != MQTT_OK) {
        printf("Error while publishing data: %d\n", res);
    }
    return res;
}

int main(void)
{
    printf("Starting MQTT client\n");

    if (inet_pton(AF_INET, BROKER_IP, &broker_endpoint.addr.ipv4) != 1) {
        puts("Failed to parse broker IP");
        return 1;
    }

    mqtt_connect(&client, "esp32_client", &broker_endpoint, NULL, 0, 0, 0, 1, 5);

    while (1) {
        publish_data(&client, pub_topic, message);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}