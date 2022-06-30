#include "Widget.h"
#include "ASRWidget.h"
#include "ChartWidget.h"
#include "TrialWidget.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/xrc/xh_richtext.h>
#include <wx/richtext/richtextctrl.h>
#endif

#include <nlohmann/json.hpp>

#include <boost/log/trivial.hpp>

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <streambuf>

using namespace std;

class MyApp : public wxApp {
public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent &evt);
 
  // Menu button press listeners
  void onTrialPress(wxCommandEvent &event);
  void onASRPress(wxCommandEvent &event);
  void onChartPress(wxCommandEvent &event);
  
  void Update();

  void LoadAppConfig();

  vector<Widget *> widgets;
  
  wxFrame *frame;

  wxSizer *panel_sizer; // Sizer object for panel switching
  wxPanel *current_panel; // Need access to current panel to hide on switch
  wxPanel *utterance_panel;
  wxPanel *trial_panel;
  wxPanel *chart_panel;

  nlohmann::json app_configuration;
  string mqtt_host;
  string mqtt_port;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  // Load app configuration
  LoadAppConfig();

  // Load XRC files
  wxXmlResource::Get()->AddHandler(new wxRichTextCtrlXmlHandler);
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->Load("templates/frontend.xrc");

  // Initialize and load wxFrame
  frame = new wxFrame();
  wxXmlResource::Get()->LoadFrame(frame, NULL,  "Frontend");

  // Load wxPanels
  trial_panel = (wxPanel *)frame->FindWindowByName("TRIAL_PANEL");
  utterance_panel = (wxPanel *)frame->FindWindowByName("UTTERANCE_PANEL");
  chart_panel = (wxPanel *)frame->FindWindowByName("CHART_PANEL");
  current_panel = utterance_panel; // Set default panel
  panel_sizer = current_panel->GetContainingSizer(); // Can't access sizers with FindWindowByName

  
  // Bind button press event listeners
  const char *trial_button_name = "TRIAL_BUTTON";
  const char *asr_button_name = "ASR_BUTTON";
  const char *chart_button_name = "CHART_BUTTON";
  int trial_button_id = XRCID(trial_button_name);
  int asr_button_id = XRCID(asr_button_name);
  int chart_button_id = XRCID(chart_button_name);

  frame->Bind(wxEVT_BUTTON, &MyApp::onTrialPress, this, trial_button_id);
  frame->Bind(wxEVT_BUTTON, &MyApp::onASRPress, this, asr_button_id);
  frame->Bind(wxEVT_BUTTON, &MyApp::onChartPress, this, chart_button_id);
  
  widgets.push_back(
      new ASRWidget(utterance_panel, "ASRWidget", mqtt_host, mqtt_port));
  widgets.push_back(
      new ChartWidget(chart_panel, "ChartWidget", mqtt_host, mqtt_port));
  widgets.push_back(
      new TrialWidget(trial_panel,"TrialWidget", mqtt_host, mqtt_port));



    // Show default frame
    frame->Show();
    frame->Maximize();
    frame->Refresh();
  
    Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
  
    return true;
}

void MyApp::Update() {
  for (int i = 0; i < this->widgets.size(); i++) {
    this->widgets[i]->Update();
  }
}

void MyApp::LoadAppConfig() {
  fstream stream("config/app.json");
  string hold((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
  try {
    app_configuration = nlohmann::json::parse(hold);

  } catch (exception e) {
    BOOST_LOG_TRIVIAL(error)
        << "Unable to load app.json configuration file";
    exit(0);
  }
  mqtt_host = app_configuration["mqtt_host"];
  mqtt_port = app_configuration["mqtt_port"];
}

void MyApp::onIdle(wxIdleEvent &evt) {
  this->Update();
  evt.RequestMore(); // render continuously, not only once on idle
}

void MyApp::onTrialPress(wxCommandEvent &event){
	current_panel->Hide();
	current_panel = trial_panel;
	trial_panel->Show();
	panel_sizer->Layout();
}
void MyApp::onASRPress(wxCommandEvent &event){
	current_panel->Hide();
	current_panel = utterance_panel;
	utterance_panel->Show();
	panel_sizer->Layout();
}
void MyApp::onChartPress(wxCommandEvent &event){
	current_panel->Hide();
	current_panel = chart_panel;
	chart_panel->Show();
	panel_sizer->Layout();
}
