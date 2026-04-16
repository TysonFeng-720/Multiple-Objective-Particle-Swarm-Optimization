#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <direct.h>
#include <algorithm>
#include <map>
#include <numeric> 
#include <cmath>
#include <set>
//#include <gmpxx.h>

#define stocknum 60//多少檔
#define day 280//天
#define exptime 1//實驗次數//
#define	startfunds  10000000//初始資金
#define particle 100//10顆粒子//
#define changeyear 10//多少年滑動視窗(測試期)//
#define windows 13
#define W 0.4
#define dimension 2
#define ArchiveSize 100
#define GridSize 30

using namespace std;
using std::string;

void Create_File(int startyear, int endyear);
int Look_YQHM(int windowsfor, int& change);
void Data_In();
void Reini(int particlefor);
int Grid_ChooseGbest();
void Update_X_V(int particlefor,int Gbest);
void Calc(int particlefor, int generationsfor, int expfor);
long long Combinatorial_to_Numerical(int particlefor);
void Update_Archive(int generationsfor, int expfor);
void UpdatePbest();
void ArchiveController();
void Quick_Sort(int left, int right);
void Delete_Duplicate_Solution();
void io_Data_Place(int monthfor, int windowsfor);
void out_Pareto_Front_Place(int monthfor, int windowsfor);
void out_Pareto_Front(chrono::time_point<chrono::high_resolution_clock> time);
void out_Summart_Table(int windowsfor, chrono::time_point<chrono::high_resolution_clock> time);

string fundname;
string FileLocation;
string Algorithmname;
string datapalce;
string Sliding_Windows[windows] = { "H#", "H2H" , "H2M", "H2Q", "M#", "M2M", "Q#", "Q2M", "Q2Q", "Y2H", "Y2M", "Y2Q", "Y2Y" };//13個滑動視窗
string stock[stocknum][day] = {};//股價含名字
string in;//股價檔案名稱
string out;//輸出檔案名稱
string dataplace;//股價檔案路徑&位置
string outavg;//平均輸出路徑&檔案名稱
string windowsplace;//輸出檔案路徑&位置
string paretofront_outfile;
string totaltableplace;//總表輸出檔案位置

double
stockrate[stocknum][day] = { 0 },//股價
level[particle][stocknum][day] = { 0 },//資金水位
trend[particle], //趨勢值
truereturn[particle],
risk[particle],
expectedreturn[particle],
totallevel[particle][day] = { 0 },
leftfunds[particle][stocknum] = { 0 },//單檔股票剩餘資金
Pbest[particle][dimension],
PbestPickStock[particle][stocknum];

long long v[particle],
x[particle],
PbestNummerical[particle];

int
days,
fitstocknum,
slidingdown,
slidingup,
fitexptime,
generations,
fitparticle,
fitchangeyear,
pickstocknum[particle] = { 0 },
year = 0,
pickstock[particle][stocknum] = { 0 },//選到的股票
buystocknum[particle][stocknum],//股票購買的數量
add,//年份
Num_of_Calc;

vector<long long> ParetoFrontNumerical;
vector<int>ParetoFrontStockNum = { INT_MAX },
ParetoFrontExp,
ParetoFrontGeneration,
ParetoFrontNumCalc,
ParetoFrontDenstiy;
vector<string> ParetoFrontPickStock = { "" };
vector<vector<int>> ParetoFrontStock = { ParetoFrontStockNum };
vector<double> 
ParetoFrontTrend = { DBL_MIN },
ParetoFrontRisk = { DBL_MAX },
ParetoFrontReturn = { DBL_MIN };
