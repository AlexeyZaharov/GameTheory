#pragma once
#include "Matrix.h"
#include <tuple>

std::tuple<Table, Table, double> AnaliticMethodMixedStrategy(const Matrix& matrix);
std::tuple<Table, Table, double, double> BrownRobinsonMethodMixedStrategy(const Matrix& matrix, bool debug = true);

std::tuple<double, double, double> AnaliticMethod—onvex—oncaveGame(double a, double b, double c, double d, double e);
std::tuple<double, double, double> NumericMethod—onvex—oncaveGame(double a, double b, double c, double d, double e);