#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/random/rand32.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t tx_buffer[128];
static uint8_t rx_buffer[128];
static uint8_t payload[] = "work or not work";

void prepare_fds(struct mqtt_client *client) {
	fds[0].fd = client->transport.tcp.sock;
	fds[0].events = ZSOCK_POLLIN;
}

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			printk("MQTT connect failed %d
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
		printk("MQTT PUBLISH event received
");
		break;
	case MQTT_EVT_PINGRESP:
		printk("MQTT PINGRESP event received
");
		break;
	default:
		printk("MQTT event %d received
", evt->type);
		break;
	}
}

int main(void) {
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(BROKER_PORT);
	zsock_inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);

	mqtt_client_init(&client);
	client.broker = &broker;
	client.evt_cb = mqtt_event_handler;
	client.client_id.utf8 = "zephyr_mqtt_client";
	client.client_id.size = strlen(client.client_id.utf8);
	client.password = NULL;
	client.user_name = NULL;
	client.protocol_version = MQTT_VERSION_3_1_1;

	client.rx_buf = rx_buffer;
	client.rx_buf_size = sizeof(rx_buffer);
	client.tx_buf = tx_buffer;
	client.tx_buf_size = sizeof(tx_buffer);

	mqtt_connect(&client);

	struct pollfd fds[1];
	prepare_fds(&client);

	while (1) {
		mqtt_input(&client);
		mqtt_ping(&client);

		struct mqtt_publish_param param;
		param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
		param.message.topic.topic.utf8 = TOPIC;
		param.message.topic.topic.size = strlen(TOPIC);
		param.message.payload.data = payload;
		param.message.payload.len = strlen(payload);
		param.message_id = sys_rand32_get();
		param.dup_flag = 0;
		param.retain_flag = 0;

		mqtt_publish(&client, &param);

		k_sleep(INTERVAL);
	}

	return 0;
}
