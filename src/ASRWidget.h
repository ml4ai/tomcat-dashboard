#pragma once

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "Widget.h"

class ASRWidget : public Widget {
public:
  ASRWidget(wxPanel *panel, std::string type, std::string mqtt_host,
            std::string mqtt_port);
  void Update() override;

protected:
  void OnMessage(std::string topic, std::string message) override;

private:
  // wx Components
  wxPanel *panel;
  wxTextCtrl *text_box;
};
