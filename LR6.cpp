#include "LR6.h"
#include "Matrix.h"

#include <unordered_set>
#include <cstdlib>
#include <ctime>

bool LimitedX(const Table& X)
{
	static double epsilon = 0.000001;

	std::vector<double> Xrow;
	for (const auto& elem : X)
	{
		Xrow.push_back(elem[0]);
	}

	for (const auto& elemOne : Xrow)
	{
		for (const auto& elemTwo : Xrow)
		{
			if (std::abs(elemOne - elemTwo) > epsilon)
				return false;
		}
	}

	return true;
}

void GetGoodX(const Matrix& A, Table X)
{
	std::cout << "Initial X:" << std::endl << "X: (";
	for (size_t i = 0; i < X.size() - 1; ++i)
	{
		std::cout << X[i][0] << ", ";
	}
	std::cout << X[X.size() - 1][0] << ")" << std::endl;

	while (!LimitedX(X))
	{
		X = (A * Matrix(X)).GetMatrix();
	}

	std::cout << "Good X:" << std::endl << "X: (";
	for (size_t i = 0; i < X.size() - 1; ++i)
	{
		std::cout << X[i][0] << ", ";
	}
	std::cout << X[X.size() - 1][0] << ")" << std::endl;
}

void LR6()
{
	std::srand(std::time(0));
	Table table;
	for (size_t i = 0; i < 10; ++i)
	{
		std::vector<double> vec;
		for (size_t j = 0; j < 10; ++j)
		{
			vec.push_back(std::rand() % 1001);
		}

		size_t sum = 0;
		for (auto& vecElem : vec)
		{
			sum += vecElem;
		}

		for (auto& vecElem : vec)
		{
			vecElem /= sum;
		}

		table.push_back(vec);
	}
	writeTable(table);

	Table X;
	for (size_t j = 0; j < 10; ++j)
	{
		X.push_back(std::vector<double>{static_cast<double>(std::rand() % 100)});
	}

	Matrix A(table);
	GetGoodX(A, X);

	size_t firstAgents = (std::rand() % 5) + 1;
	size_t scondAgents = (std::rand() % (7 - firstAgents)) + 1;

	std::unordered_set<size_t> firstAgentsNumbers;
	std::unordered_set<size_t> secondAgentsNumbers;
	for (size_t i = 0; i < firstAgents; ++i)
	{
		size_t num = std::rand() % 10;
		while (firstAgentsNumbers.find(num) != firstAgentsNumbers.end())
			num = std::rand() % 10;

		firstAgentsNumbers.insert(num);
	}

	for (size_t i = 0; i < scondAgents; ++i)
	{
		size_t num = std::rand() % 10;
		while (firstAgentsNumbers.find(num) != firstAgentsNumbers.end() || secondAgentsNumbers.find(num) != secondAgentsNumbers.end())
			num = std::rand() % 10;

		secondAgentsNumbers.insert(num);
	}

	std::cout << "Agents of first gamer (interval - [30, 130)):" << std::endl;
	for (const auto& elem : firstAgentsNumbers)
	{
		std::cout << elem + 1 << ' ';
	}

	std::cout << std::endl << "Agents of second gamer (interval - (-130, -30]):" << std::endl;
	for (const auto& elem : secondAgentsNumbers)
	{
		std::cout << elem  + 1<< ' ';
	}
	std::cout << std::endl;
	
	for (size_t j = 0; j < 10; ++j)
	{
		if (firstAgentsNumbers.find(j) != firstAgentsNumbers.end())
			X[j][0] = static_cast<double>(std::rand() % 100) + 30;

		if (secondAgentsNumbers.find(j) != secondAgentsNumbers.end())
			X[j][0] = -(static_cast<double>(std::rand() % 100) + 30);
	}
	GetGoodX(A, X);
}