
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

static int mqtt_connect(struct mqtt_client *client)
{
    int rc;
    struct sockaddr_storage broker;

    broker.sa_family = AF_INET;
    ((struct sockaddr_in *)&broker)->sin_addr.s_addr = htonl(0x2F666767); // 47.102.103.1
    ((struct sockaddr_in *)&broker)->sin_port = htons(MQTT_BROKER_PORT);

    mqtt_client_init(client);

    client->broker = &broker;
    client->client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client->client_id.size = strlen(MQTT_CLIENT_ID);
    client->password = (uint8_t *)MQTT_PASSWORD;
    client->user_name = (uint8_t *)MQTT_USERNAME;
    client->protocol_version = MQTT_VERSION_3_1_1;

    rc = mqtt_connect_common(client, NULL, NULL, 0, NULL, NULL, 0);
    if (rc != 0) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", rc);
        return rc;
    }

    return 0;
}

static int mqtt_publish(struct mqtt_client *client, const char *topic, const char *payload)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = (uint8_t *)payload;
    param.message.payload.len = strlen(payload);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(client, &param);
}

void main(void)
{
    struct mqtt_client client;
    int rc;

    rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("MQTT connection failed");
        return;
    }

    while (1) {
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        rc = mqtt_publish(&client, MQTT_TOPIC, state);
        if (rc != 0) {
            LOG_ERR("MQTT publish failed, error: %d", rc);
        } else {
            LOG_INF("Published state: %s", state);
        }

        k_sleep(K_SECONDS(5));
    }
}
