#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <errno.h>

#define MQTT_BROKER_ADDR CONFIG_NET_CONFIG_MY_IPV4_ADDR
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"
#define PAYLOAD_LEN (sizeof(MQTT_PAYLOAD) - 1)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct zsock_pollfd fds[1];
static int nfds;

void prepare_fds(struct mqtt_client *client)
{
	fds[0].fd = client->transport.tcp.sock;
	fds[0].events = ZSOCK_POLLIN;
	
	if (client->tx_buf_size > 0) {
		fds[0].events |= ZSOCK_POLLOUT;
	}
	
	nfds = 1;
}

void wait(struct mqtt_client *client, int timeout)
{
	zsock_poll(fds, nfds, timeout);
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			printk("MQTT connect failed: %d", evt->result);
			return;
		}
		printk("MQTT client connected!");
		prepare_fds(c);
		break;

	case MQTT_EVT_DISCONNECT:
		printk("MQTT client disconnected %d", evt->result);
		break;

	case MQTT_EVT_PUBLISH:
		printk("MQTT PUBLISH event received.");
		break;

	case MQTT_EVT_PUBACK:
		if (evt->result != 0) {
			printk("MQTT PUBACK error %d.", evt->result);
			return;
		}
		printk("PUBACK packet id: %u", evt->param.puback.message_id);
		break;

	default:
		break;
	}
}

int init_broker(void)
{
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(MQTT_BROKER_PORT);
	zsock_inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
	
	return 0;
}

void configure_mqtt(void)
{
	mqtt_client_init(&client);
	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = (uint8_t *)"zephyr_client";
	client.client_id.size = strlen("zephyr_client");
	client.password = NULL;
	client.user_name = NULL;
	client.protocol_version = MQTT_VERSION_3_1_1;
	
	client.rx_buf = rx_buffer;
	client.rx_buf_size = sizeof(rx_buffer);
	client.tx_buf = tx_buffer;
	client.tx_buf_size = sizeof(tx_buffer);
	client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

void publish(struct mqtt_client *client, enum mqtt_qos qos)
{
	struct mqtt_publish_param param;
	
	param.message.topic.qos = qos;
	param.message.topic.topic.utf8 = (uint8_t *)MQTT_PUB_TOPIC;
	param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
	param.message.payload.data = MQTT_PAYLOAD;
	param.message.payload.len = PAYLOAD_LEN;
	param.message_id = sys_rand32_get();
	param.dup_flag = 0;
	param.retain_flag = 0;
	
	mqtt_publish(client, &param);
}

void main(void)
{
	printk("Starting MQTT sample...");
	init_broker();
	configure_mqtt();
	
	mqtt_connect(&client);
	
	while (1) {
		publish(&client, MQTT_QOS_0_AT_MOST_ONCE);
		wait(&client, K_SECONDS(5));
		mqtt_input(&client);
		mqtt_live(&client);
		
		if (client.unacked_ping > 2) {
			mqtt_disconnect(&client);
			configure_mqtt();
			mqtt_connect(&client);
		}
	}
}