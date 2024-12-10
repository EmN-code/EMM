#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t *data;
    ssize_t bytes_sent;
    const char *uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";

    /* Initialize socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Initialize CoAP packet */
    coap_packet_init(&request, data, sizeof(data), 1, COAP_TYPE_CON, 
                     COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, 
                              strlen(uri_path));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    /* Send CoAP packet */
    bytes_sent = sendto(sock, request.data, request.offset, 0,
                        (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bytes_sent < 0) {
        printk("Failed to send data\n");
    }

    close(sock);
}