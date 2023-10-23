#include "mqtt.h"
#include <netdb.h>
#include <stdio.h>
#include "posix_sockets.h"
#include <unistd.h>

extern inline double decode(long microVolt) { return (microVolt/(2.4*1000000)-2.5); }

#define HEXSIZE 6

long hexStringToNum(const char hexstring[]) {

    return -1;
}

void publish_callback(void** unused, struct mqtt_response_publish *published)
{
    /* not used in this example */
}

void* client_refresher(void* client)
{
    while(1)
    {
        mqtt_sync((struct mqtt_client*) client);
        usleep(100000U);
    }
    return NULL;
}

int main() {
    const char* addr = "10.42.0.99";
    const char* port = "1883";
    const char* topic = "KXR";

    // Read in 6-characters
    char hexString[HEXSIZE + 1];

    // time delta is 10 ms
    int timeDelta = 10;

    int curTimeMilliseconds = 0;

    /* open the non-blocking TCP socket (connecting to the broker) */
    int sockfd = open_nb_socket(addr, port);

    if (sockfd == -1) {
        perror("Failed to open socket: ");
        return -1;
    }

    /* setup a client */
    struct mqtt_client client;
    uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
    uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */
    mqtt_init(&client, sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);
    /* Create an anonymous session */
    const char* client_id = NULL;
    /* Ensure we have a clean session */
    uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    /* Send connection request to the broker. */
    mqtt_connect(&client, client_id, NULL, NULL, 0, "KXR", "kxr", connect_flags, 400);

    /* check that we don't have any errors */
    if (client.error != MQTT_OK) {
        fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
        return -1;
    }

    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    pthread_t client_daemon;
    if(pthread_create(&client_daemon, NULL, client_refresher, &client)) {
        fprintf(stderr, "Failed to start client daemon.\n");
        return -1;
    }

    char application_message[256];
    strcpy(application_message, "Hip Hip Hooray");
    while(1) {
        mqtt_publish(&client, topic, application_message, strlen(application_message) + 1, MQTT_PUBLISH_QOS_0);

        usleep(10000);
    }

    printf("Pub\n");



    return 0;
}
