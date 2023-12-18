#pragma once

#include "Player.hpp"
#include "controllers/KeyboardPlayerController.hpp"
#include "controllers/RemotePlayerController.hpp"
#include "device/Device.hpp"
#include "device/DrawDevice.hpp"
#include "device/InputDevice.hpp"
#include "network/Network.hpp"
#include <map>
#include <memory>

class Application {
  private:
    const ServerTypes type;
    std::shared_ptr<RemotePlayerProvider> remotePlayerProvider;
    Players players;
    std::unique_ptr<Device> device;
    std::shared_ptr<DrawDevice> draw_device;
    std::shared_ptr<InputDevice> input_device;

  public:
    Application(ServerTypes type, const char* ip, uint16_t port,
                uint8_t local_players);

    void Run();

  private:
    void createLocalPlayers(uint8_t local_players);
    void createServerPlayers();
    void createClientPlayers();

    PlayerPtr createLocalPlayer(uint8_t index);
    void registerKeyboardController(PlayerControllerPtr controller);
    RemotePlayerProvider* getRemotePlayerProvider(const char* ip,
                                                  uint16_t port);
    void processEvents();
};