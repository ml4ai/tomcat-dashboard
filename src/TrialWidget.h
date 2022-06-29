#pragma once

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "Widget.h"

class TrialWidget : public Widget {
public:
  TrialWidget(wxPanel *panel, std::string type, std::string mqtt_host,
              std::string mqtt_port);
  void Update() override;

protected:
  void OnMessage(std::string topic, std::string message) override;

private:
  // wx Components
  wxPanel *panel;
  wxMenuItem *start_trial;
  wxMenuItem *stop_trial;

  // wx event handlers for button press
  void OnStart(wxCommandEvent &event);
  void OnStop(wxCommandEvent &event);
};
