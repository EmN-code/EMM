#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/kernel.h>

#define COAP_MAX_MSG_LEN 256
#define COAP_PORT 5683

static int send_coap_request(struct sockaddr_in *addr) {
    int sock;
    char data[COAP_MAX_MSG_LEN];
    struct coap_packet request;

    /* Create UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return -1;
    }

    /* Initialize CoAP packet */
    if (coap_packet_init(&request, data, sizeof(data), 1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id()) < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    /* Append URI path */
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)"coap", strlen("coap"));
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)"test", strlen("test"));

    /* Append payload */
    coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    /* Send CoAP request */
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        printk("Failed to send CoAP message\n");
        close(sock);
        return -1;
    }

    close(sock);
    return 0;
}

int main(void) {
    struct sockaddr_in addr;

    printk("Starting CoAP client example\n");

    /* Set server address */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, "47.102.103.1", &addr.sin_addr);

    while (1) {
        send_coap_request(&addr);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
