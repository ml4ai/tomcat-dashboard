#pragma once

#include <wx/wxprec.h>
#include <wx/xrc/xmlres.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <mathplot.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/print.h>
#include <wx/sizer.h>

#include <mutex>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "Widget.h"

class ChartWidget : public Widget {

public:
  ChartWidget(wxPanel *panel, std::string type, std::string mqtt_host,
              std::string mqtt_port);

  void Update() override;

protected:
  void OnMessage(std::string topic, std::string message) override;

private:
  // Configuration values
  std::string panel_name;
  std::string x_axis_field;
  std::string x_axis_label;
  std::string y_axis_field;
  std::string y_axis_label;

  // wxComponents
  wxPanel *panel;
  mpWindow *chart;

  // Data components
  mpFXYVector *data_vector;
  std::vector<double> x_axis_data;
  std::vector<double> y_axis_data;
};
