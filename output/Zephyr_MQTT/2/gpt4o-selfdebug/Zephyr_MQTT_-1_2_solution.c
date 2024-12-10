#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define MQTT_TOPIC "esp32/state"

static struct mqtt_client client;
static struct sockaddr_storage broker;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT connect failed: %d\n", evt->result);
        } else {
            printk("MQTT connected!\n");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected: %d\n", evt->result);
        break;
    case MQTT_EVT_PUBLISH:
        printk("MQTT publish received\n");
        break;
    default:
        printk("Unhandled MQTT event: %d\n", evt->type);
        break;
    }
}

void connect_mqtt(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    mqtt_client_init(&client);

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");

    mqtt_connect(&client);
}

void publish_state(void) {
    static const char *msg = "work or not work";
    struct mqtt_publish_param param;

    param.message.payload.data = msg;
    param.message.payload.len = strlen(msg);
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
}

void main(void) {
    printk("Starting MQTT publish demo\n");
    connect_mqtt();

    while (1) {
        publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}
