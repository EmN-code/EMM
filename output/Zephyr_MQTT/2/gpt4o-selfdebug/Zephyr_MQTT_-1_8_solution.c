#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/websocket.h>
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <string.h>
#include <sys/printk.h>
#include <sys/util.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "zephyr_client"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[] = "work or not work";
static bool connected = false;

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("Connected to MQTT broker\n");
                connected = true;
            } else {
                printk("MQTT connect failed\n");
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            connected = false;
            break;
        default:
            break;
    }
}

static void connect_to_broker(void) {
    struct mqtt_sec_config tls_config;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    mqtt_client_init(&client);

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    mqtt_connect(&client);
}

static void publish_state(void) {
    struct mqtt_publish_param param;
    if (!connected) {
        connect_to_broker();
        return;
    }

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)"state";
    param.message.topic.topic.size = strlen("state");
    param.message.payload.data = payload;
    param.message.payload.len = sizeof(payload) - 1;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(&client, &param);
}

void main(void) {
    printk("MQTT sample application starting\n");
    connect_to_broker();
    while (1) {
        publish_state();
        k_sleep(K_SECONDS(5));
    }
}
