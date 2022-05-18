#pragma once
#include <string>
#include <thread>
#include <memory>

#include <nlohmann/json.hpp>

#include "mqtt/client.h"

class TrialListener{	
	public:
		TrialListener(std::string mqtt_host, std::string mqtt_port);

		std::string GetTrialId();
		std::string GetExperimentId();
		nlohmann::json GetTrialMessage();
		bool InTrial();

	private:
		bool in_trial = false;
		nlohmann::json trial_message;
		
		std::string mqtt_host,  mqtt_port;
		std::thread mqtt_thread;
		std::unique_ptr<mqtt::client> mqtt_client;

		// Connect to MQTT bus and subscibe to trial message
		void Connect();
		// Begin listening for incoming messages
		void Loop();
		// Handler called for incoming messages
		void OnMessage(std::string message);
};
