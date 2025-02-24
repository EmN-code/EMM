#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <string.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define MQTT_CLIENTID "zephyr_publisher"
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"

#define STACK_SIZE 1024
#define THREAD_PRIORITY K_PRIO_PREEMPT(8)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct pollfd fds;

static bool connected = false;

static void broker_init(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &broker.sin_addr);
}

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

static void mqtt_init(void) {
    mqtt_client_init(&client);

    client.broker = (struct sockaddr *)&broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);

    client.protocol_version = MQTT_VERSION_3_1_1;

    static uint8_t rx_buffer[128];
    static uint8_t tx_buffer[128];
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    broker_init();
}

static void mqtt_connect(void) {
    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect MQTT: %d\n", rc);
    }
}

static void publish_state(bool work) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);

    if (work) {
        param.message.payload.data = (uint8_t *)MQTT_PAYLOAD_WORK;
        param.message.payload.len = strlen(MQTT_PAYLOAD_WORK);
    } else {
        param.message.payload.data = (uint8_t *)MQTT_PAYLOAD_NOT_WORK;
        param.message.payload.len = strlen(MQTT_PAYLOAD_NOT_WORK);
    }

    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
}

void main(void) {
    mqtt_init();
    mqtt_connect();

    while (1) {
        if (connected) {
            publish_state(true); // Publish that it is working
        } else {
            mqtt_connect();
        }
        k_sleep(K_SECONDS(5));
    }
}