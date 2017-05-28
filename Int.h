

#ifndef INFINITE_INT_H
#define INFINITE_INT_H

#include<stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <iostream>
#include <algorithm>
#include <bitset>

#ifdef __x86_64__
typedef uint64_t ubyte ;
typedef uint32_t ubytehalf ;
#define ubyteMAX _UI64_MAX
#define ubytehalfMAX _UI32_MAX
const int bits = 64;
const int halfbits = 32;
#else
typedef uint32_t ubyte ;
typedef uint16_t ubytehalf ;
#define ubyteMAX _UI32_MAX
#define ubytehalfMAX _UI16_MAX
const int bits = 32;
const int halfbits = 16;
#endif

class Int {
	protected :
		ubyte * data;
		int _size;
		bool _sign;
		
		int checkData(ubyte*tmp,int size);
		inline void printbyte(int i) const;
		inline void printhex(int i) const;
		void init(ubyte * tmp , int size ,int sign );
		void init(int64_t n);
		
		friend int bitwise_compare(const Int& a, const Int& b);
		friend Int indigenous_multiplication(const Int& a, const Int& b);
		friend Int bitwise_multiplication(const Int& a, const Int& b);
		friend Int karatsuba_multiplication(const Int& a, const Int& b);
		friend void bitwise_division(const Int& n, const Int& d,Int * q,Int *r);
		
	public :
		Int(): data(NULL) , _size(0) ,_sign(0) {} ;
		Int(int64_t n,int size);
		Int(int64_t n);
		Int(const Int& i);
		
		virtual ~Int();
		
		Int& operator=(const Int& i);
		Int& operator++();
		Int operator++(int);
		Int& operator--();
		Int operator--(int);
		Int operator~() const ;
		
		friend Int operator>>(const Int& a,const int b);
		friend Int operator<<(const Int& a,const int b);
		friend Int operator^(const Int& a,const Int& b);
		friend Int operator|(const Int& a,const Int& b); 
		friend Int operator&(const Int& a,const Int& b);
		
		friend Int operator+(const Int& a, const Int& b);
		friend Int operator-(const Int& a, const Int& b);
		friend Int operator*(const Int& a, const Int& b);
		friend Int operator/(const Int& a, const Int& b);
		
		friend Int operator-(const Int& n);
		friend Int operator+(const Int& n);
		
		friend bool operator<(const Int& a,const Int& b);
		friend bool operator>(const Int& a,const Int& b);
		friend bool operator==(const Int& a,const Int& b);
		friend bool operator!=(const Int& a,const Int& b);
		friend bool operator<=(const Int& a,const Int& b);
		friend bool operator>=(const Int& a,const Int& b);
		
		friend std::ostream& operator<<(std::ostream& out,const Int& b);
		int64_t toint();
		
		void printdata() const ;
		bool getBit(int bitpos ) const ;
		void setBit(int bitpos , bool value ); 
		
		int sign() const ;
		void sign(int s) ;
		int size() const ;
		void size(int s) ;
};

Int fromString(const char * str);
Int abs(const Int& a);
Int pow(Int a,Int b);
Int powInt(int a,int b);
Int pow(Int a,int b);

#endif
