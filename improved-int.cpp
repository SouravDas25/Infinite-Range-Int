

#include "Int.cpp"

 
int main()
{
	Int a = _I64_MAX , b = 64 , c ;
	a.printdata();
	b.printdata();
	c = a + b ;
	//c = c + a;
	c.printdata();
	
	c = a << 65;
	c.printdata();
	c = c >> 90;
	c.printdata();
	c = a * b ;
	c.printdata();
}
