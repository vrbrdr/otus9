#pragma once

#include "../network/Message.hpp"
#include "../network/Network.hpp"
#include <SFML/Network.hpp>

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
    SfmlNetExchange(std::shared_ptr<sf::TcpSocket> client);

    ~SfmlNetExchange() {}
};
