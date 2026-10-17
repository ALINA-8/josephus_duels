#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Звірка швидких формул із прямою симуляцією для n = 1..3000
// та перевірка чисел Якобсталя і рахунку гри для правила B.
// Якщо все збігається, програма виводить OK.

// ---------- пряма симуляція (еталон) ----------
int josephus(vector<int> a)
{
int i=0;
while (a.size()>1)
{
i=(i+1)%a.size();
a.erase(a.begin()+i);
if (i==a.size()) i=0;
}
return a[0];
}

vector<int> range(int n)
{
vector<int> a;
for (int i=1; i<=n; i++) a.push_back(i);
return a;
}

int simClassic(int n)
{
return josephus(range(n));
}

int simA(int n)
{
vector<int> a=range(n);
while (a.size()>1)
{
vector<int> b;
for (int i=1; i<a.size(); i+=2) b.push_back(a[i]);
a=b;
}
return a[0];
}

int simB(int n)
{
vector<int> a=range(n);
int start=1;
while (a.size()>1)
{
vector<int> b;
for (int i=start; i<a.size(); i+=2) b.push_back(a[i]);
a=b;
start=1-start;
}
return a[0];
}

int simC(int n)
{
vector<int> a={n};
for (int i=1; i<n; i++) a.push_back(i);
return josephus(a);
}

int simD(int n)
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

int simE(int n)
{
vector<int> a={1};
for (int i=n; i>=2; i--) a.push_back(i);
return josephus(a);
}

// ---------- швидкі формули ----------
int top(int n)
{
int p=1;
while (p*2<=n) p*=2;
return p;
}

int fastClassic(int n)
{
return 2*(n-top(n))+1;
}

int fastA(int n)
{
return top(n);
}

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
int fastB(int n)
{
return g(n);
}

int fastC(int n)
{
int k=n-top(n);
if (k==0) return n;
return 2*k;
}

int fastD(int n)
{
if (n==1) return 1;
return 2*(n/2+1-fastD(n/2));
}

int fastE(int n)
{
int p=top(n);
int k=n-p;
if (k==0) return 1;
return p-k+1;
}

// число Якобсталя U(i) = (2^i - (-1)^i)/3: 1, 1, 3, 5, 11, 21, 43, ...
long long U(int i)
{
long long znak=-1;
if (i%2==0) znak=1;
return ((1LL<<i)-znak)/3;
}

// Блоки правила B мають йти групами (x, y, x, x, y, y), x=U(2k-1), y=U(2k),
// а рахунок гри при n=2*4^k-1 має дорівнювати
// (10(4^k-1)-3k)/9 : (8(4^k-1)+3k)/9 (гравець 1 : гравець 2).
// Перевірка йде до n=2^26, тому триває кілька секунд.
bool checkJacobsthal()
{
int N=1<<26;
vector<long long> lens;
long long wins1=0;
long long wins2=0;
long long power4=4;
int k=1;
int last=2;
int count=0;
bool ok=true;

for (int n=2; n<=N; n++)
{
int x=fastClassic(n);
int y=fastB(n);
int current=0;
if (x>y) current=1;
if (x<y) current=-1;

if (current==1) wins1++;
if (current==-1) wins2++;

if (n==2*power4-1) // кінець k-ї групи
{
long long p1=(10*(power4-1)-3*k)/9;
long long p2=(8*(power4-1)+3*k)/9;
if (wins1!=p1 || wins2!=p2) ok=false;
power4*=4;
k++;
}

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

for (int grupa=0; (grupa+1)*6<=lens.size(); grupa++)
{
long long x=U(2*grupa+1);
long long y=U(2*grupa+2);
long long ochikuvano[6]={x, y, x, x, y, y};
for (int t=0; t<6; t++)
{
if (lens[grupa*6+t]!=ochikuvano[t]) ok=false;
}
}
cout<<"груп перевірено: "<<lens.size()/6<<"\n";
return ok;
}

int main() {
int bad=0;

for (int n=1; n<=3000; n++)
{
if (simClassic(n)!=fastClassic(n)) bad++;
if (simA(n)!=fastA(n)) bad++;
if (simB(n)!=fastB(n)) bad++;
if (simC(n)!=fastC(n)) bad++;
if (simD(n)!=fastD(n)) bad++;
if (simE(n)!=fastE(n)) bad++;
}

// сходинки правила B: a_1=2, a_(j+1)=4a_j-2 має дорівнювати (4^j+2)/3
long long a=2;
long long pow4=4;
for (int j=1; j<=25; j++)
{
if (a!=(pow4+2)/3) bad++;
a=4*a-2;
pow4*=4;
}

// групи блоків правила B: x_1=3, x_(i+1)=4x_i-1 має дорівнювати (2^(2i+1)+1)/3
long long x=3;
long long pow2=8;
for (int i=1; i<=25; i++)
{
if (x!=(pow2+1)/3) bad++;
x=4*x-1;
pow2*=4;
}

if (!checkJacobsthal()) bad++;

if (bad==0) cout<<"OK\n";
else cout<<"є розбіжності: "<<bad<<"\n";
}
