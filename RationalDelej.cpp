#include "RationalDelej.h"

#include <iostream>

bool IsSuperAdditive(const std::unordered_map<std::bitset<4>, int>& haracteristicalFunction)
{
	std::bitset<4> ZERO(0);
	for (std::size_t i = 1; i < haracteristicalFunction.size(); ++i)
	{
		std::bitset<4> S(i);
		for (std::size_t j = i + 1; j < haracteristicalFunction.size(); ++j)
		{
			std::bitset<4> T(j);

			if ((S & T) == ZERO)
			{
				int SWin = haracteristicalFunction.at(S);
				int TWin = haracteristicalFunction.at(T);
				int SOrTWin = haracteristicalFunction.at(S | T);
				if (SOrTWin < SWin + TWin)
				{
					std::cout << "v({" << (S | T).to_string() << "}) < " << "v({" << S.to_string() << "}) + v({" << T.to_string() << "})" << std::endl;
					std::cout << SOrTWin << " < " << SWin << " + " << TWin << std::endl;
					return false;
				}
				std::cout << "v({" << (S | T).to_string() << "}) >= " << "v({" << S.to_string() << "}) + v({" << T.to_string() << "})" << std::endl;
				std::cout << SOrTWin << " >= " << SWin << " + " << TWin << std::endl;
			}
		}
	}

	return true;
}

bool IsConvex(const std::unordered_map<std::bitset<4>, int>& haracteristicalFunction)
{
	std::bitset<4> ZERO(0);
	for (std::size_t i = 1; i < haracteristicalFunction.size(); ++i)
	{
		std::bitset<4> S(i);
		for (std::size_t j = i + 1; j < haracteristicalFunction.size(); ++j)
		{
			std::bitset<4> T(j);

			int SWin = haracteristicalFunction.at(S);
			int TWin = haracteristicalFunction.at(T);
			int SOrTWin = haracteristicalFunction.at(S | T);
			int SAndTWin = haracteristicalFunction.at(S & T);
			if (SAndTWin + SOrTWin < SWin + TWin)
			{
				std::cout << "v({" << (S & T).to_string() << "}) + " << "v({" << (S | T).to_string() << "}) < " << "v({" << S.to_string() << "}) + v({" << T.to_string() << "})" << std::endl;
				std::cout << SAndTWin << " + " << SOrTWin << " < " << SWin << " + " << TWin << std::endl;
				return false;
			}
			std::cout << "v({" << (S & T).to_string() << "}) + " << "v({" << (S | T).to_string() << "}) >= " << "v({" << S.to_string() << "}) + v({" << T.to_string() << "})" << std::endl;
			std::cout << SAndTWin << " + " << SOrTWin << " >= " << SWin << " + " << TWin << std::endl;
		}
	}

	return true;
}

std::vector<double> GetShepliVector(const std::unordered_map<std::bitset<4>, int>& haracteristicalFunction)
{
	static std::unordered_map<size_t, size_t> FACTORIALS = {
		{0, 1},
		{1, 1},
		{2, 2},
		{3, 6},
		{4, 24}
	};
	std::bitset<4> ZERO(0);

	std::vector<double> Shepli;
	for (int i = 8; i > 0; i >>= 1)
	{
		std::bitset<4> gamer(i);
		double sum = 0;
		for (size_t j = 0; j < haracteristicalFunction.size(); ++j)
		{
			std::bitset<4> T(j);
			if ((gamer & T) != ZERO)
			{
				sum += FACTORIALS[T.count() - 1] * FACTORIALS[4 - T.count()] * (haracteristicalFunction.at(T) - haracteristicalFunction.at(T ^ gamer));
			}
		}

		Shepli.push_back(sum / FACTORIALS[4]);
	}

	return Shepli;
}

std::vector<double> GetRationalDelej(const std::unordered_map<std::bitset<4>, int>& haracteristicalFunction)
{
	if (IsSuperAdditive(haracteristicalFunction))
	{
		std::cout << "Game is superadditive" << std::endl << std::endl;
	}
	else
	{
		std::cout << "Game is not superadditive" << std::endl << std::endl;
	}

	if (IsConvex(haracteristicalFunction))
	{
		std::cout << "Game is convex" << std::endl << std::endl;
	}
	else
	{
		std::cout << "Game is not convex" << std::endl << std::endl;
	}

	return GetShepliVector(haracteristicalFunction);
}