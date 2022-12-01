#pragma once
#include<algorithm>
#include<cassert>
#include<iostream>
using namespace std;
class MyIntVector {

public:
	MyIntVector();
	MyIntVector(size_t size);
	MyIntVector(const MyIntVector & v);
	~MyIntVector();
	

	int& operator[](size_t where);
	
	
	void pop_back();
	void push_back(int x);
	
	MyIntVector& operator=(const MyIntVector& source); 

	void operator+=(const MyIntVector & addend);

	MyIntVector& operator+(const MyIntVector& v);
	MyIntVector& operator-(const MyIntVector& v);
	MyIntVector& operator*(const MyIntVector& v);

	MyIntVector& operator-();
	bool operator==(const MyIntVector& v);
	MyIntVector& operator()(int n);
	
	size_t size() const { return used; };
	size_t capacity() const { return space; };
	
	
	void reserve(size_t n);
	bool isEmpty() const;
	void clear();
	
private:
	int * data;
	size_t used;
	size_t space;
};

class Different_size {};

class Bad_area {};