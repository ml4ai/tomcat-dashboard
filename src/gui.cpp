// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>
#include <string>
#include <vector>

#include "ASRWidget.h"
#include "TrialWidget.h"
#include "Widget.h"
//#include "ScoreWidget.h"

using namespace std;

class MyApp : public wxApp {
public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent &evt);
  void Update();

  vector<Widget *> widgets;
  wxFrame *utterance_frame;
  wxFrame *trial_frame;
  wxFrame *score_frame;
};

wxIMPLEMENT_APP(MyApp);


bool MyApp::OnInit() {
  std::string mqtt_host = "localhost";
  std::string mqtt_port = "1883";

  // Load XRC files
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->LoadAllFiles("templates");

  // Generate ASRWidgets
  trial_frame = new wxFrame();
  if (wxXmlResource::Get()->LoadFrame(trial_frame, NULL, "Trial Frame")) {
    widgets.push_back(
        new TrialWidget(trial_frame, "TrialWidget", mqtt_host, mqtt_port));

    /*trial_frame->Show();
    trial_frame->Maximize();
    trial_frame->Refresh();*/
  } else {
    std::cout << "Failed to load Trial frame" << std::endl;
  }

  // Generate ASRWidgets
  utterance_frame = new wxFrame();
  if (wxXmlResource::Get()->LoadFrame(utterance_frame, NULL,
                                      "Utterance Frame")) {
    widgets.push_back(
        new ASRWidget(utterance_frame, "ASRWidget", mqtt_host, mqtt_port));

    utterance_frame->Show();
    utterance_frame->Maximize();
    utterance_frame->Refresh();
  } else {
    std::cout << "Failed to load Utterance frame" << std::endl;
  }

  Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
  return true;
}

void MyApp::Update() {
  // Update ASR Widgets
  for (int i = 0; i < this->widgets.size(); i++) {
    this->widgets[i]->Update();
  }
}

void MyApp::onIdle(wxIdleEvent &evt) {
  this->Update();
  evt.RequestMore(); // render continuously, not only once on idle
}
