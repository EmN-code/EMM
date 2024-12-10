#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_DBG);

#define WIFI_SSID "your_ssid"
#define WIFI_PASS "your_password"
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "device/state"

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct k_work_delayable pub_work;
static bool work_state = true;

static void wifi_connect(void)
{
    struct wifi_connect_req_params wifi_params = {0};
    wifi_params.ssid = WIFI_SSID;
    wifi_params.ssid_length = strlen(WIFI_SSID);
    wifi_params.psk = WIFI_PASS;
    wifi_params.psk_length = strlen(WIFI_PASS);
    wifi_params.channel = WIFI_CHANNEL_ANY;
    wifi_params.security = WIFI_SECURITY_TYPE_PSK;

    struct net_if *iface = net_if_get_default();
    wifi_mgmt_connect(iface, &wifi_params);
}

static void mqtt_evt_handler(struct mqtt_client * const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        LOG_INF("MQTT client connected!");
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT client disconnected!");
        break;
    default:
        break;
    }
}

static void publish_state(struct k_work *work)
{
    int ret;
    struct mqtt_publish_param param;

    char payload[20];
    snprintf(payload, sizeof(payload), "State: %s", work_state ? "work" : "not work");

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = MQTT_PUB_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.messageId = k_uptime_get_32();
    param.dup_flag = 0;
    param.retain_flag = 0;

    ret = mqtt_publish(&client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    }

    /* Re-schedule the work to run again after 5 seconds */
    k_work_reschedule(&pub_work, K_SECONDS(5));
}

void main(void)
{
    int ret;

    wifi_connect();

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    struct mqtt_utf8 username = { .utf8 = NULL, .size = 0 };
    struct mqtt_utf8 password = { .utf8 = NULL, .size = 0 };

    struct mqtt_client *client = &client;
    mqtt_client_init(client);

    client->broker = (struct sockaddr *)&broker;
    client->evt_cb = mqtt_evt_handler;
    client->client_id.utf8 = "zephyr_client";
    client->client_id.size = strlen("zephyr_client");
    client->user_name = &username;
    client->password = &password;
    client->protocol_version = MQTT_VERSION_3_1_1;

    ret = mqtt_connect(client);
    if (ret != 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    k_work_init_delayable(&pub_work, publish_state);
    k_work_reschedule(&pub_work, K_SECONDS(5));
}