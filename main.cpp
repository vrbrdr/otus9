#include "Application.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

void print_help() {
    using namespace std;
    cout << "available options: " << endl;
    cout << "l1"
         << " local game, single player" << endl;
    cout << "l2"
         << " local game, dual play, second keys: w a s d" << endl;
    cout << "s1 port [ip] (default ANY)"
         << " network servr game, single local player" << endl;
    cout << "s2 port [ip] (default ANY)"
         << " network servr game, dual local player, second keys: w a s d"
         << endl;
    cout << "c  port [ip] (default 127.0.0.1)"
         << " network  client game" << endl;
}

bool read_players(std::string& mode, uint8_t* local_players) {
    if (mode.size() != 2) {
        print_help();
        return false;
    }

    switch (mode[1]) {
    case '1':
        *local_players = 1;
        return true;

    case '2':
        *local_players = 2;
        return true;

    default:
        return false;
    }
}

bool read_options(int argc, char* argv[], ServerTypes* stype,
                  uint8_t* local_players, uint16_t* port, std::string* ip) {

    if (argc < 2) {
        return false;
    }

    std::string mode{argv[1]};
    // std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper);
    for (auto& c : mode) {
        c = (char)toupper((char)c);
    }

    bool read_local_count;
    bool read_network;
    switch (mode[0]) {
    case 'L':
        *stype = ServerTypes::LOCAL;
        read_local_count = true;
        read_network = false;
        break;

    case 'S':
        *stype = ServerTypes::SERVER;
        read_local_count = true;
        read_network = true;
        break;

    case 'C':
        *stype = ServerTypes::CLIENT;
        read_local_count = false;
        read_network = true;
        *local_players = 1;
        break;

    default:
        return false;
    }

    if (!read_local_count && mode.size() != 1 ||
        read_local_count && !read_players(mode, local_players)) {
        return false;
    }

    if (read_network) {
        if (argc != 3 && argc != 4) {
            return false;
        }

        auto iport = std::stoi(argv[2]);
        if (iport <= 0 || iport >= 0xFFFF) {
            std::cout << "Invalid port value: " << iport << std::endl;
            return false;
        }

        *port = (uint16_t)iport;
        if (argc == 4) {
            *ip = argv[3];
        }

    } else if (argc != 2) {
        return false;
    }

    if ((*ip).empty()) {
        if (*stype == ServerTypes::CLIENT) {
            *ip = "127.0.0.1";
        } else if (*stype == ServerTypes::SERVER) {
            *ip = "0.0.0.0";
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    ServerTypes stype;
    uint8_t local_players;
    uint16_t port;
    std::string ip;

    if (!read_options(argc, argv, &stype, &local_players, &port, &ip)) {
        print_help();
        return 1;
    }

    try {
        Application app(stype, ip.c_str(), port, local_players);
        app.Run();
    } catch (const std::exception& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    std::cout << "Exit" << std::endl;
    return 0;
}
