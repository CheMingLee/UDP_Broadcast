#include <winsock2.h>
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

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        cout << "Error in setting Broadcast option";
        closesocket(sock);
        return 0;
    }

    struct sockaddr_in Recv_addr;

    int len = sizeof(struct sockaddr_in);

    char sendMSG[] = "Broadcast message from SLAVE TAG";

    char recvbuff[50] = "";
    int recvbufflen = 50;

    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(MYPORT);
    Recv_addr.sin_addr.s_addr = INADDR_BROADCAST; // this isq equiv to 255.255.255.255
    // better use subnet broadcast (for our subnet is 172.30.255.255)
    //Recv_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr*)&Recv_addr, sizeof(Recv_addr));

    recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Recv_addr, &len);

    cout << "\n\n\tReceived message from Reader is =>  " << recvbuff;
    
    cout << "\n\n\tpress any key to CONT...";
    _getch();

    closesocket(sock);
    WSACleanup();
}
