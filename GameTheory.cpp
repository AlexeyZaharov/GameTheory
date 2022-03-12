#include "Methods.h"
#include "Matrix.h"

#include <iostream>

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

int main()
{
	LR1();
	LR2();
	return 0;
}