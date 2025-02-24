
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
    struct sockaddr_storage broker;
    int ret;

    broker.sa_family = AF_INET;
    ((struct sockaddr_in *)&broker)->sin_addr.s_addr = htonl(0x2F666701); // 47.102.103.1
    ((struct sockaddr_in *)&broker)->sin_port = htons(MQTT_BROKER_PORT);

    ret = mqtt_client_connect(client, (struct sockaddr *)&broker, NULL);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
    }

    return ret;
}

static void mqtt_publish(struct mqtt_client *client, const char *payload)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
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
        while (1) {
            mqtt_publish(&client, "work");
            k_sleep(K_SECONDS(5));
            mqtt_publish(&client, "not work");
            k_sleep(K_SECONDS(5));
        }
    }

    mqtt_client_disconnect(&client);
}
