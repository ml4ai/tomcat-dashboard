
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

#include <nlohmann/json.hpp>

#include "ASRWidget.h"

using namespace std;

ASRWidget::ASRWidget(wxFrame *frame) : Widget(this->type){
	// Set frame
	this->frame = frame;
	
	// Update ID
	this->id = this->current_id;
	this->current_id++;
	
	// Get data from configuration
	this->playername = this->configuration[to_string(this->id)]["playername"];
	this->component_name = this->configuration[to_string(this->id)]["component_name"];

	// Initialize widget
	this->Initialize();
}

ASRWidget::~ASRWidget(){

}

void ASRWidget::on_message(const std::string& topic, const std::string& message){

	// Parse JSON response
	nlohmann::json response;
	try{
		response = nlohmann::json::parse(message);
	}
	catch(exception e){
		std::cout << "Failed to parse ASR message" << std::endl;
		return;
	
	}

	// Check for playername
	if(response["data"].contains("participant_id")){
		if(response["data"]["participant_id"] == this->playername){	
			string message_text = response["data"]["text"];
			this->mutex.lock();
				this->queue.push(message_text);
			this->mutex.unlock();
		}
	}
	else{
		std::cout << response.dump() << std::endl;
	}
}

void ASRWidget::Initialize(){
	this->static_text = (wxStaticText *)this->frame->FindWindowByName(this->component_name);
	if(static_text == nullptr){
		std::cout << "Failed to find component: " << this->component_name << " in type: " << this->type << std::endl;
	}
}

void ASRWidget::Update(){
	this->mutex.lock();
		if(this->queue.empty()){
			this->mutex.unlock();
			return;
		}
		std::string text = this->queue.front();
		this->queue.pop();
	this->mutex.unlock();
	this->UpdatePrivate(text);
}

void ASRWidget::UpdatePrivate(string text){
	this->static_text->SetLabel(text);
}
