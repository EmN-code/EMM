#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(coap_client, LOG_LEVEL_DBG);

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define REQUEST_URI "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static int send_coap_request(void)
{
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t payload[] = COAP_PAYLOAD;
    uint8_t token[8];
    int r;
    uint8_t *data;
    int data_len;

    /* Create socket */
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return -errno;
    }

    /* Set up server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    zsock_inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    /* Prepare CoAP request */
    r = coap_packet_init(&request, data, data_len, 1, COAP_TYPE_CON, sizeof(token),
                         token, COAP_METHOD_POST, coap_next_id());
    if (r) {
        LOG_ERR("Failed to initialize CoAP packet: %d", r);
        goto cleanup;
    }

    /* Append URI path option */
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, REQUEST_URI, strlen(REQUEST_URI));
    if (r) {
        LOG_ERR("Failed to append URI path: %d", r);
        goto cleanup;
    }

    /* Set payload */
    r = coap_packet_append_payload_marker(&request);
    if (r) {
        LOG_ERR("Failed to set payload marker: %d", r);
        goto cleanup;
    }

    r = coap_packet_append_payload(&request, payload, sizeof(payload) - 1);
    if (r) {
        LOG_ERR("Failed to set payload: %d", r);
        goto cleanup;
    }

    /* Send CoAP request */
    r = zsock_sendto(sock, data, data_len, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        LOG_ERR("Failed to send CoAP request: %d", errno);
        r = -errno;
    }

cleanup:
    zsock_close(sock);
    return r;
}

void main(void)
{
    LOG_INF("Starting CoAP client");
    
    int ret = send_coap_request();
    if (ret < 0) {
        LOG_ERR("Failed to send CoAP POST request");
    } else {
        LOG_INF("CoAP POST request sent successfully");
    }
}