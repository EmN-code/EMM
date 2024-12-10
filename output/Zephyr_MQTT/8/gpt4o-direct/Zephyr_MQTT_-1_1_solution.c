
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

LOG_MODULE_REGISTER(mqtt_client);

static int mqtt_connect(int sock)
{
    struct mqtt_client client;
    mqtt_client_init(&client);

    client.broker = &sock;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.user_name.utf8 = (uint8_t *)MQTT_USERNAME;
    client.user_name.size = MQTT_USERNAME ? strlen(MQTT_USERNAME) : 0;
    client.password.utf8 = (uint8_t *)MQTT_PASSWORD;
    client.password.size = MQTT_PASSWORD ? strlen(MQTT_PASSWORD) : 0;

    return mqtt_connect(&client);
}

static int mqtt_publish(int sock, const char *topic, const char *payload)
{
    struct mqtt_client client;
    mqtt_client_init(&client);

    client.broker = &sock;
    client.message.topic.utf8 = (uint8_t *)topic;
    client.message.topic.size = strlen(topic);
    client.message.payload.utf8 = (uint8_t *)payload;
    client.message.payload.size = strlen(payload);
    client.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    client.message.retained = 0;

    return mqtt_publish(&client);
}

void main(void)
{
    int sock;
    struct sockaddr_in broker_addr;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return;
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        close(sock);
        return;
    }

    if (mqtt_connect(sock) < 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        close(sock);
        return;
    }

    while (1) {
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        if (mqtt_publish(sock, MQTT_TOPIC, state) < 0) {
            LOG_ERR("Failed to publish message");
        } else {
            LOG_INF("Published: %s", state);
        }
        k_sleep(K_SECONDS(5));
    }

    close(sock);
}
