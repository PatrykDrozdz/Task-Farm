#include "EasyBMP.h"
using namespace std;

int main( int argc, char* argv[] )
{
	long sumowanie = 0;
	long srednia_luminancja = 0;

	if( argc != 3 )
	{
		cout << "Zastosowanie: Kolor BMP do skali szarosci <nazwa_pliku_wejsciowego> <nazwa_pliku_wyjsciowego>" << endl << endl;
		return 1;
	}

	// Deklaracja i czytanie bitmpay
	BMP wejscie;
	wejscie.ReadFromFile( argv[1] );

	// Konwersja do skali szarosci kazdego piksela za pomoca RGB-> YUV
	for( int j = 0 ; j < wejscie.TellHeight() ; j++)
	{
		for( int i = 0 ; i < wejscie.TellWidth() ; i++)
		{
			int Temp = (int) floor( 0.299*wejscie(i,j)->Red +
			0.587*wejscie(i,j)->Green +
			0.114*wejscie(i,j)->Blue );

			sumowanie += Temp;

			ebmpBYTE TempBYTE = (ebmpBYTE) Temp;
			wejscie(i,j)->Red = TempBYTE;
			wejscie(i,j)->Green = TempBYTE;
			wejscie(i,j)->Blue = TempBYTE;
		}
	}

	srednia_luminancja = ( sumowanie / (wejscie.TellWidth()*wejscie.TellHeight()) );

	cout << "Sumowanie wynosi = " << sumowanie << endl;
	cout << "Srednia luminancja bitmapy wynosi = " << srednia_luminancja << endl;

	for( int j = 0 ; j < wejscie.TellHeight() ; j++)
	{
		for( int i = 0 ; i < wejscie.TellWidth() ; i++)
		{
			int Temp = (int) floor( 0.299*wejscie(i,j)->Red +
			0.587*wejscie(i,j)->Green +
			0.114*wejscie(i,j)->Blue );

			if(Temp > srednia_luminancja)
			{
				wejscie(i,j)->Red = 0;
				wejscie(i,j)->Green = 0;
				wejscie(i,j)->Blue = 0;
			}
			else
			{
				wejscie(i,j)->Red = 255;
				wejscie(i,j)->Green = 255;
				wejscie(i,j)->Blue = 255;
			}
		}
	}

	// Utworzenie tabeli kolorww w skali szarosci, jezli zajdzie taka koniecznosc
	if( wejscie.TellBitDepth() < 16 )
	{
		CreateGrayscaleColorTable( wejscie );
	}

	// Zapisanie pliku wyjsciowego
	wejscie.WriteToFile( argv[2] );

	return 0;
}
