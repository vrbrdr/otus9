#include "Application.hpp"
#include <iostream>
#include<stdexcept>

int main(int argc, char* argv[]) {
    // s 127.0.0.1:34765
    // c 127.0.0.1:34765
    auto stype = ServerTypes::LOCAL;
    const char* ip = "127.0.0.1";
    uint16_t port = 34765;
    uint8_t locals=1;

    if (argc > 1) {
        auto arg1 = toupper(argv[1][0]);
        if (arg1 == 'S') {
            stype = ServerTypes::SERVER;
            std::cout << "Server:\\" << ip << ":" << port << std::endl;

        } else if (arg1 == 'C') {
            stype = ServerTypes::CLIENT;
            locals = 0;
            std::cout << "Client:\\" << ip << ":" << port << std::endl;
        }else if (arg1 == '1') {
            stype = ServerTypes::LOCAL;
            locals = 1;
        }else if (arg1 == '2') {
            stype = ServerTypes::LOCAL;
            locals = 1;
        }

    }

    try {
        Application app(stype, ip, port, locals);
        app.Run();
    } catch (const std::exception& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    std::cout << "Exit" << std::endl;
}
