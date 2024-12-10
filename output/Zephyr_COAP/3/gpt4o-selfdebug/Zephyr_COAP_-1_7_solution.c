#include <net/socket.h>
#include <net/coap.h>
#include <net/coap_packet.h>
#include <zephyr.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static int send_coap_post_request(void)
{
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[128];
    int ret;

    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    /* Initialize CoAP packet */
    ret = coap_packet_init(&request, request_buffer, sizeof(request_buffer),
                           1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, 0);
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    /* Set URI path */
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                    (uint8_t *)COAP_PATH, strlen(COAP_PATH));
    if (ret < 0) {
        printk("Failed to set URI path\n");
        close(sock);
        return -1;
    }

    /* Add payload */
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to set payload marker\n");
        close(sock);
        return -1;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD,
                                     strlen(COAP_PAYLOAD));
    if (ret < 0) {
        printk("Failed to append payload\n");
        close(sock);
        return -1;
    }

    /* Send the CoAP request */
    ret = sendto(sock, request_buffer, coap_packet_get_len(&request), 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent to %s:%d\n", SERVER_ADDR, SERVER_PORT);

    close(sock);
    return 0;
}

void main(void)
{
    int ret = send_coap_post_request();
    if (ret < 0) {
        printk("Error sending CoAP request\n");
    } else {
        printk("CoAP POST request successful\n");
    }
}