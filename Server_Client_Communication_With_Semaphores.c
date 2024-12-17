#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
/*
*@author Brook Gebreeyesus 
*This code will simulate a handshaking procedure in the network between a client and server
*/

// declaring the client and server semaphore values
sem_t semaphoreClient;
sem_t semaphoreServer;

void *client(void *args)
{   
    //printing the client sending the connection signal first
    printf("client sends connection signal to server\n");
    sleep(1);
    //posting the client and waiting for the server before the client can print recieved acknowlegment
    sem_post(&semaphoreClient);
    sem_wait(&semaphoreServer);
    printf("Client recieved acknowlegement\n");
    sleep(1);
    //posting client so that it prints sending the final confirmation before the server printing recieving the final confirmation
    sem_post(&semaphoreClient);
    printf("Client sending final confirmation\n");
    sleep(1);
}

void *server(void *args)
{   
    //printing server recieved connection signal after client sent the signal
    printf("Server recieved connection signal\n");
    sleep(1);
    sem_wait(&semaphoreClient);
    printf("Server sending acknowlegement\n");
    sleep(1);
    //posting server and waiting for client before server can print revieved final confirmation from client
    sem_post(&semaphoreServer);
    sem_wait(&semaphoreClient);
    sleep(1);
    printf("Server Recieved final confirmation\n");
}

int main(int argc, char *argv[])
{

    //declaring client and server threads
    pthread_t clientThread;
    pthread_t serverThread;

    //intializing the semaphore values
    sem_init(&semaphoreClient, 0, 0);
    sem_init(&semaphoreServer, 0, 0);

        // creating threads to pass both the client and server 
        if (pthread_create(&clientThread, NULL, &client, NULL) != 0)
        {
            perror("failed to create thread");
        }
        sleep(1);
        if (pthread_create(&serverThread, NULL, &server, NULL) != 0)
        {
            perror("failed to create thread");
        }
    
        if (pthread_join(clientThread, NULL) != 0)
        {
            perror("failed to create thread");
        }
        if (pthread_join(serverThread, NULL) != 0)
        {
            perror("failed to create thread");
        }
    //destroying client and server semaphores at the end to free up resources
    sem_destroy(&semaphoreClient);
    sem_destroy(&semaphoreServer);
    return 0;
}