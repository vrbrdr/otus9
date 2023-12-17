#include "Application.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    // s 127.0.0.1:34765
    // c 127.0.0.1:34765
    auto stype = ServerTypes::LOCAL;
    const char* ip = "127.0.0.1";
    uint16_t port = 34765;

    if (argc > 1) {
        auto arg1 = toupper(argv[1][0]);
        if (arg1 == 'S') {
            stype = ServerTypes::SERVER;
            std::cout << "Server:\\" << ip << ":" << port << std::endl;

        } else if (arg1 == 'C') {
            stype = ServerTypes::CLIENT;
            std::cout << "Client:\\" << ip << ":" << port << std::endl;
        }
    }

    Application app(stype, ip, port, (uint8_t) 1);
    app.Run();

    std::cout << "Exit" << std::endl;
}
