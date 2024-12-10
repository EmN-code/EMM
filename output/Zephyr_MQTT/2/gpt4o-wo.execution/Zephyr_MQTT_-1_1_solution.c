#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_STATE "device/state"

static struct mqtt_client client;
static struct sockaddr_storage broker;

void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT connected!
");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected!
");
        break;
    case MQTT_EVT_PUBACK:
        printk("MQTT publication acknowledged!
");
        break;
    default:
        printk("MQTT event: %d
", evt->type);
        break;
    }
}

void connect_to_broker() {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDRESS, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = "zephyr-client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.password = NULL;
    client.user_name = NULL;

    mqtt_connect(&client);
}

void publish_state(bool work) {
    struct mqtt_publish_param param;

    param.message.topic.topic.utf8 = TOPIC_STATE;
    param.message.topic.topic.size = strlen(TOPIC_STATE);
    param.message.payload.data = work ? "work" : "not work";
    param.message.payload.len = strlen(param.message.payload.data);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    mqtt_publish(&client, &param);
}

void main(void) {
    int rc;

    printk("Connecting to MQTT broker...
");
    connect_to_broker();

    while (1) {
        rc = mqtt_input(&client);
        if (rc != 0) {
            printk("mqtt_input failed: %d
", rc);
            break;
        }

        rc = mqtt_live(&client);
        if (rc != 0 && rc != -EAGAIN) {
            printk("mqtt_live failed: %d
", rc);
            break;
        }

        publish_state(true);
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
}