#include <zephyr/zephyr.h>
#include <sys/printk.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_CLIENTID "ESP32_MQTT_Client"
#define TOPIC_STATE "esp32/state"
#define TIMEOUT K_SECONDS(5)
#define PUBLISH_INTERVAL K_SECONDS(5)

static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];
static uint8_t payload[] = "work";

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct zsock_pollfd fds[1];

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected!\n");
            break;
        default:
            break;
    }
}

void connect_to_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(SERVER_PORT);
    zsock_inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    int rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Could not connect to MQTT broker: %d\n", rc);
    }
}

void mqtt_publish_state(void) {
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC_STATE;
    param.message.topic.topic.size = strlen(TOPIC_STATE);
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload);
    param.message_id = sys_rand32_get();
    param.retain_flag = 0;

    int rc = mqtt_publish(&client, &param);
    if (rc != 0) {
        printk("Failed to publish message: %d\n", rc);
    }
}

void main(void) {
    printk("ESP32 MQTT client example\n");

    connect_to_broker();

    while (1) {
        mqtt_publish_state();
        k_sleep(PUBLISH_INTERVAL);
    }
}