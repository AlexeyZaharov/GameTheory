#include "Matrix.h"

Matrix::Matrix(const Table& table)
{
	matrix = table;
	findMinmaxMaxmin = false;
}

const Table& Matrix::GetMatrix() const
{
	return matrix;
}

const Table& Matrix::GetRevertMatrix() const
{
	if (!revertMatrix.empty())
		return revertMatrix;

	int determinator = GetDeterminator();
	for (auto i = 0; i < matrix.size(); ++i)
	{
		std::vector<double> row;
		for (auto j = 0; j < matrix[i].size(); ++j)
		{
			std::unordered_set<int> erasedRows = { j };
			std::unordered_set<int> erasedColumns = { i };
			row.push_back(std::pow(-1, i + j) * GetDeterminatorOfSubmatrix(erasedRows, erasedColumns) / determinator);
		}
		revertMatrix.push_back(row);
	}

	return revertMatrix;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Matrix::GetMaxminAndMinmax() const
{
	if (findMinmaxMaxmin)
		return { maxmin, minmax };

	for (auto i = 0; i < matrix.size(); ++i)
	{
		int minj = 0;
		for (auto j = 0; j < matrix[i].size(); ++j)
		{
			if (matrix[i][j] < matrix[i][minj])
			{
				minj = j;
			}
		}

		if (i == 0 || matrix[maxmin.first][maxmin.second] < matrix[i][minj])
			maxmin = { i, minj };
	}

	for (auto j = 0; j < matrix[0].size(); ++j)
	{
		int maxi = 0;
		for (auto i = 0; i < matrix.size(); ++i)
		{
			if (matrix[i][j] > matrix[maxi][j])
			{
				maxi = i;
			}
		}

		if (j == 0 || matrix[minmax.first][minmax.second] > matrix[maxi][j])
			minmax = { maxi, j };
	}

	findMinmaxMaxmin = true;
	return { maxmin, minmax };
}

int Matrix::GetDeterminator() const
{
	std::unordered_set<int> erasedRows;
	std::unordered_set<int> erasedColumns;
	return GetDeterminatorOfSubmatrix(erasedRows, erasedColumns);
}

int Matrix::GetDeterminatorOfSubmatrix(std::unordered_set<int>& erasedRows,
									   std::unordered_set<int>& erasedColumns, int currentRow) const
{
	if (erasedRows.size() + 1 == matrix.size())
	{
		int row = 0;
		int column = 0;

		for (auto i = 0; i < matrix.size(); ++row, ++i)
			if (erasedRows.find(row) == erasedRows.end())
				break;

		for (auto j = 0; j < matrix[currentRow].size(); ++column, ++j)
			if (erasedColumns.find(column) == erasedColumns.end())
				break;

		return matrix[row][column];
	}

	int determinator = 0;
	int sign = 1;

	if (erasedRows.find(currentRow) != erasedRows.end())
		return currentRow == 0 ? GetDeterminatorOfSubmatrix(erasedRows, erasedColumns, currentRow + 1) : 0;

	erasedRows.emplace(currentRow);
	for (auto j = 0; j < matrix[currentRow].size(); ++j)
	{
		if (erasedColumns.find(j) != erasedColumns.end())
			continue;

		erasedColumns.emplace(j);
		determinator += sign * matrix[currentRow][j] * GetDeterminatorOfSubmatrix(erasedRows, erasedColumns, currentRow + 1);
		erasedColumns.erase(j);

		sign *= -1;
	}
	erasedRows.erase(currentRow);

	return determinator;
}

Matrix operator*(const Matrix& left, const Matrix& right)
{
	Table matrixProduct;
	for (auto i = 0; i < left.matrix.size(); ++i)
	{
		std::vector<double> rowColumnProduct;
		for (auto j = 0; j < right.matrix[0].size(); ++j)
		{
			double product = 0;
			for (auto k = 0; k < left.matrix[i].size(); ++k)
			{
				product += left.matrix[i][k] * right.matrix[k][j];
			}
			rowColumnProduct.push_back(product);
		}
		matrixProduct.push_back(rowColumnProduct);
	}

	return Matrix(matrixProduct);
}

Matrix operator*(const Matrix& matrix, double num)
{
	Matrix newMatrix = matrix;
	for (auto& row : newMatrix.matrix)
	{
		for (auto& elem : row)
		{
			elem *= num;
		}
	}

	return newMatrix;
}
