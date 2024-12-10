
#include <zephyr.h>
#include <net/net_if.h>
#include <net/wifi_mgmt.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "zephyr_client"

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;
static struct mqtt_utf8 client_id;
static struct sockaddr_in broker;

void wifi_connect(void) {
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
        LOG_ERR("WiFi connection failed");
    } else {
        LOG_INF("WiFi connected");
    }
}

void mqtt_publish(struct mqtt_client *client) {
    static const char *payload = "work";
    static int toggle = 0;

    if (toggle) {
        payload = "not work";
    }
    toggle = !toggle;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);

    publish_param.message.topic = topic;
    publish_param.message.payload.data = (uint8_t *)payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.message.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.retain = 0;
    publish_param.dup_flag = 0;
    publish_param.message_id = 1;

    if (mqtt_publish(client, &publish_param) != 0) {
        LOG_ERR("MQTT publish failed");
    } else {
        LOG_INF("MQTT publish succeeded");
    }
}

void mqtt_connect(void) {
    client_id.utf8 = MQTT_CLIENT_ID;
    client_id.size = strlen(MQTT_CLIENT_ID);

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client_ctx.broker = &broker;
    client_ctx.client_id = client_id;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = -1;
    client_ctx.evt_cb = NULL;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client_ctx) != 0) {
        LOG_ERR("MQTT connection failed");
    } else {
        LOG_INF("MQTT connected");
    }
}

void main(void) {
    wifi_connect();
    mqtt_connect();

    while (1) {
        if (client_ctx.internal.state == MQTT_STATE_CONNECTED) {
            mqtt_publish(&client_ctx);
        }
        k_sleep(K_SECONDS(5));
    }
}
