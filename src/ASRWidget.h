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
		inline static const std::string type = "ASRWidget";
		inline static int current_id = 1;
		int id = -1;
	public:
		ASRWidget(wxFrame *frame);
		~ASRWidget();

		void Update() override;
	protected:
		void on_message(const std::string& topic,
                    const std::string& message) override;

	private:
		// Framework
		std::mutex mutex;
		std::queue<std::string> queue;
	
		// Config values	
		std::string playername;
		std::string component_name;

		// wx Components
		wxStaticText *static_text;

	private:
		void Initialize();
		void UpdatePrivate(std::string);
};

