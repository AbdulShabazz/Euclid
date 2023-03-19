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
#include "PowerUInt64.h"

namespace std
{

	/*
	DATE: 2023-MAR-13 22:56:48 EDT
	DESCRIPTION: This class implements 64-bit integer types of arbitrary length for x86 64-bit processors.
	USAGE: This class is designed to be used in the same way as the standard int64_t type, but with the added ability to use the arithmetic operators with the __x86i64Int data type having arbitrary size.
	STYLEGUIDE: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/
	VERSION: 0.0.1 (Major.Minor.Patch)
	*/

	class __x86i64Int
	{
	public:

		// Constructors
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

		// Get the number
		std::vector<int> GetDigits() const
		{
			return digits_;
		}

		friend std::ostream& operator<<(std::ostream& os, const __x86i64Int& x)
		{
			const std::string str = x.to_string();
			os << str + "n";
			return os;
		}

		std::string to_string() const
		{
			std::string str;
			for (const int& it : digits_)
			{
				str += char(it + '0');
			}
			return std::move(str);
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

		__x86i64Int operator+ (const __x86i64Int& other) const
		{
			__x86i64Int result{ "0" };
			if (sign_ == other.sign_)
			{
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
			}
			else 
			{
				result = *this - (-other);
			}
			return result;
		}

		__x86i64Int operator- (const __x86i64Int& other) const
		{
			__x86i64Int result{ "0" };
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

		__x86i64Int operator-= (const __x86i64Int& rhs)
		{
			__x86i64Int result{ "0" };
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

			result = *this;
			return result;
		}

		__x86i64Int operator* (const __x86i64Int& other) const
		{
			auto rowProductVecUInt64 = [](const __x86i64Int& InDigitsVecUInt64Ref,
				const int& InOtherUIntRef,
				__x86i64Int& OutResultVecUInt64Ref)
			{
				int carry = 0;
				const uint64_t& J = OutResultVecUInt64Ref.digits_.size();
				for (uint64_t j = 0; ((j < J) || (carry > 0)); j++)
				{
					int prod = carry +
						OutResultVecUInt64Ref.digits_[j] +
						InOtherUIntRef *
						(j < InDigitsVecUInt64Ref.digits_.size() ?
							InDigitsVecUInt64Ref.digits_[j] : int{ 0 });
					OutResultVecUInt64Ref.digits_[j] = prod % BASE;
					carry = static_cast<int>(prod / BASE);
				}
			};

			std::vector<__x86i64Int> IntermediateResultVecUInt64;
			const uint64_t I = digits_.size();
			const uint64_t II = digits_.size() + other.digits_.size();

			// Initialize thread pool with a fixed number of threads
			const size_t MaxConcurrentThreads = std::thread::hardware_concurrency();
			bool useThreadPoolFlag = (MaxConcurrentThreads > 2U);
			const size_t numThreads = (useThreadPoolFlag ? MaxConcurrentThreads : 2U) - 1;
			std::vector<std::future<void>> futures;

			for (uint64_t i = 0; i < I; i++)
			{
				__x86i64Int temp{ "0" };
				temp.digits_.resize(II, 0);
				IntermediateResultVecUInt64.push_back(temp);
			}

			auto processChunk = [&](uint64_t start, uint64_t end)
			{
				for (uint64_t i = start; i < end; i++)
				{
					rowProductVecUInt64(*this, other.digits_[i], IntermediateResultVecUInt64[i]);
				}
			};

			for (size_t i = 0; i < numThreads; ++i)
			{
				uint64_t startIndex = (I * i) / numThreads;
				uint64_t endIndex = (I * (i + 1)) / numThreads;
				futures.push_back(std::async(std::launch::async, processChunk, startIndex, endIndex));
			}

			for (auto& future : futures)
			{
				future.wait();
			}

			SumArrayRefInParallel(IntermediateResultVecUInt64);
			IntermediateResultVecUInt64[0].trimLeadingZeros();
			IntermediateResultVecUInt64[0].sign_ = sign_ * other.sign_;
			return std::move(IntermediateResultVecUInt64[0]);
		}

		__x86i64Int operator/ (const __x86i64Int& other) const
		{
			if (other == __x86i64Int{ "0" })
			{
				throw std::invalid_argument("divide by zero");
			}
			__x86i64Int result{ "0" };
			__x86i64Int remainder{ "0" };
			result.digits_.resize(digits_.size());
			for (int64_t i = digits_.size() - 1; i >= 0; i--)
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

		__x86i64Int abs() const
		{
			__x86i64Int result = *this;
			result.sign_ = 1;
			return result;
		}

		__x86i64Int operator= (const __x86i64Int& rhs)
		{
			digits_ = rhs.digits_;
			__x86i64Int result = *this;
			return result;
		}

		bool operator< (const __x86i64Int& other) const
		{
			bool resultFlag = true;
			if (digits_.size() != other.digits_.size())
			{
				resultFlag = (digits_.size() * sign_ < other.digits_.size() * other.sign_);
			}
			else
			{
				for (int64_t i = digits_.size() - 1; i >= 0; i--)
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

		friend __x86i64Int operator&(const __x86i64Int& lhs, const __x86i64Int& rhs)
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
			__x86i64Int dividend = *this;
			__x86i64Int divisor = rhs.abs();
			__x86i64Int quotient{ "0" };
			__x86i64Int remainder{ "0" };
			const int64_t I = static_cast<int64_t>(dividend.digits_.size() - 1);
			for (int64_t i = I; i > 0; i--)
			{
				remainder *= __x86i64Int{ std::to_string(BASE) };
				remainder += __x86i64Int{ std::to_string(dividend.digits_[i - 1]) };
				const std::pair<__x86i64Int, __x86i64Int>& result = remainder / divisor;
				PushAtFront(quotient, result.first);
				remainder %= divisor;
			}
			std::reverse(quotient.digits_.begin(), quotient.digits_.end());
			__x86i64Int result = *this = remainder;
			return result;
		}

		friend __x86i64Int operator% (const __x86i64Int& lhs, const __x86i64Int& rhs)
		{
			__x86i64Int result = lhs;
			result %= rhs;
			return result;
		}

		bool operator<= (const __x86i64Int& other) const
		{
			return *this < other || *this == other;
		}

		bool operator> (const __x86i64Int& other) const
		{
			return !(*this <= other);
		}

		bool operator>= (const __x86i64Int& other) const
		{
			return !(*this < other);
		}

		bool operator== (const __x86i64Int& other) const
		{
			return (sign_ == other.sign_ && digits_ == other.digits_);
		}

		bool operator!= (const __x86i64Int& other) const
		{
			return !(*this == other);
		}

		__x86i64Int operator*= (const __x86i64Int& other)
		{
			__x86i64Int result = *this = *this * other;
			return result;
		}

		__x86i64Int operator+= (const __x86i64Int& rhs)
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
			__x86i64Int result = *this;
			return result;
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

		std::pair<__x86i64Int, __x86i64Int>&& operator/= (const __x86i64Int& divisor)
		{
			__x86i64Int quotient{ *this };
			return std::move(quotient / divisor);
		}

		/**
		Example:
		__x86i64Int dividend{ 12n }, divisor{ 12n };
		std::pair<__x86i64Int, __x86i64Int> result = dividend / divisor;
		__x86i64Int quotient = result.first;
		__x86i64Int remainder = result.second;
		*/
		std::pair<__x86i64Int, __x86i64Int> operator/ (const __x86i64Int& divisor)
		{
			// Knuth implementation...

			__x86i64Int dividend{ *this };
			__x86i64Int quotient{};
			__x86i64Int remainder{};

			// Step 1. Handle base cases

			// Base case 1: Division by zero
			if (divisor.digits_.empty())
			{
				throw std::runtime_error("Divide by zero");
			}

			// Base case 2: Dividend is smaller than the divisor
			if (dividend < divisor)
			{
				return std::make_pair(quotient, remainder); // Return zero as the quotient
			}

			// Base case 6: Dividend is a multiple of the divisor
			if (dividend == divisor)
			{
				quotient.digits_.push_back(1);
				return std::make_pair(quotient, remainder);
			}

			// Base case 3: Divisor is a single-digit number
			if (divisor.digits_.size() == 1)
			{
				int single_digit = divisor.digits_[0];
				return dividend.DivideByDigit(single_digit);
			}

			// Base case 4: Dividend is a single-digit number
			if (dividend.digits_.size() == 1)
			{
				int single_digit = dividend.digits_[0];
				quotient.digits_.push_back(single_digit / divisor.digits_[0]);
				remainder = __x86i64Int{ std::to_string(single_digit % divisor.digits_[0]) };
				return std::make_pair(quotient, remainder);
			}

			// Base case 5: Dividend is a power of 10
			if (dividend.digits_.size() > divisor.digits_.size() &&
				dividend.digits_.back() == 0)
			{
				quotient.digits_.resize(dividend.digits_.size() - divisor.digits_.size(), 0);
				dividend.digits_.resize(divisor.digits_.size());
			}

			// Base case 7: Dividend is a power of 10 and a multiple of the divisor
			if (dividend.digits_.size() == divisor.digits_.size() &&
				dividend.digits_.back() == 0)
			{
				quotient.digits_.resize(dividend.digits_.size() - divisor.digits_.size(), 0);
				dividend.digits_.resize(divisor.digits_.size());
			}

			// Step 2: Normalize dividend and divisor
			/*
			This implementation multiplies both the dividend and divisor by a normalization factor 
			to ensure the divisor's most significant digit is greaterThan or equalTo 5 
			so that the divisor's most significant digit is greater than or equal to half the base 
			(in our case, base 10). Normalization helps improve the efficiency and accuracy 
			of the quotient estimation in the algorithm... 
			The normalization factor is calculated as 10 / (1 + divisor.digits_.back()).
			*/
			int normalization_factor = 10 / (1 + divisor.digits_.back());
			__x86i64Int normalized_dividend = dividend * __x86i64Int{ std::to_string(normalization_factor) };
			__x86i64Int normalized_divisor = divisor * __x86i64Int{ std::to_string(normalization_factor) };

			// Step 3: Determine the number of chunks and create a loop to iterate through them
			uint64_t n = uint64_t{ normalized_dividend.digits_.size() };
			uint64_t m = uint64_t{ normalized_divisor.digits_.size() };
			uint64_t chunk_count = n - m;

			quotient.digits_.resize(chunk_count + 1, 0);

			// Loop through chunks in reverse (from most significant to least significant)
			for (int64_t i = chunk_count; i >= 0; i--)
			{
				// Step 4: Estimate the quotient using the most significant digits
				int q_clamp = int{ normalized_dividend.digits_[i + m] * 10 +
					normalized_dividend.digits_[i + m - 1] } /
					int{ normalized_divisor.digits_[m - 1] };

				// Clamp the estimate to the maximum value of a single digit (9)
				if (q_clamp > 9)
				{
					q_clamp = 9;
				}

				// Step 5: Refine the estimated quotient and compute the remainder
				__x86i64Int r_clamp = normalized_dividend.GetNumberSubrange(i, m + 1) - __x86i64Int{ std::to_string(q_clamp) } * normalized_divisor;

				// Adjust the estimate if necessary
				while (r_clamp.sign_ < 1)
				{
					q_clamp--;
					r_clamp += normalized_divisor;
				}

				// Update the dividend with the remainder
				normalized_dividend.SetNumberSubrange(i, r_clamp);

				// Save the quotient for this chunk
				quotient.digits_[i] = q_clamp;
			}

			// Step 6: Finalize the quotient
			quotient.trimLeadingZeros();

			return std::make_pair(quotient, remainder);
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
			for (int& val : ret.digits_)
			{
				val = digits_[StartIndexInt64++];
			}
			return std::move(ret);
		}

		__x86i64Int GetNumberSubrange(int64_t start_index, const int64_t& length) const
		{
			__x86i64Int SubrangeSliceUInt64{ "0" };
			if (start_index >= digits_.size()) 
			{
				// NOP //
			}
			else
			{
				const __x86i64Int I = _min(
					__x86i64Int{ std::to_string(start_index + length) }, 
					__x86i64Int{ std::to_string(digits_.size()) });
				__x86i64Int StartIndex{ std::to_string(start_index) };
				for (__x86i64Int i = StartIndex; i < I; i++)
				{
					SubrangeSliceUInt64.digits_.push_back(digits_[i]);
				}
			}
			return std::move(SubrangeSliceUInt64);
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
			for (const int& val : InConstUInt64ObjRef.digits_)
			{
				digits_[start_index++] = val;
			}
			RangeSetFlag = true;
			return RangeSetFlag;
		}

		// Conversion operator to std::uint64_t for
		// use as literal index in array elements
		operator std::uint64_t&&() const
		{
			// Assuming you have a method or member variable that represents the value
			// Return the value as std::uint64_t for the conversion
			uint64_t i = 0;
			uint64_t ret = 0;
			std::vector<int> result = digits_;
			std::reverse(result.begin(), result.end());
			for (const int& val : result)
			{
				ret += val * PowerUInt64<uint64_t>(BASE, i++);
			}
			return std::move(ret);
		}

	private:
		std::pair<__x86i64Int, __x86i64Int> DivideByDigit(const int& divisor) const
		{
			if (divisor > 9 || divisor < 1)
			{
				throw std::runtime_error("Invalid single-digit divisor for single-digit division.");
			}
			int remainder = 0;
			__x86i64Int quotient{ "0" };
			for (const int& val : digits_)
			{
				int temp = remainder * 10 + val;
				__x86i64Int iDividend = __x86i64Int{ std::to_string(temp) };
				__x86i64Int iDivisor = __x86i64Int{ std::to_string(divisor) };
				const std::pair<__x86i64Int, __x86i64Int>& result = iDividend / iDivisor;
				PushAtFront(quotient, result.first);
				remainder = temp % divisor;
			}
			quotient.trimLeadingZeros();
			return std::make_pair(quotient, __x86i64Int{ std::to_string(remainder) });
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