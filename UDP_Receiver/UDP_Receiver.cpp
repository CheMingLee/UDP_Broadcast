﻿#include <winsock2.h>
#include <iostream>
#include <conio.h>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define MYPORT 10000    // the port users will be connecting to

int main()
{
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    SOCKET sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    char broadcast = '1';

    // This option is needed on the socket in order to be able to receive broadcast messages

    // If not set the receiver will not receive broadcast messages in the local network.
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        cout << "Error in setting Broadcast option";
        closesocket(sock);
        return 0;
    }

    struct sockaddr_in Recv_addr;
    struct sockaddr_in Sender_addr;

    int len = sizeof(struct sockaddr_in);
    
    char recvbuff[50];
    int recvbufflen = 50;
    //char sendMSG[] = "Broadcast message from READER";
    char sendMSG[256];

    char hostName[256];
    char IPName[256];
    struct hostent* pHost;
    struct sockaddr_in SocketAddress;

    //Get Host IP
    gethostname(hostName, sizeof(hostName));
    pHost = gethostbyname(hostName);
    memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[1], pHost->h_length);
    strcpy(IPName, inet_ntoa(SocketAddress.sin_addr));
    cout << "Address: " << IPName << endl;
    strcpy(sendMSG, IPName);

    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(MYPORT);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
    {
        cout << "Error in BINDING" << WSAGetLastError();
        _getch();
        closesocket(sock);
        return 0;
    }

    recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Sender_addr, &len);

    cout << "\n\n\tReceived Message is : " << recvbuff;
    cout << "\n\n\tPress Any to send message";
    _getch();

    if (sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr*)&Sender_addr, sizeof(Sender_addr)) < 0)
    {
        cout << "Error in Sending." << WSAGetLastError();
        cout << "\n\n\t\t Press any key to continue....";
        _getch();
        closesocket(sock);
        return 0;
    }
    else
        cout << "\n\n\n\tREADER sends the broadcast message Successfully";

    cout << "\n\n\tpress any key to CONT...";
    _getch();

    closesocket(sock);
    WSACleanup();
}
