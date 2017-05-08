
#ifndef INFINITE_INT_C
#define INFINITE_INT_C

#include "Int.h"

using namespace std;

ubyte howMuchOver(ubyte  a, ubyte b )
{
	return a - abs( ubyteMAX - b );
}

ubyte willOverflow( ubyte  a, ubyte b)
{
   if( a > abs(ubyteMAX - b) ) return 1 ;
   return 0;
}

Int::Int(int64_t n ) : data(NULL) , size(0) ,sign (0)
{
	uint64_t a = std::abs(n);
	if(n<0) a = ~a + 1ull , sign = 1 ;
	data = new ubyte[sizeof(int64_t)/sizeof(ubyte)];
	*(int64_t*)data = a;
	size = sizeof(int64_t)/sizeof(ubyte);
}

Int::Int(const Int& i) : data(NULL) , size(i.size) ,sign (i.sign)
{
	data = new ubyte[size];
	memcpy(data,i.data,size*sizeof(ubyte));
}

Int& Int::operator=(const Int& i)
{
	if(this == &i) return *this;
	if(data) delete data;
	data = new ubyte[i.size];
	memcpy(data,i.data,i.size*sizeof(ubyte));
	sign = i.sign;
	size = i.size;
	return *this;
}

Int::~Int()
{
	delete data;
}

int Int::checkData(ubyte*tmp,int size)
{
	int cnt = 0;
	for(int i = size-1;i>=0;--i)
	{
		if(tmp[i] != 0x0)return size - cnt;
		cnt++;
	}
	return 1;
}

void Int::init(ubyte * tmp , int size ,int sign )
{
	size = checkData(tmp,size);
	if(data) delete data;
	data = new ubyte[size];
	this->sign = sign;
	this->size = size;
	memcpy(data,tmp,size*sizeof(ubyte));
}

Int Int::operator~() const 
{
	Int t(*this);
	for(int i=0;i<size;i++)
	{
		t.data[i] = ~t.data[i];
	}
	return t;
}

Int operator^(const Int& a,const Int& b)
{
	int i,til = max(a.size,b.size),sign;
	ubyte tmp[til];
	ubyte aa,bb;
	for(i=0;i<til;i++)
	{
		aa = (i<a.size)? a.data[i]: ( a.get_sign() )? ubyteMAX : 0x0;
		bb = (i<b.size)? b.data[i]: ( b.get_sign() )? ubyteMAX : 0x0;
		tmp[i] = aa ^ bb;
	}
	sign = a.get_sign() ^ b.get_sign();
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int operator|(const Int& a,const Int& b)
{
	int i,til = max(a.size,b.size),sign;
	ubyte tmp[til];
	ubyte aa,bb;
	for(i=0;i<til;i++)
	{
		aa = (i<a.size)? a.data[i]: ( a.get_sign() )? ubyteMAX : 0x0;
		bb = (i<b.size)? b.data[i]: ( b.get_sign() )? ubyteMAX : 0x0;
		tmp[i] = aa | bb;
	}
	sign = a.get_sign() | b.get_sign();
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int operator&(const Int& a,const Int& b)
{
	int i,til = max(a.size,b.size),sign;
	ubyte tmp[til];
	ubyte aa,bb;
	for(i=0;i<til;i++)
	{
		aa = (i<a.size)? a.data[i]: ( a.get_sign() )? ubyteMAX : 0x0;
		bb = (i<b.size)? b.data[i]: ( b.get_sign() )? ubyteMAX : 0x0;
		tmp[i] = aa & bb;
	}
	sign = a.get_sign() & b.get_sign();
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int operator+(const Int& a, const Int& b)
{
	int i,til = max(a.size,b.size),sign;
	ubyte tmp[til+1];
	memset(tmp,0,(til+1)*sizeof(ubyte));
	ubyte cc = 0,aa,bb;
	for(i=0;i<til;i++)
	{
		aa = (i<a.size)? a.data[i]: ( a.get_sign() )? ubyteMAX : 0x0;
		bb = (i<b.size)? b.data[i]: ( b.get_sign() )? ubyteMAX : 0x0;
		tmp[i] = aa + bb + cc; 
		cc = ( willOverflow(aa,bb) ) ? willOverflow(aa,bb) : willOverflow(aa+bb,cc) ;
	}
	if( a.get_sign() == b.get_sign() )
	{
		if( cc == 1 ) tmp[i++] = cc;
		sign = a.get_sign();
	}
	else sign = !cc;
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int operator-(const Int& n, const Int& i)
{
	Int tmp = (~i) + 1;//tmp.printdata()//tmp.sign
	tmp.sign = !tmp.sign;
	return n + tmp;
}

Int operator<<(const Int& a,const int b)
{
	if(b<0)return 0;
	int i,til = a.size,srt = b/bits,cnt = 0;
	int ss = til+srt+1,sh=b;
	ubyte tmp[ss];
	memset(tmp,0,(ss)*sizeof(ubyte));
	ubyte aa,bb=0;
	sh = sh%bits;
	for(i=srt;i<ss;i++)
	{
		aa = (cnt<a.size)? a.data[cnt++]: ( a.get_sign() )? ubyteMAX : 0x0;
		tmp[i] = (aa << sh); 
		tmp[i] |= bb ;
		bb = (sh)? aa >> (bits-sh):0x0 ;
	}
	Int r;
	r.init(tmp,i,a.get_sign());
	return r;
}

Int operator>>(const Int& a,const int b)
{
	if( b > (bits*a.size) ) return 0;
	int i,til = a.size,srt = b/bits,cnt = a.size-1;
	int sh=b;
	ubyte tmp[til];
	memset(tmp,0,(til)*sizeof(ubyte));
	ubyte aa,bb=0;
	sh = sh%bits;
	for( i=til-srt-1 ; i>=0 ; --i ) // 1237016955531111108
	{
		aa = (cnt>=0)? a.data[cnt--]: ( a.get_sign() )? ubyteMAX : 0x0;
		tmp[i] = (aa >> sh);
		tmp[i] |= bb ;
		bb = (sh)? aa << (bits-sh):0x0 ;
	}
	Int r;
	r.init(tmp,til,a.get_sign());
	return r;
}

bool Int::getBit(int bitpos ) const 
{
	if(bitpos>size*bits || bitpos < 0) throw "INDEX OUT OF RANGE ERROR";
	int i = bitpos/bits;
	int m = bitpos%bits;
	return (data[i]>>m)&0x1;
}

Int operator*(const Int& a, const Int& b)
{
	Int n = 0;
	int i = 0 ;
	int til = b.size*bits;
	for(i=b.size*bits-1 ; i >=0 ; --i)
	{
		if(b.getBit(i))
		{
			til = i+1;
			break;
		}
	}
	for(i=0 ; i < til ; i++)
	{
		//cout << "TEST : ";n.printdata();
		if(b.getBit(i))
		{
			n = n + (a << i);
		}
	}
	return n;
}

inline void Int::printhex(int p)
{
	for(int i= (sizeof(ubyte))-1 ;i>=0;--i) 
	{
		int b = ( data[p] >> (i*CHAR_BIT) ) & 0xff;
		cout << hex << b ;
		if(i>0)printf(":");
	}
}

inline void Int::printbyte(int n) //7FFFFFFFFFFFFFFE
{
	for(int i= (sizeof(ubyte)*CHAR_BIT)-1 ;i>=0;--i)printf("%I64x",(data[n]>>i)&0x1);
}

void Int::printdata()
{
	if( get_sign() )printf("-");
	for(int i=size-1;i>=0;--i)
	{
		printbyte(i);
		if(i>0)cout << '-' ;
	}
	cout << endl;
}

int Int::get_sign() const
{
	return ( sign ) ;
}

#endif
