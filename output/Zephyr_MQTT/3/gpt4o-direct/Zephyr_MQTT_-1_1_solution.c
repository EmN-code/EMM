
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

LOG_MODULE_REGISTER(mqtt_client);

static int mqtt_connect(struct mqtt_client *client)
{
    struct sockaddr_in broker;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client->broker = &broker;
    client->client_id.utf8 = (u8_t *)MQTT_CLIENT_ID;
    client->client_id.size = strlen(MQTT_CLIENT_ID);
    client->user_name.utf8 = (u8_t *)MQTT_USERNAME;
    client->user_name.size = MQTT_USERNAME ? strlen(MQTT_USERNAME) : 0;
    client->password.utf8 = (u8_t *)MQTT_PASSWORD;
    client->password.size = MQTT_PASSWORD ? strlen(MQTT_PASSWORD) : 0;

    return mqtt_connect_async(client);
}

static void mqtt_event_handler(struct mqtt_client *const client,
                               const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected!");
        break;
    default:
        break;
    }
}

static void publish_state(struct mqtt_client *client)
{
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    struct mqtt_publish_param param;

    param.message.topic.utf8 = (u8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.utf8 = (u8_t *)state_str;
    param.message.payload.size = strlen(state_str);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_publish(client, &param);

    state = !state;
}

void main(void)
{
    struct mqtt_client client;
    mqtt_client_init(&client);

    if (mqtt_connect(&client) != 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        return;
    }

    client.evt_cb = mqtt_event_handler;

    while (1) {
        mqtt_input(&client);
        if (client.is_connected) {
            publish_state(&client);
        }
        k_sleep(K_SECONDS(5));
    }
}
