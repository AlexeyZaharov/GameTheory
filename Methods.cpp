#include "Methods.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

std::tuple<Table, Table, double> AnaliticMethodMixedStrategy(const Matrix& matrix)
{
	Table u_ = {
		{1, 1, 1}
	};
	Table ut_ = {
		{1},
		{1},
		{1}
	};
	Matrix u(u_);
	Matrix ut(ut_);

	double denominator = (u * matrix.GetRevertMatrix() * ut).GetMatrix()[0][0];

	Table X = (u * matrix.GetRevertMatrix() * (1 / denominator)).GetMatrix();
	Table Y = (matrix.GetRevertMatrix() * ut * (1 / denominator)).GetMatrix();
	double v = 1 / denominator;
	return {X, Y, v};
}

std::tuple<Table, Table, double, double> BrownRobinsonMethodMixedStrategy(const Matrix& matrix, bool debug)
{
	std::srand(std::time(nullptr));
	std::vector<std::vector<std::string>> head = {
			{"k", "Choice A", "Choice B"}
	};

	auto table = matrix.GetMatrix();
	// k, choice A, choice B, win A x3, lost B x3, V[k]/k, v[k]/k, e
	std::vector<double> lastRow, newRow;
	std::vector<double> X, Y;
	size_t numStrategyA = table.size(), numStrategyB = table[0].size();
	double bestStrategyA = 0, bestStrategyB = 0;
	std::pair<double, double> bestWinBestLost = { std::numeric_limits<double>::max(), -std::numeric_limits<double>::max() };//0 };

	newRow = { 0, 0, 0 };
	for (auto i = 0; i < numStrategyA; ++i)
	{
		newRow.push_back(0);
		X.push_back(0);
		head[0].push_back("Win X" + std::to_string(i));
	}

	for (auto j = 0; j < numStrategyB; ++j)
	{
		newRow.push_back(0);
		Y.push_back(0);
		head[0].push_back("Lose Y" + std::to_string(j));
	}

	newRow.push_back(0);
	newRow.push_back(0);
	newRow.push_back(std::numeric_limits<double>::max());

	head[0].push_back("v[k]/k");
	head[0].push_back("v_[k]/k");
	head[0].push_back("e");

	if (debug)
		writeTable(head);

	while (newRow[3 + numStrategyA + numStrategyB + 2] > 0.1)
	{
		lastRow = newRow;

		++newRow[0];
		newRow[1] = bestStrategyA;
		newRow[2] = bestStrategyB;

		++X[bestStrategyA];
		++Y[bestStrategyB];

		for (auto i = 0; i < table.size(); ++i)
			newRow[3 + i] = lastRow[3 + i] + table[i][newRow[2]];

		for (auto j = 0; j < table[newRow[1]].size(); ++j)
			newRow[3 + numStrategyA + j] = lastRow[3 + numStrategyA + j] + table[newRow[1]][j];

		double maxWin = 0, minLost = std::numeric_limits<double>::max();
		std::vector<double> StrategyA, StrategyB;
		maxWin = *std::max_element(newRow.begin() + 3, newRow.begin() + 3 + numStrategyA);
		for (auto i = 3; i < 3 + numStrategyA; ++i)
		{
			if (newRow[i] == maxWin)
				StrategyA.push_back(i - 3);
		}
		bestStrategyA = StrategyA[std::rand() % StrategyA.size()];

		minLost = *std::min_element(newRow.begin() + 3 + numStrategyA, newRow.begin() + 3 + numStrategyA + numStrategyB);
		for (auto i = 3 + numStrategyA; i < 3 + numStrategyA + numStrategyB; ++i)
		{
			if (newRow[i] == minLost)
				StrategyB.push_back(i - 3 - numStrategyA);
		}
		bestStrategyB = StrategyB[std::rand() % StrategyB.size()];

		newRow[3 + numStrategyA + numStrategyB] = maxWin / newRow[0];
		newRow[3 + numStrategyA + numStrategyB + 1] = minLost / newRow[0];

		bestWinBestLost.first = std::min(newRow[3 + numStrategyA + numStrategyB], bestWinBestLost.first);
		bestWinBestLost.second = std::max(newRow[3 + numStrategyA + numStrategyB + 1], bestWinBestLost.second);
		newRow[3 + numStrategyA + numStrategyB + 2] = bestWinBestLost.first - bestWinBestLost.second;

		if (debug)
			writeTable(std::vector<std::vector<double>>{newRow});
	}

	Table Xs = (Matrix({ X }) * (1 / newRow[0])).GetMatrix();
	Table Ys = (Matrix({ Y }) * (1 / newRow[0])).GetMatrix();
	double v = (bestWinBestLost.first + bestWinBestLost.second) / 2;
	double e = newRow[3 + numStrategyA + numStrategyB + 2];

	return {Xs, Ys, v, e};
}

double GetFuncValue(double a, double b, double c, double d, double e, double x, double y)
{
	return a*x*x + b*y*y + c*x*y + d*x + e*y;
}

std::tuple<double, double, double> AnaliticMethod—onvex—oncaveGame(double a, double b, double c, double d, double e)
{
	double y = (2 * a * e - c * d) / (c * c - 4 * a * b);
	double x = y >= -d/c ? -(c * y + d) / (2 * a) : 0;
	double H = GetFuncValue(a, b, c, d, e, x, y);
	return { x, y, H };
}

std::tuple<double, double, double> NumericMethod—onvex—oncaveGame(double a, double b, double c, double d, double e)
{
	double x, y, H;
	for (auto i = 1; i < 10; ++i)
	{
		Table Xn, Yn, Hn;
		double v, epsilon;
		for (auto j = 0; j <= i; ++j)
		{
			std::vector<double> row;
			for (auto k = 0; k <= i; ++k)
				row.push_back(GetFuncValue(a, b, c, d, e, static_cast<double>(j) / i, static_cast<double>(k) / i));
			Hn.push_back(row);
		}

		writeTable(Hn);

		std::pair<int, int> maxmin, minmax;
		Matrix matrix(Hn);
		std::tie(maxmin, minmax) = matrix.GetMaxminAndMinmax();
		std::cout << "maxmin = " << matrix.GetMatrix()[maxmin.first][maxmin.second] << "; minmax = " << matrix.GetMatrix()[minmax.first][minmax.second] << std::endl;
		if (matrix.GetMatrix()[maxmin.first][maxmin.second] == matrix.GetMatrix()[minmax.first][minmax.second])
		{
			std::cout << "Has saddle point: " << matrix.GetMatrix()[maxmin.first][maxmin.second] << std::endl;
			x = static_cast<double>(maxmin.first) / i;
			y = static_cast<double>(maxmin.second) / i;
			H = matrix.GetMatrix()[maxmin.first][maxmin.second];
		}
		else
		{
			std::cout << "Hasn`t saddle point. Search solution..." << std::endl;
			std::tie(Xn, Yn, v, epsilon) = BrownRobinsonMethodMixedStrategy(matrix, false);
			auto bestX = std::max_element(Xn[0].begin(), Xn[0].end());
			auto bestY = std::max_element(Yn[0].begin(), Yn[0].end());

			x = static_cast<double>(bestX - Xn[0].begin()) / i;
			y = static_cast<double>(bestY - Yn[0].begin()) / i;
			H = GetFuncValue(a, b, c, d, e, x, y);
		}

		std::cout << "x = " << x << std::endl;
		std::cout << "y = " << y << std::endl;
		std::cout << "H = " << H << std::endl;
	}

	return { x, y, H };
}
