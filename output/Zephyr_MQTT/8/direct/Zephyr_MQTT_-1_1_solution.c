
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

LOG_MODULE_REGISTER(mqtt_example, LOG_LEVEL_DBG);

static int mqtt_connect(struct mqtt_client *client)
{
    int rc;
    struct sockaddr_in broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(client);

    client->broker = &broker;
    client->client_id.utf8 = (u8_t *)MQTT_CLIENT_ID;
    client->client_id.size = strlen(MQTT_CLIENT_ID);
    client->password = NULL;
    client->user_name = NULL;
    client->protocol_version = MQTT_VERSION_3_1_1;

    rc = mqtt_connect_async(client);
    if (rc) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", rc);
    }

    return rc;
}

static int mqtt_publish(struct mqtt_client *client, const char *payload)
{
    int rc;
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = (u8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (u8_t *)payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    rc = mqtt_publish(client, &param);
    if (rc) {
        LOG_ERR("Failed to publish message, error: %d", rc);
    }

    return rc;
}

void main(void)
{
    struct mqtt_client client;
    int rc;

    LOG_INF("Starting MQTT example");

    rc = mqtt_connect(&client);
    if (rc) {
        return;
    }

    while (1) {
        const char *state = (sys_rand32_get() % 2 == 0) ? "work" : "not work";
        LOG_INF("Publishing state: %s", state);

        rc = mqtt_publish(&client, state);
        if (rc) {
            break;
        }

        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
}
