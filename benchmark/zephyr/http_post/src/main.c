
#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/http/client.h>

// wifi information
#define WIFI_SSID "EmNets-301"
#define WIFI_PSK "eagle402"

// http information
#define HTTP_SERVER_ADDR "10.214.149.209"
#define HTTP_SERVER_PORT 14008
#define HTTP_URL "/zephyr/http"
#define MAX_RECV_BUF_LEN 512

static int sock;
static uint8_t recv_buf[MAX_RECV_BUF_LEN];

static int connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;
// static struct net_mgmt_event_callback ipv4_mgmt_cb;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)cb->info;

	if (status->status) {
		printf("Connection request failed (%d)\n", status->status);
	} else {
		printf("WIFI Connected\n");
		connected = 1;
	}
}

static void handle_wifi_disconnect_result(struct net_mgmt_event_callback *cb){
	const struct wifi_status *status = (const struct wifi_status *)cb->info;
	
	if (status->status)
    {
        printk("Disconnection request (%d)\n", status->status);
    }
    else
    {
        printk("Disconnected\n");
        connected = 0;
    }
}

static void handle_ipv4_result(struct net_if *iface){
	int i = 0;
	for(i=0;i<NET_IF_MAX_IPV4_ADDR;i++){
		char buf[NET_IPV4_ADDR_LEN];
		// if(iface->config.dhcpv4.state != NET_ADDR_DHCP){
		// 	printk("DHCP failed\n");
		// 	continue;
		// }
		printk("IPv4 address: %s\n", net_addr_ntop(AF_INET,&iface->config.ip.ipv4->unicast[i].ipv4.address.in_addr,buf,sizeof(buf)));
	}
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
				    uint32_t mgmt_event, struct net_if *iface)
{
	switch (mgmt_event) {
		case NET_EVENT_WIFI_CONNECT_RESULT:
			handle_wifi_connect_result(cb);
			break;
		case NET_EVENT_WIFI_DISCONNECT_RESULT:
			handle_wifi_disconnect_result(cb);
			break;
		case NET_EVENT_IPV4_ADDR_ADD:
			handle_ipv4_result(iface);
			break;
		default:
			break;
	}
}

void wifi_status(void){
    struct net_if *iface = net_if_get_default();
    
    struct wifi_iface_status status = {0};

    if (net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status,	sizeof(struct wifi_iface_status)))
    {
        printk("WiFi Status Request Failed\n");
    }

    printk("\n");

    if (status.state >= WIFI_STATE_ASSOCIATED) {
        printk("SSID: %-32s\n", status.ssid);
        printk("Band: %s\n", wifi_band_txt(status.band));
        printk("Channel: %d\n", status.channel);
        printk("Security: %s\n", wifi_security_txt(status.security));
        printk("RSSI: %d\n", status.rssi);
		char buf[NET_IPV4_ADDR_LEN];
		printk("IPv4 address: %s\n", net_addr_ntop(AF_INET,&iface->config.ip.ipv4->unicast[NET_IF_MAX_IPV4_ADDR-1].ipv4.address.in_addr,buf,sizeof(buf)));

    }
}

void wifi_connect(void)
{
	int nr_tries = 10;
	int ret = 0;

	net_mgmt_init_event_callback(&wifi_shell_mgmt_cb, wifi_mgmt_event_handler, NET_EVENT_WIFI_CONNECT_RESULT);
	// net_mgmt_init_event_callback(	&ipv4_mgmt_cb,
	// 			     				wifi_mgmt_event_handler,
	// 			     				NET_EVENT_WIFI_CONNECT_RESULT);


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


	connected = 0;

	printf("WIFI try connecting to %s...\n", WIFI_SSID);

	/* Let's wait few seconds to allow wifi device be on-line */
	while (nr_tries-- > 0) {
		ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params,
			       sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			break;
		}

		printf("Connect request failed %d. Waiting iface be up...\n", ret);
		k_msleep(500);
	}

	while (connected == 0) {
		k_msleep(100);
	}
}

static void response_cb(struct http_response *rsp, enum http_final_call final_data, void *user_data){
	if(final_data == HTTP_DATA_MORE){
		printk("Partial data received (%zd bytes)\n", rsp->data_len);
	}else if (final_data == HTTP_DATA_FINAL){
		printk("All the data received (%zd bytes)\n", rsp->data_len);
	}
	printk("Response status %s\n", rsp->http_status);
	printk("Response to %s\n", (const char *)user_data);
}

void send_http_post(void){
	// setup socket
	int ret =0;
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;//ipv4
	server_addr.sin_port = htons(HTTP_SERVER_PORT);
	inet_pton(AF_INET, HTTP_SERVER_ADDR, &server_addr.sin_addr);//set server address
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0){
		printk("Cannot create socket\n");
		return;
	}
	ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0)
	{
		printk("Cannot connect to HTTP server\n");
	}
	
	// setup http request
	struct http_request req;

	memset(&req, 0, sizeof(req));

	req.method = HTTP_POST;
	req.url = HTTP_URL;
	req.host = HTTP_SERVER_ADDR;
	req.protocol = "HTTP/1.1";
	req.payload = "Hello from esp32!";
	req.payload_len = strlen(req.payload);
	req.response = response_cb;
	req.recv_buf = recv_buf;
	req.recv_buf_len = sizeof(recv_buf);

	ret = http_client_req(sock, &req, 3 * MSEC_PER_SEC, "HTTP POST");

	close(sock);
}

int main(void){
	
	wifi_connect();
	wifi_status();
	send_http_post();

	// printk("successed\n");

	return 0;
}