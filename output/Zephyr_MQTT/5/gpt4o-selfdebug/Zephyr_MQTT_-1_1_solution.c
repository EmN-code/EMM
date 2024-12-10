#include <zephyr/sys/printk.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

#define TOPIC "esp32/status"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

static void prepare_mqtt_client(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    pub_param.message.topic.qos = MQTT_QOS_0;
    pub_param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    pub_param.message.topic.topic.size = strlen(TOPIC);
    pub_param.message.payload.data = (void *)"work or not work";
    pub_param.message.payload.len = strlen("work or not work");
    pub_param.message_id = 0;
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;
}

static void mqtt_event_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT connect failed: %d
", evt->result);
            return;
        }
        printk("MQTT client connected!
");
        break;

    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected: %d
", evt->result);
        break;

    case MQTT_EVT_PUBLISH:
        printk("MQTT message published: id=%d len=%d
", evt->param.publish.message_id, evt->param.publish.message.payload.len);
        break;

    default:
        break;
    }
}

void main(void) {
    printk("Starting MQTT client example
");
    prepare_mqtt_client();

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("MQTT connect error: %d
", rc);
        return;
    }

    while (1) {
        rc = mqtt_live(&client);
        if (rc != 0) {
            printk("ERROR: mqtt_live: %d
", rc);
            break;
        }
        k_sleep(PUBLISH_INTERVAL);

        rc = mqtt_publish(&client, &pub_param);
        if (rc != 0) {
            printk("ERROR: mqtt_publish: %d
", rc);
            break;
        }
    }

    mqtt_disconnect(&client);
}