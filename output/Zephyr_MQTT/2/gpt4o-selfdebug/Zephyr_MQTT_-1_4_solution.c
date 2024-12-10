#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <net/tls_credentials.h>
#include <random/rand32.h>
#include <sys/printk.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_DBG);

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 client_id = MQTT_UTF8_LITERAL("mqtt_client");

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883

#define APP_SLEEP_MSECS 5000
#define CLIENT_ID_SIZE 15

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static void prepare_mqtt_broker(void)
{
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);
}

static void broker_init(void)
{
	prepare_mqtt_broker();
}

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT connect failed %d", evt->result);
		} else {
			LOG_INF("MQTT client connected!");
		}
		break;

	case MQTT_EVT_PUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBACK error %d", evt->result);
		} else {
			LOG_INF("PUBACK packet id: %u", evt->param.puback.message_id);
		}
		break;

	default:
		break;
	}
}

static void mqtt_client_init(void)
{
	mqtt_client_init(&client);

	broker_init();

	client.broker = &broker;
	client.client_id.utf8 = client_id.utf8;
	client.client_id.size = strlen(client_id.utf8);
	client.evt_cb = mqtt_evt_handler;
	client.client_id = client_id;
	client.password = NULL;
	client.user_name = NULL;
	client.protocol_version = MQTT_VERSION_3_1_1;
	client.rx_buf = rx_buffer;
	client.rx_buf_size = sizeof(rx_buffer);
	client.tx_buf = tx_buffer;
	client.tx_buf_size = sizeof(tx_buffer);

	client.transport.type = MQTT_TRANSPORT_NON_SECURE;

	struct mqtt_utf8 password = {
		.utf8 = NULL,
		.size = 0
	};

	client.password = &password;

	struct mqtt_utf8 username = {
		.utf8 = NULL,
		.size = 0
	};

	client.user_name = &username;
}

static void publish_message(void)
{
	struct mqtt_publish_param param;
	char message[] = "work";

	param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
	param.message.topic.topic.utf8 = (uint8_t *)"sensors";
	param.message.topic.topic.size = strlen("sensors");
	param.message.payload.data = message;
	param.message.payload.len = strlen(message);
	param.message_id = sys_rand32_get();
	param.dup_flag = 0U;
	param.retain_flag = 0U;

	mqtt_publish(&client, &param);
}

void main(void)
{
	mqtt_client_init();

	client.broker = &broker;

	if (mqtt_connect(&client) != 0) {
		LOG_ERR("MQTT connect failed");
		return;
	}

	while (1) {
		publish_message();
		k_sleep(K_MSEC(APP_SLEEP_MSECS));
	}
}