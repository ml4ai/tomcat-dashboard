#pragma once 

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "MosquittoClient.h"

class Widget : public MosquittoClient {
	public:
		Widget(std::string type);
		~Widget();

		virtual void Update() = 0;
	protected:
		nlohmann::json configuration;
		wxFrame *frame;
	private:
		std::string type;
		std::vector<std::string> topics;
	private:
		void LoadConfig();
		void ParseConfig();
		void Subscribe();	
};
