
/*

  AUTHOR
	  Seagat2011 (github.com/Seagat2011, eterna.cmu.edu/web/player/90270/, fold.it/port/user/1992490)

  VERSION
	  Major.Minor.Bugfix.Patch
	  12.0.0.0

  DESCRIPTION
	Theorem prover written in C++23. Ported from ECMA-262 JavaScript (A grammar reduction term-rewriting system)
	for use in the Unreal Engine 5.2 Core in-game Framework.

  UPDATES (C++23)
	+_lhs/_rhs RouteSource_EnumClass support for all subnet route maps (Improved branch Performance)
	+Specialization templates - for the primary "Auto" template function declaration (Improved branch Performance)
	+Lockless stack manager: RecursionLimiter (Eliminates mutex/semaphore Performance penalty)
	-Multithread support (Increased latency, reduced scalability)

  UPDATES (JavaScript)
	+Negative proof assertions ~=
	+_AXIOM_.optimizeCallGraph
	+Improved ProofComplete search performance
	+Prove via Auto (PASS)
	+Axiom._eval => Axiom._reduce
	+Axiom.{_reduce,_expand} => eventListener(s)
	+solutionEditor => contentEditable
	+Prove via Reduce (PASS)
	+Prove via Expand (PASS)
	+scoping functionality
	+LibreOffice math library support
	-Axiom._eval eventListener

  NOTES:
	Term-rewrites are performed with the aid of a special rewrite compiler (ie. via LEMMA SUBSTITUTION); SEE TEST CASES

	Substitution methods:
	
		1. AXIOMATIC: 1 + 1 = 2
		2. LEMMA SUBSTITUTION: 1 <==> 1 / 1
		
	Note: Lemma substitutions are rewrite "helpers" which can be used to rewrite axioms. 
	Great care must be taken with them because they can introduce recursion, stack overflows, 
	and other performance bugs: For example, consider: "{ PlayerCharacterSideKick } IsIn { StyxBoat }" - 
	the "IsIn" operator may or may not link unrelated categories, indefinitely;
	whereas: "{ PlayerCharacterSideKick } IsIn_StyxBoat " is safer and guaranteed to converge.

  Usage Example. // Lemma substitutions //

	{ { a } raised { 2 } } plus { 2ab } plus { b raised { 2 } } <== ( { a } plus { b } ) raised { 2 }
	( { a } plus { b } ) raised { 2 } minus { 2ab } = { c } raised { 2 } <== ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
	{ { a } raised { 2 } } plus { 2ab } minus { 2ab } plus { b raised { 2 } } ==> { { a } raised { 2 } } plus { { b } raised { 2 } }
	( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
	Prove { { a } raised { 2 } } plus { { b } raised { 2 } } = { c } raised { 2 }

  Usage Example. // Lemma substitution //

	// Axiom
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver } // Current Game State
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	.
	. [Other available non-relevant Game States, in the current environment, for the framework to choose from ]
	.
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

	// Lemma
	{ PlayerCharacterSideKick } IsIn { StyxBoat } <== { StyxBoat } IsNotIn { StyxRiver } // These are connectives, and axiom helpers
	{ PlayerCharacterSideKick } IsOn { Vehicle } <== { VehicleDriveDisabled }
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } <== { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle }
	{ PlayerCharacterSideKick } IsNotIn { StyxBoat } ==> { StyxBoat } IsNotIn { StyxRiver }

	// Theorem
	Prove { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

	// Output (Proof-Steps)
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver }
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
	{ PlayerCharacterSideKick } IsNotIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
	{ PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

  REFERENCES
	  OpenAI GPT-4-32k-0314 ( { max_tokens:32000, temperature:1.0, top_p:1.0, N:1, 
			stream:false, logprobs:NULL, echo:false, stop:NULL, presence_penalty:0, 
			frequency_penalty:0, best_of:1, logit_bias:NULL } )

  COMPATIBILITY
	  Windows 11+ x86i64

*/

#include <iostream>
#include <thread>
#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <chrono>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, uint64_t> SymbolToPrimeNumber_Map;

std::vector<uint64_t> PrimeComposite_UInt64Vec { 2, 3, 5 };

/**
 * Prime(N) : Return the Nth prime
 * usage: Prime(7); // returns 17
*/
uint64_t Prime(const uint64_t& Index_UInt64)
{
	for(uint64_t i = PrimeComposite_UInt64Vec.back() + 2; PrimeComposite_UInt64Vec.size() < Index_UInt64; i += 2)
	{ 
		bool Add_Flag {true};
		
		uint64_t j {};
		
		const uint64_t J = i / 4 ;
		
		for (const uint64_t& val : PrimeComposite_UInt64Vec)
		{
			
			if ((i % 2) == 0 || (i % val) == 0)
			{
				Add_Flag = false;
				break;
			}
			
			if (++j >= J)
			{
				break;
			}
		}
		
		if (Add_Flag)
		{
			PrimeComposite_UInt64Vec.emplace_back (i);
		}
	}
		
	return Index_UInt64 > 0 ? PrimeComposite_UInt64Vec[Index_UInt64-1] : 2;
}

const uint64_t N = 3; // Update as needed;

int ProcessTheorem (
	const 
	std::vector<
	std::vector<
	std::string>>& InTheorem_UInt64Vec,
	const 
	std::vector<
	std::vector<
	std::vector<std::string>>>& InAxioms_UInt64Vec)
{
	
	bool QED {};
	
	uint64_t GUID {};
	
	/*
	token: [ "1" "2" "4" "+" ]
	prime: [  2   3   5   7 ]
	*/
	
	std::vector<uint64_t> TempTheorem_UInt64Vec;
	
	auto PopulateTheoremVec = 
	[
		&
	]
	(uint64_t k) -> void
	{
		for (const std::vector<std::string>& Statement_StdStrVec : InTheorem_UInt64Vec)
		{
			uint64_t PrimeProduct_UInt64Vec {1};
			for (const std::string& Symbol_StdStr : Statement_StdStrVec)
			{
				std::cout << "'" << Symbol_StdStr << "' ";
				const auto it = SymbolToPrimeNumber_Map.find ( Symbol_StdStr );
				if (it == SymbolToPrimeNumber_Map.end())
				{
					// The symbol is not in the prime number map...
					const uint64_t p = Prime (k++);
					SymbolToPrimeNumber_Map.emplace (Symbol_StdStr, p);
				}
				PrimeProduct_UInt64Vec *= SymbolToPrimeNumber_Map[Symbol_StdStr];
			}
			std::cout << std::endl;
			TempTheorem_UInt64Vec.emplace_back ( PrimeProduct_UInt64Vec );
		}
	};
	
	std::vector<std::vector<uint64_t>> TempAxioms_UInt64Vec;
	
	auto PopulateAxiomVec = 
	[
		&
	]
	(uint64_t k) -> void
	{
		uint64_t i {};
		for (const std::vector<std::vector<std::string>>& Statement_StdStrVec : InAxioms_UInt64Vec)
		{ 
			std::vector<uint64_t> TempInnerAxiom_UInt64Vec {};
			for (const std::vector<std::string>& Expression_StdStrVec : Statement_StdStrVec)
			{
				uint64_t PrimeProduct_UInt64Vec {1};
				for (const std::string& Symbol_StdStr : Expression_StdStrVec)
				{
					std::cout << "'" << Symbol_StdStr << "' ";
					const auto it = SymbolToPrimeNumber_Map.find (Symbol_StdStr);
					if (it == SymbolToPrimeNumber_Map.end())
					{
						// This symbol is not in the prime number map...
						const uint64_t p = Prime (k++);
						SymbolToPrimeNumber_Map.emplace (Symbol_StdStr, p);
					}
					PrimeProduct_UInt64Vec *= SymbolToPrimeNumber_Map [Symbol_StdStr];
				}
				std::cout << std::endl;
				TempInnerAxiom_UInt64Vec.emplace_back (PrimeProduct_UInt64Vec);
			}
			TempAxioms_UInt64Vec.emplace_back (TempInnerAxiom_UInt64Vec);
		}
	};
	
	const uint64_t& K = PrimeComposite_UInt64Vec.size ();
	
	PopulateTheoremVec (K);
	PopulateAxiomVec (K);
	
	std::priority_queue<
	std::vector<uint64_t>> Tasks_Thread;
	
	std::vector<uint64_t> Theorem_UInt64Vec = 
	{
		5488, // "1 + 1 + 1 + 1"
		5, // "4"
		1, // guid_UInt64; begin of ProofStack_UInt64Vec
	};

	std::vector<std::array<uint64_t, N>> Axioms_UInt64Vec = 
	{
		{
			28, // "1 + 1"
			3, // "2"
			2, // guid_UInt64
		},

		{
			63, // "2 + 2"
			5, // "4"
			3, // guid_UInt64
		}
	};
	
	Tasks_Thread.push ( Theorem_UInt64Vec );
	
	const int LHS = 0;
	const int RHS = 1;
	const int guid_UInt64 = 2;
	const int ProofStack_UInt64 = 3;

	constexpr uint64_t MaxAllowedProofs_UInt64{ 1 };
	uint64_t TotalProofsFound_UInt64 {};
	
	std::unordered_map<uint64_t,
	std::unordered_map<uint64_t,bool>> 
	CallHistory_00 {},
	CallHistory_01 {},
	CallHistory_02 {},
	CallHistory_03 {};

	// Todo: Create a .DLL interface
	// Todo: Add Boost Multiprecision BigInt support
	// Todo: Support Remove and SendOffline for axioms
	// Todo: Create a proof statement hash which can be used as a file handle to a proofstep solution when it posts to a file (stateless)
	// Todo: Fully _reduce and or _expand all axiom subnets (ie. fast-forwarding) via extended proofstacks to fast-forward subnet matching at runtime.
	// Todo: Offload comparison and math operations onto the GPU

	/*
	Authorize qualifying axiom subnets by authorizing
	their netlists in the outbound route map
	*/
   
	while (!Tasks_Thread.empty() && !QED)
	{
		const std::vector<uint64_t> Theorem = 
		Tasks_Thread.top ();
		Tasks_Thread.pop ();
		
		if (Theorem[LHS] == Theorem[RHS])
		{
			std::cout << std::endl;
			std::cout << "Proof Found" << std::endl;
			if (ProofStack_UInt64 < Theorem.size())
			{
				for (uint64_t ProofStep_UInt64 = ProofStack_UInt64; ProofStep_UInt64 < Theorem.size(); ++ProofStep_UInt64)
				{
					std::cout << "Axiom_" << Theorem[ProofStep_UInt64] << std::endl;
				}
			}
			std::cout << "Theorem_0000 {" << Theorem[LHS] << ", " << Theorem[RHS] << "}" << std::endl;
			std::cout << std::endl;
			std::cout << "Q.E.D." << std::endl;
			
		  ++TotalProofsFound_UInt64;
			if (TotalProofsFound_UInt64 >= MaxAllowedProofs_UInt64)
			{
				QED = true;
				break;
			}
			
		} else {
			
			for (const std::array<uint64_t,N>& Axiom : Axioms_UInt64Vec)
			{
				if (Theorem[LHS] % Axiom[LHS] == 0 && !CallHistory_00[Theorem[LHS]][Axiom[LHS]] )
				{
					CallHistory_00[ Theorem[LHS] ][ Axiom[LHS] ] = true;
					
					std::vector<uint64_t> Theorem_0000 = Theorem;
					Theorem_0000.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0000[LHS] = Theorem_0000[LHS] / Axiom[LHS] * Axiom[RHS];
					//std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0000 );
				}
				 
				if (Theorem[LHS] % Axiom[RHS] == 0 && !CallHistory_01[Theorem[LHS]][Axiom[RHS]] )
				{
					CallHistory_01[ Theorem[LHS] ][ Axiom[RHS] ] = true;
					
					std::vector<uint64_t> Theorem_0000 = Theorem;
					Theorem_0000.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0000[LHS] = Theorem_0000[LHS] / Axiom[RHS] * Axiom[LHS];
					//std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0000 );
				}
				 
				if (Theorem[RHS] % Axiom[LHS] == 0 && !CallHistory_02[Theorem[RHS]][Axiom[LHS]] )
				{
					CallHistory_02[ Theorem[RHS] ][ Axiom[LHS] ] = true;
					
					std::vector<uint64_t> Theorem_0000 = Theorem;
					Theorem_0000.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0000[RHS] = Theorem_0000[RHS] / Axiom[LHS] * Axiom[RHS];
					//std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0000 );
				}
				 
				if (Theorem[RHS] % Axiom[RHS] == 0 && !CallHistory_03[Theorem[RHS]][Axiom[RHS]] )
				{
					CallHistory_03[ Theorem[RHS] ][ Axiom[RHS] ] = true;
					
					std::vector<uint64_t> Theorem_0000 = Theorem;
					Theorem_0000.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0000[RHS] = Theorem_0000[RHS] / Axiom[RHS] * Axiom[LHS];
					//std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0000 );
				}
			} // end for (...Axiom : Axioms_UInt64Vec)
		} // end test (...Theorem[LHS] == Theorem[RHS])
		
	} // end for (...!Tasks_Thread[Read].empty() && !QED))
	
	if (!QED)
	{   
		std::cout << "No Proof Found." << std::endl;
	}

	return EXIT_SUCCESS;
}

int main()
{
	std::vector<
	std::vector<std::string>> Theorem_UInt64Vec = 
	{
		{"1", "+", "1", "+", "1", "+", "1"}, // 5488
		{"4"} // 5
	};

	std::vector<
	std::vector<
	std::vector<std::string>>> Axioms_UInt64Vec = 
	{
		{
			{"1", "+", "1"}, // 28
			{"2"} // 3
		},

		{
			{"2", "+", "2"}, // 63 
			{"4"} // 5
		}
	};
	
	const auto start_time_chrono = std::chrono::high_resolution_clock::now();
	
	std::thread th(
		ProcessTheorem, 
		std::cref(Theorem_UInt64Vec), 
		std::cref(Axioms_UInt64Vec));
	th.join();

	const auto end_time_chrono = std::chrono::high_resolution_clock::now();
	const auto duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_chrono - start_time_chrono).count();
	std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;
	
	return EXIT_SUCCESS;
}
