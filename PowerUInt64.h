#pragma once

#include <type_traits>

namespace std
{
	template<typename Number>
	concept NumericDataType = std::is_arithmetic_v<Number>;

	template<NumericDataType NumberDataType_ = uint64_t>
	NumberDataType_ PowerUInt64(const NumberDataType_ base, const NumberDataType_ exponent)
	{
		NumberDataType_ result = 1;
		NumberDataType_ base_pow = base;
		NumberDataType_ exp = exponent;
		while (exp > 0)
		{
			if (exp & 1)
			{
				result *= base_pow;
			}
			base_pow *= base_pow;
			exp >>= 1;
		}
		return result;
	};
}
