#pragma once

#include "../controllers/RemotePlayerController.hpp"
#include "../controllers/RemoteServerController.hpp"
#include "Message.hpp"
#include "Network.hpp"
#include <SFML/Network.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

class SfmlNetExchange : public NetExchange {
  private:
    const std::shared_ptr<sf::TcpSocket> client;

    // размерность MessageHeader::totalLenght  = ushort,т.ч. точно хватит
    const int buf_max_size = 0xffff;
    unsigned char buffer[0xffff];
    size_t buf_size = 0;

    Message readMessage();
    bool findHeader(size_t* idx, size_t length, MessageHeader** header);

  protected:
    void Send(const Message& msg) override;
    Message Receive() override;

  public:
    SfmlNetExchange(std::shared_ptr<sf::TcpSocket> client)
        : client{client} {

        this->client->setBlocking(false);
    }

    ~SfmlNetExchange() {}
};

class SftmTcpGameServer : public RemotePlayerProvider {
  private:
    const sf::IpAddress ip;
    const unsigned short port;
    sf::TcpListener listener;
    std::shared_ptr<sf::TcpSocket> client;

  public:
    SftmTcpGameServer(const char* ip, unsigned short port)
        : ip{ip}, port{port} {
        listener.setBlocking(false);

        std::cout << "create SftmTcpGameServer(" << ip << ":" << port << ")"
                  << std::endl;

        if (listener.listen(port) != sf::Socket::Done) {
            throw net_exception();
        }
    }

    ~SftmTcpGameServer() override {
        listener.close();
        RemotePlayerProvider::~RemotePlayerProvider();
    }

    std::shared_ptr<PlayerController> AcceptClient() override {
        if (!client) {
            client = std::make_shared<sf::TcpSocket>();
        }

        switch (listener.accept(*client)) {
        case sf::Socket::Done:
            std::cout << "Connection received" << std::endl;
            return std::make_shared<RemotePlayerController>(
                std::make_shared<SfmlNetExchange>(client));

        case sf::Socket::NotReady:
            return nullptr;

        default:
            throw net_exception();
        }
    }
};

class SftmTcpGameClient : public RemotePlayerProvider {
  private:
    const sf::IpAddress ip;
    const unsigned short port;
    std::shared_ptr<sf::TcpSocket> client;
    std::thread read_thread;

  public:
    SftmTcpGameClient(const char* ip, unsigned short port)
        : ip{ip}, port{port} {}

    ~SftmTcpGameClient() override {
        RemotePlayerProvider::~RemotePlayerProvider();
    }

    std::shared_ptr<PlayerController> AcceptClient() override {
        static std::chrono::milliseconds timespan(1000);
        static sf::Time sf_time{};

        if (!client) {
            client = std::make_shared<sf::TcpSocket>();
            // client->setBlocking(false);
        }

        sf::Socket::Status status = client->connect(ip, port);
        if (status == sf::Socket::Done) {
            std::cout << "Client connected" << std::endl;
            // client->setBlocking(false);
            return std::make_shared<RemoteServerController>(
                std::make_shared<SfmlNetExchange>(client));
        }

        std::cout << "Client connecting ..., "
                  << "status: " << (int)status << std::endl;

        std::this_thread::sleep_for(timespan);
        return nullptr;
    }
};

void SfmlNetExchange::Send(const Message& msg) {
    size_t total_sended = 0;
    size_t sended = 0;

    while (total_sended < msg.lenght) {
        auto res = client->send((void*)(msg.message + total_sended),
                                msg.lenght - total_sended, sended);

        switch (res) {
        case sf::Socket::Done:
            break;

        case sf::Socket::NotReady:
            break;

        case sf::Socket::Partial:
            break;

        default:
            return;
        }

        total_sended += sended;
    }
}

Message SfmlNetExchange::Receive() {
    auto last_message = readMessage();
    
    if (!last_message.IsEmpty()) {
        for (Message tmp;; tmp = readMessage()) {
            if (tmp.IsEmpty()) {
                return std::move(last_message);
            }
            last_message = std::move(tmp);
        }
    }

    return std::move(last_message);
}

Message SfmlNetExchange::readMessage() {
    size_t received = 0;
    auto res =
        client->receive(buffer + buf_size, buf_max_size - buf_size, received);

    if (res != sf::Socket::Done) {
        return Message{};
    }

    auto total_received = received + buf_size;
    MessageHeader* header = nullptr;
    size_t idx = 0;

    if (!findHeader(&idx, total_received, &header)) {
        // не нашли header, какой-то мусор, ищем дальше
        buf_size = 0;
        return Message{};
    }

    if (header->totalLenght > (total_received - idx)) {
        // не весь пакет еще получен, ждем дальше
        return Message{};
    }

    auto msg = Message(buffer + idx, header->totalLenght);
    idx += msg.lenght;

    buf_size = total_received - idx;
    memcpy(buffer, buffer + idx, buf_size);

    return msg;
}

bool SfmlNetExchange::findHeader(size_t* idx, size_t length,
                                 MessageHeader** header) {
    auto local_idx = *idx;
    for (; local_idx + sizeof(MessageHeader) < length; ++local_idx) {
        auto tmp_header = (MessageHeader*)(buffer + local_idx);
        if (tmp_header->prefix == headerPrefix) {
            *idx = local_idx;
            *header = tmp_header;
            return true;
        }
    }

    return false;
}