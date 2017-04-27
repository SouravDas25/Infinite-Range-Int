
/** @cond ADS_INTERNAL */

#ifndef ADSIMPROVEDINTEGER_C
#define ADSIMPROVEDINTEGER_C


#include "integer.h"

byte RightNibble(byte bcddigit)
{
	return bcddigit&15;
}

byte LeftNibble(byte bcddigit)
{
	return (bcddigit & 240)>>4;
}

byte PackedBCD(byte msb,byte lsb)
{
	byte bcd = 0;
	bcd |= RightNibble(lsb);
	bcd <<= 4;
	bcd |= RightNibble(msb);
	return bcd;
}

byte PackedBCD9compliment(byte a)
{
	byte ls = 9 - PackedBCDLSB(a) ;
	byte ms = 9 - PackedBCDMSB(a) ;
	return PackedBCD(ms,ls);
}

byte PackedBCDAdd(byte a,byte b,byte * carry)
{
	byte ls = PackedBCDLSB(a) + PackedBCDLSB(b) + *carry;
	if(ls>9)ls += 6,*carry = LeftNibble(ls);
	else *carry =0;
	ls = RightNibble(ls);
	byte ms = PackedBCDMSB(a) + PackedBCDMSB(b) + *carry;
	if(ms>9)ms += 6,*carry = LeftNibble(ms);
	else *carry =0;
	ms = RightNibble(ms);
	return PackedBCD(ms,ls);
}

void nines_compliment( byte * self,int size)
{
	int i;byte z;
    for(i=0;i<size-1;i++)
    {
    	z = self[i];
        self[i] = PackedBCD9compliment(z);
    }
    z = self[i];
    self[i] = PackedBCD9compliment(z) ;
}

void tens_compliment( byte * self,int size)
{
	byte t = PackedBCD((byte)0,(byte)1);
    nines_compliment(self,size);
    byte carry =0,a;int i;
    self[0] = PackedBCDAdd(self[0],t,&carry);
	for(i=1;i<size;i++)
	{
		a = self[i];
		self[i] = PackedBCDAdd(a,0,&carry);
	}
}

Integer::Integer(int n): data(NULL), sign(0) , blocksize(0) 
{
	byte tmp[11];//range of int64 = -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 approx 20 digit
	int sign = 0,bs= 0;
	if( n < 0 ) sign = 1 ;
	n = std::abs(n);
	while(n>0)
	{
		byte lsb = static_cast<byte>(n%10);
		n = n/10;
		byte msb =  static_cast<byte>(n%10);
		n = n/10;
		tmp[bs++] = PackedBCD(msb,lsb);
	}
	if(!bs)
	{
		tmp[bs++] = 0;
	}
	if(sign == 1) tens_compliment(tmp,bs);
	this->init(tmp,bs,sign);
}

Integer::Integer(const char *  num)  : data(NULL), sign(0) , blocksize(0) 
{
	int sign = (num[0] == '-')? 1:0;
	int size = strlen(num),cbt=0,i,stop = sign;
	for(i=sign;i<size;i++)
	{
		if( this->altonum(num[i]) != 0 )
		{
			break;
		}
		stop = i;
	}
	int block = ((size-stop)+1)>>1;
	block = (block == 0)?1:block;
	byte tmp[block] ;
	memset(tmp,0,block);
	for(i=size-1;i>=stop;i-=2)
	{
		byte msb,lsb;
		lsb = this->altonum(num[i]);
		msb = (i-1 >= 0+sign) ? this->altonum(num[i-1]):0;
		tmp[cbt++] = PackedBCD(msb,lsb);
	}
	if(sign == 1) tens_compliment(tmp,block);
	this->init(tmp,block,sign);
}

int Integer::altonum(char ch)
{
	int i = ch;
	if (i>=48 && i<= 57)
	{
		return i-48;
	}
	else
	{
		throw ch;
	}
}

void Integer::init(const byte *  num,int blocksize,int sign)
{
	if(blocksize == 1 && num[0] == 0) sign = 0;
	byte *tmp = (byte*)calloc(blocksize,sizeof(byte));
	memcpy(tmp,num,blocksize);
	if(data) free(data);
	this->data = tmp;
	this->blocksize = blocksize;
	this->sign = sign;
}

Integer::Integer(const Integer& a)  : data(NULL), sign(0) , blocksize(0) 
{
	this->init(a.data,a.blocksize,a.sign);
}

Integer Integer::NinesCompliment() const
{
	byte tmp[this->blocksize] ;
	memset(tmp,0,this->blocksize);
	memmove(tmp,this->data,this->blocksize);
	Integer i;
	nines_compliment(tmp,this->blocksize);
	i.init(tmp,this->blocksize,0);
	return i;
}


Integer::~Integer()
{
	free(data);
	//cout << " Destructor" << endl ;
}

int Integer::strip(byte * tmp,int size,int sign)
{
	int i ;int cmp = (sign)?9:0;
	for(i=size-1;i>=0;i--)
	{
		if(RightNibble(tmp[i]) != cmp)
		{
			return i+1;
		}
		if(LeftNibble(tmp[i]) != cmp )
		{
			return i+1;
		}
	}
	return 1;
}

Integer Integer::TensCompliment() const
{
	byte tmp[this->blocksize] ;
	memset(tmp,0,this->blocksize);
	memmove(tmp,this->data,this->blocksize);
	tens_compliment(tmp,this->blocksize);
	/*if(RightNibble(a->data[a->blocksize-1]) == 0)*/
	/*tmp[a->blocksize-1] = getPackedBCD(0,LeftNibble(tmp[a->blocksize-1]));*/
	Integer i;
	i.init(tmp,this->blocksize,0);
	return i;
}

Integer Integer::prepareSub() const
{
	Integer na = this->TensCompliment();
	na.sign = 1^this->sign;
	return na;
}

void printblockdata(byte * tmp,int size)
{
	int i ;printf("\nBD = ");
	for(i=0;i<size;i++)
	{
		printf("%d%d",LeftNibble(tmp[i]),RightNibble(tmp[i]));
	}
}

ostream& operator<<(ostream& out,const Integer& n)
{
	if(n.blocksize <= 0) return out;
	int i ;byte z;
	//out << "\n";
	byte tmp[n.blocksize];
	memset(tmp,0,n.blocksize);
	memmove(tmp,n.data,n.blocksize);
	if(n.sign == 1)
	{
		out << '-';
		tens_compliment(tmp,n.blocksize);
	}
	i = n.blocksize-1;
	z = tmp[i];
	if(PackedBCDMSB(z)) out << (int)PackedBCDMSB(z);
	out << (int)PackedBCDLSB(z);
	for(i=i-1;i>=0;i--)
	{
		z = tmp[i];
		out << (int)PackedBCDMSB(z) << (int)PackedBCDLSB(z);
	}
	return out;
}

Integer Integer::abs()
{
	byte tmp[this->blocksize];
	memset(tmp,0,this->blocksize);
	memmove(tmp,this->data,this->blocksize);
	if(this->sign == 1) tens_compliment(tmp,this->blocksize);
	Integer i;
	i.init(tmp,this->blocksize,0);
	return i;
}

int Integer::compare(const Integer& b) const
{
	if(this->sign < b.sign) return 1;
	if(this->sign > b.sign) return -1;
	if(this->blocksize>b.blocksize) return 1;
	if(this->blocksize<b.blocksize) return -1;
	int i;
	for(i=this->blocksize-1;i>=0;i--)
	{
		char ad = PackedBCDMSB(this->data[i]),bd = PackedBCDMSB(b.data[i]);
		if(ad>bd) return 1;
		if(ad<bd) return -1;
		ad = PackedBCDLSB(this->data[i]),bd = PackedBCDLSB(b.data[i]);
		if(ad>bd) return 1;
		if(ad<bd) return -1;
	}
	return 0;
}

bool Integer::operator<(const Integer& n) const
{
	if( this->compare(n) == -1 ) return true;
	return false;
}

bool Integer::operator<(const int& n) const
{
	Integer i(n);
	return *this < i;
}

bool operator<(const int& n,const Integer& i)
{
	Integer a(n);
	return a < i;
}

bool Integer::operator>(const Integer& n) const 
{
	if( this->compare(n) == 1 ) return true;
	return false;
}

bool Integer::operator>(const int& n) const
{
	Integer i(n);
	return *this > i;
}

bool operator>(const int& n, const Integer& i)
{
	Integer a(n);
	return a > i;
}

bool Integer::operator==(const Integer& n) const 
{
	if( this->compare(n) == 0 ) return true;
	return false;
}

bool Integer::operator==(const int& n) const 
{
	Integer i(n);
	return *this == i;
}

bool operator==(const int& n,const Integer& i)
{
	Integer a(n);
	return a == i;
}

bool Integer::operator<=(const Integer& n) const 
{
	int b = this->compare(n);
	if(  b <= 0 ) return true;
	return false;
}

bool Integer::operator<=(const int& n) const 
{
	Integer i(n);
	return *this <= i;
}

bool operator<=(const int& n,const Integer& i)
{
	Integer a(n);
	return a <= i;
}

bool Integer::operator>=(const Integer& n) const
{
	int b = this->compare(n);
	if(  b >= 0 ) return true;
	return false;
}

bool Integer::operator>=(const int& n) const
{
	Integer a(n);
	return *this >= a;
}

bool operator>=(const int& n,const Integer& i)
{
	Integer a(n);
	return a >= i;
}

bool Integer::operator!=(const Integer& n) const 
{
	if( this->compare(n) != 0 ) return true;
	return false;
}

bool Integer::operator!=(const int& n) const
{
	Integer a(n);
	return *this != a;
}

bool operator!=(const int& n,const Integer& i)
{
	Integer a(n);
	return a != i;
}

Integer& Integer::operator=(const Integer& n)  
{
	if( this == &n ) return *this;
	this->init(n.data,n.blocksize,n.sign);
	return *this;
}

Integer& Integer::operator=(const int& n)  
{
	Integer i(n);
	*this = i;
	return *this;
}

Integer Integer::operator+(const Integer& b)
{
	int sign = 0;
	int lena = this->blocksize,lenb = b.blocksize,i,maxbuf;
	maxbuf = (lena>=lenb)? lena : lenb ;
	byte tmpc[maxbuf+1];
	memset(tmpc,0,maxbuf);byte a1,b1;
	byte carry =0;int pada = (this->sign)?0x99:0x00;int padb = (b.sign)?0x99:0x00;
	for(i=0;i<maxbuf;i++)
	{
		a1 = (i<this->blocksize)? this->data[i]:pada;
		b1 = (i<b.blocksize)? b.data[i]:padb;
		tmpc[i] = PackedBCDAdd(a1,b1,&carry);
	}
	if(this->sign == b.sign)
	{
		if(this->sign)
		{
			if(!carry)
			tmpc[i++] = PackedBCD( (byte)0,(byte)9-(!carry)),maxbuf++;
			sign = this->sign;
		}
		else
		{
			if(carry)
			tmpc[i++] = PackedBCD(0,carry),maxbuf++;
			sign = this->sign;
		}
	}
	else
	{
		sign = !carry;
	}
	maxbuf = Integer::strip(tmpc,maxbuf,sign);
	Integer n;
	n.init(tmpc,maxbuf,sign);
	return n;
}

Integer Integer::operator+(const int& n)
{
	Integer i(n);
	return *this + i;
}

Integer Integer::operator-(const Integer& n)
{
	Integer cb = n.prepareSub();
	Integer sub = *this + cb ;
	return sub;
}

Integer Integer::operator-(const int& n)
{
	Integer i(n);
	return *this - i;
}

Integer Integer::operator*(const Integer& n)
{
	if( n == 0 || *this == 0 ) return 0;
	Integer a(0);
	for(int i = 0 ; i < n ; i++)
	{
		//cout << a << endl;
		a = a + *this;
		//getch();
	}
	return a;
}

Integer Integer::operator*(const int& n)
{
	Integer i(n);
	return *this * i;
}

Integer Integer::operator/(const Integer& n)
{
	if( n == 0 ) throw "divide by zero";
	Integer a(0),b(*this);
	while(b>0)
	{
		if(b>=n)a++;
		b = b - n;
	}
	return a;
}

Integer Integer::operator/(const int& n)
{
	Integer i(n);
	return *this / i;
}

Integer Integer::operator%(const Integer& n)
{
	if( n == 0 ) throw "divide by zero";
	Integer b(*this);
	while(b>=n)
	{
		b = b - n;
	}
	return b;
}

Integer Integer::operator++()
{
	*this = *this + 1 ;
	return *this;
}

Integer Integer::operator++(int)
{
	Integer a(*this);
	*this = *this + 1 ;
	return a;
}

#endif
/** @endcond  */
