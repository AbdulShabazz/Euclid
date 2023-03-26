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
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <string>
#include <limits>
#include "PowerUInt64.h"
#include "__x86i64Int.h"
#include "../boost/multiprecision/cpp_int.hpp"

namespace EuclidProverLib
{

	enum class API_EXPORT BracketType { CurlyBraces, SquareBrackets, Parentheses };

	template <BracketType type>
	struct BracketTraits {};

	template <>
	struct BracketTraits<BracketType::CurlyBraces>
	{
		static constexpr std::string Open = "{";
		static constexpr std::string Close = "}";
	};

	template <>
	struct BracketTraits<BracketType::SquareBrackets>
	{
		static constexpr std::string Open = "[";
		static constexpr std::string Close = "]";
	};

	template <>
	struct BracketTraits<BracketType::Parentheses>
	{
		static constexpr std::string Open = "(";
		static constexpr std::string Close = ")";
	};

	class API_EXPORT CurlyBraceElide
	{
	public:
		/**
		Example:
		std::vector<std::string> input = { "{", "{", "{", "1", "}", "}", "+", "{", "{", "1", "}", "}", "}", "=", "{", "{", "2", "}", "}" };
		const auto& output = CurlyBraceElide::Elide<BracketType::CurlyBraces>(input); // { "{", "1", "}", "+", "{", "1", "}", "=", "{", "2", "}" }
		*/
		template <BracketType type>
		static std::vector<std::string> Elide(const std::vector<std::string>& input)
		{
			static_assert(std::is_same_v<decltype(type), BracketType>, "Invalid bracket type");
			const char openBrace = BracketTraits<type>::Open;
			const char closeBrace = BracketTraits<type>::Close;
			std::vector<std::string> output;
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
			return output;
		}
		/**
		Example:
		const auto& output = CurlyBraceElide::Elide<BracketType::CurlyBraces>({ "{", "{", "{", "1", "}", "}", "+", "{", "{", "1", "}", "}", "}", "=", "{", "{", "2", "}", "}" }); // { "{", "1", "}", "+", "{", "1", "}", "=", "{", "2", "}" }
		*/
		template <BracketType type>
		static std::vector<std::string> Elide(const std::initializer_list<std::string>& input)
		{
			static_assert(std::is_same_v<decltype(type), BracketType>, "Invalid bracket type");
			const std::vector<std::string>& input2(input);
			return Elide<type>(input2);
		}
	};

	// Initialize static members.
	uint64_t currentPrimeUInt64 = 947;

	// Calculate the first 160 primes.
	std::vector<uint64_t> primes = 
	{
		2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
		73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
		179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
		283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
		421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
		563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677,
		683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827,
		829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947/*, 953*/ 
	};

	constexpr uint64_t InitPrimeComposite (const std::vector<uint64_t>& _primes)
	{
		uint64_t result = 1;
		for (const uint64_t& val : _primes)
		{
			result *= val;
		}
		return result;
	};

	uint64_t PrimeCompositeUInt64 = InitPrimeComposite(primes);

	class API_EXPORT PrimeNumberGen
	{
	public:
		/** 
		Example: 
		const uint64_t output = PrimeNumberGen::NextPrimeUInt64(); // returns 3

		Example: 
		const uint64_t input = 162;
		const uint64_t output = PrimeNumberGen::NextPrimeUInt64(input); // returns 953
		*/
		static uint64_t NextPrimeUInt64(const uint64_t& index = ULLONG_MAX)
		{
			uint64_t result = 0;
			uint64_t maxSize = primes.size();
			// If index is in primes, return primes[index].
			if (index < maxSize)
			{
				result = primes.at(index);
			}
			// Else, calculate the next prime and add it to primes[].
			else
			{
				uint64_t nextPrimeUInt64 = currentPrimeUInt64 + 2;
				bool FindNextPrimeFlag = (index != ULLONG_MAX);
				do
				{
					while (!IsPrimeUInt64(nextPrimeUInt64))
					{
						nextPrimeUInt64 += 2;
					}
					primes.push_back(nextPrimeUInt64);
					maxSize++;
					PrimeCompositeUInt64 *= nextPrimeUInt64; // Update PrimeCompositeUInt64.
					nextPrimeUInt64 += 2; // Next odd integer.

				} while (maxSize <= index && FindNextPrimeFlag);

				result = currentPrimeUInt64 = primes.back();
			}
			// Todo: Add a check to see if the next prime is greater than 1/2 the max value of uint64_t.
			// Todo: Compute intermediate sums between this and previous primes, for later use.
			// Todo: Compute intermediate products between this and previous primes, for later use.
			// Todo: Compute intermediate quotients between this and previous primes, for later use.
			// Todo: Compute intermediate remainders between this and previous primes, for later use.
			// Todo: Cache prime number constituents for this integer, for later use.
			// Return prime at this index.
			return result;
		}

		/**
		Example:
		const uint64_t input = 162;
		const bool IsPrimeFlag = PrimeNumberGen::IsPrimeUInt64(input); // returns false
		*/
		static bool IsPrimeUInt64(const uint64_t& n)
		{
			return (((PrimeCompositeUInt64 / n) % 1) != 0);
		}
	};

	template<BracketType EuclidBracket>
	class API_EXPORT EuclidProver;

	/**
	Example:
	int main()
	{
		using namespace EuclidProverLib;

		// Instantiate Prover (module)
		EuclidProver<BracketType::CurlyBraces> Euclid{};

		// Add axioms
		Euclid.Axiom({ "{", "1", "}","+","{", "1", "}","=","{", "2", "}" }); // axiom_0
		Euclid.Axiom({ "{", "2", "}","+","{", "2", "}","=","4" }); // axiom_1

		// Add supporting lemmas
		Euclid.Lemma({ "{", "1", "}","+","{", "0", "}","<==>","{", "1", "}" }); // lemma_0

		// Conduct proof
		std::vector<std::string> proof = { "{", "4", "}", "=", "{", "1", "}","+","{", "1", "}","+","{", "1", "}", "+", "{", "1", "}" };
		std::vector<std::vector<std::string>> path;
		if (Euclid.Prove(proof, path))
		{
			std::cout << "Proof:\n";
			Euclid.PrintPath(path);
		}
		else
		{
			std::cout << "Proof failed\n";
		}

		// Optional Solver: Expand
		std::vector<std::vector<std::string>> expand_path;
		if (Euclid.ProveViaExpand(proof, expand_path))
		{
			std::cout << "Proof via Expand:\n";
			Euclid.PrintPath(expand_path);
		}
		else
		{
			std::cout << "Proof via Expand failed\n";
		}

		// Optional Solver: Reduce
		std::vector<std::vector<std::string>> reduce_path;
		if (Euclid.ProveViaReduce(proof, reduce_path))
		{
			std::cout << "Proof via Reduce:\n";
			Euclid.PrintPath(reduce_path);
		}
		else
		{
			std::cout << "Proof via Reduce failed\n";
		}

		return 0;
	}
	*/
	template<>
	class API_EXPORT EuclidProver<BracketType::CurlyBraces>
	{
	public:
		EuclidProver(const std::string openBrace = "{",
			const std::string closeBrace = "}") : 
			_openBrace{openBrace},
			_openBraceST{ "st"+openBrace },
			_closeBrace{ closeBrace }
		{

		}

		bool Axiom(const std::vector<std::string>& InAxiomConstStdStrVecRef)
		{
			// Check if the curly brace scope is valid
			if (!CurlyBraceScopeChecker(InAxiomConstStdStrVecRef))
			{
				return false;
			}

			// Split the equation into left-hand side (LHS) and right-hand side (RHS)
			if (!SplitEquation(InAxiomConstStdStrVecRef, AxiomLHS, AxiomRHS))
			{
				return false;
			}

			// Check if the axiom lengths are valid
			if (!AxiomLengthsAreValid(AxiomLHS, AxiomRHS))
			{
				return false;
			}

			// Update LemmaLHSPrimeComposite and LemmaRHSPrimeComposite
			if (!CalculatePrimeComposites(AxiomLHS, 
				AxiomRHS, 
				AxiomLHSPrimeComposite, 
				AxiomRHSPrimeComposite))
			{
				return false;
			}

			return true;
		}

		bool Axiom(const std::initializer_list<std::string>& InAxiomInitListConstStdStringRef)
		{
			const std::vector<std::string>& InAxiomVecConstStdStringRef{ InAxiomInitListConstStdStringRef };
			return Axiom(InAxiomVecConstStdStringRef);
		}

		bool Lemma(const std::vector<std::string>& InLemmaConstStdStringVecRef)
		{
			// Check if the curly brace scope is valid
			if (!CurlyBraceScopeChecker(InLemmaConstStdStringVecRef))
			{
				return false;
			}

			// Split the equation into left-hand side (LHS) and right-hand side (RHS)
			if (!SplitEquation(InLemmaConstStdStringVecRef, LemmaLHS, LemmaRHS))
			{
				return false;
			}

			// Check if the Lemma lengths are valid
			if (!AxiomLengthsAreValid(LemmaLHS, LemmaRHS))
			{
				return false;
			}

			// Update LemmaLHSPrimeComposite and LemmaRHSPrimeComposite
			if (!CalculatePrimeComposites(LemmaLHS, 
				LemmaRHS,
				LemmaLHSPrimeComposite, 
				LemmaRHSPrimeComposite))
			{
				return false;
			}

			return true;
		}

		bool Lemma(const std::initializer_list<std::string>& InLemmaInitListConstCharRef)
		{
			const std::vector<std::string>& InLemmaVecConstCharRef{ InLemmaInitListConstCharRef };
			return Lemma(InLemmaVecConstCharRef);
		}

		bool Prove(const std::vector<std::string>& InProofVecConstCharRef,
			std::vector<std::vector<std::string>>& OutPath2DVecCharRef)
		{
			bool ResultFoundFlag = false;
			return ResultFoundFlag;
		}

		bool ProveViaReduce(const std::vector<std::string>& InProofVecChar,
			std::vector<std::vector<std::string>>& OutReducePathVec2DCharRef)
		{
			bool ResultFoundFlag = false;
			return ResultFoundFlag;
		}

		bool ProveViaExpand(const std::vector<std::string>& InProofVecConstChar,
			std::vector<std::vector<std::string>>& OutExpandPathVec2DCharRef)
		{
			bool ResultFoundFlag = false;
			return ResultFoundFlag;
		}

		void PrintPath(const std::vector<std::vector<std::string>>& InPathVec2DConstChar) const
		{

		}

	protected:
		std::string _openBrace;
		std::string _openBraceST; // strict comprehension enforcement
		std::string _closeBrace;
		const std::string LemmaImplies = "<==>";
		const std::string LemmaImpliesRHS = "==>";
		const std::string LemmaImpliesLHS = "<==";
		std::vector<std::vector<std::string>> ProofStackLHS{};
		std::vector<std::vector<std::string>> ProofStackRHS{};
		std::vector<std::vector<std::string>> LemmaLHS{};
		std::vector<std::vector<std::string>> LemmaRHS{};
		std::vector<std::vector<std::string>> AxiomLHS{};
		std::vector<std::vector<std::string>> AxiomRHS{};
		std::vector<std::vector<std::string>> ProofLHS{};
		std::vector<std::vector<std::string>> ProofRHS{};

		// Prime Composite Library. Cached to disk for performance
		std::vector<uint64_t> AxiomLHSPrimeComposite{};
		std::vector<uint64_t> AxiomRHSPrimeComposite{};
		std::vector<uint64_t> LemmaLHSPrimeComposite{};
		std::vector<uint64_t> LemmaRHSPrimeComposite{};
		std::vector<uint64_t> ProofLHSPrimeComposite{};
		std::vector<uint64_t> ProofRHSPrimeComposite{};
		// Usage: primes[ TokenLibraryStdStringToUInt64PrimesIndexMap["{"] ]
		std::unordered_map <std::string, uint64_t> TokenLibraryStdStringToUInt64PrimesIndexMap; 

		bool CalculatePrimeComposites(const std::vector<std::vector<std::string>>& InLHS,
			const std::vector<std::vector<std::string>>& InRHS, 
			std::vector<uint64_t>& OutLHSPrimeComposite,
			std::vector<uint64_t>& OutRHSPrimeComposite)
		{
			bool ResultFlag = false;

			uint64_t LHSPrimeCompositeUInt64 = 1;

			for (const auto& LHSAxiomRef : InLHS)
			{
				uint64_t ExponentUInt64 = 1;
				for (const auto& AxiomTokenRef : LHSAxiomRef)
				{
					const uint64_t& NumberBaseUInt64 = GetPrimeUInt64(AxiomTokenRef);
					LHSPrimeCompositeUInt64 *= std::PowerUInt64(NumberBaseUInt64, ExponentUInt64++);
				}
				OutLHSPrimeComposite.push_back(LHSPrimeCompositeUInt64);
				LHSPrimeCompositeUInt64 = 1;
			}

			uint64_t RHSPrimeCompositeUInt64 = 1;

			for (const auto& RHSAxiomRef : InRHS)
			{
				uint64_t ExponentUInt64 = 1;
				for (const auto& AxiomTokenRef : RHSAxiomRef)
				{
					const uint64_t& NumberBaseUInt64 = GetPrimeUInt64(AxiomTokenRef);
					RHSPrimeCompositeUInt64 *= std::PowerUInt64(NumberBaseUInt64, ExponentUInt64++);
				}
				OutRHSPrimeComposite.push_back(RHSPrimeCompositeUInt64);
				RHSPrimeCompositeUInt64 = 1;
			}

			ResultFlag = true;
			return ResultFlag;
		}

		bool AxiomLengthsAreValid(const  std::vector <std::vector<std::string>> InLhs,
			const  std::vector <std::vector<std::string>> InRhs) const
		{
			return (InLhs.size() > 0 && InRhs.size() > 0);
		}

		uint64_t GetPrimeUInt64(const std::string& InConstStdStr)
		{
			uint64_t ResultUInt64{};
			const auto iter = TokenLibraryStdStringToUInt64PrimesIndexMap.find(InConstStdStr);
			if (iter == TokenLibraryStdStringToUInt64PrimesIndexMap.end())
			{
				PrimeNumberGen::NextPrimeUInt64();
				const uint64_t i =
					TokenLibraryStdStringToUInt64PrimesIndexMap[InConstStdStr] =
					primes.size() - 1;
				ResultUInt64 = primes[i];
			}
			else
			{
				ResultUInt64 = primes[iter->second];
			}
			return ResultUInt64;
		}

		uint64_t GetPrimeUInt64Index(const std::string& InConstStdString)
		{
			uint64_t ResultUInt64{};
			const auto iter = TokenLibraryStdStringToUInt64PrimesIndexMap.find(InConstStdString);
			if (iter == TokenLibraryStdStringToUInt64PrimesIndexMap.end())
			{
				PrimeNumberGen::NextPrimeUInt64();
				ResultUInt64 = 
					TokenLibraryStdStringToUInt64PrimesIndexMap[InConstStdString] =
						primes.size() - 1;
			}
			else
			{
				ResultUInt64 = iter->second;
			}
			return ResultUInt64;
		}

		bool FoundTentativeMatchFlag(const uint64_t& InAxiomLHSPrimeCompositeUInt64, 
			const uint64_t& InAxiomRHSPrimeCompositeUInt64)
		{
			return ((InAxiomRHSPrimeCompositeUInt64 / InAxiomLHSPrimeCompositeUInt64) % 1 == 0);
		}

		bool CurlyBraceScopeChecker(const std::vector<std::string>& InAxiomConstStdStrVecRef)
		{
			bool NoOpenBracesFlag = true;
			int openBraces = 0;
			for (const auto& token : InAxiomConstStdStrVecRef)
			{
				if (token == _openBrace || 
					token == _openBraceST)
				{
					openBraces++;
				}
				else if (token == _closeBrace)
				{
					openBraces--;
				}
			}
			NoOpenBracesFlag = (openBraces == 0);
			return NoOpenBracesFlag;
		}

		bool SplitEquation(const std::vector<std::string>& InAxiomConstStdStrVecRef,
			std::vector<std::vector<std::string>>& OutAxiomLHS,
			std::vector<std::vector<std::string>>& OutAxiomRHS)
		{
			bool result = false;
			bool FoundEqualsSignFlag = false;
			std::vector<std::string> lhs;
			std::vector<std::string> rhs;
			const std::unordered_map <std::string, bool> AssignmentOP = { { "=", true}, {"==>", true}, {"<==", true}, {"<==>", true} };
			for (const std::string& str : InAxiomConstStdStrVecRef)
			{
				if (AssignmentOP.find(str) != AssignmentOP.end())
				{
					FoundEqualsSignFlag = true;
					if (rhs.size())
					{
						OutAxiomRHS.push_back(rhs);
						rhs = {};
					}
					continue;
				}
				if (!FoundEqualsSignFlag) 
				{
					lhs.push_back(str); 
				}
				else
				{
					rhs.push_back(str);
				}
			}
			OutAxiomLHS.push_back(lhs);
			result = FoundEqualsSignFlag;
			return result;
		}
	};

	template<>
	class API_EXPORT EuclidProver<BracketType::Parentheses> : public EuclidProver<BracketType::CurlyBraces>
	{
	public:
		EuclidProver() : EuclidProver<BracketType::CurlyBraces>( "(", ")" )
		{

		}
	};

	template<>
	class API_EXPORT EuclidProver<BracketType::SquareBrackets> : public EuclidProver<BracketType::CurlyBraces>
	{
	public:
		EuclidProver() : EuclidProver<BracketType::CurlyBraces>( "[", "]" )
		{

		}
	};
}