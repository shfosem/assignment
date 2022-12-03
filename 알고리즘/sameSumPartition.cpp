#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>

#define ULONG_MAX 4294967295

using namespace std;

unsigned int dp[500251][1001]; 
stack<int> s;
queue<int> q;
bool overflow = false;

/*
    주어진 숫자만큼 정수를 입력받아 vector에 저장 후 정렬하여 반환
    @param 
        n: 정수의 개수
        v: 정수를 담을 vector
*/
void make_sorted_input(int n, vector<int>& v)
{
    int temp;
    for(int i=0; i<n;i++)
    {    
        cin >> temp;
        v.push_back(temp);
    }
    sort(v.begin(),v.end());

    return;
}

/*
    주어진 벡터에서 0~index까지 저장된 값들을 조합하여 목표 값을 만들 수 있는 조합의 개수를 반환하는 함수
    @param
        v: int를 담은 벡터
        target : 목표 값
        index : 사용할 vector의 최대 index
    @return 
        값이 2^32-1(ULONG_MAX)보다 클 경우 -1 반환
*/
long long combination(vector<int>& v, int target, int index)
{ 
    if(overflow)
        return -1;
     if(index<0)
        return 0;
    if(target==0)
        return 0;
    if(v[index]>target){
        return dp[target][index]= combination(v,target,index-1);
    }
    else if(v[index]== target){
        if(dp[target][index]==ULONG_MAX)
            return dp[target][index]=combination(v,target,index-1)+1;
        return dp[target][index];
    }
    else{

        if(dp[target][index-1]==ULONG_MAX)
            dp[target][index-1]=combination(v, target,index-1);
         
         if(dp[target-v[index]][index-1]==ULONG_MAX)
            dp[target-v[index]][index-1]=combination(v, target-v[index], index-1);
         
         if(dp[target-v[index]][index-1]+  dp[target][index-1]< dp[target][index-1]) //detect overflow
        {
              overflow= true;
               return -1;
        }
        else
            return dp[target][index] = dp[target-v[index]][index-1]+ dp[target][index-1];
        
    }
}

/*
    각 테스트케이스 별로 문제를 해결을 위해 호출하는 함수
    overflow를 확인하기 위한 전역변수와 memoization에 사용된 dp 배열을 초기화하고
    target 값을 설정하여 combination 함수를 최초로 호출
    @param
        v : 서로 다른 자연수를 담고 있는 벡터 v 
        sum : 주어진 벡터 v의 원소 값들의 합
    @return
        각 테스트케이스별 결과 값
*/
long long solve(vector<int>& v, int sum)
{
    if(v.size()==2)
        return 0;
    if(sum%2!=0)  // if sum is odd, can't make partition
        return 0;
    
    int target = sum/2;
    //initilaize
    overflow = false;
    fill(&dp[0][0],&dp[500250][1001], ULONG_MAX);

    return combination(v, target, v.size()-2);
}

/*
    벡터 v에서 0~index까지의 값들을 조합해서 target 값을 만들 수 있는 원소들을 stack에 push하는 함수
    @param 
        v : 서로 다른 자연수를 담고 있는 벡터 v 
        target : 목표 값
        index : 사용할 vector의 최대 index
    @return
        partition을 만들 수 있는지 참/거짓
*/
bool make_partition(vector<int>& v, int target, int index)  // first target == sum/2, index = v.size() -1
{   
    if(index<0)
        return false;
    if(target==0)
        return true;
    
    if(target == v[index])
    {
        s.push(v[index]);
        return true;
    }
    else if(target>v[index])
    {   
        s.push(v[index]);
       if(make_partition(v, target-v[index], index-1))
        return true;
        else{
            s.pop();
            if(make_partition(v, target,index-1))
                return true;
        }
       
    }
    else if(target<v[index])
    {
          if(make_partition(v,target,index-1))
            return true;
    }
    return false;

}
/*
    make_partition에서 push한 스택 값과 벡터 값을 비교하면서
    stack에 없는 값은 queue에 push 하고,
    stack에 존재하는 값으로 하나의 partition을 출력한 후,
    queue에 담은 값들로 나머지 partition을 출력하는 함수
    @param 
        v : 서로 다른 자연수를 담고 있는 벡터 v 
*/

void print_partition(vector<int>& v)
{
    int index=0;
    while(v[index]<s.top()){
        if(index==v.size()-2)
            break;
        q.push(v[index++]);
    }
    cout << "{" << s.top();
    s.pop();
    index++;
   
    while(!s.empty())
    {
        if(v[index]==s.top())
        {
            cout << "," << s.top(); 
            s.pop();
            index++;
            continue;
        }
        else if(v[index]<s.top())
        {
            q.push(v[index]);
            index++;
        }
    }
    cout << "},";

    while(index<=v.size()-1)
    {
        q.push(v[index]);
        index++;
    }
    cout << "{"<< q.front();
    q.pop();
    while(!q.empty())
    {
        cout << "," << q.front();
        q.pop();
    }
    cout << "}\n" ; 
}

int main(void)
{

    string str;
    int n;
    int sum;
    //

    while(true){
    vector<int> v;
    getline(cin, str);
    if(str=="EOI")
        break;
     n = atoi(str.c_str());
     make_sorted_input(n, v);
     cin.ignore();
     sum = 0;
     for(auto& i : v)
         sum+=i;

     long long result = solve(v,sum);
    
    if(result<0)
         cout << "NUMEROUS\n"; 
    else
         cout << result << "\n";
    
    if(result!=0){
        if(make_partition(v,sum/2,v.size()-1))   
            print_partition(v); 
        }
    }

    return 0;
}