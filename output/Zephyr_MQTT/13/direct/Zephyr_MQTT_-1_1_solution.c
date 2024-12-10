
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

LOG_MODULE_REGISTER(main);

static int mqtt_connect(struct mqtt_client *client)
{
    int rc;
    struct sockaddr_storage broker;

    broker.sa_family = AF_INET;
    ((struct sockaddr_in *)&broker)->sin_addr.s_addr = htonl(0x2F666767); // 47.102.103.1
    ((struct sockaddr_in *)&broker)->sin_port = htons(MQTT_BROKER_PORT);

    rc = mqtt_client_connect(client, (struct sockaddr *)&broker, sizeof(broker), MQTT_USERNAME, MQTT_PASSWORD, NULL, NULL);
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
    param.message.topic.topic.utf8 = topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = payload;
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
    mqtt_client_init(&client);

    if (mqtt_connect(&client) != 0) {
        return;
    }

    while (1) {
        const char *state = (k_uptime_get() % 10000 < 5000) ? "work" : "not work";
        if (mqtt_publish(&client, MQTT_TOPIC, state) != 0) {
            break;
        }
        k_sleep(K_SECONDS(5));
    }

    mqtt_client_disconnect(&client);
}
