#pragma once

#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "Mosquitto.h"

class MosquittoClient : public Mosquitto {
  public:
    MosquittoClient();
    ~MosquittoClient();
  
  protected:
    void on_message(const std::string& topic,
                    const std::string& message) override;

  private:
    // MQTT Client
    std::string mqtt_host = "localhost";
    int mqtt_port = 1883;
    std::thread loop_thread;
};
