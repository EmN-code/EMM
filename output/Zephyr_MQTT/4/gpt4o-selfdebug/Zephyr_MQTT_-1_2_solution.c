#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/socket.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_storage broker;
static bool connected = false;

void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                connected = true;
                printk("MQTT client connected!\n");
            } else {
                printk("MQTT connect failed: %d\n", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            connected = false;
            printk("MQTT disconnected: %d\n", evt->result);
            break;
        default:
            break;
    }
}

void mqtt_connect(struct mqtt_client *client) {
    struct sockaddr *broker_addr = (struct sockaddr *)&broker;
    broker_addr->sa_family = AF_INET;
    net_sin(broker_addr)->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &net_sin(broker_addr)->sin_addr);

    struct mqtt_sec_config mqtt_sec_cfg = {0};

    struct mqtt_client_init_param param = {
        .broker = broker_addr,
        .evt_cb = mqtt_evt_handler,
        .client_id = {.UTF8 = "zephyr_client"},
        .protocol_version = MQTT_VERSION_3_1_0,
    };

    mqtt_init(client, &param);

    int result = mqtt_connect(client);
    if (result != 0) {
        printk("Failed to connect to MQTT broker: %d\n", result);
    }
}

void publish_state(struct mqtt_client *client) {
    const char *state = "work";
    struct mqtt_publish_param param = {
        .message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE,
        .message.topic.topic.utf8 = "state",
        .message.topic.topic.size = strlen("state"),
        .message.payload.data = state,
        .message.payload.len = strlen(state),
        .dup_flag = 0,
        .retain_flag = 0,
    };

    mqtt_publish(client, &param);
}

void main(void) {
    printk("Starting MQTT sample\n");

    mqtt_connect(&client);

    while (1) {
        if (connected) {
            publish_state(&client);
        }
        k_sleep(K_SECONDS(5));
    }
}