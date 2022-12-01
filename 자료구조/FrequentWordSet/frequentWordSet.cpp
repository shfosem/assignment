#include<iostream>
#include<functional>
#include<string>
#include<cstring>
#include<unordered_map>
#include<iostream>
#include<fstream>
#include<time.h>
using namespace std;

int main(void) {
	clock_t begin_input, end_input, begin_save, end_save, begin_find, end_find;

	begin_input = clock();
	unordered_map<string, int> m;
	string strsize;
	size_t size;
	string input;
	int value = 1;
	
	cin >> size;
	cin >> input;
	
	/*ifstream myfile;
	myfile.open("testcase9.txt");*/
	//test 9 = 20번 test 6  = 5번 test 0 = 6
	//test 2 = 6    test3 = vrzl 9번  test4 == homework 9번
	//test1 qkz = 90번  // test 7 20번
	/*if (myfile.is_open()) {
		
		getline(myfile, strsize);
		getline(myfile, input);
	}
	else cout << "Unable to open file";
	*/
	end_input = clock();
	cout << "입력시간: " << ((end_input - begin_input)/CLOCKS_PER_SEC) << endl;
	//size = stoi(strsize);
	
	
	begin_save = clock();
	for (size_t i = 0; i < input.size()-size+1; i++) {
		unordered_map<string, int>::iterator got = m.find(input.substr(i, size));
		if (got == m.end())
			m.insert(make_pair(input.substr(i, size), value));
			
		else
			got->second++;

	}


	end_save = clock();
	cout << "저장시간: " << ((end_save - begin_save) / CLOCKS_PER_SEC) << endl;

	begin_find = clock();
	string result;
	int max = 0;
	for (auto itr = m.begin(); itr != m.end(); itr++) {
		
		if (itr->second >= max) {
			max = itr->second;
			result = itr->first;
		}
		
	}
	
	end_find = clock();
	cout << "찾는시간: " << ((end_find - begin_find) / CLOCKS_PER_SEC) << endl;
	cout << result << "\n" <<  "how many times: " << max << endl;

	

	return 0;
}