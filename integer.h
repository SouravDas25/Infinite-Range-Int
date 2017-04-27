/** @cond ADS_INTERNAL */

#ifndef ADSIMPROVEDINTEGER_H
#define ADSIMPROVEDINTEGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cmath>
#include <conio.h>
#include<iostream>

typedef int8_t byte;
using namespace std;

#define PackedBCDMSB RightNibble
#define PackedBCDLSB LeftNibble

class Integer {
	
	protected :
		byte * data;
		short int sign;
		short int blocksize;
		
		int altonum(char ch);
		void init(const byte *  num, int blocksize, int sign);
		Integer prepareSub() const ;
		static int strip(byte * tmp,int size,int sign);
	
	public :
		Integer() : data(NULL), sign(0) , blocksize(0) {};
		Integer(const char * );
		Integer(int n);
		Integer(const Integer& i);
		
		Integer NinesCompliment() const ;
		Integer TensCompliment() const ;
		
		~Integer();
		Integer abs();
		int compare(const Integer& n) const ;
		
		bool operator<(const Integer& n) const ;
		bool operator<(const int& n) const ;
		friend bool operator<(const int& n,const Integer& i) ;
		
		bool operator>(const Integer& n) const ;
		bool operator>(const int& n) const ;
		friend bool operator>(const int& n, const Integer& i) ;
		
		bool operator==(const Integer& n) const ;
		bool operator==(const int& n) const ;
		friend bool operator==(const int& n,const Integer& i) ;
		
		bool operator<=(const Integer& n) const ;
		bool operator<=(const int& n) const ;
		friend bool operator<=(const int& n,const Integer& i) ;
		
		bool operator>=(const Integer& n) const ;
		bool operator>=(const int& n) const ;
		friend bool operator>=(const int& n,const Integer& i) ;
		
		bool operator!=(const Integer& n) const ;
		bool operator!=(const int& n) const ;
		friend bool operator!=(const int& n,const Integer& i) ;
		
		Integer operator++();
		Integer operator++(int);
		
		Integer operator+(const Integer& n);
		Integer operator+(const int& n);
		
		Integer operator-(const Integer& n);
		Integer operator-(const int& n);
		
		Integer operator*(const Integer& n);
		Integer operator*(const int& n);
		
		Integer operator/(const Integer& n);
		Integer operator/(const int& n);
		
		Integer operator%(const Integer& n);
		
		Integer& operator=(const Integer& n);
		Integer& operator=(const int& n);
		
		friend ostream& operator<<(ostream& out,const Integer& i);
};

#endif
/** @endcond  */
