
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <mathplot.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/print.h>
#include <wx/sizer.h>

#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>

#include <boost/log/trivial.hpp>
#include <nlohmann/json.hpp>

#include "ScoreWidget.h"

using namespace std;

ScoreWidget::ScoreWidget(wxPanel *panel, string type, string mqtt_host, string mqtt_port) : Widget(type, mqtt_host, mqtt_port) {
  // Set Frame
  this->panel = panel;

  // Get data from configuration
  panel_name = configuration["panel_name"];
  x_axis_field = configuration["x_axis_field"];
  x_axis_label = configuration["x_axis_label"];
  y_axis_field = configuration["y_axis_field"];
  y_axis_label = configuration["y_axis_label"];

  // Create and attach MathPlot chart to frame
  chart = new mpWindow(panel, -1, wxPoint(0, 0), wxSize(500, 500),
                             wxSUNKEN_BORDER);
  chart->AddLayer(new mpScaleX(x_axis_label, mpALIGN_BOTTOM, true));
  chart->AddLayer(new mpScaleY(y_axis_label, mpALIGN_LEFT, true));
  std::cout << "HERE" << std::endl;  

  data_vector = new mpFXYVector();
  x_axis_data.insert(x_axis_data.end(), {});
  y_axis_data.insert(y_axis_data.end(), {});
  
  std::cout << "HERE" << std::endl;  
  data_vector->SetData(x_axis_data, y_axis_data);

  wxPen vectorpen(*wxBLUE, 5, wxSOLID);
  data_vector->SetPen(vectorpen);
  chart->AddLayer(data_vector);

  std::cout << "HERE" << std::endl;  
  chart->Fit();
}


void ScoreWidget::OnMessage(std::string topic,
                            std::string message) {
  nlohmann::json update;

  // Parse JSON response
  nlohmann::json response;
  try {
    response = nlohmann::json::parse(message);
  } catch (exception e) {
    std::cout << "Failed to parse Score message" << std::endl;
    return;
  }

  // Check for existence of x_axis_field in message
  if (response["data"].contains(x_axis_field)) {
    update["x_point"] = response["data"][x_axis_field];
  } 
  else{
	BOOST_LOG_TRIVIAL(error) << "Missing fields in update message";
	return;
  }

  
  // Check for existence of y_axis_field in message
  if (response["data"].contains(y_axis_field)) {
    update["y_point"] = response["data"][y_axis_field];
  } 
  else{
	BOOST_LOG_TRIVIAL(error) << "Missing fields in update message"; 	
	return;
  }

  PushUpdate(update);
}

void ScoreWidget::Update() {
  // Check for update
  if (UpdateQueued()) {
    	nlohmann::json update = GetUpdate();
	std::cout << update["x_point"] << std::endl;
	std::cout << update["y_point"] << std::endl;
	x_axis_data.push_back(update["x_point"]);
	y_axis_data.push_back(update["y_point"]);
	data_vector->SetData(x_axis_data, y_axis_data);

	// mpWindow must be refreshed after setting data
	chart->Refresh();
  }
}

