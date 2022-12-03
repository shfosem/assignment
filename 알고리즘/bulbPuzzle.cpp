//2017203038 
#include <iostream>

using namespace std;

bool bulb[20][20];

bool final_state[20][20];

/*
    최종 상태가 모두 꺼져있는지 확인
    param
        n : 퍼즐의 크기
*/
bool check_finalstate_off(int n)
{
    for(int i =0 ;i< n; i++)
        for(int j=0; j<n; j++)
            if(final_state[i][j]==true)
                return false;    
    return true;
}


/*
    최종 정답 출력
    param
        n: 퍼즐의 크기
*/
void print_bulbSwitch(int n)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            bulb[i][j]? cout<< "O " : cout << "# ";
        }
        cout << '\n';
    }
}

/*
    전구 스위치 켜기
    param:
        x : x좌표
        y : y좌표
        n : 퍼즐의 크기
*/
void click_button(int x, int y, int n)
{
    // 누른 스위치 나타내기
    bulb[x][y]=!bulb[x][y];
    final_state[x][y]=!final_state[x][y];
    if(x>0)
        final_state[x-1][y]=!final_state[x-1][y];
    if(y>0)
        final_state[x][y-1]=!final_state[x][y-1];
    if(x<n-1)
        final_state[x+1][y]=!final_state[x+1][y];
     if(y<n-1)
        final_state[x][y+1]=!final_state[x][y+1];
}

/*
    경우의 수 탐색
    non-promising 하다고 판단되는 경우 false를 반환하고,
    promising 한 경우 다음 스위치를 키며 진행
    param:
        x : x좌표
        y : y좌표
        n : 퍼즐의 크기
*/
bool dfs(int x, int y, int n)
{   
    if(check_finalstate_off(n))
        return true;
    else if(x>=n || y>= n)
        return false;
    
    if(x>0 && y>0 && final_state[x-1][y-1]) // non-promising
    {
        return false;
    }
   
   else if(y==0 && x>1 && final_state[x-2][n-1])   // non-promising
        return false;
    
    //make next index;
    int tempx=x;
    int tempy=y+1;
    if(tempy>=n)
    {
        tempx+=1;
        tempy=0;
    }

    click_button(x,y,n);
    if(dfs(tempx,tempy,n)) return true; 
    click_button(x,y,n);
    return dfs(tempx,tempy,n);
}

/*
    문제를 해결할 수 있는지 판별
    0,0부터 최초의 dfs 실행
    param
        n: 퍼즐의 크기
*/
bool solve(int n)
{   
    
    return dfs(0,0,n);

}

int main(void)
{   
    int n;
    cin >> n;
    //cin.ignore();
    // INPUT
    char bulb_state;
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
        {
         cin >> bulb_state;
         if(bulb_state=='#')
            final_state[i][j]=false;
        else if( bulb_state == 'O')
            final_state[i][j]=true;
        }
        cout << '\n';
   
    if(solve(n))
        print_bulbSwitch(n);
    else
        cout << "no solution.\n";

    return 0;
}



/*
* void click_check(int x, int y, int n)
* {
*     check[x][y]=!check[x][y];
*     if(x>0)
*         check[x-1][y]=!check[x-1][y];
*     if(y>0)
*         check[x][y-1]=!check[x][y-1];
*     if(x<n-1)
*         check[x+1][y]=!check[x+1][y];
*      if(y<n-1)
*        check[x][y+1]=!check[x][y+1];
* }
* void checkplus(int n)
* {
*      for(int i=0; i<n; i++)
*         for(int j=0; j<n; j++)
*         {
*          if(bulb[i][j]==true)
*             click_check(i,j,n);
*         }
* }
* void print_check(int n)
* {
*     for(int i=0; i<n; i++)
*     {
*         for(int j=0; j<n; j++)
*         {
*             check[i][j]? cout<< "O " : cout << "# ";
*         }
*         cout << '\n';
*     }
* }
*/