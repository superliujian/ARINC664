#include "tomiddleware.h"

SOCKET client_socket;

int Init_664_Work_Mode(void) {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa)) {
        fprintf(stderr, "WSAStartup failed, error code is %d.\n", WSAGetLastError());
        return -1;
    }

    client_socket = socket(AF_INET, SOCK_STREAM , 0);    

    if (client_socket  == INVALID_SOCKET) {
        fprintf(stderr, "Create socket failed, error code is %d.\n", WSAGetLastError());
        return -1;
    }

    SOCKADDR_IN serveraddr;   
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(10087);
    serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");   

    if (connect(client_socket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) != 0) {
        fprintf(stderr, "Connect failed, error code is %d.\n", WSAGetLastError());
        return -1;
    }

    fprintf(stdout, "Socket_init ok\n");

    return 0;
}

int Send_To_664_Middleware(unsigned char *sendbuf, int size) {
    int len = send(client_socket, (char *)sendbuf, size, 0);

    if (len == size) {
        fprintf(stdout, "Socket send sucessfully, send %d bytes.\n", len);
        return len;
    } else {
        fprintf(stderr, "Socket send failed, size = %d, sended len = %d!\n", size, len);
        return 0;
    }
}

int Recv_Data_From_664_Middleware(unsigned char *recvbuf, int size) {
    fd_set rfds;
    FD_ZERO(&rfds);                
    FD_SET(client_socket, &rfds);  

    struct timeval tv = {0};             

    int retval = select(0, &rfds, NULL, NULL, &tv);   
    if (retval == -1) {
        fprintf(stderr, "Socket recv select failed, error code is %d\n.", WSAGetLastError());
        return -1;
    }
    else if (retval == 0) {
        return 0;
    } else {
        int len = 0;

        if (FD_ISSET(client_socket, &rfds)) {
            memset(recvbuf, 0, size);

            len = recv(client_socket, (char *)recvbuf, size, 0);
            if (len > 0) {
                fprintf(stdout, "Socket recv sucessfully, recv %d size!\n", len);
            } else {
                fprintf(stderr, "Socket recv failed, error code is %d.\n", WSAGetLastError());
            }
        }

        return len;
    }
}

int Recv_Mib_Info_From_664_Middleware(unsigned char *recvbuf, int size) {
    int len = recv(client_socket, (char *)recvbuf, size, 0);
    if (len > 0) {
        fprintf(stdout, "Socket recv successfully, recv %d size!\n", len);
    } else {
        fprintf(stderr, "Socket recv failed, error code is %d.\n", WSAGetLastError());
    }
    return len;
}
