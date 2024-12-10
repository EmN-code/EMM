#include <zephyr.h>
#include <net/mqtt.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mqtt_publisher, LOG_LEVEL_DBG);

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define STATE_MSG "work or not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;

static void prepare_fds(struct mqtt_client *c)
{
	if (c->transport.type == MQTT_TRANSPORT_NON_SECURE) {
		struct pollfd fds[1];
		fds[0].fd = c->transport.tcp.sock;
		fds[0].events = POLLIN;
		(void)poll(fds, 1, mqtt_keepalive_time_left(c));
	}
}

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT connect failed %d", evt->result);
		}
		break;
	case MQTT_EVT_DISCONNECT:
		LOG_INF("MQTT client disconnected %d", evt->result);
		break;
	case MQTT_EVT_PUBACK:
		if (evt->result != 0) {
			LOG_ERR("MQTT PUBACK error %d", evt->result);
		}
		break;
	default:
		break;
	}
}

static void broker_init(void)
{
	broker.sin_family = AF_INET;
	broker.sin_port = htons(BROKER_PORT);
	inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
}

static void client_init(struct mqtt_client *client)
{
	mqtt_client_init(client);

	client->broker = &broker;
	client->evt_cb = mqtt_evt_handler;
	client->client_id.utf8 = (uint8_t *)"zephyr_publisher";
	client->client_id.size = strlen("zephyr_publisher");
	client->protocol_version = MQTT_VERSION_3_1_1;

	client->rx_buf = k_malloc(128);
	client->rx_buf_size = 128;
	client->tx_buf = k_malloc(128);
	client->tx_buf_size = 128;

	client->transport.type = MQTT_TRANSPORT_NON_SECURE;
	client->transport.tcp.sock = -1;

	pub_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
	pub_param.message.topic.topic.utf8 = (uint8_t *)STATE_TOPIC;
	pub_param.message.topic.topic.size = strlen(STATE_TOPIC);
	pub_param.message.payload.data = STATE_MSG;
	pub_param.message.payload.len = strlen(STATE_MSG);
	pub_param.message_id = sys_rand32_get();
	pub_param.dup_flag = 0U;
	pub_param.retain_flag = 0U;
}

void main(void)
{
	broker_init();
	client_init(&client);

	while (1) {
		mqtt_connect(&client);
		mqtt_publish(&client, &pub_param);
		prepare_fds(&client);
		mqtt_input(&client);
		mqtt_live(&client);
		mqtt_disconnect(&client);

		k_sleep(PUBLISH_INTERVAL);
	}
}