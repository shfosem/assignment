//2017203038 ¼­½ÂÇö
#include "MyIntVector.h"
#include <iostream>

using namespace std;

void print_MyIntVector(MyIntVector& v)
{

	for (int i = 0; i < v.size(); i++)
		cout << v[i] << " ";
	cout << "\n" << "used: " << v.size() << " capacity: " << v.capacity() << endl;
}

void check_basement()
{
	MyIntVector v1;

	size_t size = 10;
	MyIntVector v2(size);

	MyIntVector v3(10);

	cout << "check_basement()" << endl;
	cout << "size and capacity of v1 " << v1.size() << " " << v1.capacity() <<endl; //check size(), capacity()
	cout << "size and capacity of v2 " << v2.size() << " " << v2.capacity() <<endl;
	
	cout << "size and capacity of v3 " << v3.size() << " " << v3.capacity() <<endl;
	
	v1.reserve(30);
	cout << "size and capacity of v1 " << v1.size() << " " << v1.capacity() << endl;
	

}

void check_push_popback()
{

	MyIntVector v1;
	
	for (int i = 0; i < 15; i++)
		v1.push_back(i);
	cout << "check_push_popback()" << endl;
	
	cout << "v1: ";
	print_MyIntVector(v1);
	
	
	cout << "pop_back 5 entries" << endl;
	for(int i = 0; i< 5; i++)
		v1.pop_back();
	cout << "v1: ";
	print_MyIntVector(v1);
	
}

void check_isEmpty_clear()
{
	MyIntVector v1;
	for (int i = 0; i < 10; i++)
		v1.push_back(i);
	cout << "v1: ";
	print_MyIntVector(v1);
	cout << "isEmpty(): " << v1.isEmpty() << endl;
	
	v1.clear();
	cout <<"After claer() " << "v1: ";
	print_MyIntVector(v1);
	cout << "isEmpty(): " << v1.isEmpty() << endl;
}

void check_basic_operator()
{
	MyIntVector v1;
	v1.push_back(3); v1.push_back(6); v1.push_back(9);
	MyIntVector v2;
	v2.push_back(7); v2.push_back(11); v2.push_back(13);
	cout << "v1: ";
	print_MyIntVector(v1);
	cout << "v2: ";
	print_MyIntVector(v2);
	v1 += v2;
	cout << "After += v1: ";
	print_MyIntVector(v1);
}
void check_binary_operator() 
{
	MyIntVector v1;
	MyIntVector v2;
	MyIntVector v3;
	MyIntVector v4;
	MyIntVector v5;
	MyIntVector errorvector;
	v1.push_back(3); v1.push_back(6); v1.push_back(9);
	v2.push_back(7); v2.push_back(11); v2.push_back(13);
	errorvector.push_back(1);
	cout << "v1: ";
	print_MyIntVector(v1);
	cout << "v2: ";
	print_MyIntVector(v2);

	v3 = v1 + v2;
	cout << "v1 + v2 = v3 : ";
	print_MyIntVector(v3);

	v4 = v1 - v2;
	cout << "v1 - v2 = v4 : ";
	print_MyIntVector(v4);

	v5 = v1 * v2;
	cout << "v1 * v2 = v5 : ";
	print_MyIntVector(v5);

	

}

void check_assignment_operator()
{
	MyIntVector v1;
	MyIntVector v2;
	for (int i = 0; i < 10; i++)
		v1.push_back(3 * i);
	cout << "v1: ";
	print_MyIntVector(v1);
	cout << "v2: ";
	print_MyIntVector(v2);
	v2 = v1;
	cout << "After v2 = v1 ->v2: ";
	print_MyIntVector(v2);

}

void check_unary_operator()
{
	MyIntVector v1;
	MyIntVector v2;
	for (int i = 1; i < 10; i++)
		v1.push_back(2 * i);
	cout << "v1: ";
	print_MyIntVector(v1);
	v2 = -v1;
	cout << "v2=-v1 so v2: ";
	print_MyIntVector(v2);

	cout << "check == operator" << endl;
	v2(3);
	cout << "v2(3) so v2: ";
	print_MyIntVector(v2);

}

void isSameVector(MyIntVector& v1,MyIntVector& v2)
{
	cout << "v1: ";
	print_MyIntVector(v1);

	cout << "v2: ";
	print_MyIntVector(v2);

	if ((v1 == v2) == true)
		cout << "v1 == v2" << endl;
	else
		cout << "v1 != v2" << endl;


}
void check_sameoperator()
{
	MyIntVector v1;
	MyIntVector v2;
	isSameVector(v1, v2);
	for (int i = 0; i < 5; i++)
		v1.push_back(i + 3);
	isSameVector(v1, v2);
	v2 = v1;
	isSameVector(v1, v2);

	v2.clear();
	isSameVector(v1, v2);
	
}


void check_my_assignment() 
{
	check_basement();
	check_push_popback();
	check_isEmpty_clear();
	check_basic_operator();
	check_assignment_operator();
	check_binary_operator();
	check_unary_operator();
	check_sameoperator();
}

int main(void) 
{

	check_my_assignment();

	return 0;
}
