#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(coap_client);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_BUFFER_SIZE 256
#define COAP_TOKEN "token"
#define COAP_TOKEN_MAX_LEN 8
#define COAP_PATH "/coap/test"

void main(void) {
    struct sockaddr_in server;
    struct coap_packet request;
    uint8_t coap_buf[COAP_BUFFER_SIZE];
    int sock;

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

    coap_packet_init(&request, coap_buf, sizeof(coap_buf),
                     COAP_VERSION_1, COAP_TYPE_CON, COAP_TOKEN_MAX_LEN,
                     (uint8_t *)COAP_TOKEN, COAP_METHOD_POST, 0);

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, 
                                (uint8_t *)COAP_PATH, strlen(COAP_PATH));

    const char payload[] = "Hello, COAP Cloud";
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    if (sendto(sock, coap_packet_get_data(&request), coap_packet_get_total_size(&request),
               0, (struct sockaddr *)&server, sizeof(server)) < 0) {
        LOG_ERR("Failed to send CoAP request");
    } else {
        LOG_INF("CoAP request sent successfully");
    }

    close(sock);
}