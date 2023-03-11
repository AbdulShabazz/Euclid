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
#include <thread>

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
		static uint64_t NextPrimeUInt64(const uint64_t& index = static_cast<uint64_t>(1e+6))
		{
			const uint64_t uint64Max = static_cast<uint64_t>(1e+6);
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
				bool FindNextPrimeFlag = (index != uint64Max);
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

			// Return prime at this index.
			return result;
		}
		/**
		Example:
		const uint64_t input = 162;
		const uint64_t output = PrimeNumberGen::IsPrimeUInt64(input); // false
		*/
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
		bool Axiom(const std::vector<std::string>& InAxiomVecConstCharRef)
		{
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
	private:
		const std::string _openBrace = "{";
		const std::string _closeBrace = "}";
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
					uint64_t prime = GetPrimeToken(str);
					uint64_t LhsSizeUInt64 = static_cast<uint64_t>(lhs.size());
					uint64_t PowerUInt64 = std::pow<uint64_t>(prime, LhsSizeUInt64);
					OutAxiomLHSPrimeCompositeUInt64 *= PowerUInt64;
				}
				else 
				{
					rhs.push_back(str);
					uint64_t prime = GetPrimeToken(str);
					uint64_t RhsSizeUInt64 = static_cast<uint64_t>(rhs.size());
					uint64_t PowerUInt64 = std::pow<uint64_t>(prime, RhsSizeUInt64);
					OutAxiomRHSPrimeCompositeUInt64 *= PowerUInt64;
				}
			}
			int lhsOpenBraces = 0;
			int rhsOpenBraces = 0;
			auto countOpenBraces = [](const std::vector<std::string>& conststringvec,
				int& openBraces,
				const std::string& OpenBrace,
				const std::string& CloseBrace)
			{
				for (const std::string& str : conststringvec)
				{
					if (str == OpenBrace)
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
				std::cref(_closeBrace));
			std::thread rhsThread(countOpenBraces, 
				std::cref(rhs), 
				std::ref(rhsOpenBraces), 
				std::cref(_openBrace), 
				std::cref(_closeBrace));
			lhsThread.join();
			rhsThread.join();
			//bool NoOpenBracesFlag = true;
			if (lhsOpenBraces)
			{
				std::cout << "Warning - Missing equals '=' in lhs axiom or lemma. Axioms and lemmas must have 'lhs... = rhs...'." << std::endl;
				NoOpenBracesFlag = false;
			}
			if (rhsOpenBraces)
			{
				std::cout << "Warning - Missing equals '=' in rhs axiom or lemma. Axioms and lemmas must have 'lhs... = rhs...'." << std::endl;
				NoOpenBracesFlag = false;
			}
			return (NoOpenBracesFlag && FoundEqualsSignFlag);
		}
	};

	template<>
	class API_EXPORT EuclidProver<BracketType::Parentheses>
	{
	public:
	private:
		const std::string _openBrace = "(";
		const std::string _closeBrace = ")";
	};

	template<>
	class API_EXPORT EuclidProver<BracketType::SquareBrackets>
	{
	public:
	private:
		const std::string _openBrace = "[";
		const std::string _closeBrace = "]";
	};
}