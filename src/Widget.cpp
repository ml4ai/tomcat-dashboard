#include <chrono>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>

#include <boost/log/trivial.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include "mqtt/client.h"

#include "Widget.h"

using namespace std;

Widget::Widget(string type, string mqtt_host, string mqtt_port) {
  this->type = type;
  this->mqtt_host = mqtt_host;
  this->mqtt_port = mqtt_port;

  // Launch TrialListener
  trial_listener = make_unique<TrialListener>(mqtt_host, mqtt_port);

  // Start listening for Widget messages
  LoadConfig();
  Connect();
  mqtt_thread = thread([this] { this->Loop(); });
}

void Widget::LoadConfig() {
  fstream stream("config/config.json");
  string hold((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
  try {
    configuration = nlohmann::json::parse(hold)[type];

  } catch (exception e) {
    BOOST_LOG_TRIVIAL(error)
        << "Unable to load configuration for type: " << type;
  }

  if (this->configuration.contains("topics")) {
    topics = configuration["topics"].get<vector<string>>();
  } else {
    BOOST_LOG_TRIVIAL(error)
        << "Configuration missing topics list for type: " << type << std::endl;
    BOOST_LOG_TRIVIAL(error) << configuration.dump();
  }
}

void Widget::Connect() {
  string uuid = to_string(boost::uuids::random_generator()());
  string client_id = uuid + "_" + type;
  string server_address = "tcp://" + mqtt_host + ":" + mqtt_port;
  mqtt_client = make_unique<mqtt::client>(server_address, client_id);

  mqtt_client->start_consuming();

  auto connOpts =
      mqtt::connect_options_builder()
          .clean_session(true)
          .automatic_reconnect(chrono::seconds(2), chrono::seconds(30))
          .finalize();

  mqtt_client->connect(connOpts);
  Subscribe();
}

void Widget::Subscribe() {
  for (string topic : topics) {
    mqtt_client->subscribe(topic, 2);
  }
}

void Widget::Loop() {
  while (true) {
    auto msg = mqtt_client->consume_message();
    string payload = msg->get_payload_str();
    string topic = msg->get_topic();
    OnMessage(topic, payload);
  }
}

void Widget::PushUpdate(nlohmann::json update) {
  std::lock_guard<std::mutex> lock(update_mutex); // Lock mutex
  update_queue.push(update);
}

bool Widget::UpdateQueued() {
  std::lock_guard<std::mutex> lock(update_mutex); // Lock mutex
  return !update_queue.empty();
}

nlohmann::json Widget::GetUpdate() {

  std::lock_guard<std::mutex> lock(update_mutex); // Lock mutex
  nlohmann::json update = update_queue.front();
  update_queue.pop();

  return update;
}
