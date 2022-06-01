#pragma once 
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <nlohmann/json.hpp>
#include "mqtt/client.h"

#include "TrialListener.h"

class Widget{
	public:
		Widget(std::string type, std::string mqtt_host, std::string mqtt_port);	
		virtual void Update() = 0;
	protected:
		std::string type;	
		nlohmann::json configuration;

		std::unique_ptr<TrialListener> trial_listener;	
	
		// Functions for queue implementation
		void PushUpdate(nlohmann::json update);
		bool UpdateQueued();
		nlohmann::json GetUpdate();

		// OnMessage called when message recieved, override in Subclass	
		virtual void OnMessage(std::string topic, std::string message) = 0;
	private:
		
		// MQTT bus data
		std::string mqtt_host, mqtt_port;
		std::thread mqtt_thread;
		std::unique_ptr<mqtt::client> mqtt_client;
		
		void Connect();	
		void Subscribe();
		void Loop();

		// Configuration files data
		std::vector<std::string> topics;
		
		void LoadConfig();

		// Queue data
		std::queue<nlohmann::json> update_queue;
		std::mutex update_mutex;
};
