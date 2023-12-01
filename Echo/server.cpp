#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#define buff_size 1024
using namespace std;

int main() {
    // Создание сокета
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        cout << "Socket creation error\n";
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in self_addr;
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(12241);
    self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Привязка сокета к адресу
    int b = bind(s,(const sockaddr*) &self_addr,sizeof(self_addr));
    if(b == -1) {
        cout << "Binding error\n";
        close(s);
        return 1;
    }

    // Перевод сокета в режим прослушивания
    int queue_len = 10;
    int l = listen(s,  queue_len);
    if (l == -1) {
        cout << "Listen error\n";
        close(s);
        return 1;
    }

    // Основной цикл обработки соединений
    while(true) {
        sockaddr_in client_addr;
        socklen_t len = sizeof (sockaddr_in);
        // Принятие входящего соединения
        int work_sock = accept(s, (sockaddr*)&client_addr, &len);
        if(work_sock == -1) {
            cout << "Accept error\n";
            continue;
        } 
        else {
            cout << "Successful client connection!\n";
            unique_ptr<char[]> message(new char[buff_size]);
            // Получение сообщения от клиента
            int recv_res = recv(work_sock, message.get(), sizeof(message), 0);
            if (recv_res == -1) {
                cout << "Receive error\n";
                close(work_sock);
                continue;
            }
            else if (recv_res == 0) {
                cout << "Client disconnected\n";
                close(work_sock);
                continue;
            }
            cout << "Message from client: " << '"'; 
            for (size_t i = 0; i < buff_size; i++) {
            		cout << message[i];
            		}
            cout<< '"' << endl;
            // Отправка сообщения клиенту
            int send_res = send(work_sock, message.get(), recv_res, 0);
            if (send_res == -1) {
                cout << "Send error\n";
                close(work_sock);
                continue;
            }
            cout << "Message was returned to client!\n";
        }
        // Закрытие сокета
        close(work_sock);
    }
    close(s);
    return 0;
}
