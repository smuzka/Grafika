#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
 m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
 this->SetBackgroundColour(wxColor(192, 192, 192));
 m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
 m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

void GUIMyFrame1::m_scrolledWindow_update( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_b_grayscale_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.ConvertToGreyscale();
}

void GUIMyFrame1::m_b_blur_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Blur(5);
}

void GUIMyFrame1::m_b_mirror_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Mirror();
}

void GUIMyFrame1::m_b_replace_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Replace(254,0,0,0,0,255);
}

void GUIMyFrame1::m_b_rescale_click( wxCommandEvent& event )
{	
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Rescale(320,240);
	m_scrolledWindow->ClearBackground();
}

void GUIMyFrame1::m_b_rotate_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org.Rotate(30 * 3.14 / 190, wxPoint(0, 0));

}

void GUIMyFrame1::m_b_rotate_hue_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org;
	Img_Cpy.RotateHue(0.5);
}

void GUIMyFrame1::m_b_mask_click( wxCommandEvent& event )
{
	Img_Cpy = Img_Org;
	Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
	m_scrolledWindow->ClearBackground();

}

void GUIMyFrame1::m_s_brightness_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
Brightness(m_s_brightness->GetValue() - 100);
Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
Contrast(m_s_contrast->GetValue() - 100);
Repaint();
}

void GUIMyFrame1::m_b_prewitt_click( wxCommandEvent& event )
{
	unsigned int size = 3 * Img_Org.GetSize().GetWidth() * Img_Org.GetSize().GetHeight();
	unsigned char* orgData = Img_Org.GetData();
	unsigned char* newData = (unsigned char*)malloc(size);

	int width = Img_Org.GetSize().GetWidth();
	int s[6] = {};

	for (int k = 0; k < size; k++) {
		s[0] = k - 3 * width - 3;
		s[1] = k - 3 * width + 3;
		s[2] = k - 3;
		s[3] = k + 3;
		s[4] = k + 3 * width - 3;
		s[5] = k - 3 * width + 3;

		for (int i = 0; i < 6; i++) {
			if (s[i] < 0) {
				s[i] += 3 * width;
			}
			else if (s[i] > size) {
				s[i] -= 3 * width;
			}
		}

		int newValue = -orgData[s[0]] + orgData[s[1]] - orgData[s[2]] + orgData[s[3]] - orgData[s[4]] + orgData[s[5]];

		if(newValue < 0){
			newValue = 0;
		}
		else if (newValue > 255) {
			newValue = 255;
		}


		newData[k] = newValue;


		

	}




	Img_Cpy.SetData(newData);


 // TO DO: Pionowa maska Prewitta
}

void GUIMyFrame1::m_b_threshold_click( wxCommandEvent& event )
{
	unsigned int size = 3 * Img_Org.GetWidth() * Img_Org.GetHeight();
	unsigned char* orgData = Img_Org.GetData();
	unsigned char* newData = (unsigned char*)malloc(size);

	for (unsigned i = 0; i < size; i++) {
		if (orgData[i] > 128) {
			newData[i] = 255;
		}
		else {
			newData[i] = 0;
		}

	}

	Img_Cpy.SetData(newData);
	

}


void GUIMyFrame1::Contrast(int value)
{
	unsigned int size = 3 * Img_Org.GetWidth() * Img_Org.GetHeight();
	unsigned char* orgData = Img_Org.GetData();
	unsigned char* newData = (unsigned char*)malloc(size);

	double factor = value / 100.0 + 1;

	for (int i = 0; i < size; i++) {
		int newcolor = (orgData[i] - 128) * factor + 128;
		if (newcolor < 0)
			newData[i] = 0;
		else if (newcolor > 254)
			newData[i] = 255;
		else
			newData[i] = newcolor;
	}

	Img_Cpy.SetData(newData);

}

void GUIMyFrame1::Repaint()
{
 wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
 wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
 m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
 dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::Brightness(int value)
{
	unsigned int size = 3 * Img_Org.GetWidth() * Img_Org.GetHeight();
	unsigned char* orgData = Img_Org.GetData();
	unsigned char* newData = (unsigned char*)malloc(size);

	for (unsigned i = 0; i < size; i++) {
		if (orgData[i] + value < 0)
			newData[i] = 0;
		else if (orgData[i] + value > 255)
			newData[i] = 255;
		else
			newData[i] = orgData[i] + value;
	}

	Img_Cpy.SetData(newData);

}