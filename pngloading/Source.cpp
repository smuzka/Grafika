#include <wx/wxprec.h>
#include <wx/colordlg.h>
#include <wx/dcbuffer.h>
#include <wx/filefn.h> 

class MyApp : public wxApp {
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
	MyFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Jakub Smuga 3"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(600, 400), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~MyFrame();
private:
	void WxSaveButton(wxCommandEvent& e);
	void WxkebabCheckbox(wxCommandEvent& e);
	void WxkebabScrollbar(wxScrollEvent& e);
	void WxStarButton(wxCommandEvent& e);
	void WxTextUpdate(wxCommandEvent& e);
	void Form_Paint(wxPaintEvent& e);
	void Form_Update(wxUpdateUIEvent& e);

	void Draw();

	wxPanel* drawingpanel;
	wxButton* saveButton;
	wxCheckBox* kebabCheckbox;
	wxScrollBar* kebabBar;
	wxGauge* kebabGauge;
	wxButton* starButton;
	wxTextCtrl* textControl;
	wxChoice* SymbolChoice;
	wxImage saveImage;
	wxBitmap bufferBitmap;
	wxImage kebabImage;
	wxImage hatImage;
	wxBitmap kebabBitmap;
	wxBitmap hatBitmap;
	wxString _text = "Kebab";
	wxColour colorStar = wxColour(*wxBLACK);

	wxPoint star[5] = { wxPoint(-70, -90), wxPoint(-45, -45), wxPoint(-100, -70), wxPoint(-40, -70), wxPoint(-90, -45) };



	enum {
		ID_WXBUTTON_SAVE = 1,
		ID_WXCHECKBOX_kebab = 2,
		ID_WXSCROLLBAR_kebab = 3,
		ID_WXGAUGE_kebab = 4,
		ID_WXBUTTON_STAR = 5,
		ID_WXTEXTCTRL_TEXT = 6,
		ID_WXCHOISE_CELESTIAL = 7,
	};
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	MyFrame* frame = new MyFrame(NULL);
	frame->Show(true);
	return true;
}

MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style) {
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	// window
	wxBoxSizer* windowSizer;
	windowSizer = new wxBoxSizer(wxHORIZONTAL);
	// panel
	wxBoxSizer* panelSizer;
	panelSizer = new wxBoxSizer(wxVERTICAL);
	drawingpanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	drawingpanel->SetBackgroundColour(wxColour(255, 255, 255));
	panelSizer->Add(drawingpanel, 1, wxEXPAND | wxALL, 5);
	windowSizer->Add(panelSizer, 1, wxEXPAND, 5);
	wxBoxSizer* menuSizer;
	// menu
	menuSizer = new wxBoxSizer(wxVERTICAL);
	saveButton = new wxButton(this, ID_WXBUTTON_SAVE, wxT("Zapis do pliku"), wxDefaultPosition, wxDefaultSize, 0);
	menuSizer->Add(saveButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	// checkbox
	kebabCheckbox = new wxCheckBox(this, ID_WXCHECKBOX_kebab, wxT("kebab"), wxDefaultPosition, wxDefaultSize, 0);
	menuSizer->Add(kebabCheckbox, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	// bar
	kebabBar = new wxScrollBar(this, ID_WXSCROLLBAR_kebab, wxDefaultPosition, wxDefaultSize);
	kebabBar->SetScrollbar(0, 1, 100, 1);
	kebabBar->Disable();
	menuSizer->Add(kebabBar, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 5);
	// gauge
	kebabGauge = new wxGauge(this, ID_WXGAUGE_kebab, 100, wxDefaultPosition, wxDefaultSize);
	kebabGauge->SetValue(0);
	menuSizer->Add(kebabGauge, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	starButton = new wxButton(this, ID_WXBUTTON_STAR, wxT("kolor gwiazdki"), wxDefaultPosition, wxDefaultSize, 0);
	menuSizer->Add(starButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	textControl = new wxTextCtrl(this, ID_WXTEXTCTRL_TEXT, _text, wxDefaultPosition, wxDefaultSize, 0);
	menuSizer->Add(textControl, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	// Choice
	wxArrayString SymbolChoiceChoices;
	SymbolChoice = new wxChoice(this, ID_WXCHOISE_CELESTIAL, wxDefaultPosition, wxDefaultSize, SymbolChoiceChoices, 0);
	SymbolChoice->Append("gwiazdka");
	SymbolChoice->Append("ksiê¿yc");
	SymbolChoice->Append("s³onko");
	SymbolChoice->SetSelection(0);
	menuSizer->Add(SymbolChoice, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	windowSizer->Add(menuSizer, 0, wxEXPAND, 5);


	this->SetSizer(windowSizer);
	this->Layout();
	this->Centre(wxBOTH);


	if (!wxFileExists("kebab.png")) {
		if (wxMessageDialog(NULL, "Cannot load kebab", "Error", wxOK).ShowModal() == wxID_OK) Close(true);
		return;
	}
	kebabImage.AddHandler(new wxPNGHandler);
	kebabImage.LoadFile("kebab.png", wxBITMAP_TYPE_PNG);
	kebabBitmap = wxBitmap(kebabImage.Rotate180());

	if (!wxFileExists("hat.png")) {
		if (wxMessageDialog(NULL, "Cannot load hat", "Error", wxOK).ShowModal() == wxID_OK) Close(true);
		return;
	}
	hatImage.AddHandler(new wxPNGHandler);
	hatImage.LoadFile("hat.png", wxBITMAP_TYPE_PNG);
	hatBitmap = wxBitmap(hatImage);


	Bind(wxEVT_BUTTON, &MyFrame::WxSaveButton, this, ID_WXBUTTON_SAVE);
	Bind(wxEVT_CHECKBOX, &MyFrame::WxkebabCheckbox, this, ID_WXCHECKBOX_kebab);
	Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::WxkebabScrollbar, this, ID_WXSCROLLBAR_kebab);
	Bind(wxEVT_BUTTON, &MyFrame::WxStarButton, this, ID_WXBUTTON_STAR);
	Bind(wxEVT_TEXT, &MyFrame::WxTextUpdate, this, ID_WXTEXTCTRL_TEXT);
	Bind(wxEVT_PAINT, &MyFrame::Form_Paint, this);
	Bind(wxEVT_UPDATE_UI, &MyFrame::Form_Update, this);
}

MyFrame::~MyFrame() {
	delete drawingpanel;
	delete saveButton;
	delete kebabCheckbox;
	delete kebabBar;
	delete kebabGauge;
	delete starButton;
	delete textControl;
	delete SymbolChoice;
}

void MyFrame::WxSaveButton(wxCommandEvent& WXUNUSED(e)) {
	wxFileDialog saveFileDialog(this, "Choose a file", "", "", "All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}

	saveImage = bufferBitmap.ConvertToImage();
	saveImage.AddHandler(new wxJPEGHandler);
	saveImage.AddHandler(new wxPNGHandler);
	saveImage.SaveFile(saveFileDialog.GetPath());
}

void MyFrame::WxkebabCheckbox(wxCommandEvent& WXUNUSED(e)) {
	kebabBar->Enable(kebabCheckbox->IsChecked());
}

void MyFrame::WxkebabScrollbar(wxScrollEvent& WXUNUSED(e)) {
	kebabGauge->SetValue(kebabBar->GetThumbPosition());
	Draw();
}

void MyFrame::WxStarButton(wxCommandEvent& WXUNUSED(e)) {
	wxColourDialog chooseColourDialog(this, nullptr);
	if (chooseColourDialog.ShowModal() == wxID_CANCEL)
		return;

	colorStar = chooseColourDialog.GetColourData().GetColour();
}

void MyFrame::WxTextUpdate(wxCommandEvent& WXUNUSED(e)) {
	_text = textControl->GetValue();
}

void MyFrame::Form_Paint(wxPaintEvent& WXUNUSED(e)) {
	Draw();
}

void MyFrame::Form_Update(wxUpdateUIEvent& WXUNUSED(e)) {
	Draw();
}

void MyFrame::Draw() {
	std::unique_ptr<wxClientDC> clientDC(new wxClientDC(drawingpanel));
	bufferBitmap = wxBitmap(drawingpanel->GetSize());
	std::unique_ptr<wxBufferedDC> MyDC(new wxBufferedDC(clientDC.get(), bufferBitmap));

	MyDC->SetDeviceOrigin(drawingpanel->GetSize().x / 2, drawingpanel->GetSize().y / 2);

	MyDC->SetBackground(*wxWHITE_BRUSH);
	MyDC->Clear();

	double kebabFactor = 0.1 * kebabGauge->GetValue() * sin(M_PI * kebabGauge->GetValue() / 100);

	MyDC->DrawLine(0, 0, 0, 50);
	MyDC->DrawLine(0, 10, -40 - kebabFactor, 30 - 0.4 * kebabGauge->GetValue());
	MyDC->DrawLine(0, 10, 40, 30);
	MyDC->DrawLine(0, 50, -30, 100);
	MyDC->DrawLine(0, 50, 30, 100);

	if (kebabCheckbox->IsChecked()) {
		kebabBitmap = wxBitmap(kebabImage.Rotate180().Rotate(-kebabBar->GetThumbPosition() / 30, wxPoint(-40, 30)));
		MyDC->DrawBitmap(kebabBitmap, -40 - kebabFactor - kebabBitmap.GetWidth() / 2, 30 - 0.4 * kebabGauge->GetValue() - kebabBitmap.GetWidth() / 2, true);
	}

	MyDC->DrawCircle(0, -22, 22);
	MyDC->DrawEllipse(-12, -29, 11, 6);

	if (kebabCheckbox->IsChecked()) {
		MyDC->DrawEllipticArc(-14, -16, 28, 8, 180, 360);
		MyDC->DrawEllipse(3, -29, 11, 6);
	}
	else {
		MyDC->DrawEllipticArc(-14, -16, 28, 10, 0, 180);
		MyDC->DrawEllipse(4, -31, 6, 11);
	}

	MyDC->DrawBitmap(hatBitmap, -35, -85, false);


	wxString text(_text);
	MyDC->DrawText(text, -90, 90);
	wxString textRotated(_text);
	MyDC->SetFont(wxFont(40, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, "Brush Script MT"));
	MyDC->DrawRotatedText(_text, 50, 100, 90);

	if (SymbolChoice->GetSelection() == 0) {
		MyDC->SetBrush(wxBrush(colorStar));
		MyDC->SetPen(wxPen(colorStar));
		MyDC->DrawPolygon(5, star, 0, 0);
	}
	else if (SymbolChoice->GetSelection() == 1) {
		MyDC->SetBrush(*wxLIGHT_GREY_BRUSH);
		MyDC->SetPen(*wxLIGHT_GREY_PEN);
		MyDC->DrawCircle(-70, -70, 20);
		MyDC->SetBrush(*wxWHITE_BRUSH);
		MyDC->SetPen(*wxWHITE_PEN);
		MyDC->DrawCircle(-80, -80, 22);
		MyDC->SetBrush(*wxBLACK_BRUSH);
		MyDC->SetPen(*wxBLACK_PEN);
		MyDC->DrawCircle(-60, -70, 2);
		MyDC->DrawLine(-70, -63, -64, -59);
	}
	else {
		MyDC->SetBrush(*wxYELLOW_BRUSH);
		MyDC->SetPen(*wxYELLOW_PEN);
		MyDC->DrawCircle(-80, -80, 20);
	}



}