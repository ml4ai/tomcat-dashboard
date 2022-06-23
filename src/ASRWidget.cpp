#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <streambuf>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>

#include <boost/log/trivial.hpp>

#include <nlohmann/json.hpp>

#include "ASRWidget.h"

using namespace std;

map<string, string> participant_color;

ASRWidget::ASRWidget(wxFrame *frame, string type, string mqtt_host,
                     string mqtt_port)
    : Widget(type, mqtt_host, mqtt_port) {
  // Set variables
  this->frame = frame;
  

  // Load wxTextCtrl component
  string component = configuration["components"]["wxTextCtrl"];
  wxTextCtrl *s = (wxTextCtrl *)frame->FindWindowByName(component);
  if (s == nullptr) {
    BOOST_LOG_TRIVIAL(error)
        << "Failed to find component: " << component << " in type: " << type;
  } else {
    text_box = s;
  }
}

void ASRWidget::OnMessage(std::string topic, std::string message) {
  nlohmann::json update;

  // Parse JSON response
  nlohmann::json response;
  try {
    response = nlohmann::json::parse(message);
  } catch (exception e) {
    BOOST_LOG_TRIVIAL(error) << "Failed to parse ASR message";
    return;
  }

  // Generate update from message
  try {
    string text = response["data"]["text"];
    string participant_id = response["data"]["participant_id"];
    string time_stamp = response["header"]["timestamp"];
      if (participant_color.find(participant_id) != participant_color.end()) {
          update["color"] = participant_color.at(participant_id);
      }
      else {
          nlohmann::json trial_message = trial_listener->GetTrialMessage();
          vector<nlohmann::json> client_info =
              trial_message["data"]["client_info"].get<vector<nlohmann::json>>();
          for (nlohmann::json client : client_info) {
              if (client["participant_id"] == participant_id) {
                  string color = client["callsign"];
                  boost::algorithm::to_lower(color);
                  participant_color[participant_id] = color;
                  BOOST_LOG_TRIVIAL(error) << client["callsign"];
                  update["color"] = color;
              }
          }
      }
    
    update["text"] = text;
    update["participant_id"] = participant_id;
    update["timestamp"] = time_stamp;
    

  } catch (exception e) {
    BOOST_LOG_TRIVIAL(error)
        << "Missing fields in update message for type: " << type;
    return;
  }

  // Push update
  PushUpdate(update);
}


void ASRWidget::Update() {
  // Check for update
  if (UpdateQueued()) {
    nlohmann::json update = GetUpdate();
    string participant_id = update["participant_id"];
    string text = update["text"];
    string time_stamp = update["timestamp"];
    string color = update["color"];
    wxFont smallFont(7, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    text_box -> SetFont(smallFont);
      
    text_box->AppendText(time_stamp+ " ");
      
    wxFont bigFont(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    text_box ->SetFont(bigFont);
      wxString s(color);
      
    text_box ->SetDefaultStyle(wxTextAttr(s));
    text_box->AppendText(participant_id + ": ");
      text_box ->SetDefaultStyle(wxTextAttr("white"));
    text_box->AppendText(text + "\n");
  }
}
