#pragma once
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

using Table = std::vector<std::vector<double>>;

template <typename T>
void writeTable(const std::vector<std::vector<T>>& table)
{
	for (auto i = 0; i < table.size(); ++i)
	{
		for (auto j = 0; j < table[i].size() + 1; ++j)
		{
			if (j == 0)
				std::cout << '|';
			else
				std::cout << std::setw(8) << std::setprecision(2) << std::fixed << table[i][j - 1] << '|';
		}
		std::cout << std::endl << ' ' << std::setw(9 * table[i].size()) << std::setfill('-') << ' ' << std::endl << std::setfill(' ') << std::endl;
	}
}

void writeTable(const std::vector<std::vector<std::pair<double, double>>>& table, const std::vector<std::pair<size_t, size_t>>& NashOptimums, const std::vector<std::pair<size_t, size_t>>& ParetoOptimums)
{
	for (size_t i = 0; i < table.size(); ++i)
	{
		for (size_t j = 0; j < table[i].size() + 1; ++j)
		{
			std::pair<size_t, size_t> point = {i, j-1};
			std::string color = "";
			if (j == 0)
				std::cout << '|';
			else
			{
				if (std::find(NashOptimums.begin(), NashOptimums.end(), point) != NashOptimums.end() && std::find(ParetoOptimums.begin(), ParetoOptimums.end(), point) != ParetoOptimums.end())
				{
					color = "\033[31m";//system("Color 0A"); // RED
				}
				else if (std::find(NashOptimums.begin(), NashOptimums.end(), point) != NashOptimums.end())
				{
					color = "\033[32m";//system("Color 0B"); // GREEN
				}
				else if (std::find(ParetoOptimums.begin(), ParetoOptimums.end(), point) != ParetoOptimums.end())
				{
					color = "\033[34m";//system("Color 0E"); // BLUE
				}
				else
				{
					color = "\033[37m";//system("Color 0F"); // WHITE
				}
				std::cout << color << std::setw(2) << '(' << std::setprecision(2) << std::fixed << std::setw(6) << table[i][j - 1].first << ", " << std::setw(6) << table[i][j - 1].second << std::setw(2) << ')' << " |";
			}
		}
		std::cout << std::endl << ' ' << std::setw(20 * table[i].size()) << std::setfill('-') << ' ' << std::endl << std::setfill(' ') << std::endl;
	}
}

class Matrix
{
public:
	Matrix(const Table& table);

	const Table& GetMatrix() const;
	const Table& GetRevertMatrix() const;

	std::pair<std::pair<int, int>, std::pair<int, int>> GetMaxminAndMinmax() const;

	friend Matrix operator*(const Matrix&, const Matrix&);
	friend Matrix operator*(const Matrix&, double);
	friend Matrix operator*(double num, const Matrix& matrix);

private:
	int GetDeterminator() const;

	int GetDeterminatorOfSubmatrix(std::unordered_set<int>& erasedRows,
								   std::unordered_set<int>& erasedColumns, int currentRow = 0) const;

	Table matrix;
	mutable Table revertMatrix;

	mutable std::pair<int, int> minmax;
	mutable std::pair<int, int> maxmin;
	mutable bool findMinmaxMaxmin;
};
