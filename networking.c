#include <stdbool.h>
#include <unistd.h> 
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>


#include "networking.h"
#include "defines.h"
#include "game.h"


void serialize(const DataPacket *data, char *buffer) {
    memcpy(buffer, data, sizeof(DataPacket));
}

void deserialize(const char *buffer, DataPacket *data) {
    memcpy(data, buffer, sizeof(DataPacket));
}

void *client_func(void *data) {
    Game *game = (Game*)data;
    int sock;
    struct sockaddr_in server_addr;
    char buffer[sizeof(DataPacket)];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR: socket creation failed");
        return NULL;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("ERROR: socket connection failed");
        close(sock);
        return NULL;
    }
    Point p = {.x = 69, .y = 13};
    DataPacket send_data = { .player_id = game->players[0]->id,  .direction = p};
    DataPacket recv_data; 

    while (true) {
        serialize(&send_data, buffer);
        send(sock, buffer, sizeof(DataPacket), 0);
        printf("%u sent\n", send_data.player_id);

        recv(sock, buffer, sizeof(DataPacket), 0);
        deserialize(buffer, &recv_data);

        printf("%u connected\n", recv_data.player_id);


        sleep(1);
    }
    return NULL;
}


void start_networking(Game *game) {
    pthread_t tcp_client_thread;
    if (pthread_create(&tcp_client_thread, NULL, client_func, game) != 0) {
        printf("ERROR: thread create failed");
    }
}