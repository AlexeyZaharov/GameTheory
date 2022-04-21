#include "Methods.h"
#include "Matrix.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

void LR1()
{
	Table table = {
		{8, 12, 10},
		{1, 6, 19},
		{17, 11, 11}
	};

	Matrix matrix(table);

	std::cout << "Matrix:" << std::endl;
	writeTable(matrix.GetMatrix());

	std::pair<int, int> maxmin, minmax;
	std::tie(maxmin, minmax) = matrix.GetMaxminAndMinmax();
	std::cout << "maxmin = " << matrix.GetMatrix()[maxmin.first][maxmin.second] << "; minmax = " << matrix.GetMatrix()[minmax.first][minmax.second] << std::endl;
	if (matrix.GetMatrix()[maxmin.first][maxmin.second] == matrix.GetMatrix()[minmax.first][minmax.second])
	{
		std::cout << "Has saddle point: " << matrix.GetMatrix()[maxmin.first][maxmin.second] << std::endl;
		return;
	}
	else
	{
		std::cout << "Hasn`t saddle point. Search solution..." << std::endl;
	}

	Table X, Y;
	double v, e;

	std::tie(X, Y, v) = AnaliticMethodMixedStrategy(matrix);

	std::cout << std::endl << std::setw(17) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::cout << "Analityc method" << std::endl;
	std::cout << std::setw(17) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::cout << "X*:" << std::endl;
	writeTable(X);

	std::cout << "Y*:" << std::endl;
	writeTable(Y);

	std::cout << "v: " << v << std::endl;


	std::cout << std::endl << std::setw(22) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::cout << "Brown-Robinson method" << std::endl;
	std::cout << std::setw(22) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::tie(X, Y, v, e) = BrownRobinsonMethodMixedStrategy(matrix);

	std::cout << "X*:" << std::endl;
	writeTable(X);

	std::cout << "Y*:" << std::endl;
	writeTable(Y);

	std::cout << "v: " << v << std::endl;
	std::cout << "epsilon: " << e << std::endl;
}

void LR2()
{
	double a = -3;
	double b = 2.4;
	double c = 6;
	double d = -0.6;
	double e = -4.8;
	if (a > 0 || b < 0)
	{
		std::cout << "Ñonvex-concave condition is bad!" << std::endl;
		return;
	}

	double x, y, H;

	std::cout << std::endl << std::setw(17) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::cout << "Analityc method" << std::endl;
	std::cout << std::setw(17) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::tie(x, y, H) = AnaliticMethodÑonvexÑoncaveGame(a, b, c, d, e);

	std::cout << "X: " << x << std::endl;
	std::cout << "Y: " << y << std::endl;
	std::cout << "H: " << H << std::endl;


	std::cout << std::endl << std::setw(17) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::cout << "Numeric method" << std::endl;
	std::cout << std::setw(17) << std::setfill('-') << " " << std::setfill(' ') << std::endl;
	std::tie(x, y, H) = NumericMethodÑonvexÑoncaveGame(a, b, c, d, e);

	std::cout << "X: " << x << std::endl;
	std::cout << "Y: " << y << std::endl;
	std::cout << "H: " << H << std::endl;
}

Table GetRandomTable10On10()
{
	Table table;
	std::srand(std::time(0));

	for (size_t i = 0; i < 10; ++i)
	{
		std::vector<double> vec;
		for (size_t j = 0; j < 10; ++j)
		{
			vec.push_back(std::rand() % 20);
		}
		table.push_back(vec);
	}

	return table;
}

void LR3_random()
{
	Matrix matrixA(GetRandomTable10On10());
	Matrix matrixB(GetRandomTable10On10());

	std::cout << "Random matrix:" << std::endl;

	std::vector<std::vector<std::pair<double, double>>> matrix;

	for (size_t i = 0; i < matrixA.GetMatrix().size(); ++i)
	{
		std::vector<std::pair<double, double>> row;
		for (size_t j = 0; j < matrixB.GetMatrix()[i].size(); ++j)
		{
			row.push_back({matrixA.GetMatrix()[i][j], matrixB.GetMatrix()[i][j]});
		}
		matrix.push_back(row);
	}

	writeTable(matrix, GetNashOptimums(matrixA, matrixB), GetParetoOptimums(matrixA, matrixB));
}

void LR3_check()
{
	Table tableA1 = {
		{1, 2},
		{1, 0}
	};
	Table tableB1 = {
		{1, 1},
		{2, 0}
	};

	Table tableA2 = {
		{4, 0},
		{0, 1}
	};
	Table tableB2 = {
		{1, 0},
		{0, 4}
	};

	Table tableA3 = {
		{-5, 0},
		{-10, -1}
	};
	Table tableB3 = {
		{-5, -10},
		{0, -1}
	};

	std::vector<std::pair<std::pair<Table, Table>, std::string>> tuple = {
		{{tableA1, tableB1}, std::string("Perekrestok")},
		{{tableA2, tableB2}, std::string("Semeinyi spor")},
		{{tableA3, tableB3}, std::string("Zakluchennye")}
	};

	for (size_t i = 0; i < 3; ++i)
	{
		Matrix matrixA(tuple[i].first.first);
		Matrix matrixB(tuple[i].first.second);

		std::cout << tuple[i].second << std::endl;

		std::vector<std::vector<std::pair<double, double>>> matrix;

		for (size_t i = 0; i < matrixA.GetMatrix().size(); ++i)
		{
			std::vector<std::pair<double, double>> row;
			for (size_t j = 0; j < matrixA.GetMatrix()[i].size(); ++j)
			{
				row.push_back({matrixA.GetMatrix()[i][j], matrixB.GetMatrix()[i][j]});
			}
			matrix.push_back(row);
		}

		writeTable(matrix, GetNashOptimums(matrixA, matrixB), GetParetoOptimums(matrixA, matrixB));
	}
}


void LR3_with_matrix()
{
	Table tableA = {
		{5, 7},
		{11, 6}
	};

	Matrix matrixA(tableA);

	Table tableB = {
		{8, 4},
		{7, 9}
	};

	Matrix matrixB(tableB);

	std::cout << "Matrix:" << std::endl;

	std::vector<std::vector<std::pair<double, double>>> matrix;

	for (size_t i = 0; i < tableB.size(); ++i)
	{
		std::vector<std::pair<double, double>> row;
		for (size_t j = 0; j < tableB[i].size(); ++j)
		{
			row.push_back({tableA[i][j], tableB[i][j]});
		}
		matrix.push_back(row);
	}

	writeTable(matrix, GetNashOptimums(matrixA, matrixB), GetParetoOptimums(matrixA, matrixB));

	Table u_ = {
		{1, 1}
	};
	Table ut_ = {
		{1},
		{1}
	};
	Matrix u(u_);
	Matrix ut(ut_);

	double v1 = 1/(u * matrixA.GetRevertMatrix() * ut).GetMatrix()[0][0];
	Table Y = (v1*matrixA.GetRevertMatrix()*ut).GetMatrix();
	std::cout << "Y: " << std::endl;
	writeTable(Y);
	std::cout << "v1 = " << v1 << std::endl;

	double v2 = 1/(u * matrixB.GetRevertMatrix() * ut).GetMatrix()[0][0];
	Table X = (v2*u*matrixB.GetRevertMatrix()).GetMatrix();
	std::cout << "X: " << std::endl;
	writeTable(X);
	std::cout << "v2 = " << v2 << std::endl;
}

void LR3()
{
	LR3_check();
	LR3_random();
	LR3_with_matrix();
}

int main()
{
	LR1();
	LR2();
	return 0;
}
