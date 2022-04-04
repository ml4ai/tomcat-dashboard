///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////

#define UTTERANCE_TEXT_1 1000
#define UTTERANCE_TEXT_2 1001
#define UTTERANCE_TEXT_3 1002

///////////////////////////////////////////////////////////////////////////////
/// Class UtteranceFrame
///////////////////////////////////////////////////////////////////////////////
class UtteranceFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button3;
		wxButton* m_button4;
		wxButton* m_button5;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;

		// Virtual event handlers, override them in your derived class
		virtual void m_button1OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button2OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button3OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button4OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button5OnButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		UtteranceFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~UtteranceFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class ScoreFrame
///////////////////////////////////////////////////////////////////////////////
class ScoreFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button3;
		wxButton* m_button4;
		wxButton* m_button5;
		wxPanel* m_panel1;

		// Virtual event handlers, override them in your derived class
		virtual void m_button1OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button2OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button3OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button4OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_button5OnButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		ScoreFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ScoreFrame();

};

