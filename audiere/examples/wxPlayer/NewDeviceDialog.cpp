#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif


#include "NewDeviceDialog.h"


BEGIN_EVENT_TABLE(NewDeviceDialog, wxDialog)
  EVT_BUTTON(-1, NewDeviceDialog::OnButton)
END_EVENT_TABLE()


NewDeviceDialog::NewDeviceDialog(wxWindow* parent)
  : wxDialog(parent, -1, wxString("New Device"))
{
  // size of text controls
  static const wxSize size(300, 22);

  audiere::GetSupportedAudioDevices(m_devices);

  m_device = new wxChoice(this, -1, wxDefaultPosition, size);
  m_device->Append("autodetect: Choose default device");
  for (unsigned i = 0; i < m_devices.size(); ++i) {
    m_device->Append((m_devices[i].name + ": " + m_devices[i].description).c_str());
  }
  m_device->SetSelection(0);

  // create parameters box
  m_parameters = new wxTextCtrl(this, -1, "", wxDefaultPosition, size);

  // create vertical sizer
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(m_device,     0, wxALIGN_CENTER | wxALL, 5);
  sizer->Add(m_parameters, 0, wxALIGN_CENTER | wxALL, 5);

  // button bar
  m_ok     = new wxButton(this, -1, "OK");
  m_cancel = new wxButton(this, -1, "Cancel");
  wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(m_ok,     0, wxALIGN_CENTER | wxALL, 5);
  buttonSizer->Add(m_cancel, 0, wxALIGN_CENTER | wxALL, 5);

  sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 4);

  SetAutoLayout(true);
  SetSizer(sizer);

  m_device->SetFocus();
  m_ok->SetDefault();

  sizer->Fit(this);
  sizer->SetSizeHints(this);
}


const std::string& NewDeviceDialog::getDevice() const {
  return m_device_str;
}


const std::string& NewDeviceDialog::getParameters() const {
  return m_parameters_str;
}


void NewDeviceDialog::OnButton(wxCommandEvent& event) {
  if (event.GetEventObject() == m_ok) {
    int value = m_device->GetSelection();
    if (value == 0) {
      m_device_str = "autodetect";
    } else {
      m_device_str = m_devices[value - 1].name;
    }

    m_parameters_str = m_parameters->GetValue();
    EndModal(wxID_OK);
  } else if (event.GetEventObject() == m_cancel) {
    EndModal(wxID_CANCEL);
  }
}
