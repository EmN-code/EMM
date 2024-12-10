#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>

#define COAP_BUF_SIZE 128
#define COAP_MAX_MSG_LEN 256
#define COAP_HEADER_SIZE 4
#define COAP_MAX_REQUESTS 3

void main(void)
{
    struct sockaddr_in addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[COAP_BUF_SIZE];
    const char *coap_uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";
    struct coap_option options[2];

    /* Specify server address */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &addr.sin_addr);

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    /* Setup CoAP packet */
    coap_packet_init(&request, request_buffer, COAP_BUF_SIZE, COAP_VERSION_1, COAP_TYPE_CON, COAP_TOKEN_MAX_LEN, NULL, COAP_METHOD_POST, coap_next_id());

    /* Set CoAP options */
    coap_option_setup(options, &request, COAP_OPTION_URI_PATH, coap_uri_path);

    /* Set CoAP payload and send */
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    /* Send CoAP request */
    sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&addr, sizeof(addr));

    /* Close socket */
    close(sock);
}

// Ensure project's prj.conf has the following content to define missing configurations
// CONFIG_COAP_CLIENT_MESSAGE_HEADER_SIZE=64
// CONFIG_COAP_CLIENT_MESSAGE_SIZE=256
// CONFIG_COAP_CLIENT_MAX_REQUESTS=3