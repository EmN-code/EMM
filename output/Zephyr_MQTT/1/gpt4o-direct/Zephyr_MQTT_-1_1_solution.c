
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

LOG_MODULE_REGISTER(mqtt_example, LOG_LEVEL_INF);

static int mqtt_connect(struct mqtt_client *client)
{
    int rc;
    struct sockaddr_storage broker;

    broker.sa_family = AF_INET;
    ((struct sockaddr_in *)&broker)->sin_addr.s_addr = htonl(0x2F666767); // 47.102.103.1
    ((struct sockaddr_in *)&broker)->sin_port = htons(MQTT_BROKER_PORT);

    rc = mqtt_client_connect(client, (struct sockaddr *)&broker, NULL, NULL, NULL, NULL);
    if (rc) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", rc);
    }

    return rc;
}

static void mqtt_publish(struct mqtt_client *client, const char *topic, const char *payload)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(client, &param);
    if (rc) {
        LOG_ERR("Failed to publish message, error: %d", rc);
    } else {
        LOG_INF("Published: %s", payload);
    }
}

void main(void)
{
    struct mqtt_client client;
    mqtt_client_init(&client);

    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.password = MQTT_PASSWORD;
    client.user_name = MQTT_USERNAME;

    if (mqtt_connect(&client) == 0) {
        LOG_INF("Connected to MQTT broker");

        while (1) {
            const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
            mqtt_publish(&client, MQTT_TOPIC, state);
            k_sleep(K_SECONDS(5));
        }
    } else {
        LOG_ERR("MQTT connection failed");
    }
}
