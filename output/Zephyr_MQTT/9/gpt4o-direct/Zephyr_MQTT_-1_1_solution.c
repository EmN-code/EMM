
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

LOG_MODULE_REGISTER(main);

static int mqtt_connect(struct mqtt_client *client)
{
    int rc;
    struct sockaddr_storage broker;

    broker.ss_family = AF_INET;
    struct sockaddr_in *broker_addr = (struct sockaddr_in *)&broker;
    broker_addr->sin_addr.s_addr = htonl(0x2F666767); // 47.102.103.1 in hex
    broker_addr->sin_port = htons(MQTT_BROKER_PORT);

    rc = mqtt_client_connect(client, (struct sockaddr *)&broker, NULL, NULL, NULL, NULL);
    if (rc != 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", rc);
        return rc;
    }

    return 0;
}

static int mqtt_publish(struct mqtt_client *client, const char *topic, const char *payload)
{
    int rc;
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = (uint8_t *)payload;
    param.message.payload.len = strlen(payload);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    rc = mqtt_publish(client, &param);
    if (rc != 0) {
        LOG_ERR("Failed to publish message: %d", rc);
        return rc;
    }

    return 0;
}

void main(void)
{
    struct mqtt_client client;
    struct mqtt_client_config config;
    struct mqtt_client_buf buf;

    memset(&client, 0, sizeof(client));
    memset(&config, 0, sizeof(config));
    memset(&buf, 0, sizeof(buf));

    config.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    config.client_id.size = strlen(MQTT_CLIENT_ID);
    config.user_name.utf8 = (uint8_t *)MQTT_USERNAME;
    config.user_name.size = MQTT_USERNAME ? strlen(MQTT_USERNAME) : 0;
    config.password.utf8 = (uint8_t *)MQTT_PASSWORD;
    config.password.size = MQTT_PASSWORD ? strlen(MQTT_PASSWORD) : 0;

    mqtt_client_init(&client, &config, &buf);

    if (mqtt_connect(&client) != 0) {
        return;
    }

    while (1) {
        const char *state = "work"; // or "not work" based on your logic
        if (mqtt_publish(&client, MQTT_TOPIC, state) != 0) {
            break;
        }
        k_sleep(K_SECONDS(5));
    }

    mqtt_client_disconnect(&client);
}
