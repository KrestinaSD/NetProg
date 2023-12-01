#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <memory> //unique_ptr
#include <string>
#include <unistd.h> // close
using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        cout << "Socket error\n";
        return 1;
    }

    /*// Создание адресной структуры для локального сервера
	struct sockaddr_in remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(12243); 
	inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); */

	// Создание адресной структуры для сервера кафедры
	struct sockaddr_in remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(13);
	inet_pton(AF_INET, "172.16.40.1", &remote_addr.sin_addr); 
    
    // отправляем пустой запрос на сервер
    int s = sendto(sock, nullptr, 0, 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr));
    if (s < 0){
 		cout << "Send error\n";
        return 1;
 	}
 	
 	
 	//получаем ответ от сервера
 	unique_ptr<char[]> daytime(new char[1024]);
 	int r = recv(sock, daytime.get(), 1024, 0);
    if (r == -1){
    	cout << "Receive error\n";
        return 1;
    }
    
    
    daytime[r] = 0;
    string r_mess(daytime.get(), r);
    cout << "Daytime from server: " << r_mess << endl;
    cout << "Session closed!" << endl;
    close(sock);
    return 0;
}
