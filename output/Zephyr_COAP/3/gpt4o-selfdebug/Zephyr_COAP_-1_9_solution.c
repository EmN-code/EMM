#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define COAP_VERSION 1

static struct coap_block_context blk_ctx;
static struct sockaddr_storage dst;
static int sock;

void send_coap_request(void) {
    struct coap_packet request;
    uint8_t req_data[128];
    int ret;
    struct sockaddr_in *server_addr = (struct sockaddr_in *)&dst;

    /* Prepare server address */
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr->sin_addr);

    /* Open socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    /* Initialize CoAP packet */
    ret = coap_packet_init(&request, req_data, sizeof(req_data), COAP_VERSION, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to init CoAP packet: %d\n", ret);
        return;
    }

    /* Set URI path */
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI_PATH option: %d\n", ret);
        return;
    }

    /* Set payload */
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker: %d\n", ret);
        return;
    }
    ret = coap_packet_append_payload(&request, (uint8_t *)PAYLOAD, strlen(PAYLOAD));
    if (ret < 0) {
        printk("Failed to append payload: %d\n", ret);
        return;
    }

    /* Send CoAP request */
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&dst, sizeof(dst));
    if (ret < 0) {
        printk("Failed to send CoAP request: %d\n", ret);
    }

    printk("CoAP request sent\n");
}

int main(void) {
    printk("Starting CoAP client\n");
    send_coap_request();
    return 0;
}