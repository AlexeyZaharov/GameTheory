#pragma once
#include "Matrix.h"
#include <tuple>

std::tuple<Table, Table, double> AnaliticMethodMixedStrategy(const Matrix& matrix);
std::tuple<Table, Table, double, double> BrownRobinsonMethodMixedStrategy(const Matrix& matrix, bool debug = true);

std::tuple<double, double, double> AnaliticMethodŃonvexŃoncaveGame(double a, double b, double c, double d, double e);
std::tuple<double, double, double> NumericMethodŃonvexŃoncaveGame(double a, double b, double c, double d, double e);

std::vector<std::pair<size_t, size_t>> GetParetoOptimums(const Matrix& matrixA, const Matrix& matrixB);
std::vector<std::pair<size_t, size_t>> GetNashOptimums(const Matrix& matrixA, const Matrix& matrixB);
