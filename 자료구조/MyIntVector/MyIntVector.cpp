// 2017203038 ������
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
	//precondition ���װ� ������ ���Ͱ� �̸� ����Ǿ� �־�� �Ѵ�.
	//postcondition ������ ���͸� ���׿� ����, ������ ��ȯ.
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
	//preconditon���Ͱ� �����ؾ��Ѵ�
	//postcondition �����Ͱ� �����ȴ�
	delete[] data;
}


void MyIntVector::reserve(size_t new_capacity)
{
	//preconditon	���Ͱ� �̸� ���� �Ǿ�� �Ѵ�. used�� space�� ���ƾ� �Ѵ�. ���ο� ������ �� Ŀ���Ѵ�.
	//postcondition	new_capacity�� ũ��� space�� Ȯ��ȴ�.
	int * larger_array;
	if (new_capacity == space) // check space
		return;
	if (new_capacity < used)
	{
		new_capacity = used;
		return;
	}
	larger_array = new int[new_capacity]; //new_capacity ���� �Ҵ�
	copy(data, data + used, larger_array);//�̻�
	delete[] data; //���� ������ ����
	data = larger_array;
	space = new_capacity;

}

void MyIntVector::push_back(int entry)
{
	//preconditon	���Ͱ� �̸� ���� �Ǿ�� �Ѵ�.
	//postcondition	entry�� vector[used]�� ���� �ȴ�.
	if (capacity() == 0)
		reserve(used+1); // 0�� ��� *������ ���Ƿ� +1
	else if (capacity() == size())//������ �����Ұ�� *2
		reserve(2 * used);

	data[used] = entry;
	++used;
}

void MyIntVector::pop_back()  
{
	//preconditon	������ ������ ��Ұ� �־�� �Ѵ�
	//postcondition	������ ��Ҹ� �����Ѵ�
	data[used] = NULL;//�� �������� ����
	--used;				//used �� ����
}


bool MyIntVector::isEmpty() const
{
	//preconditon	���Ͱ� �̸� ���� �Ǿ�� �Ѵ�
	//postcondition	���Ͱ� ��������� �� , ������� ������ ������ ��ȯ�Ѵ�.
	if (size() == 0)
		return true;
	return false;
}

void MyIntVector::clear()
{
	//preconditon	���Ͱ� �̸� ���� �Ǿ�� �Ѵ�, ��Ұ� �����ؾ��Ѵ�.
	//postcondition	���Ͱ� �ȴ�
	while (size() != 0)
		pop_back();
}



void MyIntVector:: operator +=(const MyIntVector& addend)
{
	//preconditon	�� ���Ͱ� ��� ��Ұ� �����ϵ��� �̸� �־�� �Ѵ�.
	//postcondition	�� ���Ͱ� �ϳ��� ����ȴ�(������ ���Ͱ� ���� �ڷ� ����)
	if (used + addend.used > space)
		reserve(used + addend.used);
	copy(addend.data, addend.data + addend.used, data + used);//���� ���� �ڷ� �����ϸ� ����
	used += addend.used;
}


MyIntVector& MyIntVector::operator+(const MyIntVector& v) //��ȯ���� reference�� �ƴϸ� �ٷ� ������Ƿ� reference
{
	//preconditon	���� �ΰ��� ��� ����Ǿ�� �ϰ� used�� ���ƾ� �Ѵ�
	//postcondition	���ʺ����� ��ҵ鿡 ������ ���Ϳ�ҵ��� �������� ���ʺ��͸� ��ȯ�Ѵ�
	if(size() != v.size())
		assert(size() != v.size());			//����� ���� ������ ����
	
	MyIntVector* answer = new MyIntVector(); 
		
	for (size_t i = 0; i < v.size(); i++)
		answer->push_back(data[i] + v.data[i]);
	
	return *answer;
}

MyIntVector& MyIntVector::operator-(const MyIntVector& v) //+�����ڿ� ����
{
	//preconditon	���� �ΰ��� ��� ����Ǿ�� �ϰ� used�� ���ƾ� �Ѵ�
	//postcondition	���ʺ����� ��ҵ鿡 ������ ���Ϳ�ҵ��� ���� ���ʺ��͸� ��ȯ�Ѵ�
	if (this->size() != v.size())
		assert(this->size() != v.size());
	MyIntVector* answer = new MyIntVector;

	for (size_t i = 0; i < v.size(); i++)
		answer->push_back(this->data[i] - v.data[i]);
	return *answer;

}


MyIntVector& MyIntVector::operator*(const MyIntVector& v) //+�����ڿ� ����
{
	//preconditon	���� �ΰ��� ��� ����Ǿ�� �ϰ� used�� ���ƾ� �Ѵ�
	//postcondition	���ʺ����� ��ҵ鿡 ������ ���Ϳ�ҵ��� �������� ���ʺ��͸� ��ȯ�Ѵ�
	if (this->size() != v.size())
		assert(this->size() != v.size());
	MyIntVector* answer = new MyIntVector();
	
	for (size_t i = 0; i < v.size(); i++)
		answer->push_back(this->data[i] * v.data[i]);
	return *answer;

}




MyIntVector& MyIntVector::operator-()
{
	// precondition ���Ͱ� �ְ� ��ҵ��� �����ؾ��Ѵ�
	// postcondition ��ҵ��� ������ ���, ����� ������ �ȴ�.
	for (size_t i = 0; i < size(); i++)
		data[i] *= -1;
	return *this;
}


MyIntVector& MyIntVector::operator()(int n)
{
//preconditon	���Ͱ� ����Ǿ�� �ϰ� ��Ұ� �����ؾ� �Ѵ�/. int���� �޴´�
//postcondition	���� ��Ʈ���� ��� ��ҵ鿡 ���Եǰ� ���͸� ��ȯ�Ѵ�.
	for (size_t i = 0; i < size(); i++)
		data[i] = n;
	return *this;
}



bool MyIntVector::operator==(const MyIntVector& v)
{
	//preconditon	���� �ΰ��� ��� ����Ǿ�� �Ѵ�
	//postcondition	used���� ������ ��ҵ��� Ȯ���ѵ� ������ �� , ���� ������ ������ ��ȯ�Ѵ�.
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




