#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI "/coap/test"

void send_coap_post_request(void) {
    struct sockaddr_in server_addr;
    int sock;
    
    /* Create UDP socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    struct coap_packet request;
    uint8_t payload[] = "Hello, COAP Cloud";

    /* Initialize CoAP packet */
    uint8_t token[8];
    uint8_t *data = coap_packet_get_data(&request);
    int ret = coap_packet_init(&request, data, sizeof(data), 1, COAP_TYPE_CON, sizeof(token), token, COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to init CoAP packet\n");
        close(sock);
        return;
    }

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)COAP_URI, sizeof(COAP_URI) - 1);
    coap_packet_append_payload_marker(&request);

    /* Add payload */
    coap_packet_append_payload(&request, payload, sizeof(payload) - 1);

    /* Send Request */
    ret = sendto(sock, coap_packet_get_data(&request), coap_packet_get_data_len(&request), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}

void main(void) {
    printk("Starting CoAP client\n");
    send_coap_post_request();
}