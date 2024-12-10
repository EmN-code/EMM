#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/types.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static bool connected = false;

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                connected = true;
            }
            break;

        case MQTT_EVT_DISCONNECT:
            connected = false;
            break;

        default:
            break;
    }
}

static int init_mqtt(void) {
    int err;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    struct mqtt_transport transport = {.type = MQTT_TRANSPORT_NON_SECURE};

    struct mqtt_client_config config = {
        .broker = &broker,
        .protocol_version = MQTT_VERSION_3_1_1,
        .client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID,
        .client_id.size = strlen(MQTT_CLIENT_ID),
        .transport = &transport,
        .evt_cb = mqtt_evt_handler
    };

    mqtt_client_init(&client, &config);

    err = mqtt_connect(&client);
    if (err) {
        return err;
    }
    return 0;
}

static void publish_state(void) {
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = (uint8_t *)(connected ? "work" : "not work");
    param.message.payload.len = connected ? 4 : 8;
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message_id = sys_rand32_get();

    mqtt_publish(&client, &param);
}

void main(void) {
    int err;

    err = init_mqtt();
    if (err) {
        printk("Failed to connect to broker, error: %d\n", err);
        return;
    }

    while (1) {
        mqtt_input(&client);
        if (connected) {
            publish_state();
        }
        k_sleep(PUBLISH_INTERVAL);
    }
}