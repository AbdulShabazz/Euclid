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
#include "__x86i64Int.h"

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

	class API_EXPORT CurlyBraceScopeChecker
	{
	public:
		/**
		Example:
		std::vector<std::string> stringvec_ = { "{", "}", "{", "{", "}", "}" };
		CurlyBraceScopeChecker::AreProperlyScoped<BracketType::CurlyBraces>(stringvec_); // returns true 
		*/
		template <BracketType type>
		static bool AreProperlyScoped(const std::vector<std::string>& chars)
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
		/**
		Example:
		std::vector<std::string> input = { "{", "{", "{", "1", "}", "}", "+", "{", "{", "1", "}", "}", "}", "=", "{", "{", "2", "}", "}" };
		const auto& output = CurlyBraceElide::Elide<BracketType::CurlyBraces>(input); // { "{", "1", "}", "+", "{", "1", "}", "=", "{", "2", "}" }
		*/
		template <BracketType type>
		static std::vector<std::string>/*&&*/ Elide(const std::vector<std::string>& input)
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
			return output;// std::move(output);
		}
		/**
		Example:
		const auto& output = CurlyBraceElide::Elide<BracketType::CurlyBraces>({ "{", "{", "{", "1", "}", "}", "+", "{", "{", "1", "}", "}", "}", "=", "{", "{", "2", "}", "}" }); // { "{", "1", "}", "+", "{", "1", "}", "=", "{", "2", "}" }
		*/
		template <BracketType type>
		static std::vector<std::string>/*&&*/ Elide(const std::initializer_list<std::string>& input)
		{
			static_assert(std::is_same_v<decltype(type), BracketType>, "Invalid bracket type");
			const std::vector<std::string>& input2(input);
			return Elide<type>(input2);// std::move(Elide<type>(input2));
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

	constexpr uint64_t PrimeComposite (const std::vector<uint64_t>& _primes)
	{
		uint64_t result = 1;
		for (const uint64_t& val : _primes)
		{
			result *= val;
		}
		return result;
	};

	uint64_t PrimeCompositeUInt64 = PrimeComposite(primes);

	class API_EXPORT PrimeNumberGen
	{
	public:
		/** 
		Example: 
		const uint64_t output = PrimeNumberGen::NextPrimeUInt64(); // 3

		Example: 
		const uint64_t input = 162;
		const uint64_t output = PrimeNumberGen::NextPrimeUInt64(input); // 953
		*/
		static uint64_t NextPrimeUInt64(const uint64_t& index = ULLONG_MAX)
		{
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
				bool FindNextPrimeFlag = (index != ULLONG_MAX);
				do
				{
					while (!IsPrimeUInt64(nextPrimeUInt64))
					{
						nextPrimeUInt64 += 2;
					}
					primes.push_back(nextPrimeUInt64);
					PrimeCompositeUInt64 *= nextPrimeUInt64;
					maxSize++;
					nextPrimeUInt64 += 2;
				} while (maxSize <= index && FindNextPrimeFlag);
				result = currentPrimeUInt64 = primes.back();
			}

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
			return ((PrimeCompositeUInt64 / n) % 1) == 0;
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
		Euclid.Lemma({ "{", "1", "}","+","{", "0", "}","=","{", "1", "}" }); // lemma_0

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
		EuclidProver() : _openBrace{ "{" },
			_openBraceST{ "st{" },
			_closeBrace{ "}" }
		{

		}
		EuclidProver(const std::string openBrace,
			const std::string openBraceST,
			const std::string closeBrace) : _openBrace{ openBrace },
			_openBraceST{ openBraceST },
			_closeBrace{ closeBrace }
		{

		}
		bool Axiom(const std::vector<std::string>& InAxiomVecConstCharRef)
		{
			std::__x86i64Int _a_{}; // OK
			std::__x86i64Int a{ 12n }; // OK
			//std::__x86i64Int aa{ 12 }; // Error
			std::__x86i64Int aaa{ 0 };
			std::__x86i64Int aaaa{ "12" }; // OK
			std::__x86i64Int b{ 0 };
			std::__x86i64Int c = std::__x86i64Int{ 12n }; // OK
			//std::__x86i64Int cc = std::__x86i64Int{ 12 }; // Error
			std::__x86i64Int ccc = std::__x86i64Int{ "12" }; // OK
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
			bool AxiomAcceptedFlag = false;
			uint64_t AxiomLHSPrimeCompositeUInt64 = 1;
			uint64_t AxiomRHSPrimeCompositeUInt64 = 1;
			if (SplitEquation(InAxiomVecConstCharRef,
				AxiomLHSPrimeCompositeUInt64,
				AxiomRHSPrimeCompositeUInt64,
				AxiomRHS,
				AxiomRHSPrimeComposite))
			{
				AxiomAcceptedFlag = isValid();
				if (AxiomAcceptedFlag)
				{
					LemmaLHS.push_back(lhs);
					LemmaRHS.push_back(lhs);
					LemmaLHSPrimeComposite.push_back(AxiomLHSPrimeCompositeUInt64);
					LemmaRHSPrimeComposite.push_back(AxiomRHSPrimeCompositeUInt64);
				}
			}
			return AxiomAcceptedFlag;
		}
		bool Axiom(const std::initializer_list<std::string>& InAxiomInitListConstStdStringRef)
		{
			const std::vector<std::string>& InAxiomVecConstStdStringRef{ InAxiomInitListConstStdStringRef };
			return Axiom(InAxiomVecConstStdStringRef);
		}
		bool Lemma(const std::vector<std::string>& InLemmaConstStdStringVecRef)
		{
			bool LemmaAcceptedFlag = false;
			uint64_t AxiomLHSPrimeCompositeUInt64 = 1;
			uint64_t AxiomRHSPrimeCompositeUInt64 = 1;
			if (SplitEquation(InLemmaConstStdStringVecRef,
				AxiomLHSPrimeCompositeUInt64,
				AxiomRHSPrimeCompositeUInt64, 
				LemmaRHS,
				LemmaRHSPrimeComposite))
			{
				LemmaAcceptedFlag = isValid();
				if(LemmaAcceptedFlag)
				{
					LemmaLHSPrimeComposite.push_back(AxiomLHSPrimeCompositeUInt64);
					LemmaRHSPrimeComposite.push_back(AxiomRHSPrimeCompositeUInt64);
				}
			}
			return LemmaAcceptedFlag;
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
		std::string _openBraceST;
		std::string _closeBrace;
		std::vector<std::string> lhs;
		std::vector<std::string> rhs;
		std::vector<std::vector<std::string>> ProofStackLHS{};
		std::vector<std::vector<std::string>> ProofStackRHS{};
		std::vector<std::vector<std::string>> LemmaLHS{};
		std::vector<std::vector<std::string>> LemmaRHS{};
		std::vector<std::vector<std::string>> AxiomLHS{};
		std::vector<std::vector<std::string>> AxiomRHS{};
		std::vector<std::vector<std::string>> ProofLHS{};
		std::vector<std::vector<std::string>> ProofRHS{};
		std::vector<uint64_t> AxiomLHSPrimeComposite{};
		std::vector<uint64_t> AxiomRHSPrimeComposite{};
		std::vector<uint64_t> LemmaLHSPrimeComposite{};
		std::vector<uint64_t> LemmaRHSPrimeComposite{};
		std::vector<uint64_t> ProofLHSPrimeComposite{};
		std::vector<uint64_t> ProofRHSPrimeComposite{};
		std::unordered_map <std::string, uint64_t> TokenLibraryStdStringToUInt64tMap; 
		bool isValid() const 
		{
			return (lhs.size() > 0 && rhs.size() > 0);
		}
		uint64_t GetPrimeToken(const std::string& str)
		{
			uint64_t ResultUInt64{};
			auto iter = TokenLibraryStdStringToUInt64tMap.find(str);
			if (iter == TokenLibraryStdStringToUInt64tMap.end())
			{
				uint64_t prime = PrimeNumberGen::NextPrimeUInt64();
				TokenLibraryStdStringToUInt64tMap[str] = prime;
				ResultUInt64 = prime;
			}
			else
			{
				ResultUInt64 = iter->second;
			}
			return ResultUInt64;
		}
		bool FoundTentativeMatchFlag(const uint64_t& InAxiomRHSPrimeCompositeUInt64,
			const uint64_t& InAxiomLHSPrimeCompositeUInt64)
		{
			return ((InAxiomRHSPrimeCompositeUInt64 / InAxiomLHSPrimeCompositeUInt64) % 1 == 0);
		}
		bool SplitEquation(const std::vector<std::string>& inAxiomConstStdStrVecRef,
			uint64_t& OutAxiomLHSPrimeCompositeUInt64,
			uint64_t& OutAxiomRHSPrimeCompositeUInt64,
			std::vector<std::vector<std::string>>& OutAxiomRHS,
			std::vector<uint64_t>& OutAxiomRHSPrimeComposite)
		{
			bool FoundEqualsSignFlag = false;
			bool NoOpenBracesFlag = true;
			int openBraces = 0;
			for (const std::string& str : inAxiomConstStdStrVecRef)
			{
				if (str == "=")
				{
					FoundEqualsSignFlag = true;
					if (rhs.size())
					{
						OutAxiomRHS.push_back(rhs);
						OutAxiomRHSPrimeComposite.push_back(OutAxiomRHSPrimeCompositeUInt64);
						OutAxiomRHSPrimeCompositeUInt64 = 1;
						rhs = {};
					}
					continue;
				}
				if (!FoundEqualsSignFlag) 
				{
					lhs.push_back(str); 
					const uint64_t prime = GetPrimeToken(str);
					const uint64_t ExponentUInt64 = static_cast<uint64_t>(lhs.size());
					const uint64_t PowerUInt64 = std::PowerUInt64<>(prime, ExponentUInt64);
					OutAxiomLHSPrimeCompositeUInt64 *= PowerUInt64;
				}
				else
				{
					rhs.push_back(str);
					const uint64_t prime = GetPrimeToken(str);
					const uint64_t ExponentUInt64 = static_cast<uint64_t>(rhs.size());
					const uint64_t PowerUInt64 = std::PowerUInt64<>(prime, ExponentUInt64);
					OutAxiomRHSPrimeCompositeUInt64 *= PowerUInt64;
				}
			}
			int lhsOpenBraces = 0;
			int rhsOpenBraces = 0;
			auto countOpenBraces = [](const std::vector<std::string>& ConstStdStringVecRef,
				int& openBraces,
				const std::string& OpenBrace,
				const std::string& OpenBraceST,
				const std::string& CloseBrace)
			{
				std::unordered_set<std::string> braceSet{ OpenBrace, OpenBraceST };				
				for (const std::string& str : ConstStdStringVecRef)
				{
					if (braceSet.contains(str))
					{
						openBraces++;
					}
					else if (str == CloseBrace)
					{
						openBraces--;
					}
				}
			};
			std::thread lhsThread(countOpenBraces, 
				std::cref(lhs), 
				std::ref(lhsOpenBraces),
				std::cref(_openBrace),
				std::cref(_openBraceST),
				std::cref(_closeBrace));
			std::thread rhsThread(countOpenBraces, 
				std::cref(rhs), 
				std::ref(rhsOpenBraces),
				std::cref(_openBrace),
				std::cref(_openBraceST),
				std::cref(_closeBrace));
			lhsThread.join();
			rhsThread.join();
			//bool NoOpenBracesFlag = true;
			if (lhsOpenBraces || rhsOpenBraces)
			{
				std::cout << "Warning - Missing equals '=' in lhs axiom or lemma. Axioms and lemmas must have 'lhs... = rhs...'." << std::endl;
				NoOpenBracesFlag = false;
			}
			return (NoOpenBracesFlag && FoundEqualsSignFlag);
		}
	};

	template<>
	class API_EXPORT EuclidProver<BracketType::Parentheses> : public EuclidProver<BracketType::CurlyBraces>
	{
	public:
		EuclidProver() : EuclidProver<BracketType::CurlyBraces>("(", "st(", ")")
		{

		}
	};

	template<>
	class API_EXPORT EuclidProver<BracketType::SquareBrackets> : public EuclidProver<BracketType::CurlyBraces>
	{
	public:
		EuclidProver() : EuclidProver<BracketType::CurlyBraces>("[", "st[", "]")
		{

		}
	};
}