
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <device.h>
#include <drivers/wifi.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(mqtt_example);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 client_id = {
	.utf8 = "zephyr_client",
	.size = 13
};

static void wifi_connect(void)
{
	const struct device *wifi_dev = device_get_binding("WIFI_0");
	if (!wifi_dev) {
		LOG_ERR("WiFi device not found");
		return;
	}

	struct wifi_connect_req_params wifi_params = {
		.ssid = "your_ssid",
		.ssid_length = 9,
		.psk = "your_password",
		.psk_length = 12,
		.channel = 0,
		.security = WIFI_SECURITY_TYPE_PSK,
	};

	if (wifi_connect(wifi_dev, &wifi_params) < 0) {
		LOG_ERR("WiFi connection failed");
	} else {
		LOG_INF("WiFi connected");
	}
}

static void mqtt_evt_handler(struct mqtt_client *const client,
			     const struct mqtt_evt *evt)
{
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result == 0) {
			LOG_INF("Connected to MQTT broker");
		} else {
			LOG_ERR("Failed to connect to MQTT broker: %d", evt->result);
		}
		break;
	case MQTT_EVT_DISCONNECT:
		LOG_INF("Disconnected from MQTT broker");
		break;
	default:
		break;
	}
}

static void mqtt_publish_state(struct mqtt_client *client, const char *state)
{
	struct mqtt_publish_param param;
	struct mqtt_topic topic = {
		.topic.utf8 = MQTT_TOPIC,
		.topic.size = sizeof(MQTT_TOPIC) - 1,
		.qos = MQTT_QOS_0_AT_MOST_ONCE
	};

	param.message.topic = topic;
	param.message.payload.data = state;
	param.message.payload.len = strlen(state);
	param.message_id = 0;
	param.dup_flag = 0;
	param.retain_flag = 0;

	if (mqtt_publish(client, &param) != 0) {
		LOG_ERR("Failed to publish message");
	} else {
		LOG_INF("Published: %s", state);
	}
}

void main(void)
{
	wifi_connect();

	struct sockaddr_in broker_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(MQTT_BROKER_PORT),
	};
	inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

	mqtt_client_init(&client_ctx);
	client_ctx.broker = &broker_addr;
	client_ctx.client_id = client_id;
	client_ctx.evt_cb = mqtt_evt_handler;
	client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
	client_ctx.rx_buf = rx_buffer;
	client_ctx.rx_buf_size = sizeof(rx_buffer);
	client_ctx.tx_buf = tx_buffer;
	client_ctx.tx_buf_size = sizeof(tx_buffer);

	if (mqtt_connect(&client_ctx) != 0) {
		LOG_ERR("Failed to connect to MQTT broker");
		return;
	}

	while (1) {
		mqtt_publish_state(&client_ctx, "work");
		k_sleep(K_SECONDS(5));
		mqtt_publish_state(&client_ctx, "not work");
		k_sleep(K_SECONDS(5));
	}
}
