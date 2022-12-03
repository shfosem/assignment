// 2017203038
#include "MyIntVector.h"

MyIntVector::MyIntVector()  //default constructor
{
	data = nullptr;
	used = 0;
	reserve(0);
}
MyIntVector::MyIntVector(size_t n) 
{
	data = nullptr;
	used = 0;
	reserve(n);
}
MyIntVector::MyIntVector(const MyIntVector& v)  //copy constructor
{
	data = new int(v.space);
	space = v.space;
	used = v.used;
	copy(v.data, v.data + used, data);
}

int& MyIntVector::operator[](size_t where) // [] operator overloading
{
	return data[where];
}


MyIntVector& MyIntVector::operator=(const MyIntVector& source) // = operator overloading
{
	//precondition 우항과 좌항의 벡터가 미리 선언되어 있어야 한다.
	//postcondition 우항의 벡터를 좌항에 대입, 좌항을 반환.
	int* new_data;
	if (this == &source)  //check self assignment 
		return *this;
	if (space != source.space)
	{
		new_data = new int[source.space];
		delete[] data;
		data = new_data;
		space = source.space;
	}
	used = source.used;
	copy(source.data, source.data + used, data);
	return *this;

}




MyIntVector::~MyIntVector() //destructor
{
	//preconditon벡터가 존재해야한다
	//postcondition 데이터가 삭제된다
	delete[] data;
}


void MyIntVector::reserve(size_t new_capacity)
{
	//preconditon	벡터가 미리 선언 되어야 한다. used가 space와 같아야 한다. 새로운 공간이 더 커야한다.
	//postcondition	new_capacity의 크기로 space가 확장된다.
	int * larger_array;
	if (new_capacity == space) // check space
		return;
	if (new_capacity < used)
	{
		new_capacity = used;
		return;
	}
	larger_array = new int[new_capacity]; //new_capacity 공간 할당
	copy(data, data + used, larger_array);//이사
	delete[] data; //기존 데이터 삭제
	data = larger_array;
	space = new_capacity;

}

void MyIntVector::push_back(int entry)
{
	//preconditon	벡터가 미리 선언 되어야 한다.
	//postcondition	entry가 vector[used]에 저장 된다.
	if (capacity() == 0)
		reserve(used+1); // 0일 경우 *오류가 나므로 +1
	else if (capacity() == size())//공간이 부족할경우 *2
		reserve(2 * used);

	data[used] = entry;
	++used;
}

void MyIntVector::pop_back()  
{
	//preconditon	삭제할 벡터의 요소가 있어야 한다
	//postcondition	마지막 요소를 삭제한다
	data[used] = NULL;//맨 마지막을 비우고
	--used;				//used 를 줄임
}


bool MyIntVector::isEmpty() const
{
	//preconditon	벡터가 미리 선언 되어야 한다
	//postcondition	벡터가 비어있으면 참 , 비어있지 않으면 거짓을 반환한다.
	if (size() == 0)
		return true;
	return false;
}

void MyIntVector::clear()
{
	//preconditon	벡터가 미리 선언 되어야 한다, 요소가 존재해야한다.
	//postcondition	빈벡터가 된다
	while (size() != 0)
		pop_back();
}



void MyIntVector:: operator +=(const MyIntVector& addend)
{
	//preconditon	두 벡터가 모두 요소가 존재하도록 미리 있어야 한다.
	//postcondition	두 벡터가 하나로 연결된다(우항의 벡터가 좌항 뒤로 붙음)
	if (used + addend.used > space)
		reserve(used + addend.used);
	copy(addend.data, addend.data + addend.used, data + used);//앞의 벡터 뒤로 복사하면 연결
	used += addend.used;
}


MyIntVector& MyIntVector::operator+(const MyIntVector& v) //반환형이 reference가 아니면 바로 사라지므로 reference
{
	//preconditon	벡터 두개가 모두 선언되어야 하고 used가 같아야 한다
	//postcondition	왼쪽벡터의 요소들에 오른쪽 벡터요소들이 더해지고 왼쪽벡터를 반환한다
	if(size() != v.size())
		assert(size() != v.size());			//사이즈가 같지 않으면 에러
	
	MyIntVector* answer = new MyIntVector(); 
		
	for (size_t i = 0; i < v.size(); i++)
		answer->push_back(data[i] + v.data[i]);
	
	return *answer;
}

MyIntVector& MyIntVector::operator-(const MyIntVector& v) //+연산자와 같음
{
	//preconditon	벡터 두개가 모두 선언되어야 하고 used가 같아야 한다
	//postcondition	왼쪽벡터의 요소들에 오른쪽 벡터요소들이 빼고 왼쪽벡터를 반환한다
	if (this->size() != v.size())
		assert(this->size() != v.size());
	MyIntVector* answer = new MyIntVector;

	for (size_t i = 0; i < v.size(); i++)
		answer->push_back(this->data[i] - v.data[i]);
	return *answer;

}


MyIntVector& MyIntVector::operator*(const MyIntVector& v) //+연산자와 같음
{
	//preconditon	벡터 두개가 모두 선언되어야 하고 used가 같아야 한다
	//postcondition	왼쪽벡터의 요소들에 오른쪽 벡터요소들이 곱해지고 왼쪽벡터를 반환한다
	if (this->size() != v.size())
		assert(this->size() != v.size());
	MyIntVector* answer = new MyIntVector();
	
	for (size_t i = 0; i < v.size(); i++)
		answer->push_back(this->data[i] * v.data[i]);
	return *answer;

}




MyIntVector& MyIntVector::operator-()
{
	// precondition 벡터가 있고 요소들이 존재해야한다
	// postcondition 요소들이 음수면 양수, 양수면 음수가 된다.
	for (size_t i = 0; i < size(); i++)
		data[i] *= -1;
	return *this;
}


MyIntVector& MyIntVector::operator()(int n)
{
//preconditon	벡터가 선언되어야 하고 요소가 존재해야 한다/. int형을 받는다
//postcondition	받은 인트형이 모든 요소들에 대입되고 벡터를 반환한다.
	for (size_t i = 0; i < size(); i++)
		data[i] = n;
	return *this;
}



bool MyIntVector::operator==(const MyIntVector& v)
{
	//preconditon	벡터 두개가 모두 선언되어야 한다
	//postcondition	used부터 각각의 요소들을 확인한뒤 같으면 참 , 같지 않으면 거짓을 반환한다.
	if (size() != v.size())
		return false;
	/*for (size_t i = 0; i < size(); ++i)
	{
		if (data[i] != v.data[i])
			return false;
	}*/
	size_t i = 0;
	while (i < size())
	{
		if (data[i] != v.data[i])
			return false;
		++i;
	}
	return true;
}




