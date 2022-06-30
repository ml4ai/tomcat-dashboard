// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/xrc/xh_richtext.h>
#include <wx/richtext/richtextctrl.h>
#endif

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "ASRWidget.h"
#include "ScoreWidget.h"
#include "TrialWidget.h"
#include "Widget.h"

using namespace std;

class MyApp : public wxApp {
public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent &evt);
 
  // Menu button press listeners
  void onTrialPress(wxCommandEvent &event);
  void onASRPress(wxCommandEvent &event);
  void onScorePress(wxCommandEvent &event);
  
  void Update();

  vector<Widget *> widgets;
  
  wxFrame *frame;

  wxSizer *panel_sizer; // Sizer object for panel switching
  wxPanel *current_panel; // Need access to current panel to hide on switch
  wxPanel *utterance_panel;
  wxPanel *trial_panel;
  wxPanel *score_panel;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  std::string mqtt_host = "localhost";
  std::string mqtt_port = "1883";

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
  score_panel = (wxPanel *)frame->FindWindowByName("SCORE_PANEL");
  current_panel = score_panel; // Set default panel
  panel_sizer = current_panel->GetContainingSizer(); // Can't access sizers with FindWindowByName

  
  // Bind button press event listeners
  const char *trial_button_name = "TRIAL_BUTTON";
  const char *asr_button_name = "ASR_BUTTON";
  const char *score_button_name = "CHART_BUTTON";
  int trial_button_id = XRCID(trial_button_name);
  int asr_button_id = XRCID(asr_button_name);
  int score_button_id = XRCID(score_button_name);

  frame->Bind(wxEVT_BUTTON, &MyApp::onTrialPress, this, trial_button_id);
  frame->Bind(wxEVT_BUTTON, &MyApp::onASRPress, this, asr_button_id);
  frame->Bind(wxEVT_BUTTON, &MyApp::onScorePress, this, score_button_id);
  
  widgets.push_back(
      new ASRWidget(utterance_panel, "ASRWidget", mqtt_host, mqtt_port));
  widgets.push_back(
      new ScoreWidget(score_panel, "ScoreWidget", mqtt_host, mqtt_port));
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
void MyApp::onScorePress(wxCommandEvent &event){
	current_panel->Hide();
	current_panel = score_panel;
	score_panel->Show();
	panel_sizer->Layout();
}
