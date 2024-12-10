#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>

#define COAP_BUF_SIZE 256
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683

void main(void) {
    uint8_t coap_buf[COAP_BUF_SIZE];
    struct coap_packet request;
    struct sockaddr_in6 addr6;
    int sock;

    /* Create a socket */
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    /* Setup address */
    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET6, COAP_SERVER_IP, &addr6.sin6_addr);

    /* Initialize CoAP packet */
    coap_packet_init(&request, coap_buf, sizeof(coap_buf), 1, COAP_TYPE_CON, strlen(COAP_PAYLOAD), (const uint8_t *)COAP_PAYLOAD, COAP_METHOD_POST, coap_next_id());

    /* Add URI path to CoAP packet */
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (const uint8_t *)"coap", strlen("coap"));
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (const uint8_t *)"test", strlen("test"));

    /* Append payload to the CoAP packet */
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (const uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    /* Send CoAP packet */
    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr6, sizeof(addr6)) < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent\n");
    }

    /* Close socket */
    close(sock);
}