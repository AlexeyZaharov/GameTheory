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

class Matrix
{
public:
	Matrix(const Table& table);

	const Table& GetMatrix() const;
	const Table& GetRevertMatrix() const;

	std::pair<std::pair<int, int>, std::pair<int, int>> GetMaxminAndMinmax() const;

	friend Matrix operator*(const Matrix&, const Matrix&);
	friend Matrix operator*(const Matrix&, double);

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
