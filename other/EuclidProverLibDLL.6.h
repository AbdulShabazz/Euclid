
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
	+Prime(k++) >> Prime([k++])
	+std::unordered_map ('symbol' == 'SYMBOL') >> std::unordered_multimap ('symbol' != 'SYMBOL')
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
	Rewrites are performed with the aid of a rewrite compiler (eg. via LEMMA SUBSTITUTION); SEE TEST CASES

	Substitution methods:
	
		1. AXIOMATIC: 1 + 1 = 2
		2. LEMMA SUBSTITUTION: 1 <==> 1 / 1
		
	Note: Lemma substitutions are rewrite "helpers" which can be used to rewrite axioms. 
	Great care must be taken with them because they can introduce recursion, stack overflows, 
	and other performance bugs: For example, consider: "{ PlayerCharacterSideKick } IsIn { StyxBoat }" - 
	the "IsIn" operator may or may not link unrelated categories, indefinitely;
	whereas: "{ PlayerCharacterSideKick } IsIn_StyxBoat " is safer and guaranteed to converge.

  Usage Example.

	// Lemmas
	{ { a } raised { 2 } } plus { 2ab } plus { b raised { 2 } } <== ( { a } plus { b } ) raised { 2 }
	( { a } plus { b } ) raised { 2 } minus { 2ab } = { c } raised { 2 } <== ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
	{ { a } raised { 2 } } plus { 2ab } minus { 2ab } plus { b raised { 2 } } ==> { { a } raised { 2 } } plus { { b } raised { 2 } }
	
	// Axioms
	( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
	
	// Theorem
	Prove { { a } raised { 2 } } plus { { b } raised { 2 } } = { c } raised { 2 }

  Usage Example.

	// Axioms
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver } // Current Game State
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	.
	. [Other available but non-relevant Game States for the framework to choose from ]
	.
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

	// Lemmas
	{ PlayerCharacterSideKick } IsIn { StyxBoat } <== { StyxBoat } IsNotIn { StyxRiver } // These are connectives, and axiom helpers
	{ PlayerCharacterSideKick } IsOn { Vehicle } <== { VehicleDriveDisabled }
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } <== { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle }
	{ PlayerCharacterSideKick } IsNotIn { StyxBoat } ==> { StyxBoat } IsNotIn { StyxRiver }

	// Theorem
	Prove { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }

	// Theorem to prove (as a C++23 std::vector)
	std::vector<std::string>> InTheoremToProve_StdStrVec  = { 
		"{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", // lhs //
		"{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}" // rhs //
	};
	
	// Proof-Steps (Output)
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

std::unordered_multimap<
std::string, uint64_t> 
SymbolToPrime_UInt64MultiMap = 
{
	{"=", 2},
	{"{", 3},
	{"}", 5},
	{"(", 7},
	{")", 11},
	{"[", 13},
	{"]", 17}
};

uint64_t PrimeCompositeVecSize_UInt64 {7};

std::vector<uint64_t> PrimeComposite_UInt64Vec { 2, 3, 5, 7, 11, 13, 17 };

/**
 * Prime() : Return the next prime in the series...
 * usage: Prime(); // returns 23
*/
uint64_t Prime()
{
	const uint64_t Index_UInt64 = PrimeCompositeVecSize_UInt64++;
	for(uint64_t i = PrimeComposite_UInt64Vec.back() + 2; PrimeComposite_UInt64Vec.size() < PrimeCompositeVecSize_UInt64; i += 2)
	{ 
		bool Add_Flag {true};
		
		uint64_t j {};
		
		const uint64_t J = i / 4 ;
		
		for (const uint64_t& val : PrimeComposite_UInt64Vec)
		{
			
			if (/*(i % 2) == 0 ||*/ (i % val) == 0)
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
		
	return PrimeComposite_UInt64Vec[Index_UInt64];
}

int ProcessTheorem ( // Generate Route Map //
	const 
	std::vector<
	std::vector<
	std::string>>& InTheorem_UInt64Vec,
	const 
	std::vector<
	std::vector<
	std::vector<
	std::string>>>& InAxioms_UInt64Vec)
{
	
	bool QED {};
	
	uint64_t GUID_UInt64 {1};
	
	/*
	token: [ "1" "2" "4" "+" ]
	prime: [  2   3   5   7 ]
	*/
	
	std::vector<uint64_t> Theorem_UInt64Vec;
	
	auto PopulateTheoremVec = 
	[
		&
	]
	() -> void
	{
		for (const std::vector<std::string>& Subnet_StdStrVec : InTheorem_UInt64Vec)
		{
			uint64_t PrimeProduct_UInt64Vec {1};
			for (const std::string& Symbol_StdStr : Subnet_StdStrVec)
			{
				 std::cout << "'" << Symbol_StdStr << "' ";
				const auto& it = SymbolToPrime_UInt64MultiMap.find (Symbol_StdStr);
				if (it != SymbolToPrime_UInt64MultiMap.end())
				{
					PrimeProduct_UInt64Vec *= it->second;
					std::cout << "Prime: " << Symbol_StdStr << "->" << it->second << ", PrimeProduct: " << PrimeProduct_UInt64Vec << std::endl;
				} else {
					// Symbol is not in the prime number multimap...
					const uint64_t p = Prime ();
					SymbolToPrime_UInt64MultiMap.emplace (Symbol_StdStr, p);
					PrimeProduct_UInt64Vec *= p;
					std::cout << "New Prime: " << Symbol_StdStr << "->" << p << ", PrimeProduct: " << PrimeProduct_UInt64Vec << std::endl;
				}
			}
			 std::cout << std::endl;
			Theorem_UInt64Vec.emplace_back ( PrimeProduct_UInt64Vec );
		}
		Theorem_UInt64Vec.emplace_back (1); // guid
	};
	
	std::vector<std::vector<uint64_t>> Axioms_UInt64Vec;
	
	auto PopulateAxiomVec = 
	[
		&
	]
	() -> void
	{
		for (const std::vector<std::vector<std::string>>& Subnet_StdStrVec : InAxioms_UInt64Vec)
		{ 
			std::vector<uint64_t> TempInnerAxiom_UInt64Vec {};
			for (const std::vector<std::string>& Expression_StdStrVec : Subnet_StdStrVec)
			{
				uint64_t PrimeProduct_UInt64Vec {1};
				for (const std::string& Symbol_StdStr : Expression_StdStrVec)
				{
					 std::cout << "'" << Symbol_StdStr << "' ";
					const auto& it = SymbolToPrime_UInt64MultiMap.find (Symbol_StdStr);
					if (it != SymbolToPrime_UInt64MultiMap.end())
					{
						PrimeProduct_UInt64Vec *= it->second;
						std::cout << "Prime: " << Symbol_StdStr << "->" << it->second << ", PrimeProduct: " << PrimeProduct_UInt64Vec << std::endl;
					} else {
						// Symbol is not in the prime number multimap...
						const uint64_t p = Prime ();
						SymbolToPrime_UInt64MultiMap.emplace (Symbol_StdStr, p);
						PrimeProduct_UInt64Vec *= p;
						std::cout << "New Prime: " << Symbol_StdStr << "->" << p << ", PrimeProduct: " << PrimeProduct_UInt64Vec << std::endl;
					}
				}
				 std::cout << std::endl;
				TempInnerAxiom_UInt64Vec.emplace_back (PrimeProduct_UInt64Vec);
			}
			TempInnerAxiom_UInt64Vec.emplace_back (++GUID_UInt64); // guid
			Axioms_UInt64Vec.emplace_back (TempInnerAxiom_UInt64Vec);
		}
	};
	
	auto OrientateTheoremVec = 
	[
		&
	]
	() -> void
	{
		uint64_t& lhs = Theorem_UInt64Vec[0];
		uint64_t& rhs = Theorem_UInt64Vec[1];
	
		if (lhs < rhs)
		{
			std::swap(lhs,rhs);
		}
	};
	
	auto OrientateAxiomVec = 
	[
		&
	]
	() -> void
	{
		for (std::vector<uint64_t>& Axiom_i : Axioms_UInt64Vec)
		{
			uint64_t& lhs = Axiom_i[0];
			uint64_t& rhs = Axiom_i[1];
	
			if (lhs < rhs)
			{
				std::swap(lhs,rhs);
			}
		}
	};
	
	PopulateTheoremVec();
	PopulateAxiomVec();
	
	OrientateTheoremVec();
	OrientateAxiomVec();
	
	/*
	std::vector<uint64_t> Theorem_UInt64Vec = 
	{
		5488, // "1 + 1 + 1 + 1"
		5, // "4"
		1, // guid_UInt64; 
		...begin of ProofStack_UInt64Vec
	};

	std::vector<std::vector<uint64_t>> Axioms_UInt64Vec = 
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
	*/
	
	std::priority_queue<
	std::vector<uint64_t>> Tasks_Thread;
	
	Tasks_Thread.push (Theorem_UInt64Vec);
	
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
	CallHistory_03 {}; // Netlist

	// Todo: Create a .DLL interface
	// Todo: Add Boost Multiprecision BigInt support to Offload the math operations onto the GPU
	// Todo: Support Remove and Offline operations for individual axioms
	// Todo: Create a proof statement hash which can be used as a file handle to a proofstep solution when it posts to a file (stateless)
	// Todo: Fully _reduce or _expand all axiom subnets (ie. employ fast-forwarding) via extended proofstacks to fast-forward subnet matching at runtime.

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
			bool lhs_Flag = false;
			for (const std::vector<std::string>& Subnet_StdStrVec : InTheorem_UInt64Vec)
			{
				if (lhs_Flag)
				{
					std::cout << "= ";
				}
				
				lhs_Flag = true;
				
				for(const std::string& Symbol_StdStr : Subnet_StdStrVec)
				{
					std::cout << Symbol_StdStr << " "; 
				}
			}
			
			std::cout << std::endl;
			
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
			
			for (const std::vector<uint64_t>& Axiom : Axioms_UInt64Vec)
			{
				if (Theorem[LHS] % Axiom[LHS] == 0/* && !CallHistory_00[Theorem[LHS]][Axiom[LHS]]*/ )
				{
					CallHistory_00.emplace(Theorem[LHS], std::unordered_map<uint64_t,bool>{{Axiom[LHS], true}});
					
					std::vector<uint64_t> Theorem_0000 {Theorem};
					Theorem_0000.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0000[LHS] = Theorem_0000[LHS] / Axiom[LHS] * Axiom[RHS];
					std::cout << "Module_0000 {" << Theorem_0000[LHS] << ", " << Theorem_0000[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0000 );
				}
				 
				if (Theorem[LHS] % Axiom[RHS] == 0/* && !CallHistory_01[Theorem[LHS]][Axiom[RHS]]*/ )
				{
					CallHistory_01.emplace(Theorem[LHS], std::unordered_map<uint64_t,bool>{{Axiom[RHS], true}});
					
					std::vector<uint64_t> Theorem_0001 {Theorem};
					Theorem_0001.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0001[LHS] = Theorem_0001[LHS] / Axiom[RHS] * Axiom[LHS];
					std::cout << "Module_0001 {" << Theorem_0001[LHS] << ", " << Theorem_0001[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0001 );
				}
				 
				if (Theorem[RHS] % Axiom[LHS] == 0 /*&& !CallHistory_02[Theorem[RHS]][Axiom[LHS]]*/ )
				{
					CallHistory_02.emplace(Theorem[RHS], std::unordered_map<uint64_t,bool>{{Axiom[LHS], true}});
					
					std::vector<uint64_t> Theorem_0002 {Theorem};
					Theorem_0002.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0002[RHS] = Theorem_0002[RHS] / Axiom[LHS] * Axiom[RHS];
					std::cout << "Module_0002 {" << Theorem_0002[LHS] << ", " << Theorem_0002[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0002 );
				}
				 
				if (Theorem[RHS] % Axiom[RHS] == 0 /*&& !CallHistory_03[Theorem[RHS]][Axiom[RHS]] */)
				{
					CallHistory_03.emplace(Theorem[RHS], std::unordered_map<uint64_t,bool>{{Axiom[RHS], true}});
					
					std::vector<uint64_t> Theorem_0003 {Theorem};
					Theorem_0003.emplace_back ( Axiom[guid_UInt64] ); // Push the Axiom ID onto proofstack //
					Theorem_0003[RHS] = Theorem_0003[RHS] / Axiom[RHS] * Axiom[LHS];
					std::cout << "Module_0003 {" << Theorem_0003[LHS] << ", " << Theorem_0003[RHS] << "}" << std::endl;
					
					Tasks_Thread.push ( Theorem_0003 );
				}
				std::cout << std::endl;
			} // end for (...Axiom : InAxioms_UInt64Vec)
		} // end test (...Theorem[LHS] == Theorem[RHS])
		
	} // end for (...!Tasks_Thread.empty() && !QED))
	
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
