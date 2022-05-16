// wxWidgets "Hello World" Program

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include "mqtt_handler.cpp"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyApp : public wxApp
{
    public:
        virtual bool OnInit();
};

class MyFrame : public wxFrame
{
    public:
        MyFrame();

    private:
        void OnStart(wxCommandEvent& event);
        void OnStop(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
};

enum
{
    ID_Start_Trial = 1,
    ID_Stop_Trial = 2
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "App")
{
    wxMenu *menuTrial = new wxMenu;
    menuTrial->Append(ID_Start_Trial, "Start Trial",
            "Start Trial");

    menuTrial->AppendSeparator();

    menuTrial->Append(ID_Stop_Trial, "Stop Trial", "Stop Trial");
    menuTrial->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuTrial, "&Trial");

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText("Welcome to the ToMCAT DashBoard!");

    Bind(wxEVT_MENU, &MyFrame::OnStart, this, ID_Start_Trial);
    Bind(wxEVT_MENU, &MyFrame::OnStop, this, ID_Stop_Trial);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnStop(wxCommandEvent& event)
{
    wxLogMessage("Stop Trial");
}

void MyFrame::OnStart(wxCommandEvent& event)
{
    // A subscriber often wants the server to remember its messages when its
    // disconnected. In that case, it needs a unique ClientID and a
    // non-clean session.

    mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);

    mqtt::connect_options connOpts;
    connOpts.set_clean_session(false);

    // Install the callback(s) before connecting.
    callback cb(cli, connOpts);
    cli.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        std::cout << "Connecting to the MQTT server..." << std::flush;
        cli.connect(connOpts, nullptr, cb);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "\nERROR: Unable to connect to MQTT server: '"
            << SERVER_ADDRESS << "'" << exc << std::endl;
    }

    // Just block till user tells us to quit.

    while (std::tolower(std::cin.get()) != 'q')
        ;

    // Disconnect

    try {
        std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
        cli.disconnect()->wait();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc << std::endl;
    }

}
