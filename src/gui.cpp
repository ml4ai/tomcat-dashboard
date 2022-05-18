// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>
#include <string>
#include <vector>

//#include "Frontend.h"
#include "Widget.h"
#include "ASRWidget.h"
//#include "ScoreWidget.h"

using namespace std;

class MyApp : public wxApp {
public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent& evt);
  void Update();
 
  vector<Widget *>widgets; 
  wxFrame *utterance_frame;
  wxFrame *score_frame;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  // Load XRC files
  wxXmlResource::Get()->InitAllHandlers();
  wxXmlResource::Get()->LoadAllFiles("templates");
  

  // Generate ASRWidgets
  utterance_frame = new wxFrame(); 
  if(wxXmlResource::Get()->LoadFrame(utterance_frame, NULL, "Utterance Frame")){
	  widgets.push_back(new ASRWidget(utterance_frame));
	  widgets.push_back(new ASRWidget(utterance_frame));
	  widgets.push_back(new ASRWidget(utterance_frame));	  
	  
	  utterance_frame->Show();
	  utterance_frame->Maximize();  
	  utterance_frame->Refresh();
  }
  else{
  	std::cout << "Failed to load Utterance frame" << std::endl; 
  }
  
  // Generate Score Widget
  /*this->score_frame = new ScoreFrame(NULL);
  this->widgets.push_back(new ScoreWidget(this->score_frame)); 
  
  // Show frame
  this->score_frame->Show();
  this->score_frame->Maximize();  
  this->score_frame->Refresh();*/
  

  Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle) ); 
  return true;
}


void MyApp::Update(){
	// Update ASR Widgets
	for(int i=0;i<this->widgets.size();i++){
		this->widgets[i]->Update();
	}
}

void MyApp::onIdle(wxIdleEvent& evt)
{
        this->Update();
	evt.RequestMore(); // render continuously, not only once on idle
}
