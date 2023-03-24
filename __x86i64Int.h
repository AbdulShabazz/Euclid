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
#include <utility>
#include <future>
#include <functional>
#include <utility>
#include <exception>
#include <sstream>
#include "PowerUInt64.h"

namespace std
{

	/*
	DATE: 2023-MAR-13 22:56:48 EDT
	DESCRIPTION: This class implements 64-bit integer types of arbitrary length for x86 64-bit processors.
	USAGE: This class is designed to be used in the same way as a standard uint64_t type, but with the added ability to use the arithmetic operators on __x86i64Int data types having arbitrary size.
	STYLEGUIDE: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/
	VERSION: 0.0.1 (Major.Minor.Patch)
	*/
	class __x86i64Int
	{
	public:

		// Constructor
		__x86i64Int(const std::string& str = "0")
		{
			uint64_t tmp = 0;
			uint64_t digit_count = 0;
			for (auto it = str.rbegin(); it != str.rend(); ++it)
			{
				if (std::isdigit(*it))
				{
					tmp = tmp * 10 + (*it - '0');
					digit_count++;

					if (digit_count == 9)
					{
						digits_.push_back(tmp);
						tmp = 0;
						digit_count = 0;
					}
				}
				else if (*it == '-')
				{
					sign_ = -1;
				}
			}
			//std::cout << "tmp: " << tmp << std::endl;
			if (tmp != 0 || digits_.empty())
			{
				digits_.push_back(tmp);
			}
			trimLeadingZeros();
			//std::cout << "digits_[0]: " << digits_[0] << std::endl;
		}

		// Get the number
		std::vector<uint64_t> GetDigits() const
		{
			return digits_;
		}

		friend std::ostream& operator<< (std::ostream& os, const __x86i64Int& value)
		{
			const std::string str = value.to_string();
			os << str;
			return os;
		}
		
		std::string to_string() const
		{
			std::string value{};
			for (const auto& num : digits_)
			{
				std::string temp = std::to_string(num);
				std::reverse(temp.begin(), temp.end());
				value += temp;
			}
			return value + "n";
		}

		__x86i64Int _max(const __x86i64Int& lhs, const __x86i64Int& rhs) const
		{
			__x86i64Int result{ lhs };
			if (rhs > result)
			{
				result = rhs;
			}
			return result;
		}

		__x86i64Int _min(const __x86i64Int& lhs, const __x86i64Int& rhs) const
		{
			__x86i64Int result{ lhs };
			if (rhs < result)
			{
				result = rhs;
			}
			return result;
		}

		bool PushAtFront(__x86i64Int& OutVecUInt64Ref, const __x86i64Int& InConstVecUInt64Ref) const
		{
			bool NoErrorsFlag = true;
			for(const int& digit : InConstVecUInt64Ref.digits_)
			{
				if (digit > -1 && digit < BASE)
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

		__x86i64Int operator- () const
		{
			__x86i64Int result = *this;
			result.sign_ = -result.sign_;
			return result;
		}

		virtual __x86i64Int operator+ (const __x86i64Int& rhs) const
		{
			__x86i64Int result{};
			if (sign_ == rhs.sign_)
			{
				result.sign_ = sign_;
				uint64_t carry = 0;
				uint64_t maxSize = (((digits_.size()) > (rhs.digits_.size())) ? (digits_.size()) : (rhs.digits_.size()));
				for (uint64_t i = 0; i < maxSize || carry; ++i)
				{
					uint64_t sum = carry +
						(i < digits_.size() ? digits_[i] : 0) +
						(i < rhs.digits_.size() ? rhs.digits_[i] : 0);
					result.digits_.push_back(sum % BASE);
					carry = sum / BASE;
					//std::cout << "Digit: " << sum % BASE << std::endl;
					//std::cout << "Carry: " << carry << std::endl;
				}
			}
			else
			{
				result = *this - (-rhs);
			}
			result.trimLeadingZeros();
			return result;
		}

		virtual __x86i64Int operator- (const __x86i64Int& rhs) const
		{
			__x86i64Int result{ "0" };
			__x86i64Int minuend = *this;
			__x86i64Int subtrahend = rhs;
			bool RHShasLargerMagnitudeFlag = (abs() >= rhs.abs());
			bool RHShasLargerMatchingMagnitudeFlag = ((abs() >= rhs.abs()) && (sign_ == rhs.sign_));
			if (RHShasLargerMagnitudeFlag)
			{
				__x86i64Int minuend = rhs;
				__x86i64Int subtrahend = *this;
				result.digits_.resize(rhs.digits_.size());
			}
			else
			{
				result.digits_.resize(digits_.size());
			}
			if (RHShasLargerMagnitudeFlag || RHShasLargerMatchingMagnitudeFlag)
			{
				result.sign_ = rhs.sign_;
			}
			else if(!RHShasLargerMagnitudeFlag)
			{
				result.sign_ = sign_;
			}
			int borrow = 0;
			for (uint64_t i = uint64_t{ 0 }; i < result.digits_.size(); i++)
			{
				int64_t diff = borrow + minuend.digits_[i];
				if (i < subtrahend.digits_.size())
				{
					diff -= subtrahend.digits_[i];
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
			return result;
		}

		__x86i64Int operator-= (const __x86i64Int& rhs)
		{
			__x86i64Int result{ "0" };
			if (digits_.size() < rhs.digits_.size())
			{
				throw std::invalid_argument("__x86i64Int: -= operation  -- Negative result");
			}
			// Perform subtraction digit by digit
			uint64_t borrow = 0;
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

			result = *this;
			return result;
		}

		__x86i64Int abs() const
		{
			__x86i64Int result = *this;
			result.sign_ = 1;
			return result;
		}

		virtual __x86i64Int operator= (const __x86i64Int& rhs)
		{
			digits_ = rhs.digits_;
			__x86i64Int result = *this;
			return result;
		}

		virtual bool operator< (const __x86i64Int& rhs) const
		{
			bool resultFlag = true;
			if (digits_.size() != rhs.digits_.size())
			{
				resultFlag = (digits_.size() * sign_ < rhs.digits_.size() * rhs.sign_);
			}
			else
			{
				for (int64_t i = digits_.size() - 1; i >= 0; i--)
				{
					if (digits_[i] != rhs.digits_[i])
					{
						resultFlag = (digits_[i] * sign_ < rhs.digits_[i] * rhs.sign_);
						break;
					}
				}
			}
			return resultFlag;
		}

		__x86i64Int operator&= (const __x86i64Int& rhs)
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
			__x86i64Int result = *this;
			return result;
		}

		friend __x86i64Int operator& (const __x86i64Int& lhs, const __x86i64Int& rhs)
		{
			__x86i64Int result = lhs;
			result &= rhs;
			return result;
		}

		__x86i64Int operator%= (const __x86i64Int& rhs)
		{
			if (rhs == __x86i64Int{ "0" })
			{
				throw std::invalid_argument("Division by zero");
			}
			// Compute the remainder using long division algorithm
			const __x86i64Int quotient{ *this }; 
			const int& sign = (*this).sign_* rhs.sign_;
			const __x86i64Int divisor = rhs.abs();
			__x86i64Int result = quotient / divisor;
			result.sign_ = sign;
			*this = result;
			(*this).digits_ = result.remainder_;
			return result.remainder();
		}

		friend __x86i64Int operator% (const __x86i64Int& lhs, const __x86i64Int& rhs)
		{
			__x86i64Int result = lhs;
			result %= rhs;
			return result;
		}

		bool operator<= (const __x86i64Int& rhs) const
		{
			return *this < rhs || *this == rhs;
		}

		bool operator> (const __x86i64Int& rhs) const
		{
			return !(*this <= rhs);
		}

		bool operator>= (const __x86i64Int& rhs) const
		{
			return !(*this < rhs);
		}

		bool operator== (const __x86i64Int& rhs) const
		{
			return (sign_ == rhs.sign_ && digits_ == rhs.digits_);
		}

		bool operator!= (const __x86i64Int& rhs) const
		{
			return !(*this == rhs);
		}

		__x86i64Int operator*= (const __x86i64Int& rhs)
		{
			__x86i64Int result = *this = *this * rhs;
			return result;
		}

		__x86i64Int& operator+=(const __x86i64Int& rhs)
		{
			// Perform addition digit by digit
			uint64_t carry = 0;
			uint64_t maxLength = std::max(digits_.size(), rhs.digits_.size());
			for (uint64_t i = 0; i < maxLength; ++i)
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
			trimLeadingZeros();
			return *this;
		}

		__x86i64Int operator++ (int) // prefix inc
		{
			__x86i64Int result{ *this };
			++result;
			return result;
		}

		__x86i64Int operator++ () // postfix inc
		{
			__x86i64Int result(*this);
			result += __x86i64Int{ "1" };
			return result;
		}

		__x86i64Int operator-- (int) // prefix dec
		{
			__x86i64Int result(*this);
			--(*this);
			return result;
		}

		__x86i64Int operator-- () // postfix dec
		{
			*this -= __x86i64Int{ "1" };
			__x86i64Int result{ *this };
			return result;
		}

		virtual __x86i64Int operator* (const __x86i64Int& rhs) const
		{
			__x86i64Int result;
			// Resize the result vector to fit the result
			result.digits_.resize(digits_.size() + rhs.digits_.size());
			// Perform multiplication digit by digit
			for (uint64_t i = 0; i < digits_.size(); ++i)
			{
				uint64_t carry = 0;
				for (uint64_t j = 0; j < rhs.digits_.size() || carry; ++j)
				{
					uint64_t product = result.digits_[i + j] + carry +
					(j < rhs.digits_.size()) ? digits_[i] * rhs.digits_[j] : 0ull;
					result.digits_[i + j] = product % BASE;
					carry = product / BASE;
				}
			}
			// Preserve the sign of the result
			result.sign_ = sign_ * rhs.sign_;
			// Remove leading zeros
			result.trimLeadingZeros();
			return result;
		}

		virtual __x86i64Int operator/ (const __x86i64Int& rhs) const
		{
			if (rhs == __x86i64Int{ "0" })
			{
				throw std::invalid_argument("divide by zero");
			}
			__x86i64Int quotient{ "0" };
			__x86i64Int remainder{ *this };
			while (remainder >= rhs)
			{
				remainder -= rhs;
				++quotient;
			}
			quotient.sign_ = sign_ * rhs.sign_;
			__x86i64Int result = quotient;
			result.sign_ = remainder.sign_ * rhs.sign_;
			result.quotient_ = quotient.digits_;
			result.remainder_ = remainder.digits_;
			return result;
		}

		__x86i64Int operator/= (const __x86i64Int& rhs)
		{
			*this = *this / rhs;
			return *this;
		}

		__x86i64Int _clone(uint64_t& InParamUInt64, const uint64_t& InParam2UInt64) const
		{
			if (InParamUInt64 < 0 || InParam2UInt64 < 0)
			{
				throw std::runtime_error("Index received is less than 0");
			}
			if (InParamUInt64 > InParam2UInt64)
			{
				throw std::runtime_error("Startindex exceeds Endindex");
			}
			if (InParamUInt64 >= digits_.size())
			{
				throw std::runtime_error("Startindex exceeds length of number");
			}
			else if (InParam2UInt64 >= digits_.size())
			{
				 throw std::runtime_error("Endindex exceeds length of number");
			}
			uint64_t& StartIndexInt64 = InParamUInt64;
			const uint64_t& EndIndexInt64 = InParam2UInt64;
			__x86i64Int ret{ "0" };
			ret.digits_.resize(EndIndexInt64 - StartIndexInt64 + 1);
			for (uint64_t& val : ret.digits_)
			{
				val = digits_[StartIndexInt64++];
			}
			return ret;
		}

		__x86i64Int GetNumberSubrange(uint64_t start_index, const uint64_t& length) const
		{
			__x86i64Int SubrangeSliceUInt64{ "0" };
			if (start_index >= digits_.size()) 
			{
				// NOP //
			}
			else
			{
				const uint64_t I = (start_index + length) < digits_.size() ? start_index + length : digits_.size();
				for (uint64_t i = start_index; i < I; i++)
				{
					SubrangeSliceUInt64.digits_.push_back(digits_[i]);
				}
			}
			return SubrangeSliceUInt64;
		}

		bool SetNumberSubrange(uint64_t start_index, const __x86i64Int& InConstUInt64ObjRef)
		{
			bool RangeSetFlag = false;
			if (start_index >= digits_.size())
			{
				throw std::runtime_error("Start index greater than length of number");
			}
			if (start_index + InConstUInt64ObjRef.digits_.size() > digits_.size())
			{
				throw std::runtime_error("Start index, plus length of number to replace in subrange, exceeds length of number");
			}
			for (const uint64_t& val : InConstUInt64ObjRef.digits_)
			{
				digits_[start_index++] = val;
			}
			RangeSetFlag = true;
			return RangeSetFlag;
		}

		// Conversion operator to std::uint64_t for
		// use as a literal index in array elements
		operator uint64_t () const
		{
			uint64_t i = 0;
			uint64_t ret = 0;
			for (auto val = digits_.rbegin(); val != digits_.rend(); ++val)
			{
				ret += *val * PowerUInt64<uint64_t>(BASE, i++);
			}
			return ret;
		}

		__x86i64Int quotient() const
		{
			__x86i64Int result = *this;
			result.digits_ = quotient_;
			return result;
		}

		__x86i64Int remainder() const
		{
			__x86i64Int result = *this;
			result.digits_ = remainder_;
			return result;
		}

	private:
		__x86i64Int DivideByDigit(const uint64_t& divisor) const
		{
			if (divisor > 9 || divisor < 1)
			{
				throw std::runtime_error("Invalid single-digit divisor for single-digit division.");
			}
			uint64_t remainder = 0;
			__x86i64Int quotient{ "0" };
			for (const int& val : digits_)
			{
				uint64_t temp = remainder * 10 + val;
				__x86i64Int iDividend = __x86i64Int{ std::to_string(temp) };
				__x86i64Int iDivisor = __x86i64Int{ std::to_string(divisor) };
				const __x86i64Int& result = iDividend / iDivisor;
				PushAtFront(quotient, result.quotient());
				remainder = temp % divisor;
			}
			quotient.trimLeadingZeros();
			__x86i64Int result{ "0" };
			result.digits_ = quotient.digits_;
			result.quotient_ = quotient.digits_;
			result.remainder_ = __x86i64Int{ std::to_string(remainder) }.digits_;
			return result;
		}

		bool SumArrayRefInParallel(const std::vector<__x86i64Int>& input) const
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
				output[i] = __x86i64Int{ "0" };
			}
			parallelSum(input, output, uint64_t{ 0 }, input.size() - 1);
			return true;
		}

		void trimLeadingZeros()
		{
			for (auto val = digits_.rbegin(); val != digits_.rend() && *val == 0; val++)
			{
				digits_.pop_back();
			}
			if (digits_.size() == 1 && digits_[0] == 0)
			{
				sign_ = 1;
			}
		}

		int sign_ = 1;

		uint64_t decimalPointAt_ = uint64_t{ 0 };

		std::vector<uint64_t> digits_;
		std::vector<uint64_t> quotient_;
		std::vector<uint64_t> remainder_;

		// Set to 1/2 the full uin64_t resolution to prevent multiplication overflow
		static const uint64_t BASE = 1e9;/*10*/;
	};
}

std::__x86i64Int operator"" n(const char* i64Str)
{
	return std::__x86i64Int{ i64Str };
}