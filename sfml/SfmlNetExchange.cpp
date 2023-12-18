#include "SfmlNetExchange.hpp"

SfmlNetExchange::SfmlNetExchange(std::shared_ptr<sf::TcpSocket> client)
    : client{client} {

    this->client->setBlocking(false);
}

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
        for (Message tmp = readMessage();; tmp = readMessage()) {
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

    if (res != sf::Socket::Done && buf_size == 0) {
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