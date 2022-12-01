# assignment
수업을 들으면서 수행했던 과제 코드 정리
====================================
+ 고급프로그래밍 :
    - 수강 시기 : 2학년 1학기(2018년 상반기)
    - 사용 언어 : C++
    - 과제 설명 :
        * 과제 1 - calculator.cpp: 엉뚱하게 돌아가는 계산기 샘플 코드가 주어졌고 문제의 조건에 따라 수정하는 과제. 다양한 연산들이 가능한 계산기였기 때문에 적절한 우선 순위대로 연산을 진행하도록 코드를 수정하기 위해 많은 고민과 다양한 시도를 하였음.
        * 과제 2 - variousShape.cpp: 출제의도가 객체지향의 상속, 다형성, 캡슐화 개념을 활용해서 소스코드를 작성해보라는 것이 명확했던 과제. 기본 과제로는 기존의 Shape을 상속하여 여러 응용 도형들을 그려보는 과제.

+ 자료구조 :
    - 수강 시기 : 2학년 2학기(2018년 하반기)
    - 사용 언어 : C++
    - 과제 설명 : 
        * 과제 1 - MyIntVector : STL 의 vector<int>와 유사하게 동작하는 MyIntVector 클래스를 구현하는 과제. 제약조건으로는 정적 배열을 사용할 수 없었으며, int형 포인터로 데이터를 담아서 저장해야 했음.
        구현한 멤버로는 Default constructor, Copy Constructor(Deep Copy), Destuctor, Assignment operator(Deep copy), Operator +=, Operator [], Binary Operator +, -, *, == , Unary Operator - , () , pop_back(), push_back(int x), capacity(), size(), reserve()등을  구현하였고 각 멤버들에 대해 테스트를 해보는 프로그램을 main.cpp에 작성하였음.

        * 과제 2 - specialCalculator : stack을 활용하여 연산을 정해진 순서대로 진행하는 계산기를 만드는 과제. 과제에서의 연산자는 @, #, & 가 있고(우선순위는 & > @==# ) 피연산자는 a, b, c 이며 괄호의 종류는 (), {}, [] 가 있다. 
        연산의 내용 또한 input 파일로 함께 들어와서 (3x3 배열의 형태로 들어오며 연산의 결과는 항상 a, b, c 중 하나), 처음에 연산을 정의하고 한줄씩 식을 입력받으며 EOI가 입력되면 결과를 한 줄씩 출력하는 계산기.
