#include "variable.h"

int main()
{
	int startyear, endyear, tableexecute = 1;
	dataplace = "TSX&NIKKEI_2016-2025";
	Algorithmname = "MOPSO";
	fitexptime = 3;
	generations = 999;
	fitparticle = 100;
	fitstocknum = 0;
	slidingdown = 1, slidingup = 14;
	startyear = 2015, endyear = 2025;
	fitchangeyear = endyear - startyear;
	fitchangeyear += 1;
	tableexecute = 1;
	cout.setf(ios_base::fixed | ios::uppercase);
	cout << setprecision(30);
	Create_File(startyear, endyear);
	for (int windowsfor = slidingdown; windowsfor < slidingup; windowsfor++) //滑動視窗
	{
		year = startyear;//幾年開始(訓練期)
		int change = 0;
		Look_YQHM(windowsfor, change);//判斷以年，半年，季或月
		for (int yearfor = 1; yearfor <= fitchangeyear; yearfor++)//換年
		{
			for (int monthfor = 1; monthfor <= change; monthfor++)//換月change
			{
				Num_of_Calc = 0;
				auto starttime = std::chrono::high_resolution_clock::now();
				srand(114);//亂數種子
				io_Data_Place(monthfor, windowsfor);//輸入股價檔&輸出檔案名稱
				out_Pareto_Front_Place(monthfor, windowsfor);
				ifstream input_file;
				input_file.open(in, ios::in);//跳過無法讀取的檔案
				if (!input_file)
					continue;
				input_file.close();
				fitstocknum = 0;
				days = 0;
				for (int stocknumfor = 0; stocknumfor < stocknum; stocknumfor++)
				{
					for (int dayfor = 0; dayfor < day; dayfor++)
					{
						stock[stocknumfor][dayfor] = "";
						stockrate[stocknumfor][dayfor] = 0;
					}
				}
				for (int particlefor = 0; particlefor < particle; particlefor++)
				{
					for (int dimensionfor = 0; dimensionfor < dimension; dimensionfor++)
					{
						if (dimensionfor == 0)
							Pbest[particlefor][dimensionfor] = -DBL_MAX;
						else
							Pbest[particlefor][dimensionfor] = DBL_MAX;
					}
					Reini(particlefor);
				}
				Data_In();//股價輸入
				vector<int>(it1) = { INT_MAX };
				vector<string>(it2) = { "" };
				vector<double>(it3) = { DBL_MIN };
				vector<double>(it4) = { DBL_MAX };
				vector<vector <int>>(it5) = { it1 };
				vector<double>(it6) = { DBL_MAX };
				vector<int>(it7) = { INT_MAX };
				vector<int>(it8) = { INT_MAX };
				vector<int>(it9) = { INT_MAX };
				vector<long long>(it10) = { INT_MAX };
				ParetoFrontStockNum.swap(it1);
				ParetoFrontPickStock.swap(it2);
				ParetoFrontReturn.swap(it3);
				ParetoFrontRisk.swap(it4);
				ParetoFrontStock.swap(it5);
				ParetoFrontTrend.swap(it6);
				ParetoFrontExp.swap(it7);
				ParetoFrontGeneration.swap(it8);
				ParetoFrontNumCalc.swap(it9);
				ParetoFrontNumerical.swap(it10);
				for (int expfor = 0; expfor < fitexptime; expfor++)
				{
					for (int particlefor = 0; particlefor < fitparticle; particlefor++)
					{
						for (int dimensionfor = 0; dimensionfor < dimension; dimensionfor++)
						{
							if (dimensionfor == 0)
								Pbest[particlefor][dimensionfor] = -DBL_MAX;
							else
								Pbest[particlefor][dimensionfor] = DBL_MAX;
						}
						Reini(particlefor);
						for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
						{
							double random = (double)rand() / (double)RAND_MAX;
							if (random > 0.5)
							{
								pickstock[particlefor][stocknumfor] = 1;
								pickstocknum[particlefor]++;
							}
							else
								pickstock[particlefor][stocknumfor] = 0;
							//cout << pickstock[particlefor][stocknumfor] << " ";
						}
						v[particlefor] = 0;
						x[particlefor] = Combinatorial_to_Numerical(particlefor);
						Calc(particlefor, 0 ,expfor);//計算
						//cout << x[particlefor] << " " << v[particlefor] << endl;
						//PbestNummerical[particlefor] = 0;
						/*for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
						{
							if (particlefor == 0)
							{
								cout << pickstock[particlefor][stocknumfor] << " ";
							}
						}*/
					}
					Update_Archive(0, expfor);
					UpdatePbest();
					for (int generationsfor = 0; generationsfor < generations; generationsfor++)
					{
						for (int particlefor = 0; particlefor < fitparticle; particlefor++)
						{
							Reini(particlefor);
							int Gbest = -1;
							if (ParetoFrontRisk[0] != DBL_MAX)
							{
								Gbest = Grid_ChooseGbest();
							}
							Update_X_V(particlefor, Gbest);
							Calc(particlefor, generationsfor, expfor);
							if (generationsfor == 1 && expfor == 0)
							{
								//cout << x[particlefor] << " " << v[particlefor] << endl;
							}
						}
						//cout << endl;
						/*if (generationsfor == 4498)//4503
						{
							for (int particlefor = 0; particlefor < fitparticle; particlefor++)
							{
								cout <<"PbestReturn " << Pbest[particlefor][1] << "PbestRisk " << Pbest[particlefor][0] << " Risk  " << risk[particlefor] << " Return " << expectedreturn[particlefor] << endl;
								cout << x[particlefor] << " " << v[particlefor] << endl;
							}
							cout << endl;
						}*/
						Update_Archive(generationsfor, expfor);
						//cout << endl;
						//Quick_Sort(0, ParetoFrontRisk.size() - 1);
						
						/*if (ParetoFrontRisk.size() > ArchiveSize)
							cout << generationsfor << " " << endl;*/
						ArchiveController();
						UpdatePbest();
						/*if (generationsfor == 4498)//4503
						{
							cout << "After Update" << endl;
							for (int particlefor = 0; particlefor < fitparticle; particlefor++)
							{
								cout << "PbestReturn " << Pbest[particlefor][1] << "PbestRisk " << Pbest[particlefor][0] << "Risk  " << risk[particlefor] << "Return " << expectedreturn[particlefor] << endl;
								//cout << x[particlefor] << " " << v[particlefor] << endl;
							}
						}*/
					}
				}
				Quick_Sort(0, ParetoFrontRisk.size() - 1);
				Delete_Duplicate_Solution();
				out_Pareto_Front(starttime);
				if (tableexecute == 1)
				{
					out_Summart_Table(windowsfor, starttime);
				}
			}
			year++;
		}
	}
}

int Grid_ChooseGbest()
 {
	map<pair<int, int>,int> grid;
	vector<pair<int, int>> particleToHypercube;
	double RiskMax, RiskMin, ReturnMax, ReturnMin;
	ParetoFrontDenstiy.clear();
	for (int paretofor = 0; paretofor < ParetoFrontRisk.size(); paretofor++)
	{
		RiskMax = *std::max_element(ParetoFrontRisk.begin(), ParetoFrontRisk.end());
		RiskMin = *std::min_element(ParetoFrontRisk.begin(), ParetoFrontRisk.end());
		ReturnMax = *std::max_element(ParetoFrontReturn.begin(), ParetoFrontReturn.end());
		ReturnMin = *std::min_element(ParetoFrontReturn.begin(), ParetoFrontReturn.end());
	}
	grid.clear();
	for (int paretofor = 0; paretofor < ParetoFrontRisk.size(); paretofor++)
	{
		int i = (int)((ParetoFrontRisk[paretofor] - RiskMin) / (RiskMax - RiskMin + 1e-9) * GridSize);
		int j = (int)((ParetoFrontReturn[paretofor] - ReturnMin) / (ReturnMax - ReturnMin + 1e-9) * GridSize);
		if (i >= GridSize)
			i = GridSize - 1;
		if (j >= GridSize)
			j = GridSize - 1;
		grid[{i, j}]++;
		particleToHypercube.push_back({ i, j });
	}
	for (const auto& coord : particleToHypercube) {
		ParetoFrontDenstiy.push_back(grid[coord]);
	}

	vector<pair<int, int>> cubeList;
	vector<double> fitness;
	double totalFitness = 0.0;

	for (auto it = grid.begin(); it != grid.end(); ++it) {
		pair<int, int> cube = it->first;
		int count = it->second;

		double fit = (count > 1) ? (10.0 / count) : 10.0;
		cubeList.push_back(cube);
		fitness.push_back(fit);
		totalFitness += fit;
	}

	vector<double> normalizedFit;
	for (double f : fitness) {
		normalizedFit.push_back(f / totalFitness);
	}

	vector<double> fitCDF;
	partial_sum(normalizedFit.begin(), normalizedFit.end(), back_inserter(fitCDF));

	double random = (double)rand() / (double)RAND_MAX;
	//cout << random << endl;
	int ChooseParticle = 0;
	pair<int, int> selectedGrid; 
	double lower = 0.0;
	double upper = 0.0;
	for (int ParticleFitRangefor = 0; ParticleFitRangefor < fitCDF.size(); ParticleFitRangefor++)
	{
		upper = fitCDF[ParticleFitRangefor];
		if (random > lower && random <= upper)
		{
			int i = cubeList[ParticleFitRangefor].first;
			int j = cubeList[ParticleFitRangefor].second;
			selectedGrid = { i, j };
		}
		lower = upper;
	}
	if (random > upper)
	{
		selectedGrid = { GridSize - 1,GridSize - 1 };
	}
	vector<int> candidates;
	for (int idx = 0; idx < ParetoFrontRisk.size(); ++idx)
	{
		int i = particleToHypercube[idx].first;
		int j = particleToHypercube[idx].second;
		if (make_pair(i, j) == selectedGrid)
			candidates.push_back(idx);
	}
	if (!candidates.empty())
	{
		int r = rand() % candidates.size();
		ChooseParticle = candidates[r];
	}
	//cout << ChooseParticle << endl;
	return ChooseParticle;
}

void UpdatePbest()
{
	//pbest更新
	/*for (int particlefor = 0; particlefor < particle; particlefor++)
	{
		if (expectedreturn[particlefor] > Pbest[particlefor][0] && risk[particlefor] < Pbest[particlefor][1])
		{
			Pbest[particlefor][0] = expectedreturn[particlefor];
			Pbest[particlefor][1] = risk[particlefor];
			for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
			{
				PbestPickStock[particlefor][stocknumfor] = pickstock[particlefor][stocknumfor];
			}
			PbestNummerical[particlefor] = Combinatorial_to_Numerical(particlefor);
			continue;
		}
		else if ((expectedreturn[particlefor] >= Pbest[particlefor][0] && risk[particlefor] >= Pbest[particlefor][1]) || (expectedreturn[particlefor] <= Pbest[particlefor][0] && risk[particlefor] <= Pbest[particlefor][1]))
		{
			double pbestrand = (double)rand() / RAND_MAX;
			//cout << particlefor << " " << expectedreturn[particlefor] << " " << risk[particlefor] << " " << pbestrand << endl;
			if (pbestrand < 0.5)
			{
				Pbest[particlefor][0] = expectedreturn[particlefor];
				Pbest[particlefor][1] = risk[particlefor];
				for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
				{
					PbestPickStock[particlefor][stocknumfor] = pickstock[particlefor][stocknumfor];
				}
				PbestNummerical[particlefor] = Combinatorial_to_Numerical(particlefor);
			}
		}
	}*/
	for (int i = 0; i < fitparticle; ++i)
	{
		// 1. 讀當前解 / 歷史最優
		double er_cur = expectedreturn[i];
		double risk_cur = risk[i];
		double er_pb = Pbest[i][0];
		double risk_pb = Pbest[i][1];

		// 2. 計算支配關係
		bool a_not_worse = (er_cur >= er_pb) && (risk_cur <= risk_pb);
		bool b_not_worse = (er_pb >= er_cur) && (risk_pb <= risk_cur);
		bool a_strict = (er_cur > er_pb) || (risk_cur < risk_pb);
		bool b_strict = (er_pb > er_cur) || (risk_pb < risk_cur);

		int dom = 0;
		if (a_not_worse && a_strict)      dom = +1;  // 新解支配 pBest
		else if (b_not_worse && b_strict) dom = -1;  // pBest 支配 新解

		// 3. 根據 dom 值更新
		if (dom == +1)
		{
			// 嚴格支配 → 一定更新 pBest
			Pbest[i][0] = er_cur;
			Pbest[i][1] = risk_cur;
			for (int j = 0; j < fitstocknum; ++j)
				PbestPickStock[i][j] = pickstock[i][j];
			PbestNummerical[i] =
				Combinatorial_to_Numerical(i);
		}
		else if (dom == 0)
		{
			// 互不支配 → 50% 機率更新
			if (((double)rand() / RAND_MAX) < 0.5)
			{
				Pbest[i][0] = er_cur;
				Pbest[i][1] = risk_cur;
				for (int j = 0; j < fitstocknum; ++j)
					PbestPickStock[i][j] = pickstock[i][j];
				PbestNummerical[i] =
					Combinatorial_to_Numerical(i);
			}
		}
	}
}

void Delete_Duplicate_Solution()
{
	set<double> uniqueParetoFront;//(Country_risk_temp.begin(), Country_risk_temp.end());
	for (int value = 0; value < ParetoFrontRisk.size(); value++)
	{
		auto result = uniqueParetoFront.insert(ParetoFrontRisk[value]);
		if (!result.second) { // 如果插入失敗，代表這個值已經存在
			ParetoFrontStock.erase(ParetoFrontStock.begin() + value);
			ParetoFrontStockNum.erase(ParetoFrontStockNum.begin() + value);
			ParetoFrontPickStock.erase(ParetoFrontPickStock.begin() + value);
			ParetoFrontReturn.erase(ParetoFrontReturn.begin() + value);
			ParetoFrontRisk.erase(ParetoFrontRisk.begin() + value);
			ParetoFrontTrend.erase(ParetoFrontTrend.begin() + value);
			ParetoFrontExp.erase(ParetoFrontExp.begin() + value);
			ParetoFrontGeneration.erase(ParetoFrontGeneration.begin() + value);
			ParetoFrontNumCalc.erase(ParetoFrontNumCalc.begin() + value);
			ParetoFrontNumerical.erase(ParetoFrontNumerical.begin() + value);
			--value;
		}
	}
}

void ArchiveController()
{
	while (ParetoFrontRisk.size() > ArchiveSize)
	{
		vector<int> ParetoFrontDensity(ParetoFrontRisk.size(), 0);
		vector<pair<int, int>> particleToHypercube;
		map<pair<int, int>, vector<int>> gridMap;
		double RiskMin = *min_element(ParetoFrontRisk.begin(), ParetoFrontRisk.end());
		double RiskMax = *max_element(ParetoFrontRisk.begin(), ParetoFrontRisk.end());
		double ReturnMin = *min_element(ParetoFrontReturn.begin(), ParetoFrontReturn.end());
		double ReturnMax = *max_element(ParetoFrontReturn.begin(), ParetoFrontReturn.end());

		for (int idx = 0; idx < ParetoFrontRisk.size(); ++idx) {
			int i = (int)((ParetoFrontRisk[idx] - RiskMin) / (RiskMax - RiskMin + 1e-9) * GridSize);
			int j = (int)((ParetoFrontReturn[idx] - ReturnMin) / (ReturnMax - ReturnMin + 1e-9) * GridSize);
			if (i >= GridSize) i = GridSize - 1;
			if (j >= GridSize) j = GridSize - 1;
			gridMap[{i, j}].push_back(idx);
			particleToHypercube.push_back({ i, j });
		}
		ParetoFrontDenstiy.clear();
		for (const auto& coord : particleToHypercube) {
			ParetoFrontDenstiy.push_back(gridMap[coord].size());
		}
		int MaxDensity = 0;
		for (auto it = gridMap.begin(); it != gridMap.end(); ++it)
		{
			pair<int, int> cube = it->first;         // 取出超方格的位置 (i, j)
			vector<int> members = it->second;        // 取出該格子裡的所有粒子 index
			if ((int)members.size() > MaxDensity)
				MaxDensity = members.size();
		}

		vector<pair<int, int>> MaxDensityCubes;
		for (auto it = gridMap.begin(); it != gridMap.end(); ++it)
		{
			pair<int, int> cube = it->first;         // 取出超方格的位置 (i, j)
			vector<int> members = it->second;        // 取出該格子裡的所有粒子 index
			if ((int)members.size() == MaxDensity)
				MaxDensityCubes.push_back(cube);
		}

		vector<int> candidateIndices;
		for (int idx = 0; idx < particleToHypercube.size(); ++idx) {
			for (const auto& cube : MaxDensityCubes) {
				if (particleToHypercube[idx] == cube) {
					candidateIndices.push_back(idx);
					break;  // 一旦匹配一個 cube 就加入，不需繼續比
				}
			}
		}

		// 找出最小 index 的粒子
		int IndexInPareto = -1;
		if (!candidateIndices.empty()) 
		{
			IndexInPareto = *min_element(candidateIndices.begin(), candidateIndices.end());
		}
		pair<int, int> selectedCube = particleToHypercube[IndexInPareto];

		vector<int> matchingParticles;
		for (int i = 0; i < particleToHypercube.size(); ++i) 
		{
			if (particleToHypercube[i] == selectedCube) 
			{
				matchingParticles.push_back(i);  // 收集該 cube 裡的粒子 index
			}
		}
		size_t deleteIndexInPareto = -1;
		int random = rand();
		//cout << random << endl;
		deleteIndexInPareto = matchingParticles[random % matchingParticles.size()];

		ParetoFrontDenstiy.erase(ParetoFrontDenstiy.begin() + deleteIndexInPareto);
		ParetoFrontStock.erase(ParetoFrontStock.begin() + deleteIndexInPareto);
		ParetoFrontStockNum.erase(ParetoFrontStockNum.begin() + deleteIndexInPareto);
		ParetoFrontPickStock.erase(ParetoFrontPickStock.begin() + deleteIndexInPareto);
		ParetoFrontReturn.erase(ParetoFrontReturn.begin() + deleteIndexInPareto);
		ParetoFrontRisk.erase(ParetoFrontRisk.begin() + deleteIndexInPareto);
		ParetoFrontTrend.erase(ParetoFrontTrend.begin() + deleteIndexInPareto);
		ParetoFrontExp.erase(ParetoFrontExp.begin() + deleteIndexInPareto);
		ParetoFrontGeneration.erase(ParetoFrontGeneration.begin() + deleteIndexInPareto);
		ParetoFrontNumCalc.erase(ParetoFrontNumCalc.begin() + deleteIndexInPareto);
		ParetoFrontNumerical.erase(ParetoFrontNumerical.begin() + deleteIndexInPareto);
	}
}

void Reini(int particlefor)
{
	trend[particlefor] = 0;
	expectedreturn[particlefor] = 0;
	risk[particlefor] = 0;
	pickstocknum[particlefor] = 0;
	for (int j = 0; j < stocknum; j++)
	{
		pickstock[particlefor][j] = 0;
		leftfunds[particlefor][j] = 0;
		buystocknum[particlefor][j] = 0;
	}
	for (int dayfor = 0; dayfor < day; dayfor++)
	{
		totallevel[particlefor][dayfor] = 0;
		for (int j = 0; j < stocknum; j++)
		{
			level[particlefor][j][dayfor] = 0;
		}
	}
}

void Update_X_V(int particlefor,int Gbest)
{
	pickstocknum[particlefor] = 0;
	double tmp1 = ((double)rand() / (double)RAND_MAX);
	double tmp2 = ((double)rand() / (double)RAND_MAX);
	//cout << Gbest << endl;
	if (Gbest == -1)
		v[particlefor] = W * v[particlefor] + tmp1 * (PbestNummerical[particlefor] - x[particlefor]);
	else
		v[particlefor] = W * v[particlefor] + tmp1 * (PbestNummerical[particlefor] - x[particlefor]) + tmp2 * (ParetoFrontNumerical[Gbest] - x[particlefor]);
	x[particlefor] = x[particlefor] + v[particlefor];
	if (x[particlefor] >= pow(2, fitstocknum)) 
	{
		x[particlefor] = (long long)pow(2, fitstocknum)-1;
		v[particlefor] = -v[particlefor];
	}
	else if (x[particlefor] < 0)
	{
		x[particlefor] = 0;
		v[particlefor] = -v[particlefor];
	}
	for (int stocknumfor = 0; stocknumfor < fitstocknum; ++stocknumfor)
	{
		pickstock[particlefor][stocknumfor] = (x[particlefor] >> stocknumfor) & 1;
		if (pickstock[particlefor][stocknumfor] == 1)
		{
			pickstocknum[particlefor]++;
		}
	}
}

void Update_Archive(int generationsfor,int expfor)
{
	//Archive更新
	for (int particlefor = 0; particlefor < fitparticle; particlefor++)
	{
		for (int i = 0; i < ParetoFrontReturn.size(); i++)
		{
			if (expectedreturn[particlefor] > ParetoFrontReturn[i] && risk[particlefor] < ParetoFrontRisk[i] && expectedreturn[particlefor] > (double)0)
			{
				ParetoFrontStock.erase(ParetoFrontStock.begin() + i);
				ParetoFrontStockNum.erase(ParetoFrontStockNum.begin() + i);
				ParetoFrontPickStock.erase(ParetoFrontPickStock.begin() + i);
				ParetoFrontReturn.erase(ParetoFrontReturn.begin() + i);
				ParetoFrontRisk.erase(ParetoFrontRisk.begin() + i);
				ParetoFrontTrend.erase(ParetoFrontTrend.begin() + i);
				ParetoFrontExp.erase(ParetoFrontExp.begin() + i);
				ParetoFrontGeneration.erase(ParetoFrontGeneration.begin() + i);
				ParetoFrontNumCalc.erase(ParetoFrontNumCalc.begin() + i);
				ParetoFrontNumerical.erase(ParetoFrontNumerical.begin() + i);
				add = 1;
				--i;
			}
			else if (expectedreturn[particlefor] >= ParetoFrontReturn[i] || risk[particlefor] <= ParetoFrontRisk[i] && expectedreturn[particlefor] > (double)0)
			{
				add = 1;
			}
			else if (expectedreturn[particlefor] <= ParetoFrontReturn[i] && risk[particlefor] >= ParetoFrontRisk[i] && expectedreturn[particlefor] > (double)0)
			{
				add = 0;
				break;
			}
		}
		if (add == 1)
		{
			string paretostocktmp = "";
			ParetoFrontStockNum.push_back(pickstocknum[particlefor]);
			vector<int>paretotmp;
			for (int i = 0; i < fitstocknum; i++)
			{
				if (pickstock[particlefor][i] == 1)
				{
					paretostocktmp = paretostocktmp + stock[i][0] + " ";
					paretotmp.push_back(1);
				}
				else
				{
					paretotmp.push_back(0);
				}
			}
			ParetoFrontStock.push_back(paretotmp);
			ParetoFrontPickStock.push_back(paretostocktmp);
			ParetoFrontReturn.push_back(expectedreturn[particlefor]);
			ParetoFrontRisk.push_back(risk[particlefor]);
			ParetoFrontTrend.push_back(trend[particlefor]);
			ParetoFrontExp.push_back(expfor);
			ParetoFrontGeneration.push_back(generationsfor);
			ParetoFrontNumCalc.push_back(Num_of_Calc);
			ParetoFrontNumerical.push_back(x[particlefor]);
		}
		add = 0;
	}
}

long long Combinatorial_to_Numerical(int particlefor)
{
	long long Numerical = 0;
	for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
	{
		if (pickstock[particlefor][stocknumfor] == 1)
			Numerical |= (1LL << stocknumfor);
	}
	return Numerical;
}

//觀察是什麼區間
int Look_YQHM(int windowsfor, int& change)
{
	if (windowsfor == 3 || windowsfor == 5 || windowsfor == 6 || windowsfor == 8 || windowsfor == 11)
	{
		change = 12;
	}
	if (windowsfor == 1 || windowsfor == 2 || windowsfor == 10)
	{
		change = 2;
	}
	if (windowsfor == 4 || windowsfor == 7 || windowsfor == 9 || windowsfor == 12)
	{
		change = 4;
	}
	if (windowsfor == 13)
	{
		change = 1;
	}
	return change;
}

void Create_File(int startyear, int endyear)
{
	string AverageLocation, SummaryTableLocation, SingleStockLocation;
	if (startfunds == 10000000)
		fundname = "10M";
	else if (startfunds == 1000000000)
		fundname = "1B";
	FileLocation = "./" + dataplace + "_" + Algorithmname + "_" + to_string(startyear) + "-" + to_string(endyear) + "(" + to_string(fitchangeyear) + ")_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname;
	_mkdir(FileLocation.c_str());
	SummaryTableLocation = FileLocation + "\\" + "Summart_Table";
	_mkdir(SummaryTableLocation.c_str());
	AverageLocation = SummaryTableLocation + "\\" + "Avearge";
	_mkdir(AverageLocation.c_str());
	for (int i = slidingdown; i < slidingup; i++)
	{
		string SlidingLocation, ParetoFrontLocation, ParetoDetailLocation;
		SlidingLocation = FileLocation + "\\" + Sliding_Windows[i - 1];
		_mkdir(SlidingLocation.c_str());
		ParetoFrontLocation = FileLocation + "\\" + Sliding_Windows[i - 1] + "\\" + "Pareto_Front";
		_mkdir(ParetoFrontLocation.c_str());
	}
	ofstream outstockavg1;
	outavg = "./" + dataplace + "\\Summary_Table\\Avearge\\Stock_Avg.csv";
	outstockavg1.open(outavg, ios::out);//創建文檔
	outstockavg1.close();
}

//計算
void Calc(int particlefor, int generationsfor, int expfor)
{
	double //趨勢值計算參數
		funds = 0,//分配資金
		fundsleft = 0,//投資組合剩餘資金
		expectedreturntemp = 0, //預期報酬暫存
		power = 0, //天數平方
		Yi = 0, //一次回歸趨勢線
		tmp = 0;//
	double //夏普值計算參數
		sptemptotallevel,
		spreturntemp = 0,
		splevelavg,
		sptemp[day],
		allsptemp = 0,
		risktemp;
	if (pickstocknum[particlefor] != 0)
	{
		funds = startfunds / pickstocknum[particlefor];
		fundsleft = startfunds % pickstocknum[particlefor];
	}
	for (int stocknumfor = 0; stocknumfor < fitstocknum; stocknumfor++)
	{
		if (pickstock[particlefor][stocknumfor] == 1)
		{
			buystocknum[particlefor][stocknumfor] = funds / stockrate[stocknumfor][0];
			leftfunds[particlefor][stocknumfor] = funds - (stockrate[stocknumfor][0] * buystocknum[particlefor][stocknumfor]);
			for (int daysfor = 0; daysfor < days; daysfor++)
			{
				level[particlefor][stocknumfor][daysfor] = stockrate[stocknumfor][daysfor] * buystocknum[particlefor][stocknumfor] + leftfunds[particlefor][stocknumfor];//每股資金水位
			}
		}
	}
	for (int daysfor = 0; daysfor < days; daysfor++)
	{
		for (int k = 0; k < fitstocknum; k++)
		{
			totallevel[particlefor][daysfor] += level[particlefor][k][daysfor];//總資金水位
		}
		totallevel[particlefor][daysfor] += fundsleft;
		expectedreturntemp += (((double)daysfor + 1) * totallevel[particlefor][daysfor]) - (((double)daysfor + 1) * startfunds);
		power += pow(daysfor + 1, 2);
	}
	expectedreturn[particlefor] = expectedreturntemp / power;//每日預期報酬
	for (int daysfor = 0; daysfor < days; daysfor++)
	{
		Yi = (expectedreturn[particlefor] * (daysfor + 1)) + startfunds;//預期資金水位(一次回歸趨勢線）
		tmp += pow((totallevel[particlefor][daysfor] - Yi), 2);//背離趨勢線的波動
	}
	risk[particlefor] = sqrt(tmp / days);//每日風險
	truereturn[particlefor] = totallevel[particlefor][days - 1] - startfunds;
	trend[particlefor] = expectedreturn[particlefor] / risk[particlefor];//趨勢值
	if (trend[particlefor] < 0)
	{
		trend[particlefor] = expectedreturn[particlefor] * risk[particlefor];
	}
	if (pickstocknum[particlefor] == 0)
	{
		risk[particlefor] = 0;//DBL_MAX
		expectedreturn[particlefor] = 0;
		trend[particlefor] = 0;
	}
	Num_of_Calc += 1;
}

//fundtrain輸出位置
void io_Data_Place(int monthfor, int windowsfor)
{
	windowsplace = FileLocation + "\\" + Sliding_Windows[windowsfor - 1] + "\\";
	string tempdataplace = "./" + dataplace + "\\" + Sliding_Windows[windowsfor - 1];
	if (windowsfor == 1 || windowsfor == 2)//H#,H2H
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q1-Q2(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q1-Q2(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 2)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q3-Q4(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q3-Q4(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 3)//H2M
	{
		if (monthfor < 5)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
		}
		else if (monthfor >= 5 && monthfor < 8)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1).csv";
		}
		else if (monthfor >= 8 && monthfor < 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
		}
		else if (monthfor >= 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
			out = windowsplace + "\\Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1).csv";
		}
	}
	if (windowsfor == 4)//H2Q
	{
		if (monthfor < 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "-Q" + to_string(monthfor + 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "-Q" + to_string(monthfor + 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor + 1 - 4) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor + 1 - 4) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 5 || windowsfor == 6)//M#,M2M
	{
		if (monthfor <= 9)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 9)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 7 || windowsfor == 9)//Q#.Q2Q
	{
		if (monthfor <= 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 8)//Q2M
	{
		if (monthfor <= 7)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 7 && monthfor < 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + " - " + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 2 - 12) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 2 - 12) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 10)//Y2H
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 2)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q3~" + to_string(year + 1) + "_Q2(" + to_string(year) + " Q1).csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q3~" + to_string(year + 1) + "_Q2(" + to_string(year) + " Q1).csv";
		}
	}
	if (windowsfor == 11)//Y2M
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 1 && monthfor < 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor == 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor > 10)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 12)//Y2Q
	{
		if (monthfor < 2)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
		else if (monthfor >= 2 && monthfor <= 4)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	if (windowsfor == 13)//Y2Y
	{
		if (monthfor == 1)
		{
			in = tempdataplace + "\\train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
			out = windowsplace + "Fund_Train\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_Fund_Train_" + to_string(year) + "(" + to_string(year) + " Q1" + ").csv";
		}
	}
	cout << in << endl;
	cout << out << endl;
}

void out_Pareto_Front(chrono::time_point<chrono::high_resolution_clock> time)
{
	ofstream outpareto;
	outpareto << fixed << setprecision(30);
	/*if (expfor == 0)
		outpareto.open(paretofront_outfile, ios::out);
	else
		outpareto.open(paretofront_outfile, ios::app);
	outpareto << "Exp" << expfor << endl;*/
	outpareto.open(paretofront_outfile, ios::out);
	outpareto << "Number of Stock in Portfolio" << "," << "Portfolio" << "," << "TrendRatio" << "," << "DailyRisk" << "," << "ExpectedReturn" << "," << "Exp" << "," << "Generation" << "," << "Number of Computations";//TrendRatio ExpectedReturn DailyRisk
	outpareto << endl;
	for (int i = 0; i < ParetoFrontStockNum.size(); i++)
	{
		if (ParetoFrontReturn[i] != DBL_MIN)
			outpareto << ParetoFrontStockNum[i] << "," << ParetoFrontPickStock[i] << "," << ParetoFrontTrend[i] << "," << ParetoFrontRisk[i] << "," << ParetoFrontReturn[i] << "," << ParetoFrontExp[i] << "," << ParetoFrontGeneration[i] << "," << ParetoFrontNumCalc[i] << endl;
	}
	outpareto << endl;
	outpareto << "Total Number of Calculations used" << "," << Num_of_Calc << endl;
	auto endtime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - time);
	outpareto << "Duration" << "," << duration.count() << endl;
	/*if (expfor == 49)
	{
		outpareto << endl;
		for (int i = 0; i < fitexptime; i++)
		{
			outpareto << LowestRisk[i] << ",";
		}
	}*/
	outpareto.close();
}

void Quick_Sort(int left, int right) {
	if (left < right) {
		// 选取枢纽元素，这里选择中间元素作为枢纽元素
		double pivot = ParetoFrontRisk[(left + right) / 2];
		int i = left;
		int j = right;

		// 划分数组
		while (i <= j) {
			while (ParetoFrontRisk[i] < pivot) {
				i++;
			}
			while (ParetoFrontRisk[j] > pivot) {
				j--;
			}
			if (i <= j) {
				swap(ParetoFrontStock[i], ParetoFrontStock[j]);
				swap(ParetoFrontStockNum[i], ParetoFrontStockNum[j]);
				swap(ParetoFrontPickStock[i], ParetoFrontPickStock[j]);
				swap(ParetoFrontReturn[i], ParetoFrontReturn[j]);
				swap(ParetoFrontRisk[i], ParetoFrontRisk[j]);
				swap(ParetoFrontTrend[i], ParetoFrontTrend[j]);
				swap(ParetoFrontExp[i], ParetoFrontExp[j]);
				swap(ParetoFrontGeneration[i], ParetoFrontGeneration[j]);
				swap(ParetoFrontNumCalc[i], ParetoFrontNumCalc[j]);
				swap(ParetoFrontNumerical[i], ParetoFrontNumerical[j]);
				i++;
				j--;
			}
		}
		// 递归调用快速排序
		Quick_Sort(left, j);
		Quick_Sort(i, right);
	}
}

//柏拉圖前沿輸出位置
void out_Pareto_Front_Place(int monthfor, int windowsfor)
{
	if (windowsfor == 1 || windowsfor == 2)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q1-Q2(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 2)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q3-Q4(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 3)
	{
		if (monthfor < 5)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1)" + ".csv";
		}
		else if (monthfor >= 5 && monthfor < 8)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 5) + "(" + to_string(year) + " Q1)" + ".csv";
		}
		else if (monthfor >= 8 && monthfor < 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1)" + ".csv";
		}
		else if (monthfor >= 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 5 - 12) + "(" + to_string(year) + " Q1)" + ".csv";
		}
	}
	if (windowsfor == 4)
	{
		if (monthfor < 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "-Q" + to_string(monthfor + 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor + 1 - 4) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 5 || windowsfor == 6)
	{
		if (monthfor <= 9)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 9)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 7 || windowsfor == 9)
	{
		if (monthfor <= 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 8)
	{
		if (monthfor <= 7)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-0" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 7 && monthfor < 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "-" + to_string(monthfor + 2) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor + 2 - 12) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 10)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 2)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q3~" + to_string(year + 1) + "_Q2(" + to_string(year) + " Q1)" + ".csv";
		}
	}
	if (windowsfor == 11)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 1 && monthfor < 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_0" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor == 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_0" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor > 10)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_" + to_string(monthfor) + "~" + to_string(year + 1) + "_" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 12)
	{
		if (monthfor < 2)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
		else if (monthfor >= 2 && monthfor <= 4)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "_Q" + to_string(monthfor) + "~" + to_string(year + 1) + "_Q" + to_string(monthfor - 1) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	if (windowsfor == 13)
	{
		if (monthfor == 1)
		{
			paretofront_outfile = windowsplace + "Pareto_Front\\" + dataplace + "_" + Algorithmname  + "_" + to_string(fitexptime) + "_" + to_string(generations) + "_" + to_string(fitparticle) + "_" + to_string(ArchiveSize) + "_" + to_string(GridSize) + "_" + to_string(W) + "_" + fundname + "_" + Sliding_Windows[windowsfor - 1] + "_" + "PF_" + to_string(year) + "(" + to_string(year) + " Q1" + ")" + ".csv";
		}
	}
	cout << paretofront_outfile << endl;
}

//總表輸出
void out_Summart_Table(int h, chrono::time_point<chrono::high_resolution_clock> time)
{
	static int totaltabletemph = 0;
	totaltableplace = FileLocation + "\\" + "Summart_Table\\" + Sliding_Windows[h - 1] + ".csv";
	cout << totaltableplace << endl;
	ofstream totaltable;
	totaltable.setf(ios_base::fixed | ios::uppercase);
	totaltable << setprecision(20);
	if (h != totaltabletemph)
	{
		totaltable.open(totaltableplace, ios::out);
		totaltabletemph = h;
	}
	else
	{
		totaltable.open(totaltableplace, ios::app);
	}
	auto endtime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - time);
	totaltable << out << "," << "Number of Pareto Front" << "," << ParetoFrontRisk.size() << "," << "Duration" << "," << duration.count();
	totaltable << endl;
	totaltable.close();
}

//資料輸入
void Data_In()
{
	int i = 0, j = 0, itemp = 0;
	ifstream input_file;
	input_file.open(in, ios::in);//讀入的檔名
	string line;
	while (getline(input_file, line))
	{
		string tmp1;
		stringstream ss(line);//字串處理
		while (getline(ss, tmp1, ','))//csv檔是逗號分隔檔，所以透過逗號來讀取檔案裡面的資料
		{
			char* ptr = const_cast<char*>(tmp1.c_str());
			for (int length = 0; length < strlen(ptr); length++)
			{
				if (ptr[length] >= 'a' && ptr[length] <= 'z' || ptr[length] >= 'A' && ptr[length] <= 'Z')
				{
					fitstocknum += 1;
					break;
				}
			}
		}
	}
	input_file.close();

	input_file.open(in, ios::in);//讀入的檔名
	while (getline(input_file, line))
	{
		if (!input_file.is_open())
		{
			break;
		}
		string tmp1;
		stringstream ss(line);//字串處理
		while (getline(ss, tmp1, ','))//csv檔是逗號分隔檔，所以透過逗號來讀取檔案裡面的資料
		{
			char* ptr = const_cast<char*>(tmp1.c_str());
			stock[i][j] = ptr;
			if (i == (fitstocknum - 1))
			{
				j++;
				i = 0;
			}
			else
			{
				i++;
			}
		}
	}
	for (i = 0; i < fitstocknum; i++)
	{
		for (j = 0; j < (day - 1); j++)
		{
			if (stock[i][j + 1] != "")
			{
				stockrate[i][j] = stod(stock[i][j + 1]);
				itemp += 1;
			}
			else
			{
				break;
			}
		}
		days = itemp;
		itemp = 0;
	}
	input_file.close();//開檔要記得關檔
}