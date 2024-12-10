#include <zephyr/net/coap.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>

LOG_MODULE_REGISTER(coap_client);

#define COAP_BUF_SIZE 128
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct sockaddr_in server_addr;
static int sock;

void main(void) {
    struct coap_packet request;
    uint8_t coap_buf[COAP_BUF_SIZE];
    int r;

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return;
    }

    /* Set server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Initialize CoAP request */
    r = coap_packet_init(&request, coap_buf, sizeof(coap_buf), COAP_VERSION_1, COAP_TYPE_CON,
                         8, coap_next_id(), COAP_METHOD_POST, NULL);
    if (r < 0) {
        LOG_ERR("Failed to initialize CoAP request: %d", r);
        close(sock);
        return;
    }

    /* Append URI path */
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        LOG_ERR("Failed to append URI path to CoAP request: %d", r);
        close(sock);
        return;
    }

    /* Set payload */
    const char payload[] = "Hello, COAP Cloud";
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        LOG_ERR("Failed to append payload marker: %d", r);
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (const uint8_t *)payload, sizeof(payload) - 1);
    if (r < 0) {
        LOG_ERR("Failed to append payload: %d", r);
        close(sock);
        return;
    }

    /* Send CoAP request */
    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        LOG_ERR("Failed to send CoAP request: %d", r);
    } else {
        LOG_INF("CoAP request sent successfully");
    }

    close(sock);
}