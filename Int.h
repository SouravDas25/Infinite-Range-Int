

#ifndef INFINITE_INT_H
#define INFINITE_INT_H

#include<stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <bitset>

#ifdef __x86_64__
typedef uint64_t ubyte ;
#define ubyteMAX _UI64_MAX
const int bits = 64;
#else
typedef uint32_t ubyte ;
#define ubyteMAX _UI32_MAX
const int bits = 32;
#endif

class Int {
	protected :
		ubyte * data;
		int size;
		bool sign;
		
		bool getBit(int bitpos ) const ;
		int checkData(ubyte*tmp,int size);
		inline void printbyte(int i);
		inline void printhex(int i);
		void init(ubyte * tmp , int size ,int sign );
		
	public :
		Int(): data(NULL) , size(0) ,sign(0) {} ;
		Int(int64_t n);
		Int(const Int& i);
		
		~Int();
		
		Int& operator=(const Int& i);
		Int operator~() const ;
		
		friend Int operator>>(const Int& a,const int b);
		friend Int operator<<(const Int& a,const int b);
		friend Int operator^(const Int& a,const Int& b);
		friend Int operator|(const Int& a,const Int& b); 
		friend Int operator&(const Int& a,const Int& b);
		
		friend Int operator+(const Int& a, const Int& b);
		friend Int operator-(const Int& a, const Int& b);
		friend Int operator*(const Int& a, const Int& b);
		
		void printdata();
		int get_sign() const ;
};

#endif
