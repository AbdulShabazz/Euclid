
/*

  AUTHOR
	  Seagat2011 
	  
	  github.com/Seagat2011
	  eterna.cmu.edu/web/player/90270/
	  fold.it/port/user/1992490

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
#include <deque>
#include <string>
#include <unordered_map>

int main()
{
	const auto start_time_chrono = std::chrono::high_resolution_clock::now();

	/*
	token: [ "1" "2" "4" "+" ]
	prime: [  2   3   5   7 ]
	*/
	
	bool QED {};

	const uint64_t N = 2; // Update as needed;

	std::array < uint64_t, 2 > Axioms_Vec;

	//std::unordered_map <uint64_t, Axiom_Struct> Axioms_LibraryMap;
	
	std::array<uint64_t, 2> Theorem_UInt64Vec = 
	{
		5488, // "1 + 1 + 1 + 1"
		5 // "4"
	};

	std::vector<std::array<uint64_t, 2>> Axioms_UInt64Vec = 
	{
		{
			28, // "1 + 1"
			3 // "2"
		},

		{
			63, // "2 + 2"
			5 // "4"
		}
	};
	
	std::deque<
	std::array<uint64_t, 2 >> Tasks_Thread;
	
	Tasks_Thread.push_back ( Theorem_UInt64Vec );
	
	constexpr int LHS = 0;
	constexpr int RHS = 1;

	constexpr uint64_t MaxAllowedProofs_UInt64{ 1 };
	uint64_t TotalProofsFound_UInt64 {};
	
	std::unordered_map<uint64_t,
	std::unordered_map<uint64_t,bool>> CallHistory {};
	
	while (!Tasks_Thread.empty() && !QED)
	{
		const std::array<uint64_t, 2 >& Theorem = 
		Tasks_Thread.front();
		Tasks_Thread.pop_front();
		
		if (Theorem[LHS] == Theorem[RHS])
		{
			std::cout << std::endl;
			std::cout << "Proof Found" << std::endl;
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
			
			for (const std::array<uint64_t,2>& Axiom : Axioms_UInt64Vec)
			{
				const bool lhs_mod_lhs_flag = Theorem[LHS] % Axiom[LHS] == 0;
				const bool lhs_mod_rhs_flag = Theorem[LHS] % Axiom[RHS] == 0;
				const bool rhs_mod_lhs_flag = Theorem[RHS] % Axiom[LHS] == 0;
				const bool rhs_mod_rhs_flag = Theorem[RHS] % Axiom[RHS] == 0;
				
				const bool lhs_call_lhs_flag = CallHistory[ Theorem[LHS] ][ Axiom[LHS] ] == false;
				const bool lhs_call_rhs_flag = CallHistory[ Theorem[LHS] ][ Axiom[RHS] ] == false;
				const bool rhs_call_lhs_flag = CallHistory[ Theorem[RHS] ][ Axiom[LHS] ] == false;
				const bool rhs_call_rhs_flag = CallHistory[ Theorem[RHS] ][ Axiom[RHS] ] == false;
				
				if (lhs_mod_lhs_flag && lhs_call_lhs_flag)
				{
					CallHistory[ Theorem[LHS] ][ Axiom[LHS] ] = true;
					
					std::array<uint64_t, 2> Theorem_0000 = Theorem;
					Theorem_0000[LHS] = Theorem_0000[LHS] / Axiom[LHS] * Axiom[RHS];
					std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push_back ( Theorem_0000 );
				}
				else
				if (lhs_mod_rhs_flag && lhs_call_rhs_flag)
				{
					CallHistory[ Theorem[LHS] ][ Axiom[RHS] ] = true;
					
					std::array<uint64_t, 2> Theorem_0000 = Theorem;
					Theorem_0000[LHS] = Theorem_0000[LHS] / Axiom[RHS] * Axiom[LHS];
					std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push_back ( Theorem_0000 );
				}
				else
				if (rhs_mod_lhs_flag && rhs_call_lhs_flag)
				{
					CallHistory[ Theorem[RHS] ][ Axiom[LHS] ] = true;
					
					std::array<uint64_t, 2> Theorem_0000 = Theorem;
					Theorem_0000[RHS] = Theorem_0000[RHS] / Axiom[LHS] * Axiom[RHS];
					std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push_back ( Theorem_0000 );
				}
				else
				if (rhs_mod_rhs_flag && rhs_call_rhs_flag)
				{
					CallHistory[ Theorem[RHS] ][ Axiom[RHS] ] = true;
					
					std::array<uint64_t, 2> Theorem_0000 = Theorem;
					Theorem_0000[RHS] = Theorem_0000[RHS] / Axiom[RHS] * Axiom[LHS];
					std::cout << "Theorem_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push_back ( Theorem_0000 );
				}
			} // end for (...Axiom : Axioms_UInt64Vec)
		} // end test (...Theorem[LHS] == Theorem[RHS])
		
	} // end for (...!Tasks_Thread[Read].empty() && !QED))
	
	if (!QED)
	{   
		std::cout << "No Proof Found." << std::endl;
	}

	const auto end_time_chrono = std::chrono::high_resolution_clock::now();
	const auto duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_chrono - start_time_chrono).count();
	std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;

	return EXIT_SUCCESS;
}
