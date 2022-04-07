#ifndef __ChartClass__
#define __ChartClass__

#include <memory>
#include "ConfigClass.h"

class Matrix;

class ChartClass
{
    private:
        std::shared_ptr<ConfigClass> cfg;
        int x_step;         // liczba odcinkow z jakich bedzie sie skladal wykres
        double x_min,x_max; // zakres zmiennej x
        double y_min,y_max; // zakres wartosci przyjmowanych przez funkcje
        double GetFunctionValue(double x); // zwraca wartosci rysowanej funkcji
        //wxGraphicsContext* moje;
        double _step;
        
        void line2d(wxDC* dc, Matrix t,double x1,double y1,double x2,double y2);
        // powyzej sugerowana funkcja transformujaca odcinki (jesli ktos ma 
        // inny pomysl mozna ja usunac)
       
    public:
        ChartClass(std::shared_ptr<ConfigClass> c);
        void Set_Range();   // ustala wartosci zmiennych x_min,y_min,x_max,y_max
        double Get_Y_min(); // zwraca y_min
        double Get_Y_max(); // zwraca y_max
        void Draw(wxDC *dc, int w, int h);  // rysuje wykres
        void DrawAxes(wxDC *dc, Matrix t);
        void PrintText(wxDC* dc, Matrix t, double value, double x, double y);

        Matrix CenterMatrixTranslation(int w, int h);
        Matrix RotationMatrix(int w, int h);
        Matrix TranslateMatrix();

};

#endif
