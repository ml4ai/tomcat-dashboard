
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <mathplot.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/print.h>
#include <wx/filename.h>

#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <queue>
#include <mutex>
#include <utility>

#include <nlohmann/json.hpp>

#include "ScoreWidget.h"

using namespace std;

ScoreWidget::ScoreWidget(wxFrame *frame) : Widget(this->type){	
	// Set Frame
	this->frame = frame;

	// Get data from configuration
	this->panel_name = this->configuration["panel_name"];

	this->Initialize();
}

ScoreWidget::~ScoreWidget(){

}

void ScoreWidget::on_message(const std::string& topic, const std::string& message){

	// Parse JSON response
	nlohmann::json response;
	try{
		response = nlohmann::json::parse(message);
	}
	catch(exception e){
		std::cout << "Failed to parse Score message" << std::endl;
		return;
	
	}

	pair<double, double> point;	
	// Check for mission_timer and scoreboard
	if(response["data"].contains("mission_timer") && response["data"].contains("scoreboard")){
		point.first=response["data"]["mission_timer"];
		double total_score=0.0;
		for( auto element : response["data"]["scoreboard"].items()){
			total_score += element.value().get<double>();
		}
		point.second=total_score;
	}
	else{
		std::cout << response.dump() << std::endl;
		return;
	}
	

	this->mutex.lock();
		this->queue.push(point);
	this->mutex.unlock();
}

void ScoreWidget::Update(){
	this->mutex.lock();
		if(this->queue.empty()){
			this->mutex.unlock();
			return;
		}
		pair<double,double> point = this->queue.front();
		this->queue.pop();
	this->mutex.unlock();
	this->UpdatePrivate(point);
}

void ScoreWidget::UpdatePrivate(pair<double, double> point){
	std::cout << "Updating chart" << std::endl;
	this->time.push_back(point.first);
	this->score.push_back(point.second);
	this->data_vector->SetData(this->time, this->score);

	// mpWindow must be refreshed after setting data
	this->chart->Refresh();
}

void ScoreWidget::Initialize(){
	// Get components
	this->panel = (wxPanel *)this->frame->FindWindowByName(this->panel_name);

	// Create and attach MathPlot chart to frame
	this->chart = new mpWindow(this->panel, -1, wxPoint(0,0), wxSize(500,500), wxSUNKEN_BORDER );
	this->chart->AddLayer(new mpScaleX(wxT("X-Axis"), mpALIGN_BOTTOM, true));
	this->chart->AddLayer(new mpScaleY(wxT("Y-Axis"), mpALIGN_LEFT, true));
	
	this->data_vector = new mpFXYVector();
	this->time.insert(this->time.end(), {});
	this->score.insert(this->score.end(), {});
	this->time.insert(this->time.end(), {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	this->score.insert(this->score.end(), {0, 20, 20, 120, 150, 190, 230, 300, 310, 320});
	this->data_vector->SetData(this->time, this->score);
	wxPen vectorpen(*wxBLUE, 5, wxSOLID);
	this->data_vector->SetPen(vectorpen);
	this->chart->AddLayer(this->data_vector);
	
	this->chart->Fit();
}
