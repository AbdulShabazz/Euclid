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
		
		uint64_t GetTokenIndexUInt64(const std::string& str) const
		{
			return TokenLibraryStdStringToUInt64PrimesIndexMap.at(str);
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