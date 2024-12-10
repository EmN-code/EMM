
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

#define SLEEP_TIME_MS 5000

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT connected\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT disconnected\n");
            break;
        default:
            break;
    }
}

static void mqtt_publish(void) {
    static bool state = false;
    const char *state_str = state ? "work" : "not work";
    state = !state;

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = state_str;
    publish_param.message.payload.len = strlen(state_str);

    if (mqtt_publish(&client_ctx, &publish_param) < 0) {
        printk("MQTT publish failed\n");
    } else {
        printk("MQTT published: %s\n", state_str);
    }
}

int main(void) {
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    wifi_connect();

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client_ctx);
    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    if (mqtt_connect(&client_ctx) < 0) {
        printk("MQTT connect failed\n");
        return -1;
    }

    while (1) {
        mqtt_publish();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
