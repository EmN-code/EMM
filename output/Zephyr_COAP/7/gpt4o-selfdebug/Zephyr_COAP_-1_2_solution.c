#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[256];
    int ret;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Error creating socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    ret = coap_packet_init(&request, buffer, sizeof(buffer),
                           1, COAP_TYPE_CON, 0,
                           COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Error initializing CoAP packet\n");
        close(sock);
        return;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                    COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (ret < 0) {
        printk("Error appending option\n");
        close(sock);
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Error appending payload marker\n");
        close(sock);
        return;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    if (ret < 0) {
        printk("Error appending payload\n");
        close(sock);
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Error sending packet\n");
    } else {
        printk("CoAP POST sent successfully\n");
    }

    close(sock);
}