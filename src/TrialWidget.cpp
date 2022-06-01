
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <queue>
#include <mutex>

#include <boost/log/trivial.hpp>

#include <nlohmann/json.hpp>

#include "TrialWidget.h"

using namespace std;

TrialWidget::TrialWidget(wxFrame *frame, string type, string mqtt_host, string mqtt_port) : Widget(type, mqtt_host, mqtt_port){
	// Set variables
	this->frame = frame;

	// Load components
	vector<string> components = configuration["components"];
	const char *start_trial_name = components[0].c_str();
	const char *stop_trial_name = components[1].c_str();
	int start_trial_id = XRCID(start_trial_name); // When using .xrc, IDs are not predictible, must use XRCID macro
	int stop_trial_id = XRCID(stop_trial_name);

	// Bind handlers
	frame->Bind(wxEVT_MENU, &TrialWidget::OnStart, this, start_trial_id);
	frame->Bind(wxEVT_MENU, &TrialWidget::OnStop, this, stop_trial_id);
}

void TrialWidget::OnMessage(std::string topic, std::string message){
	nlohmann::json update;
}

void TrialWidget::Update(){
	// Check for update
	if(UpdateQueued()){
		nlohmann::json update = GetUpdate();
	}
}

void TrialWidget::OnStart(wxCommandEvent& event){
	nlohmann::json message;
	// TODO: Create Trial Start Message
	//mqtt_client->publish("trial", message.dump());
}

void TrialWidget::OnStop(wxCommandEvent& event){
	nlohmann::json message;
	// TODO: Create Trial Stop Message
	//mqtt_client->publish("trial", message.dump());
}
