#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(coap_client);

#define COAP_BUF_SIZE 128
#define SERVER_PORT 5683
#define SERVER_ADDR "47.102.103.1"
#define COAP_URI_PATH "/coap/test"

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    uint8_t coap_buffer[COAP_BUF_SIZE];
    struct coap_packet request;
    const char payload[] = "Hello, COAP Cloud";

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    coap_packet_init(&request, coap_buffer, sizeof(coap_buffer), 1, COAP_TYPE_CON, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)COAP_URI_PATH, strlen(COAP_URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        LOG_ERR("Failed to send CoAP request");
        close(sock);
        return;
    }

    LOG_INF("CoAP POST request sent to %s:%d", log_strdup(SERVER_ADDR), SERVER_PORT);
    close(sock);
}