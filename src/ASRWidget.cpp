
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

#include <boost/log/trivial.hpp>

#include <nlohmann/json.hpp>

#include "ASRWidget.h"

using namespace std;


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
    update["text"] = text;
    update["participant_id"] = participant_id;

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

    text_box->AppendText(participant_id + ": ");
    text_box->AppendText(text + "\n");
  }
}
