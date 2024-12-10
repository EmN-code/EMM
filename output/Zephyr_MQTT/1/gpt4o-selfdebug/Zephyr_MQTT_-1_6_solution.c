#include <zephyr.h>
#include <sys/printk.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <net/wifi_mgmt.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "zephyr_publisher"
#define TOPIC "state"
#define STATE_MSG "work or not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static uint8_t payload[128];
static struct mqtt_utf8 topic = {.utf8 = (uint8_t *)TOPIC, .size = sizeof(TOPIC) - 1};

static void prepare_fds(struct mqtt_client *client)
{
	fds[0].fd = client->transport.tcp.sock;
	fds[0].events = POLLIN;
}

static void init_broker(void)
{
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(MQTT_BROKER_PORT);
	inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			printk("MQTT connect failed: %d
", evt->result);
			break;
		}
		printk("MQTT client connected!
");
		break;
	case MQTT_EVT_DISCONNECT:
		printk("MQTT client disconnected %d
", evt->result);
		break;
	case MQTT_EVT_PUBLISH:
		printk("MQTT PUBLISH event: id=%d len=%d remaining=%d
",
		       evt->param.publish.message_id,
		       evt->param.publish.message.payload.len,
		       evt->param.publish.message.payload.remaining);
		break;
	case MQTT_EVT_PUBACK:
		printk("PUBACK packet id: %u
", evt->param.puback.message_id);
		break;
	case MQTT_EVT_PUBREC:
		printk("PUBREC packet id: %u
", evt->param.pubrec.message_id);
		break;
	case MQTT_EVT_PUBCOMP:
		printk("PUBCOMP packet id: %u
", evt->param.pubcomp.message_id);
		break;
	case MQTT_EVT_SUBACK:
		printk("SUBACK packet id: %u
", evt->param.suback.message_id);
		break;
	case MQTT_EVT_UNSUBACK:
		printk("UNSUBACK packet id: %u
", evt->param.unsuback.message_id);
		break;
	case MQTT_EVT_PINGRESP:
		printk("PINGRESP packet
");
		break;
	default:
		printk("Unhandled MQTT event type: %d
", evt->type);
		break;
	}
}

static int publish_message(struct mqtt_client *client)
{
	struct mqtt_publish_param param;

	memset(&param, 0, sizeof(param));
	param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
	param.message.topic.topic = topic;
	param.message.payload.data = STATE_MSG;
	param.message.payload.len = strlen(STATE_MSG);
	param.message_id = sys_rand32_get();
	param.dup_flag = 0U;
	param.retain_flag = 0U;

	return mqtt_publish(client, &param);
}

void main(void)
{
	struct mqtt_sec_config tls_config = { 0 };
	struct mqtt_utf8 client_id = {.utf8 = (uint8_t *)MQTT_CLIENT_ID, .size = sizeof(MQTT_CLIENT_ID) - 1};
	struct mqtt_utf8 user_name = {.utf8 = (uint8_t *)"", .size = 0};
	struct mqtt_utf8 password = {.utf8 = (uint8_t *)"", .size = 0};
	struct mqtt_connect_msg msg = {
		.clean_session = 1,
		.client_id = client_id,
		.user_name = user_name,
		.password = password,
		.keep_alive = 60,
	};

	init_broker();

	mqtt_client_init(&client);

	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = client_id.utf8;
	client.client_id.size = client_id.size;
	client.protocol_version = MQTT_VERSION_3_1_1;
	client.rx_buf = rx_buffer;
	client.rx_buf_size = sizeof(rx_buffer);
	client.tx_buf = tx_buffer;
	client.tx_buf_size = sizeof(tx_buffer);
	client.transport.type = MQTT_TRANSPORT_NON_SECURE;

	int ret = mqtt_connect(&client);

	if (ret != 0) {
		printk("Failed to connect to MQTT broker: %d
", ret);
		return;
	}

	while (1) {
		ret = publish_message(&client);

		if (ret != 0) {
			printk("Failed to publish message: %d
", ret);
		}

		k_sleep(PUBLISH_INTERVAL);
	}
}