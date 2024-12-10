#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void send_coap_request(int sock)
{
    struct coap_packet request;
    uint8_t request_buffer[256];
    const char *uri_path = COAP_URI_PATH;
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    coap_packet_init(&request, request_buffer, sizeof(request_buffer), COAP_VERSION_1);
    coap_append_option_int(&request, COAP_OPTION_URI_PATH, htobe32(uri_path));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));

    sendto(sock, request_buffer, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

int main(void)
{
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    send_coap_request(sock);

    close(sock);

    printk("CoAP request sent\n");
    return 0;
}
