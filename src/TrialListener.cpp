#include <memory>
#include <string>
#include <thread>
#include <chrono>

#include <boost/log/trivial.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include "mqtt/client.h"
#include <nlohmann/json.hpp>

#include "TrialListener.h"

using namespace std;

TrialListener::TrialListener(string mqtt_host, string mqtt_port) {
  this->mqtt_host = mqtt_host;
  this->mqtt_port = mqtt_port;

  Connect();
  mqtt_thread = thread([this] { this->Loop(); });
}

string TrialListener::GetTrialId() {
  if (InTrial()) {
    std::string trial_id;
    try {
      trial_id = trial_message["msg"]["trial_id"];
      return trial_id;
    } catch (exception e) {
      BOOST_LOG_TRIVIAL(error)
          << "trial_id field is missing from trial message";
    }
  } else {
    BOOST_LOG_TRIVIAL(error) << "Can't access trial_id while not in trial";
  }
  return "";
}

string TrialListener::GetExperimentId() {
  if (InTrial()) {
    std::string experiment_id;
    try {
      experiment_id = trial_message["msg"]["experiment_id"];
      return experiment_id;
    } catch (exception e) {
      BOOST_LOG_TRIVIAL(error)
          << "experiment_id field is missing from trial message";
    }
  } else {
    BOOST_LOG_TRIVIAL(error) << "Can't access experiemnt_id while not in trial";
  }
  return "";
}

nlohmann::json TrialListener::GetTrialMessage() {
  if (InTrial()) {
    return trial_message;
  } else {
    BOOST_LOG_TRIVIAL(error) << "Can't access trial_message while not in trial";
  }
  return {};
}

vector<string> TrialListener::GetParticipants() {
  vector<string> participants;
  if (InTrial()) {
    try {
      vector<nlohmann::json> client_info =
          trial_message["data"]["client_info"].get<vector<nlohmann::json>>();
      for (nlohmann::json client : client_info) {
        participants.push_back(client["participant_id"]);
      }
    } catch (exception e) {
      BOOST_LOG_TRIVIAL(error) << "Trial start message missing client_info";
    }
  } else {
    BOOST_LOG_TRIVIAL(error)
        << "Can't access participant info while not in trial";
  }
  return participants;
}

bool TrialListener::InTrial() { return in_trial; }

void TrialListener::Connect() {
  string uuid = to_string(boost::uuids::random_generator()());
  string client_id = uuid + "_TrialListener";
  string server_address = "tcp://" + mqtt_host + ":" + mqtt_port;
  mqtt_client = make_unique<mqtt::client>(server_address, client_id);

  mqtt_client->start_consuming();

  auto connOpts = mqtt::connect_options_builder()
                        .clean_session(true)
                        .automatic_reconnect(chrono::seconds(2), chrono::seconds(30))
                        .finalize();

  mqtt_client->connect(connOpts);
  mqtt_client->subscribe("trial", 2);
}

void TrialListener::Loop() {
  while (true) {
    auto msg = mqtt_client->consume_message();
    auto payload = msg->get_payload_str();
    OnMessage(payload);
  }
}

void TrialListener::OnMessage(string message) {
  // Parse JSON message
  nlohmann::json obj;
  try {
    obj = nlohmann::json::parse(message);
  } catch (exception e) {
    BOOST_LOG_TRIVIAL(error) << "Failed to parse trial message";
  }

  // Read sub_type field
  string sub_type;
  try {
    sub_type = obj["msg"]["sub_type"];
  } catch (exception e) {
    BOOST_LOG_TRIVIAL(error) << "sub_type field is missing from trial message";
  }

  // Handle message
  if (sub_type == "start") {
    in_trial = true;
    trial_message = nlohmann::json::parse(message);
  } else if (sub_type == "stop") {
    in_trial = false;
  } else {
    BOOST_LOG_TRIVIAL(error) << "Unknown sub_type " << sub_type;
  }
}
