#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>
#include <type_traits>
#include <thread>
#include <execution>
#include <stack>

namespace std
{

	/*
	AUTHOR: Seagat2011 https://github.com/Seagat2011
	AUTHOR: AbdulShabazz https://github.com/AbdulShabazz
	DATE: 2023-MAR-13 22:56:48 EDT
	DESCRIPTION: This is a class that implements a 64-bit integer type for x86 processors.
	USAGE: This class is designed to be used in the same way as the standard int64_t type, but with the ability to use the arithmetic operators with the __x86i64Int type of arbitrary size.

		C++ Example:

			std::__x86i64Int _a_{}; 
			std::__x86i64Int a{ 12n };
			//std::__x86i64Int aa{ 12 }; // Error
			std::__x86i64Int aaa{ 0 };
			std::__x86i64Int aaaa{ "12" }; 
			std::__x86i64Int b{ 0 };
			std::__x86i64Int c = std::__x86i64Int{ 12n }; 
			//std::__x86i64Int cc = std::__x86i64Int{ 12 }; // Error
			std::__x86i64Int ccc = std::__x86i64Int{ "12" };
			std::__x86i64Int d = std::__x86i64Int{ 12n };
			std::__x86i64Int e = 12n;
			std::__x86i64Int f(0);
			e = a + b;
			e = c * d;
			e = a / b;
			e = a - b;
			a++;
			++a;
			a--;
			--a;
			e = a % b;
			e = 12n;
			e %= b;
			e = 12n;
			e += b;
			e = 12n;
			e -= b;
			e = 12n;
			e *= b;
			bool g = a < b;
			g;
			g = a > b;
			g;
			g = a <= b;
			g;
			g = a >= b;
			g;
			g = a == b;
			g;
			g = a != b;
			g;
			std::__x86i64Int h = std::__x86i64Int{ a & b };
			h = 12n;
			h &= b;

		STYLEGUIDE: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/
		VERSION: 0.0.1 (Major.Minor.Patch)
	*/

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

	class __x86i64Int
	{
	public:

		__x86i64Int(const std::string& str = "0")
		{
			for (auto it = str.rbegin(); it != str.rend(); ++it)
			{
				if (*it >= '0' && *it <= '9')
				{
					digits_.push_back(*it - '0');
				}
				else if (*it == '-')
				{
					sign_ = -1;
				}
			}
			if (digits_.empty())
			{
				digits_.push_back(0);
			}
			trimLeadingZeros();
		}

		friend std::ostream& operator<<(std::ostream& os, const __x86i64Int& x)
		{
			const std::string str = x.to_string();
			os << str + "n";
			return os;
		}

		std::string& to_string() const
		{
			std::string str;
			for (const int& it : digits_)
			{
				str += char(it + '0');
			}
			return str;
		}

		__x86i64Int& _max(const __x86i64Int& lhs, const __x86i64Int& rhs) const
		{
			__x86i64Int result{ lhs };
			if (rhs > result)
			{
				result = rhs;
			}
			return result;
		}

		__x86i64Int& _min(const __x86i64Int& lhs, const __x86i64Int& rhs) const
		{
			__x86i64Int result{ lhs };
			if (rhs < result)
			{
				result = rhs;
			}
			return result;
		}

		bool _pushFront(__x86i64Int& OutVecUInt64Ref, const __x86i64Int& InConstVecUInt64Ref)
		{
			bool NoErrorsFlag = true;
			for(const int& digit : InConstVecUInt64Ref.digits_)
			{
				if (digit >= 0 && digit < BASE)
				{
					OutVecUInt64Ref.digits_.insert(OutVecUInt64Ref.digits_.begin(), digit);
				}
				else
				{
					NoErrorsFlag = false;
					continue;
				}
			}
			return NoErrorsFlag;
		}

		__x86i64Int& operator-() const
		{
			__x86i64Int result = *this;
			result.sign_ = -result.sign_;
			return result;
		}

		__x86i64Int& operator+(const __x86i64Int& other) const
		{
			if (sign_ == other.sign_)
			{
				__x86i64Int result{ 0 };
				const __x86i64Int& val = _max(__x86i64Int{ std::to_string(digits_.size()) }, __x86i64Int{ std::to_string(other.digits_.size()) });
				result.digits_.resize( val.digits_.size() );
				int carry = 0;
				for (uint64_t i = uint64_t{ 0 }; i < result.digits_.size(); i++)
				{
					int sum = carry;
					if (i < digits_.size())
					{
						sum += digits_[i];
					}
					if (i < other.digits_.size())
					{
						sum += other.digits_[i];
					}
					result.digits_[i] = sum % BASE;
					carry = sum / BASE;
				}
				if (carry > 0)
				{
					result.digits_.push_back(carry);
				}
				result.sign_ = sign_;
				return result;
			}
			else 
			{
				return *this - (-other);
			}
		}

		__x86i64Int& operator-(const __x86i64Int& other) const
		{
			__x86i64Int result{ 0 };
			if (sign_ == other.sign_)
			{
				if (abs() >= other.abs())
				{
					result.digits_.resize(digits_.size());
					int borrow = 0;
					for (uint64_t i = uint64_t{ 0 }; i < result.digits_.size(); i++)
					{
						int diff = borrow + digits_[i];
						if (i < other.digits_.size())
						{
							diff -= other.digits_[i];
						}
						if (diff < 0)
						{
							diff += BASE;
							borrow = -1;
						}
						else
						{
							borrow = 0;
						}
						result.digits_[i] = diff;
					}
					result.trimLeadingZeros();
					result.sign_ = sign_;
					// return result;
				}
				else
				{
					result = -(other - *this); //return -(other - *this);
				}
			}
			else
			{
				result = *this + (-other); // return *this + (-other);
			}
			return result;
		}

		__x86i64Int& operator-=(const __x86i64Int& rhs)
		{
			if (digits_.size() < rhs.digits_.size())
			{
				throw std::invalid_argument("__x86i64Int: -= operation  -- Negative result");
			}
			// Perform subtraction digit by digit
			int borrow = 0;
			for (uint64_t i = uint64_t{ 0 }; i < rhs.digits_.size(); ++i)
			{
				borrow = (digits_[i] < rhs.digits_[i] + borrow) ? 1 : 0;
				digits_[i] = BASE + digits_[i] - rhs.digits_[i] - borrow;
			}
			for (uint64_t i = rhs.digits_.size(); i < digits_.size(); ++i)
			{
				borrow = (digits_[i] < borrow) ? 1 : 0;
				digits_[i] = BASE + digits_[i] - borrow;
			}
			trimLeadingZeros();
			return *this;
		}

		__x86i64Int& operator*(const __x86i64Int& other) const
		{
			auto rowProductVecUInt64 = [](const __x86i64Int& InDigitsVecUInt64Ref,
				const int& InOtherUIntRef,
				__x86i64Int& OutResultVecUInt64Ref)
			{
				int carry = 0;
				const uint64_t& J = OutResultVecUInt64Ref.digits_.size();
				for (uint64_t j = 0; ((j < J) || (carry > 0)); j++)
				{
					int prod = carry + OutResultVecUInt64Ref.digits_[j] + InOtherUIntRef * (j < InDigitsVecUInt64Ref.digits_.size() ? InDigitsVecUInt64Ref.digits_[j] : int{ 0 });
					OutResultVecUInt64Ref.digits_[j] = prod % BASE;
					carry = static_cast<int>(prod / BASE);
				}
			};
			std::vector<__x86i64Int> IntermediateResultVecUInt64;
			std::vector<thread> FusedMultiplyAddThread;
			const uint64_t I = digits_.size();
			const uint64_t II = digits_.size() + other.digits_.size();
			for (uint64_t i = 0; i < I; i++)
			{
				__x86i64Int temp{ 0 };
				temp.digits_.resize(II, 0);
				IntermediateResultVecUInt64.push_back(temp);
				FusedMultiplyAddThread.push_back(
					std::thread(rowProductVecUInt64, 
						std::cref(*this), 
						std::cref(other.digits_[i]),
						std::ref(IntermediateResultVecUInt64[i])));
			}
			for (thread& th : FusedMultiplyAddThread)
			{
				th.join();
			}
			parallelArraySum(IntermediateResultVecUInt64);
			IntermediateResultVecUInt64[0].trimLeadingZeros();
			IntermediateResultVecUInt64[0].sign_ = sign_ * other.sign_;
			return IntermediateResultVecUInt64[0];
		}

		__x86i64Int& operator/ (const __x86i64Int& other) const
		{
			if (other == __x86i64Int{ 0 })
			{
				throw std::invalid_argument("divide by zero");
			}
			__x86i64Int result{ 0 };
			__x86i64Int remainder{ 0 };
			result.digits_.resize(digits_.size());
			for (uint64_t i = digits_.size() - 1; i >= 0; i--)
			{
				remainder = remainder * __x86i64Int{ std::to_string(BASE) } + __x86i64Int{ std::to_string(digits_[i]) };
				int x = 0, y = BASE - 1;
				while (x < y)
				{
					int m = (x + y + 1) / 2;
					if (other * __x86i64Int{ std::to_string(m) } <= remainder)
					{
						x = m;
					}
					else
					{
						y = m - 1;
					}
				}
				result.digits_[i] = x;
				remainder -= other * __x86i64Int{ std::to_string(x) };
			}
			result.trimLeadingZeros();
			result.sign_ = sign_ * other.sign_;
			return result;
		}

		__x86i64Int& abs() const
		{
			__x86i64Int result = *this;
			result.sign_ = 1;
			return result;
		}

		__x86i64Int& operator=(const __x86i64Int& rhs)
		{
			digits_ = rhs.digits_;
			return *this;
		}

		bool operator<(const __x86i64Int& other) const
		{
			bool resultFlag = true;
			if (digits_.size() != other.digits_.size())
			{
				resultFlag = (digits_.size() * sign_ < other.digits_.size() * other.sign_);
			}
			else
			{
				for (uint64_t i = digits_.size() - 1; i >= 0; i--)
				{
					if (digits_[i] != other.digits_[i])
					{
						resultFlag = (digits_[i] * sign_ < other.digits_[i] * other.sign_);
						break;
					}
				}
			}
			return resultFlag;
		}

		__x86i64Int& operator&=(const __x86i64Int& rhs)
		{
			// Perform bitwise AND operation between digits_ and rhs.digits_
			// and store the result in digits_
			const __x86i64Int& tmp = _max(__x86i64Int{ std::to_string(digits_.size()) }, __x86i64Int{ std::to_string(rhs.digits_.size()) });
			const uint64_t& I = tmp.digits_.size();
			digits_.resize(I, 0);
			for (uint64_t i = 0; i < I; ++i)
			{
				digits_[i] &= (i < rhs.digits_.size() ? rhs.digits_[i] : 0);
			}
			trimLeadingZeros();
			return *this;
		}

		friend __x86i64Int& operator&(const __x86i64Int& lhs, const __x86i64Int& rhs)
		{
			__x86i64Int result = lhs;
			result &= rhs;
			return result;
		}

		__x86i64Int& operator%=(const __x86i64Int& rhs)
		{
			if (rhs == __x86i64Int{ 0 })
			{
				throw std::invalid_argument("Division by zero");
			}
			// Compute the remainder using long division algorithm
			__x86i64Int dividend = *this;
			__x86i64Int divisor = rhs.abs();
			__x86i64Int quotient{ 0 };
			__x86i64Int remainder{ 0 };
			const uint64_t I = uint64_t{ dividend.digits_.size() };
			for (uint64_t i = I; i > 0; --i)
			{
				remainder *= __x86i64Int{ std::to_string(BASE) };
				remainder += __x86i64Int{ std::to_string(dividend.digits_[i - 1]) };
				_pushFront(quotient, remainder / divisor);
				remainder %= divisor;
			}
			std::reverse(quotient.digits_.begin(), quotient.digits_.end());
			*this = remainder;
			return *this;
		}

		friend __x86i64Int operator%(const __x86i64Int& lhs, const __x86i64Int& rhs)
		{
			__x86i64Int result = lhs;
			result %= rhs;
			return result;
		}

		bool operator<=(const __x86i64Int& other) const
		{
			return *this < other || *this == other;
		}

		bool operator>(const __x86i64Int& other) const
		{
			return !(*this <= other);
		}

		bool operator>=(const __x86i64Int& other) const
		{
			return !(*this < other);
		}

		bool operator==(const __x86i64Int& other) const
		{
			return (sign_ == other.sign_ && digits_ == other.digits_);
		}

		bool operator!=(const __x86i64Int& other) const
		{
			return !(*this == other);
		}

		__x86i64Int& operator*=(const __x86i64Int& other)
		{
			*this = *this * other;
			return *this;
		}

		__x86i64Int& operator+=(const __x86i64Int& rhs)
		{
			// Perform addition digit by digit
			int carry = 0;
			const uint64_t& I = rhs.digits_.size();
			for (
				uint64_t i = uint64_t{ 0 }; 
				__x86i64Int{ std::to_string( i ) } < 
					_max(__x86i64Int{ std::to_string(digits_.size()) }, 
						__x86i64Int{ std::to_string(I) }); 
				++i)
			{
				if (i < digits_.size())
				{
					carry += digits_[i];
				}
				if (i < rhs.digits_.size())
				{
					carry += rhs.digits_[i];
				}
				if (i < digits_.size())
				{
					digits_[i] = carry % BASE;
				}
				else
				{
					digits_.push_back(carry % BASE);
				}
				carry /= BASE;
			}
			if (carry > 0)
			{
				digits_.push_back(carry);
			}
			return *this;
		}

		__x86i64Int& operator++(int) // prefix inc
		{
			__x86i64Int temp(*this);
			++temp;
			return temp;
		}

		__x86i64Int& operator++() // postfix inc
		{
			__x86i64Int temp(*this);
			temp += __x86i64Int{ "1" };
			return temp;
		}

		__x86i64Int& operator--(int) // prefix dec
		{
			__x86i64Int temp(*this);
			--(*this);
			return temp;
		}

		__x86i64Int& operator--() // postfix dec
		{
			*this -= __x86i64Int{ "1" };
			return *this;
		}

	private:

		bool parallelArraySum(const std::vector<__x86i64Int>& input) const
		{
			auto parallelSum = [&](const std::vector<__x86i64Int>& input,
				std::vector<__x86i64Int>& output,
				const uint64_t& start,
				const uint64_t& end)
			{
				std::stack<std::tuple<uint64_t, uint64_t>> tasks;
				tasks.emplace(start, end);
				while (!tasks.empty()) {
					auto [start_i, end_i] = tasks.top();
					tasks.pop();
					if (start_i == end_i)
					{
						output[start_i] = input[start_i];
						continue;
					}
					uint64_t mid = (start_i + end_i) / 2;
					std::vector<__x86i64Int> left_output(mid - start_i + 1);
					std::vector<__x86i64Int> right_output(end_i - mid);
					tasks.emplace(mid + 1, end_i);
					tasks.emplace(start_i, mid);
					std::thread left_thread([&]() {
							for (uint64_t i = start_i; i <= mid; ++i)
							{
								left_output[i - start_i] = input[i];
							}
							std::partial_sum(left_output.begin(), left_output.end(), left_output.begin());
						});

					std::thread right_thread([&]() {
							for (uint64_t i = mid + 1; i <= end_i; ++i)
							{
								right_output[i - mid - 1] = input[i];
							}
							std::partial_sum(right_output.begin(), right_output.end(), right_output.begin());
						});
					left_thread.join();
					right_thread.join();
					std::merge(left_output.begin(), left_output.end(),
						right_output.begin(), right_output.end(),
						output.begin() + start_i);
					if (start_i != 0)
					{
						output[start_i - 1] += output[end_i];
					}
				}
			};
			std::vector<__x86i64Int> output{ 0 };
			output.resize(input.size());
			const uint64_t& I = output.size();
			for (uint64_t i = 0; i < I; ++i)
			{
				output[i] = __x86i64Int{ 0 };
			}
			parallelSum(input, output, uint64_t{ 0 }, input.size() - 1);
			return true;
		}

		void trimLeadingZeros()
		{
			while (digits_.size() > 1 && digits_.back() == 0)
			{
				digits_.pop_back();
			}
			if (digits_.size() == 1 && digits_[0] == 0)
			{
				sign_ = 1;
			}
		}

		int sign_ = 1;

		uint64_t decimal_ = uint64_t{ 0 };

		std::vector<int> digits_;

		static const uint64_t BASE = 10;
	};
}

std::__x86i64Int operator"" n(const char* i64Str)
{
	return std::__x86i64Int{ i64Str };
}