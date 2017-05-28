
//#define DEBUG

#include "Int.cpp"
#include <conio.h>


 
int main()
{
	Int a = fromString("9,223,372,036,854,775,807") , b = 10 , c ;
	cout << " a : " << a << " , b : " << b << endl ;
	cout << " addition : " ;
	c = a + b ;
	cout << c << endl;
	cout << " subtraction : " ;
	c = a - b ;
	cout << c << endl;
	cout << " right shift : " ;
	c = a << 2;
	cout << c << endl;
	cout << " left shift : " ;
	c = a >> 2;
	cout << c << endl;
	cout << " multiplication : " ;
	c = a * b;
	cout << c << endl;
	cout << " division : " ;
	c = a / b;
	cout << c << endl;
}
