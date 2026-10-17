#include <iostream>
#include <vector>
#include <string>
#include <functional>
using namespace std;

// найбільший степінь двійки, не більший за n
int top(int n)
{
int p=1;
while (p*2<=n) p*=2;
return p;
}

// Гравець 1: класичний Йосип, формула 2k+1
int classic(int n)
{
return 2*(n-top(n))+1;
}

// A: лишається найбільший степінь двійки
int A(int n)
{
return top(n);
}

// B: "Число, що залишилось": g - перший прохід прибирає непарні позиції, h - парні
int h(int n);
int g(int n)
{
if (n==1) return 1;
return 2*h(n/2);
}
int h(int n)
{
if (n==1) return 1;
return 2*g((n+1)/2)-1;
}
int B(int n)
{
return g(n);
}

// C: Йосип, але першим викреслюють 1-й елемент
int C(int n)
{
int k=n-top(n);
if (k==0) return n;
return 2*k;
}

// D: непарні позиції, напрямок щоразу змінюється
int D(int n)
{
if (n==1) return 1;
return 2*(n/2+1-D(n/2));
}

// E: Йосип проти годинникової стрілки
int E(int n)
{
int p=top(n);
int k=n-p;
if (k==0) return 1;
return p-k+1;
}

// довжини блоків перемог, нічия серію обриває
void printBlocks(string name, function<int(int)> rule, int N)
{
vector<int> lens;
int last=2;
int count=0;

for (int n=2; n<=N; n++)
{
int x=classic(n);
int y=rule(n);

int current=0;
if (x>y) current=1;
if (x<y) current=-1;

if (current==last)
{
count++;
}
else
{
if (last==1 || last==-1) lens.push_back(count);
last=current;
count=1;
}
}

cout<<name<<": ";
for (int i=0; i<20 && i<lens.size(); i++) cout<<lens[i]<<" ";
cout<<"... (усього блоків: "<<lens.size()<<")\n";
}

int main() {
int N=1<<26;
printBlocks("A", A, N);
printBlocks("B", B, N);
printBlocks("C", C, N);
printBlocks("D", D, N);
printBlocks("E", E, N);
}
