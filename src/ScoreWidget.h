#pragma once 

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
#include <queue>
#include <mutex>
#include <utility>

#include "Widget.h"

class ScoreWidget : public Widget {
	
	public:
		inline static const std::string type = "ScoreWidget";
	public:
		ScoreWidget(wxFrame *frame);
		~ScoreWidget();

		void Update() override;
	protected:
		void on_message(const std::string& topic,
                    const std::string& message) override;

	private:
		// Framework
		std::mutex mutex;
		std::queue<std::pair<double, double>> queue;
		
		// Configuration values
		std::string panel_name;

		// wxComponents 
		wxPanel *panel;
		mpWindow *chart;

		// Data components
		mpFXYVector *data_vector;
		std::vector<double> time;
		std::vector<double> score;
	private:
		void Initialize();
		void UpdatePrivate(std::pair<double, double> point);
};

