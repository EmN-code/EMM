/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <errno.h>
#include <stdio.h>

#include <zephyr/net/wifi_mgmt.h>

//wifi

#define WIFI_SSID "EmNets-301"
#define WIFI_PSK "eagle402"

static int connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		printf("Connection request failed (%d)\n", status->status);
	} else {
		printf("WIFI Connected\n");
		connected = 1;
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
		.ssid_length = 0,
		.psk = WIFI_PSK,
		.psk_length = 0,
		.channel = 0,
		.security = WIFI_SECURITY_TYPE_PSK,
	};

	cnx_params.ssid_length = strlen(WIFI_SSID);
	cnx_params.psk_length = strlen(WIFI_PSK);

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


//http 
#include <zephyr/net/socket.h>
#include <zephyr/net/socketutils.h>
#include <zephyr/net/http/client.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/dns_resolve.h>

#define HTTP_PORT "14008"
#define HTTP_HOST "10.214.131.229"   
#define HTTP_URL "/data"

static const char *http_headers[] = {
	"Content-Type: application/json\r\n",
	"_ssl: false\r\n",
	NULL
};

struct context {
	int sock;
	uint8_t payload[1280];
	uint8_t resp[1280];
};
static struct context ctx;

static void response_cb(struct http_response *rsp,
			enum http_final_call final_data,
			void *user_data)
{
	if (final_data == HTTP_DATA_MORE) {
		printf("Partial data received (%zd bytes)", rsp->data_len);
	} else if (final_data == HTTP_DATA_FINAL) {
		printf("All the data received (%zd bytes)", rsp->data_len);
	}

	printf("Response status %s", rsp->http_status);
}

int socket_connect(struct context *ctx){
	struct addrinfo *addr;
	struct addrinfo hints;
	char hr_addr[INET6_ADDRSTRLEN];
	char *port;
	int dns_attempts = 3;
	int ret = -1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (IS_ENABLED(CONFIG_NET_IPV6)) {
		hints.ai_family = AF_INET6;
	} else if (IS_ENABLED(CONFIG_NET_IPV4)) {
		hints.ai_family = AF_INET;
	}
	port = HTTP_PORT;
	while(dns_attempts--){
		ret = getaddrinfo(HTTP_HOST, port, &hints, &addr);
		if (ret == 0) {
			break;
		k_sleep(K_SECONDS(1));
		}
	}
	if (ret < 0) {
		printf("Could not resolve dns, error: %d", ret);
		return ret;
	}

	ctx->sock = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
	if (ctx->sock < 0) {
		printf("socket() failed: %d\n", errno);
		// freeaddrinfo(addr);
		return -1;
	}
	if (connect(ctx->sock, addr->ai_addr, addr->ai_addrlen) < 0) {
		printf("connect() failed: %d\n", errno);
		// freeaddrinfo(addr);
		return -1;
	}
	// freeaddrinfo(addr);
	return 0;
}

int http_push(struct context *ctx,http_response_cb_t resp_cb){
	struct http_request req;
	int ret;

	memset(&req, 0, sizeof(req));

	req.method = HTTP_POST;
	req.host = HTTP_HOST;
	req.url = HTTP_URL;
	req.header_fields = http_headers;
	req.protocol = "HTTP/1.1";
	req.response = resp_cb;
	req.payload = ctx->payload;
	req.payload_len = strlen(ctx->payload);
	req.recv_buf = ctx->resp;
	req.recv_buf_len = sizeof(ctx->resp);

	ret = http_client_req(ctx->sock, &req, 1000 * MSEC_PER_SEC, ctx);
	close(ctx->sock);
	ctx->sock = -1;
	return -1;
}

int main(void)
{
	wifi_connect();
	const char *message = "Hello";
	strcpy(ctx.payload,message);
	socket_connect(&ctx);
	http_push(&ctx,response_cb);

	return 0;
}
