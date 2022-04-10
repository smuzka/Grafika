#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"



struct Point {
    float x, y, z;
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
    int R, G, B;
    Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
    Point begin, end;
    Color color;
    Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
 m_staticText25->SetLabel(_("Obr\u00F3t X:"));
 m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
 m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

 WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
 WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
 WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

 WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
 WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
 WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

 WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
 WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
 WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click( wxCommandEvent& event )
{
 wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

 if (WxOpenFileDialog.ShowModal() == wxID_OK)
 {
  double x1, y1, z1, x2, y2, z2;
  int r, g, b;

  std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
  if (in.is_open())
  {
   data.clear();
   while (!in.eof())
   {
    in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
    data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
   }
   in.close();
  }
 }
}

void GUIMyFrame1::Scrolls_Updated( wxScrollEvent& event )
{
WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


Repaint();
}


void GUIMyFrame1::Repaint()
{

    wxClientDC dc1(WxPanel);
    wxBufferedDC dc(&dc1);

    int w, h;
    WxPanel->GetSize(&w, &h);
    

    dc.SetBackground(wxBrush(RGB(255, 255, 255)));
    dc.Clear();
    dc.SetClippingRegion(0, 0, w, h);


    for (auto element : data) {
        DrawSegment(&dc, element, w, h);
    }

}

Matrix4 GUIMyFrame1::TranslationMatrix() {
    Matrix4 buffer;

    buffer.data[0][0] = 1;
    buffer.data[1][1] = 1;
    buffer.data[2][2] = 1;
    buffer.data[0][3] = (WxSB_TranslationX->GetValue() - 100.0) / 50;
    buffer.data[1][3] = (WxSB_TranslationY->GetValue() - 100.0) / 50;
    buffer.data[2][3] = (WxSB_TranslationZ->GetValue() - 100.0) / 50;

    return buffer;
}


Matrix4 GUIMyFrame1::ScalingMatrix() {
    Matrix4 buffer;
    
    buffer.data[0][0] = WxSB_ScaleX->GetValue() / 100.0;
    buffer.data[1][1] = WxSB_ScaleY->GetValue() / 100.0;
    buffer.data[2][2] = WxSB_ScaleZ->GetValue() / 100.0;

    return buffer;
}

Matrix4 GUIMyFrame1::XRotateMatrix() {
    Matrix4 buffer;
    double angle = WxSB_RotateX->GetValue() * 3.14 / 180;

    buffer.data[0][0] = 1;
    buffer.data[1][1] = cos(angle);
    buffer.data[1][2] = sin(angle);
    buffer.data[2][1] = -sin(angle);
    buffer.data[2][2] = cos(angle);

    return buffer;
}

Matrix4 GUIMyFrame1::YRotateMatrix() {
    Matrix4 buffer;
    double angle = WxSB_RotateY->GetValue() * 3.14 / 180;

    buffer.data[0][0] = cos(angle);
    buffer.data[0][2] = -sin(angle);
    buffer.data[1][1] = 1;
    buffer.data[2][0] = sin(angle);
    buffer.data[2][2] = cos(angle);

    return buffer;
}

Matrix4 GUIMyFrame1::ZRotateMatrix() {
    Matrix4 buffer;
    double angle = WxSB_RotateZ->GetValue() * 3.14 / 180;

    buffer.data[0][0] = cos(angle);
    buffer.data[0][1] = sin(angle);
    buffer.data[1][0] = -sin(angle);
    buffer.data[1][1] = cos(angle);
    buffer.data[2][2] = 1;

    return buffer;
}


Matrix4 GUIMyFrame1::CenterMatrix(int w, int h) {
    Matrix4 buffer;
    buffer.data[0][0] = w / 2;
    buffer.data[0][3] = w / 2;
    buffer.data[1][1] = -h / 2;
    buffer.data[1][3] = h / 2;

    Matrix4 trans;
    trans.data[0][0] = 1.0;
    trans.data[1][1] = 1.0;
    trans.data[3][2] = 1.0 / 2.0;

    return buffer * trans;
}


void GUIMyFrame1::DrawSegment(wxDC* dc, Segment segment, int w, int h) {

    Vector4 begin, end;
    begin.data[0] = segment.begin.x;
    begin.data[1] = segment.begin.y;
    begin.data[2] = segment.begin.z;
    end.data[0] = segment.end.x;
    end.data[1] = segment.end.y;
    end.data[2] = segment.end.z;

    Matrix4 transform = CenterMatrix(w, h) * TranslationMatrix() * XRotateMatrix() * YRotateMatrix() * ZRotateMatrix() * ScalingMatrix();

    begin = transform * begin;
    end = transform * end;

    begin.data[0] /= begin.data[3];
    begin.data[1] /= begin.data[3];
    begin.data[2] /= begin.data[3];

    end.data[0] /= end.data[3];
    end.data[1] /= end.data[3];
    end.data[2] /= end.data[3];


    dc->SetPen(wxPen(wxColor(segment.color.R, segment.color.G, segment.color.B)));
    dc->DrawLine(wxPoint(begin.data[0], begin.data[1]), wxPoint(end.data[0], end.data[1]));

}














