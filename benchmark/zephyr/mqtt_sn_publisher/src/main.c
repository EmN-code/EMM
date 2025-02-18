#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(net_mqtt_sn_publisher_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt_sn.h>

#define STACK_SIZE 2048
#define THREAD_PRIORITY K_PRIO_PREEMPT(8)
// wifi information
#define WIFI_SSID "EmNets-301"
#define WIFI_PSK "eagle402"

static int wifi_connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;

// mqtt_sn config
#define SERVER_ADDR "47.102.103.1"  
#define SERVER_PORT 1884
#define TOPIC "/test/topic"
#define PAYLOAD "Hello, world!"
#define INTERVAL_S 5
#define CONFIG_NET_SAMPLE_MQTT_SN_BUFFER_SIZE 255

static struct mqtt_sn_client client;
static struct mqtt_sn_transport_udp tp;
static struct mqtt_sn_data client_id = {
	.data = (uint8_t *)"ZEPHYR",
	.size = sizeof("ZEPHYR") - 1
};

static uint8_t tx_buffer[CONFIG_NET_SAMPLE_MQTT_SN_BUFFER_SIZE];
static uint8_t rx_buffer[CONFIG_NET_SAMPLE_MQTT_SN_BUFFER_SIZE];

static bool mqtt_sn_connected;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		LOG_ERR("Connection request failed (%d)\n", status->status);
	} else {
		LOG_INF("WIFI Connected\n");
		wifi_connected = 1;
	}
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
				    uint32_t mgmt_event, struct net_if *iface)
{
	switch (mgmt_event) {
	case NET_EVENT_WIFI_CONNECT_RESULT:
		handle_wifi_connect_result(cb);
		break;
	default:
		break;
	}
}

void wifi_connect(void)
{
	int nr_tries = 10;
	int ret = 0;

	net_mgmt_init_event_callback(&wifi_shell_mgmt_cb,
				     wifi_mgmt_event_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT);

	net_mgmt_add_event_callback(&wifi_shell_mgmt_cb);

	struct net_if *iface = net_if_get_default();
	static struct wifi_connect_req_params cnx_params = {
		.ssid = WIFI_SSID,
		.ssid_length = strlen(WIFI_SSID),
		.psk = WIFI_PSK,
		.psk_length = strlen(WIFI_PSK),
		.channel = WIFI_CHANNEL_ANY,
		.security = WIFI_SECURITY_TYPE_PSK,
		.band = WIFI_FREQ_BAND_2_4_GHZ,
	};

	wifi_connected = 0;

	LOG_INF("WIFI try connecting to %s...\n", WIFI_SSID);

	/* Let's wait few seconds to allow wifi device be on-line */
	while (nr_tries-- > 0) {
		ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params,
			       sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			break;
		}

		LOG_ERR("Connect request failed %d. Waiting iface be up...\n", ret);
		k_msleep(500);
	}

	while (wifi_connected == 0) {
		k_msleep(100);
	}
}

static void mqtt_sn_evt_handler(struct mqtt_sn_client *client, const struct mqtt_sn_evt *evt){
	switch(evt->type){
		case MQTT_SN_EVT_CONNECTED:
			LOG_INF("MQTT-SN event EVT_CONNECTED");
			mqtt_sn_connected = true;
			break;
		case MQTT_SN_EVT_DISCONNECTED:
			LOG_INF("MQTT-SN event EVT_DISCONNECTED");
			mqtt_sn_connected = false;
			break;
		case MQTT_SN_EVT_ASLEEP:
			LOG_INF("MQTT-SN event EVT_ASLEEP");
			break;
		case MQTT_SN_EVT_AWAKE:
			LOG_INF("MQTT-SN event EVT_AWAKE");
			break;
		case MQTT_SN_EVT_PUBLISH:
			LOG_INF("MQTT-SN event EVT_PUBLISH");
			// LOG_HEXDUMP_INF(evt->param.publish.data.data, evt->param.publish.data.size,"Published data");
			break;
		case MQTT_SN_EVT_PINGRESP:
			LOG_INF("MQTT-SN event EVT_PINGRESP");
			break;
	}
}

static int do_work(void){
	struct mqtt_sn_data topic = {
		.data = (uint8_t *)TOPIC,
		.size = sizeof(TOPIC) - 1
	};
	struct mqtt_sn_data payload = {
		.data = (uint8_t *)PAYLOAD,
		.size = sizeof(PAYLOAD) - 1
	};
	int err;
	err = mqtt_sn_input(&client);
	if(err < 0){
		LOG_ERR("failed: input: %d", err);
		return err;
	}


	if(mqtt_sn_connected){
		err = mqtt_sn_publish(&client, MQTT_SN_QOS_0, &topic, false, &payload);
		if(err < 0){
			LOG_ERR("failed: publish: %d", err);
			return err;
		}else{
			LOG_INF("Published: %s", PAYLOAD);
		}
	}
	return 0;
}

static void publish_thread(void){
	while(!wifi_connected){
		k_sleep(K_SECONDS(1));
	}
	int err;
	struct sockaddr_in gateway = {0};
	gateway.sin_family = AF_INET;
	gateway.sin_port = htons(SERVER_PORT);
	err = inet_pton(AF_INET, SERVER_ADDR, &gateway.sin_addr);
	if (err < 0){
		LOG_ERR("inet_pton() failed %d", err);
		return;
	}
	LOG_INF("Waiting for connection...");
	err = mqtt_sn_transport_udp_init(&tp, (struct sockaddr *)&gateway, sizeof(gateway));
	if (err < 0){
		LOG_ERR("mqtt_sn_transport_udp_init() failed %d", err);
		return;
	}
	err = mqtt_sn_client_init(&client, &client_id, &tp.tp, mqtt_sn_evt_handler, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
	if (err <0){
		LOG_ERR("mqtt_sn_client_init() failed %d", err);
		return;
	}
	err = mqtt_sn_connect(&client, false, true);
	if (err < 0){
		LOG_ERR("mqtt_sn_connect() failed %d", err);
		return;
	}
	while(err == 0){
		k_sleep(K_SECONDS(INTERVAL_S));
		err = do_work();
	}
	LOG_ERR("Exiting thread: %d", err);
}

K_THREAD_DEFINE(udp_thread_id, STACK_SIZE, publish_thread, NULL, NULL, NULL, THREAD_PRIORITY,0,-1);

void start_thread(void){
	int rc ;
	rc = k_thread_name_set(udp_thread_id, "udp");
	if(rc <0 && rc != -ENOSYS){
		LOG_ERR("Failed: k_thread_name_set() %d", rc);
		return;
	}
	k_thread_start(udp_thread_id);
	rc = k_thread_join(udp_thread_id, K_FOREVER);
	if(rc != 0){
		LOG_ERR("Failed: k_thread_join() %d", rc);
		return;
	}
}
int main(void){
	
	wifi_connect();
	start_thread();
	return 0;
}