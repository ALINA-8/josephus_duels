#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Малює обидва рисунки роботи у форматі SVG (без жодних бібліотек).
// Результат: strips.svg (смужки по інтервалах) і blocks.svg (довжини блоків).

// ---------- гравець 1 і правила гравця 2 (швидкі формули) ----------
int top(int n)
{
int p=1;
while (p*2<=n) p*=2;
return p;
}

int classic(int n)
{
return 2*(n-top(n))+1;
}

int A(int n)
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
int B(int n)
{
return g(n);
}

int C(int n)
{
int k=n-top(n);
if (k==0) return n;
return 2*k;
}

int D(int n)
{
if (n==1) return 1;
return 2*(n/2+1-D(n/2));
}

int E(int n)
{
int p=top(n);
int k=n-p;
if (k==0) return 1;
return p-k+1;
}

vector<function<int(int)>> rules={A, B, C, D, E};
string names[]={"A", "B", "C", "D", "E"};

// ---------- допоміжне для SVG ----------
string text(double x, double y, string s, string anchor="middle")
{
return "<text x='"+to_string(x)+"' y='"+to_string(y)+"' text-anchor='"+anchor+"'>"+s+"</text>\n";
}

string line(double x1, double y1, double x2, double y2)
{
return "<line x1='"+to_string(x1)+"' y1='"+to_string(y1)+"' x2='"+to_string(x2)+"' y2='"+to_string(y2)+"' stroke='black'/>\n";
}

string frame(double x, double y, double w, double h)
{
return "<rect x='"+to_string(x)+"' y='"+to_string(y)+"' width='"+to_string(w)+"' height='"+to_string(h)+"' fill='none' stroke='black'/>\n";
}

string header(double w, double h)
{
return "<svg xmlns='http://www.w3.org/2000/svg' width='"+to_string(w)+"' height='"+to_string(h)+"' viewBox='0 0 "+to_string(w)+" "+to_string(h)+"'>\n<rect width='100%' height='100%' fill='white'/>\n<g font-family='Times New Roman, serif' font-size='12'>\n";
}

// ---------- рисунок 1: смужки по інтервалах ----------
// рядок m - інтервал 2^m <= n < 2^(m+1), по горизонталі t = k / 2^m
void drawStrips(string file)
{
int M=11; // скільки рядків
int W=1024; // скільки точок по горизонталі
double pw=150, ph=187, gap=38, left=50, up=32;
string c1="#1f3b57"; // гравець 1
string c2="#f2a65a"; // гравець 2
string tie="#d9d9d9"; // нічия
double rowH=ph/M;

ofstream out(file);
out<<header(left+5*(pw+gap), up+ph+50);

for (int p=0; p<5; p++)
{
double x0=left+p*(pw+gap);

for (int m=1; m<=M; m++)
{
auto colorAt=[&](int x)
{
int n=(1<<m)+(int)((long long)x*(1<<m)/W);
int a=classic(n);
int b=rules[p](n);
if (a>b) return c1;
if (a<b) return c2;
return tie;
};

int start=0;
string cur=colorAt(0);
for (int x=1; x<=W; x++)
{
string next="";
if (x<W) next=colorAt(x);
if (next==cur) continue;
out<<"<rect x='"<<x0+start*pw/W<<"' y='"<<up+(m-1)*rowH<<"' width='"<<(x-start)*pw/W<<"' height='"<<rowH+0.3<<"' fill='"<<cur<<"' shape-rendering='crispEdges'/>\n";
start=x;
cur=next;
}
}

out<<frame(x0, up, pw, ph);
out<<text(x0+pw/2, up-10, names[p]);

for (double t : {0.0, 0.5, 1.0})
{
string label="0.5";
if (t==0) label="0";
if (t==1) label="1";
out<<line(x0+t*pw, up+ph, x0+t*pw, up+ph+4);
out<<text(x0+t*pw, up+ph+17, label);
}
out<<text(x0+pw/2, up+ph+38, "t");

if (p==0)
{
for (int m=2; m<=M; m+=2)
{
double y=up+(m-0.5)*rowH;
out<<line(x0-4, y, x0, y)<<text(x0-8, y+4, to_string(m), "end");
}
out<<text(x0-30, up+ph/2, "m", "end");
}
}
out<<"</g>\n</svg>\n";
}

// ---------- рисунок 2: довжини блоків перемог ----------
vector<int> blockLengths(function<int(int)> rule, int N)
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
return lens;
}

void drawBlocks(string file)
{
int COUNT=40; // скільки перших блоків показуємо
double ymax=18; // log2 довжини (до 2^18)
double pw=150, ph=140, gap=38, left=50, up=32;

ofstream out(file);
out<<header(left+5*(pw+gap), up+ph+50);

for (int p=0; p<5; p++)
{
double x0=left+p*(pw+gap);
vector<int> lens=blockLengths(rules[p], 1<<18);
if (lens.size()>COUNT) lens.resize(COUNT);

auto X=[&](double v) { return x0+(v-0.5)/COUNT*pw; };
auto Y=[&](double len) { return up+ph-log2(len)/ymax*ph; };

out<<frame(x0, up, pw, ph);
out<<text(x0+pw/2, up-10, names[p]);

out<<"<path fill='none' stroke='#1f3b57' stroke-width='1.5' d='M "<<X(0.5)<<" "<<Y(lens[0]);
for (int i=0; i<lens.size(); i++)
{
out<<" H "<<X(i+1.5);
if (i+1<lens.size()) out<<" V "<<Y(lens[i+1]);
}
out<<"'/>\n";

for (int v : {1, 20, 40})
{
out<<line(X(v), up+ph, X(v), up+ph+4)<<text(X(v), up+ph+17, to_string(v));
}
out<<text(x0+pw/2, up+ph+38, "номер блоку");

if (p==0)
{
for (int e : {0, 5, 10, 15})
{
double y=up+ph-e/ymax*ph;
out<<line(x0-4, y, x0, y)<<text(x0-8, y+4, to_string(e), "end");
}
out<<"<text x='"<<x0-34<<"' y='"<<up+ph/2<<"' text-anchor='middle' transform='rotate(-90 "<<x0-34<<" "<<up+ph/2<<")'>log₂(довжина блоку)</text>\n";
}
}
out<<"</g>\n</svg>\n";
}

int main() {
drawStrips("strips.svg");
drawBlocks("blocks.svg");
cout<<"Готово: strips.svg і blocks.svg\n";
}
