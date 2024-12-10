#include <zephyr/net/mqtt_socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
	ARG_UNUSED(client);
	
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
		printk("MQTT PUBLISH result %d
", evt->result);
		break;
	case MQTT_EVT_PUBACK:
		printk("MQTT PUBACK %d
", evt->result);
		break;
	default:
		printk("MQTT unknown event %d
", evt->type);
		break;
	}
}

void configure_mqtt(void)
{
	mqtt_client_init(&client);
	broker.sin_family = AF_INET;
	broker.sin_port = htons(BROKER_PORT);
	zsock_inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = (uint8_t *)"zephyrClient";
	client.client_id.size = strlen("zephyrClient");
	client.protocol_version = MQTT_VERSION_3_1_1;
	
	
	client.rx_buf = k_malloc(BUFFER_SIZE_BYTES);
	client.rx_buf_size = BUFFER_SIZE_BYTES;
	client.tx_buf = k_malloc(BUFFER_SIZE_BYTES);
	client.tx_buf_size = BUFFER_SIZE_BYTES;
}

int main(void)
{
	configure_mqtt();
	
	struct mqtt_sec_config tls_config = { 0 };
	
	int rc = mqtt_connect(&client, &tls_config);
	if (rc != 0) {
		printk("mqtt_connect: Failed! %d
", rc);
		return 0;
	}
	
	while (true) {
		pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
		pub_param.message.topic.topic.utf8 = (uint8_t *)"sensors/esp32";
		pub_param.message.topic.topic.size = strlen("sensors/esp32");
		pub_param.message.payload.data = (uint8_t *)"work";
		pub_param.message.payload.len = strlen("work");
		pub_param.message_id = sys_rand32_get();
			
		mqtt_publish(&client, &pub_param);
		k_sleep(K_SECONDS(5));
	}
	
	mqtt_disconnect(&client);
	k_free(client.rx_buf);
	k_free(client.tx_buf);
	return 0;
}