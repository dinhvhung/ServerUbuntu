#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib> // Thư viện để chạy lệnh system()
using namespace std;

// --- HÀM MỚI 1: Đếm số dòng trong một file ---
int countLines(const string& filename) {
    ifstream file(filename);
    int count = 0;
    string line;
    while (getline(file, line)) {
        // Chỉ đếm nếu dòng không trống (tùy vào cấu trúc file .agents của bạn)
        if (!line.empty()) {
            count++;
        }
    }
    return count;
}

// --- HÀM MỚI 2: Tạo file JSON config ---
void createJsonConfig(const string& mapPath, const string& agentPath, const string& taskPath) {
    // 1. Đếm số lượng agent
    int teamSize = countLines(agentPath);

    // 2. Tạo nội dung JSON
    ofstream fJson("config.json");
    fJson << "{\n";
    fJson << "    \"mapFile\": \"" << mapPath << "\",\n";
    fJson << "    \"agentFile\": \"" << agentPath << "\",\n";
    fJson << "    \"teamSize\": " << teamSize << ",\n";
    fJson << "    \"taskFile\": \"" << taskPath << "\",\n";
    fJson << "    \"version\": \"2024 LoRR\"\n";
    fJson << "}";

    fJson.close();
    cout << "Da tao file config.json voi teamSize: " << teamSize << endl;
}

// --- HÀM MỚI: Xóa bỏ khoảng trắng và ký tự xuống dòng ở đầu/cuối chuỗi ---
string trim(const string& s) {
    size_t first = s.find_first_not_of(" \n\r\t");
    if (string::npos == first) return "";
    size_t last = s.find_last_not_of(" \n\r\t");
    return s.substr(first, (last - first + 1));
}

// Hàm hỗ trợ tách chuỗi theo dấu phân cách
vector<string> split(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = s.find(delimiter, prev);
        if (pos == string::npos) pos = s.length();
        string token = s.substr(prev, pos - prev);
        tokens.push_back(token);
        prev = pos + delimiter.length();
    } while (pos < s.length() && prev < s.length());
    return tokens;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 1. Tạo Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 2. Gắn Socket vào Port 8080
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Lắng nghe ở mọi IP (WSL/VM)
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server dang doi ket noi tai port 8080..." << std::endl;

    // 3. Chấp nhận kết nối
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout << "Da ket noi thanh cong!" << std::endl;

    // 4. Đọc tin nhắn
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        // 4. Đọc tin nhắn
        int valread = recv(new_socket, buffer, 1024, 0);

        if (valread <= 0) {
                std::cout << "Client da ngat ket noi!" << std::endl;
                break;
        }

        std::cout << "Tin nhan tu Unity: " << buffer << std::endl;

        string receivedData(buffer, valread);
        vector<string> parts = split(receivedData, "---");

        if (parts.size() >= 3) {
            // Ghi file .map
            ofstream fMap("data.map");
            fMap << trim(parts[0]);
            fMap.close();

            // Ghi file .agents
            ofstream fAgents("data.agents");
            fAgents << trim(parts[1]);
            fAgents.close();

            // Ghi file .task
            ofstream fTask("data.task");
            fTask << trim(parts[2]);
            fTask.close();

            // --- GỌI HÀM TẠO CONFIG ---
            createJsonConfig("data.map", "data.agents", "data.task");

            cout << "Da cap nhat du lieu vao file map, agents, va task.\n" << endl;

        }
        // 5. Phản hồi lại
        const char *hello = "Chao Unity, minh da nhan duoc tin!";
        send(new_socket, hello, (int)strlen(hello), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
