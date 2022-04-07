#include <wx/dc.h>
#include <memory>

#include "ChartClass.h"
#include "vecmat.h"
#include <string>

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c)
{
    cfg= std::move(c);
    x_step=200;

}

void ChartClass::Set_Range()
{
 double xmin=9999.9,xmax=-9999.9,ymin=9999.9,ymax=-9999.9;
 double x,y,step;
 int i;

 xmin=cfg->Get_x_start();
 xmax=cfg->Get_x_stop();

 step=(cfg->Get_x_stop()-cfg->Get_x_start())/(double)x_step;
 x=cfg->Get_x_start();

 for (i=0;i<=x_step;i++)
  {
   y=GetFunctionValue(x);
   if (y>ymax) ymax=y;
   if (y<ymin) ymin=y;
   x=x+step;
  }

 y_min=ymin;
 y_max=ymax;
 x_min=xmin;
 x_max=xmax;
 //_step = (x_max - x_min) / x_step;
 _step = step;
}


double ChartClass::GetFunctionValue(double x)
{
 if (cfg->Get_F_type()==1) return x*x;
 if (cfg->Get_F_type()==2) return 0.5*exp(4*x-3*x*x);
 return x+sin(x*4.0);
}

void ChartClass::Draw(wxDC *dc, int w, int h)
{
    
    dc->SetBackground(wxBrush(RGB(255, 255, 255)));
    dc->Clear();
    dc->SetPen(wxPen(RGB(255, 0, 0)));
    dc->DrawRectangle(10, 10, w - 20, h - 20);
    dc->SetClippingRegion(10, 10, w - 20, h - 20);

    Matrix actuallmatrix;

    if (cfg->RotateScreenCenter()) {
        actuallmatrix = RotationMatrix(w, h) * TranslateMatrix() * CenterMatrixTranslation(w, h);
    }
    else {
        actuallmatrix = TranslateMatrix() * RotationMatrix(w, h) * CenterMatrixTranslation(w, h);
    }

    
    
    // Osie 
    dc->SetPen(wxPen(RGB(0, 0, 255)));
    DrawAxes(dc, actuallmatrix);
 
    // tekst na X
    for (double i = cfg->Get_x_start(); i < cfg->Get_x_stop(); i += (cfg->Get_x_stop() - cfg->Get_x_start())/4) {
        line2d(dc, actuallmatrix, i, -0.05, i, 0.05);
        PrintText(dc, actuallmatrix, i, i, 0);
    }

    // tekst na Y
    for (double i = Get_Y_min(); i < Get_Y_max(); i += (Get_Y_max() - Get_Y_min()) / 4) {
        line2d(dc, actuallmatrix, -0.05, i , 0.05, i);
        PrintText(dc, actuallmatrix, i, 0, i);
    }

    // Wykres
    Set_Range();
    dc->SetPen(wxPen(RGB(0, 255, 0)));
    for (double i = cfg->Get_x_start(); i < cfg->Get_x_stop(); i += (cfg->Get_x_stop() - cfg->Get_x_start()) / x_step) {
        line2d(dc, actuallmatrix, i, GetFunctionValue(i), i + _step, GetFunctionValue(i + _step));
    }


    
}

void ChartClass::DrawAxes(wxDC *dc, Matrix t) {

    line2d(dc, t, cfg->Get_x_start(), 0, cfg->Get_x_stop(), 0);
    line2d(dc, t, 0, Get_Y_min(), 0, Get_Y_max());

    line2d(dc, t, cfg->Get_x_stop(), 0, cfg->Get_x_stop() - 0.09, 0.07);
    line2d(dc, t, cfg->Get_x_stop(), 0, cfg->Get_x_stop() - 0.09, -0.07);
    line2d(dc, t, 0, Get_Y_max(), 0.05, Get_Y_max() - 0.05);
    line2d(dc, t, 0, Get_Y_max(), -0.05, Get_Y_max() - 0.05);




}

void ChartClass::PrintText(wxDC* dc, Matrix t, double value, double x, double y) {

    Vector a;
    a.Set(x, y);
    a = t * a;

    dc->DrawRotatedText(wxString::FromDouble(value, 2), a.GetX(), a.GetY(), 0);

}


void ChartClass::line2d(wxDC *dc, Matrix t, double x1, double y1, double x2, double y2)
{

    Vector first;
    Vector second;

    first.Set(x1, y1);
    second.Set(x2, y2);

    first = t * first;
    second = t * second;

    dc->DrawLine(first.GetX(), first.GetY(), second.GetX(), second.GetY());  
}

double ChartClass::Get_Y_min()
{
    Set_Range();
    return y_min;
}

double ChartClass::Get_Y_max()
{
    Set_Range();
    return y_max;
}


Matrix ChartClass::CenterMatrixTranslation(int w, int h) {
    Matrix buffer;

    Set_Range();
    double skalax = w / (cfg->Get_x1() - cfg->Get_x0());
    buffer.data[0][0] = skalax;
    double skalay = h / (cfg->Get_y1() - cfg->Get_y0());
    buffer.data[1][1] = -skalay;

    return buffer;
}

Matrix ChartClass::RotationMatrix(int w, int h) {

    double angle = cfg->Get_Alpha() * 3.14 / 180;
    Matrix buffer;

    buffer.data[0][2] = w / 2;
    buffer.data[1][2] = h / 2;
    
    buffer.data[0][0] = cos(angle);
    buffer.data[0][1] = -sin(angle);
    buffer.data[1][0] = sin(angle);
    buffer.data[1][1] = cos(angle);


    return buffer;

}


Matrix ChartClass::TranslateMatrix() {

    Matrix buffer;


    buffer.data[0][0] = 1;
    buffer.data[1][1] = 1;
    buffer.data[0][2] += cfg->Get_dX();
    buffer.data[1][2] += cfg->Get_dY();

    return buffer;
}




