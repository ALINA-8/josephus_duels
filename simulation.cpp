#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

// Йосип Флавій для довільного порядку обходу: рахуємо "раз, два",
// другого викреслюємо, рахунок іде далі від наступного
int josephus(vector<int> a)
{
int i = 0;
while(a.size()>1)
{
i=(i+1)%a.size();
a.erase(a.begin()+i);
if(i==a.size()) i=0;
}
return a[0];
}

vector<int> range(int n)
{
vector<int> a;
for (int i=1; i<=n; i++) a.push_back(i);
return a;
}

// Гравець 1: класичний Йосип, обхід 1, 2, ..., n
int classic(int n)
{
return josephus(range(n));
}

// A: щоразу викреслюємо всі непарні позиції
int A(int n)
{
vector<int> a = range(n);
while (a.size()>1)
{
vector<int> b;
for (int i=1; i<a.size(); i+=2) b.push_back(a[i]);
a=b;
}
return a[0];
}

// B: "Число, що залишилось": то непарні позиції, то парні
int B(int n)
{
vector<int> a=range(n);
int start=1; // 1 - лишаємо парні позиції, 0 - непарні
while (a.size()>1)
{
vector<int> b;
for (int i=start; i<a.size(); i+=2) b.push_back(a[i]);
a=b;
start=1-start;
}
return a[0];
}

// C: Йосип, але першим викреслюють 1-й елемент (обхід n, 1, 2, ..., n-1)
int C(int n)
{
vector<int> a={n};
for (int i=1; i<n; i++) a.push_back(i);
return josephus(a);
}

// D: непарні позиції, але напрямок щоразу змінюється
int D(int n)
{
vector<int> a=range(n);
while (a.size()>1)
{
vector<int> b;
for (int i=1; i<a.size(); i+=2) b.push_back(a[i]);
reverse(b.begin(), b.end());
a=b;
}
return a[0];
}

// E: Йосип проти годинникової стрілки (обхід 1, n, n-1, ..., 2)
int E(int n)
{
vector<int> a={1};
for (int i=n; i>=2; i--) a.push_back(i);
return josephus(a);
}

// довжини блоків перемог одного гравця поспіль, нічия серію обриває
void printBlocks(string name, function<int(int)> rule, int N)
{
cout<<name<<": ";

int last=2;
int count=0;

for (int n=2; n<=N; n++)
{
int x=classic(n);
int y=rule(n);

int current=0;
if (x>y)
{
current=1;
}
if (x<y)
{
current=-1;
}
if (current==last)
{
count++;
}
else
{
if (last==1 || last==-1)
{
cout<<count<<" ";
}
last=current;
count=1;
}
}
cout<<"\n";
}

int main() {
int N = 2500;
printBlocks("A", A, N);
printBlocks("B", B, N);
printBlocks("C", C, N);
printBlocks("D", D, N);
printBlocks("E", E, N);
}
