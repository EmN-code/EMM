
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/sys/printk.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                printk("MQTT client connected\n");
            } else {
                printk("MQTT connection failed %d\n", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected\n");
            break;
        default:
            break;
    }
}

static void mqtt_publish_state(void) {
    static const char *state = "work";
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = state;
    publish_param.message.payload.len = strlen(state);
    publish_param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    publish_param.message.retained = 0;

    if (mqtt_publish(&client_ctx, &publish_param) != 0) {
        printk("MQTT publish failed\n");
    } else {
        printk("MQTT state published\n");
    }
}

int main(void) {
    struct sockaddr_in broker;
    int rc;

    wifi_connect();

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_mqtt_client";
    client_ctx.client_id.size = strlen("zephyr_mqtt_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    rc = mqtt_connect(&client_ctx);
    if (rc) {
        printk("MQTT connect failed %d\n", rc);
        return -1;
    }

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
