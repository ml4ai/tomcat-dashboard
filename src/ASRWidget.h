#pragma once 

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <string>
#include <queue>
#include <mutex>

#include "Widget.h"

class ASRWidget : public Widget {	
	public:	
		ASRWidget(wxFrame *frame, std::string type, std::string mqtt_host, std::string mqtt_port);
		void Update() override;

	protected:
		void OnMessage(std::string topic, std::string message) override;

	private:
		// wx Components
		wxFrame *frame;
		std::vector<wxStaticText *> static_text; 
};

