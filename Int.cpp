
#ifndef INFINITE_INT_C
#define INFINITE_INT_C

#include "Int.h"

using namespace std;

/*
ubyte howMuchOver(ubyte  a, ubyte b )
{
	return a - abs( ubyteMAX - b );
}*/

ubyte willOverflow( ubyte  a, ubyte b)
{
	if(ubyteMAX == b && a > 0) return 1;
	if( a > ubyteMAX - b ) return 1 ;
	return 0;
}

Int::Int(int64_t n ) : data(NULL) , _size(0) , _sign(0)
{
	init(n);
}

Int::Int(int64_t n,int size): data(NULL) , _size(0) , _sign(0)
{
	size = size/bits+1;
	data = new ubyte[size];
	memset(data,0,sizeof(ubyte)*size);
	uint64_t a = std::abs(n);
	if(n<0) a = ~a + 1ull , sign(1) ;
	*(int64_t*)data = a;
	this->size( size ) ;
}

Int::Int(const Int& i) : data(NULL) , _size(0) , _sign(0)
{
	init(i.data,i.size(),i.sign());
}

Int& Int::operator=(const Int& i)
{
	if(this == &i) return *this;
	init(i.data,i.size(),i.sign());
	return *this;
}

Int::~Int()
{
	delete data;
}

int Int::size() const
{
	return _size;
}

void Int::size(int s)
{
	_size = s;
}

void Int::sign(int s)
{
	_sign = s ;
}

int Int::sign() const
{
	return _sign  ;
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

void Int::init(int64_t n)
{
	uint64_t a = std::abs(n);
	if(n<0) a = ~a + 1ull , sign(1) ;
	if(data) delete data;
	data = new ubyte[sizeof(int64_t)/sizeof(ubyte)];
	*(int64_t*)data = a;
	size( sizeof(int64_t)/sizeof(ubyte) ) ;
}

void Int::init(ubyte * tmp , int size ,int sign )
{
	size = checkData(tmp,size);
	if(data) delete data;
	data = new ubyte[size];
	this->sign(sign) ;
	this->size(size) ;
	memcpy(data,tmp,size*sizeof(ubyte));
}

Int Int::operator~() const 
{
	Int t(*this);
	for(int i=0;i<size();i++)
	{
		t.data[i] = ~t.data[i];
	}
	return t;
}

Int operator^(const Int& a,const Int& b)
{
	int i,til = max(a.size(),b.size()),sign;
	ubyte tmp[til];
	ubyte aa,bb;
	for(i=0;i<til;i++)
	{
		aa = ( i<a.size() )? a.data[i]: ( a.sign() )? ubyteMAX : 0x0;
		bb = ( i<b.size() )? b.data[i]: ( b.sign() )? ubyteMAX : 0x0;
		tmp[i] = aa ^ bb;
	}
	sign = a.sign() ^ b.sign();
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int operator|(const Int& a,const Int& b)
{
	int i,til = max(a.size(),b.size()),sign;
	ubyte tmp[til];
	ubyte aa,bb;
	for(i=0;i<til;i++)
	{
		aa = ( i<a.size() )? a.data[i]: ( a.sign() )? ubyteMAX : 0x0;
		bb = ( i<b.size() )? b.data[i]: ( b.sign() )? ubyteMAX : 0x0;
		tmp[i] = aa | bb;
	}
	sign = a.sign() | b.sign();
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int operator&(const Int& a,const Int& b)
{
	int i,til = max(a.size(),b.size()),sign;
	ubyte tmp[til];
	ubyte aa,bb;
	for(i=0;i<til;i++)
	{
		aa = ( i<a.size() )? a.data[i]: ( a.sign() )? ubyteMAX : 0x0;
		bb = ( i<b.size() )? b.data[i]: ( b.sign() )? ubyteMAX : 0x0;
		tmp[i] = aa & bb;
	}
	sign = a.sign() & b.sign();
	Int r;
	r.init(tmp,i,sign);
	return r;
}

bool operator<(const Int& a,const Int& b)
{
	if( bitwise_compare(a,b) == -1 ) return true;
	return false;
}

bool operator>(const Int& a,const Int& b)
{
	if( bitwise_compare(a,b) == 1 ) return true;
	return false;
}

bool operator==(const Int& a,const Int& b)
{
	if( bitwise_compare(a,b) == 0 ) return true;
	return false;
}

bool operator!=(const Int& a,const Int& b)
{
	if( bitwise_compare(a,b) != 0 ) return true;
	return false;
}

bool operator<=(const Int& a,const Int& b)
{
	if( bitwise_compare(a,b) < 1 ) return true;
	return false;
}

bool operator>=(const Int& a,const Int& b)
{
	if( bitwise_compare(a,b) > -1 ) return true;
	return false;
}


int bitwise_compare(const Int& a, const Int& b)
{
	if(!a.sign() > !b.sign())return 1;
	if(!a.sign() < !b.sign())return -1;
	if( a.size() > b.size() ) return 1;
	else if( a.size() < b.size() )return -1;
	int i;
	for(i=a.size()-1;i>=0;--i)
	{
		if(a.data[i] > b.data[i]) return 1;
		if(a.data[i] < b.data[i]) return -1;
	}
	return 0;
}

Int& Int::operator++()
{
	*this = *this + 1;
	return *this;
}

Int Int::operator++(int)
{
	Int tmp(*this);
	++*this;
	return tmp;
}

Int operator+(const Int& n)
{
	return n;
}

Int operator+(const Int& a, const Int& b)
{
	int i,til = max(a.size(),b.size()),sign;
	ubyte tmp[til+1];
	memset(tmp,0,(til+1)*sizeof(ubyte));
	ubyte cc = 0,aa,bb;
	for(i=0;i<til;i++)
	{
		aa = ( i<a.size() )? a.data[i]: ( a.sign() )? ubyteMAX : 0x0;
		bb = ( i<b.size() )? b.data[i]: ( b.sign() )? ubyteMAX : 0x0;
		tmp[i] = aa + bb + cc; 
		cc = ( willOverflow(aa,bb) ) ? willOverflow(aa,bb) : willOverflow(aa+bb,cc) ;
	}
	if( a.sign() == b.sign() )
	{
		if( cc == 1 ) tmp[i++] = cc;
		sign = a.sign();
	}
	else sign = !cc;
	Int r;
	r.init(tmp,i,sign);
	return r;
}

Int& Int::operator--()
{
	*this = *this - 1 ;
	return *this;
}

Int Int::operator--(int)
{
	Int tmp(*this);
	*this = *this - 1 ;
	return tmp;
}

Int operator-(const Int& n)
{
	Int tmp = (~n) + 1;
	tmp.sign(!tmp.sign()) ;
	return tmp;
}

Int operator-(const Int& n, const Int& i)
{
	Int tmp = -i;
	return n + tmp;
}

Int operator<<(const Int& a,const int b)
{
	if(b<0)return 0;
	int i,til = a.size(),srt = b/bits,cnt = 0;
	int ss = til+srt+1,sh=b;
	ubyte tmp[ss];
	memset(tmp,0,(ss)*sizeof(ubyte));
	ubyte aa,bb=0;
	sh = sh%bits;
	for(i=srt;i<ss;i++)
	{
		aa = (cnt<a.size())? a.data[cnt++]: ( a.sign() )? ubyteMAX : 0x0;
		tmp[i] = (aa << sh); 
		tmp[i] |= bb ;
		bb = (sh)? aa >> (bits-sh):0x0 ;
	}
	Int r;
	r.init(tmp,i,a.sign());
	return r;
}

Int operator>>(const Int& a,const int b)
{
	if( b > (bits*a.size()) ) return 0;
	int i,til = a.size(),srt = b/bits,cnt = a.size()-1;
	int sh=b;
	ubyte tmp[til];
	memset(tmp,0,(til)*sizeof(ubyte));
	ubyte aa,bb=0;
	sh = sh%bits;
	for( i=til-srt-1 ; i>=0 ; --i ) // 1237016955531111108
	{
		aa = (cnt>=0)? a.data[cnt--]: ( a.sign() )? ubyteMAX : 0x0;
		tmp[i] = (aa >> sh);
		tmp[i] |= bb ;
		bb = (sh)? aa << (bits-sh):0x0 ;
	}
	Int r;
	r.init(tmp,til,a.sign());
	return r;
}

void Int::setBit(int bitpos , bool value )
{
	if(bitpos>size()*bits || bitpos < 0) throw "INDEX OUT OF RANGE ERROR";
	int i = bitpos/bits;
	int m = bitpos%bits;
	ubyte b = 1;
	if(value) data[i] |= b << m;
	else data[i] &= ~(b << m);
}

bool Int::getBit(int bitpos ) const 
{
	if(bitpos>size()*bits-1 || bitpos < 0) throw "INDEX OUT OF RANGE ERROR";
	int i = bitpos/bits;
	int m = bitpos%bits;
	return (data[i]>>m)&0x1;
}

ubyte LSB(ubyte bcddigit)
{
	return bcddigit&ubytehalfMAX;
}

ubyte MSB(ubyte bcddigit)
{
	return (bcddigit & ( (ubyte)ubytehalfMAX << (sizeof(ubytehalf)*CHAR_BIT) )) >> (sizeof(ubytehalf)*CHAR_BIT) ;
}

Int operator*(const Int& a, const Int& b)
{
	Int c,aa,bb;
	aa = (a<0)?-a:a;
	bb = (b<0)?-b:b;
	c = indigenous_multiplication(aa,bb);
	if(a.sign() != b.sign()) c.sign(1);
	return c;
}

Int indigenous_multiplication(const Int& a, const Int& b)
{
	ubytehalf * ad = (ubytehalf *)a.data , *bd = (ubytehalf *)b.data;
	Int n = 0;
	int i,p;
	ubyte aa,bb;ubyte cc=0,dd;
	for(i=0;i<b.size()*2;i++)
	{
		bb = bd[i];
		ubyte tmp[a.size()+1];
		ubytehalf * tmpim = (ubytehalf *)tmp;
		memset(tmp,0,sizeof(ubyte)*(a.size()+1));
		cc=0;
		for(p = 0;p<a.size()*2;p++)
		{
			aa = ad[p];
			dd = aa*bb+cc;
			cc = MSB(dd);
			tmpim[p] = LSB(dd);
			
			#ifdef DEBUG
			cout << " p , i = " << p << " , " << i << endl << hex ;
			cout << "aa = " << aa << endl ;
			cout << "bb = " << bb << endl;
			cout << "dd = " << dd << endl;
			cout << "cc = " << cc << endl;
			cout << "tmpim[p] = " << tmpim[p] << endl ;
			cout << dec ;
			#endif
		}
		if(cc) 
		{
			tmpim[p++] = cc;
			
			#ifdef DEBUG
			cout << hex << "Carry tmpim[p] = " << tmpim[p-1] << endl << dec;
			#endif
		}
		Int j ;
		j.init(tmp,a.size()+1,0);
		j = j<<(i*halfbits);
		n = n + j;
		
		#ifdef DEBUG
		cout << "j : " ;
		j.printdata();
		cout << "n : " ;
		n.printdata();
		#endif
	}
	return n;
}

Int karatsuba_multiplication(const Int& a, const Int& b)
{
	ubytehalf * ad = (ubytehalf *)a.data , *bd = (ubytehalf *)b.data;
	Int n = 0;
	int i,p;
	ubyte aa,bb, dd;
	for(i=0;i<a.size()*2;i++)
	{
		aa = ad[i];
		for(p = 0;p<b.size()*2;p++)
		{
			bb = bd[p];
			dd = aa*bb;
			Int j = dd;
			j = j << ((i+p)*halfbits) ;
			n = n + j;
			
			#ifdef DEBUG
			cout << "j : " ;
			j.printdata();
			cout << "n : " ;
			n.printdata();
			cout << "itr : " << i << endl;
			#endif
		}
	}
	return n;
}

Int bitwise_multiplication(const Int& a, const Int& b)
{
	Int n = 0;
	int i = 0 ;
	int til = b.size()*bits;
	for(i=b.size()*bits-1 ; i >=0 ; --i)
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

void bitwise_division(const Int& n, const Int& d,Int * _q,Int *_r)
{
	if(d==0) throw "Divide By Zero Error";
	Int q(0,n.size()*bits) ,r(0,n.size()*bits);
	for(int i = n.size()*bits-1;i>=0;--i)
	{
		r = r << 1;
		r.setBit(0,n.getBit(i));
		if(r >= d)
		{
			r = r - d ;
			q.setBit(i,1);
		}
		#ifdef DEBUG
		cout << "I : " << dec << i << endl;
		cout << "n : " ;
		n.printdata();
		cout << "r : " ;
		r.printdata();
		cout << "q : " ;
		q.printdata();
		cout << endl;
		#endif
	}
	if(_q)*_q = q;
	if(_r)*_r = r;
}

Int operator/(const Int& a, const Int& b)
{
	Int q ;
	Int c,aa,bb;
	aa = (a.sign())?-a:a;
	bb = (b.sign())?-b:b;
	
	#ifdef DEBUG
	cout << "aa : " ;
	aa.printdata();
	cout << "bb : " ;
	bb.printdata();
	cout << endl;
	#endif
	
	bitwise_division(aa,bb,&q,NULL);
	if(a.sign() != b.sign()) q.sign(1);
	return q;
}

inline void Int::printhex(int p) const
{
	for(int i= (sizeof(ubyte))-1 ;i>=0;--i) 
	{
		int b = ( data[p] >> (i*CHAR_BIT) ) & 0xff;
		cout << hex << b ;
		if(i>0)printf(":");
	}
}

inline void Int::printbyte(int n) const //7FFFFFFFFFFFFFFE
{
	for(int i= (sizeof(ubyte)*CHAR_BIT)-1 ;i>=0;--i)printf("%I64x",(data[n]>>i)&0x1);
}

void Int::printdata() const
{
	if( sign() )printf("-");
	for(int i=size()-1;i>=0;--i)
	{
		printhex(i);
		if(i>0)cout << '-' ;
	}
	cout << endl;
}

int altonum(char ch)
{
	int i = ch;
	if (isdigit(i))
	{
		return ch-'0';
	}
	else if(isalpha(i))
	{
		if(isupper(i)) return (i-65)+10;
		else return (i-97)+10;
	}
	else
	{
		throw i;
	}
}

Int fromString(const char * str)
{
	int len = strlen(str),i,cnt = 0,base = 10,sign = 0;
	Int sum = 0 ;
	if( *str == '-') sign = 1,str++;
	if( *str == '0' ) base = 8,str++;
	if( *str == 'x' ) base = 16,str++;
	else if( *str == 'b' ) base = 2,str++;
	for(i=len-1;i>=0;--i)
	{
		if(isalnum(str[i]))
		{
			int a = altonum(str[i]);
			if(a>=base) throw "Invalid Number in current base Error";
			sum = sum + a*powInt(base,cnt++);
		}
	}
	sum.sign(sign);
	return sum;
}

int64_t Int::toint()
{
	if(size()<2)
	{
		return data[0];
	}
	return 0;
}

ostream& printInt(const Int& n,ostream& out)
{
	char num[n.size()*20];int cnt = 0;
	if(n.sign()) out << '-' ;
	Int tmp = abs(n);
	while(tmp>0)
	{
		Int r = 0;
		bitwise_division(tmp,10,&tmp,&r);
		int a = r.toint();
		num[cnt++] = a + '0';
	}
	num[cnt++] = '\0';
	strrev(num);
	out << num;
	return out;
}

ostream& operator<<(ostream& a,const Int& b)
{
	return printInt(b,a);
}

Int abs(const Int& a)
{
	if(a<0)return -a;
	return a;
}

Int powInt(int a,int b)
{
	Int acc = 1,aa = a;
	for(int i = 0;i<b;++i)
	{
		acc = acc * aa;
	}
	return acc;
}

Int pow(const Int& a,int b)
{
	Int acc = 1;
	for(int i = 0;i<b;++i)
	{
		acc = acc * a;
	}
	return acc;
}

Int pow(const Int& a,const Int& b)
{
	Int acc = 1;
	for(Int i = 0;i<b;++i)
	{
		acc = acc * a;
	}
	return acc;
}

#endif
