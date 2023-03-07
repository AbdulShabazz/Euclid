#pragma once

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the EUCLIDPROVERLIBDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// API_EXPORT functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EUCLIDPROVERLIBDLL_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

#include <iostream>
#include <vector>
#include <type_traits>
#include <initializer_list>

#include <stdint.h>
#include <cmath>

namespace EuclidProverLib
{

	enum class  BracketType
	{
		CurlyBraces,
		SquareBrackets,
		Parentheses
	};

	template <BracketType type>
	struct BracketTraits {};

	template <>
	struct API_EXPORT BracketTraits<BracketType::CurlyBraces>
	{
		static constexpr char Open = '{';
		static constexpr char Close = '}';
	};

	template <>
	struct API_EXPORT BracketTraits<BracketType::SquareBrackets>
	{
		static constexpr char Open = '[';
		static constexpr char Close = ']';
	};

	template <>
	struct API_EXPORT BracketTraits<BracketType::Parentheses>
	{
		static constexpr char Open = '(';
		static constexpr char Close = ')';
	};

	class API_EXPORT CurlyBraceScopeChecker
	{
	public:
		template <BracketType type>
		static bool IsEven(const std::vector<char>& chars)
		{
			static_assert(std::is_same_v<decltype(type), BracketType>, "Invalid bracket type");
			const char openBrace = BracketTraits<type>::Open;
			const char closeBrace = BracketTraits<type>::Close;
			int count = 0;
			for (char c : chars)
			{
				if (c == openBrace)
				{
					count++;
				}
				else if (c == closeBrace)
				{
					count--;
				}
			}
			return count == 0;
		}
	};

	class API_EXPORT CurlyBraceElide
	{
	public:
		template <BracketType type>
		static std::vector<char>/*&&*/ Elide(const std::vector<char>& input)
		{
			static_assert(std::is_same_v<decltype(type), BracketType>, "Invalid bracket type");
			const char openBrace = BracketTraits<type>::Open;
			const char closeBrace = BracketTraits<type>::Close;
			std::vector<char> output;
			bool OpenScopeFlag = false;
			for (char c : input)
			{
				if (c == openBrace)
				{
					if (!OpenScopeFlag)
					{
						output.push_back(openBrace);
						OpenScopeFlag = true;
					}
					continue;
				}
				else if (c == closeBrace)
				{
					if (OpenScopeFlag)
					{
						output.push_back(closeBrace);
						OpenScopeFlag = false;
					}
					continue;
				}
				output.push_back(c);
			}
			return output;// std::move(output);
		}
		template <BracketType type>
		static std::vector<char>/*&&*/ Elide(const std::initializer_list<char>& input)
		{
			static_assert(std::is_same_v<decltype(type), BracketType>, "Invalid bracket type");
			const std::vector<char>& input2(input);
			return Elide<type>(input2);// std::move(Elide<type>(input2));
		}
	};

	// Initialize static members.
	uint64_t currentPrimeUInt64 = 2;

	// Calculate the first 161 primes (up to 1000).
	std::vector<uint64_t> primes = {
		2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
		73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
		179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
		283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
		421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
		563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677,
		683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
		829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953 };

	class API_EXPORT PrimeNumberGen
	{
	public:
		static uint64_t NextPrimeUInt64(const uint64_t& index = 1e+6)
		{
			const uint64_t uint64Max = 1e+6;
			uint64_t result = 0;
			uint64_t maxSize = primes.size();
			// If index is less than the current size of the primes vector, return the prime at index.
			if (index < maxSize)
			{
				result = primes.at(index);
			}
			// Else, calculate the next prime and add it to the primes vector.
			else
			{
				uint64_t nextPrimeUInt64 = currentPrimeUInt64 + 2;
				bool FindNextPrimeFlag = (index == uint64Max);
				do
				{
					while (!IsPrimeUInt64(nextPrimeUInt64))
					{
						nextPrimeUInt64 += 2;
					}
					primes.push_back(nextPrimeUInt64);
					maxSize++;
					nextPrimeUInt64 += 2;
				} while (maxSize <= index && FindNextPrimeFlag);
				result = currentPrimeUInt64 = primes.back();
			}

			// Return prime at index.
			return result;
		}
		static bool IsPrimeUInt64(const uint64_t& n)
		{
			if (n <= 1)
			{
				return false;
			}
			if (n <= 3)
			{
				return true;
			}
			if (n % 2 == 0 || n % 3 == 0)
			{
				return false;
			}
			for (uint64_t i = 5; i * i <= n; i += 6)
			{
				if (n % i == 0 || n % (i + 2) == 0)
				{
					return false;
				}
			}
			return true;
		}
	};
}