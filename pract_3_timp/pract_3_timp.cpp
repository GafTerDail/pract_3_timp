#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <io.h>
#include <winsock.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996) 

using namespace std;

void error(const char* x, const int y = 1)
{
    cerr << x << endl;
    exit(y);
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    sockaddr_in* Addr1 = new (sockaddr_in);
    Addr1->sin_family = AF_INET;      
    Addr1->sin_port = 0;             
    Addr1->sin_addr.s_addr = 0;       

    sockaddr_in* Addr2 = new (sockaddr_in);
    Addr2->sin_family = AF_INET; 
    Addr2->sin_port = htons(13);
    Addr2->sin_addr.s_addr = inet_addr("82.179.90.12");

    char* buf = new char[256];
    strcpy(buf, "Какая дата и время?\n"); 
    int msgLen = strlen(buf);        

    int mySocket = socket(AF_INET, SOCK_DGRAM, 0);  //UDP
    if (mySocket == -1) {
        error("Ошибка открытия сокета", 11);
    }

    int rc = bind(mySocket, (const sockaddr*)Addr1, sizeof(sockaddr_in));
    if (rc == -1) {
        _close(mySocket);
        error("Ошибка привязки сокета с локальным адресом", 12);
    }

    rc = connect(mySocket, (const sockaddr*)Addr2, sizeof(sockaddr_in));
    if (rc == -1) {
        _close(mySocket);
        error("Ошибка подключения сокета к удаленному серверу", 13);
    }

    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) { 
        _close(mySocket);
        error("Сообщение об ошибке отправки", 14);
    }
    cout << "Мы отправляем: " << buf << endl;
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) { 
        _close(mySocket);
        error("Ошибка получения ответа", 15);
    }

    cout << "Мы получаем: " << buf << endl;

    _close(mySocket);
    delete Addr1;
    delete Addr2;
    delete[] buf;
    return 0;
}
