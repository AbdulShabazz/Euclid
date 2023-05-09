# EuclidAutomatedTheoremProver_
C++23 Edition

## Description

Euclid is a proof engine that operates symbolically and does not support library calls or built-in libraries. However, it can be utilized in game development to derive any achievable game-state, given the current state without pre-training, which is typically required for A.I. If successful, the prover will provide a path of proof-steps that prove the theorem, if it is provable, which can be used in your game.

Additionally, Euclid can be beneficial in debugging game logic.

## Instructions

To deploy the prover in your game logic, you must provide it with axioms and lemmas related to the current state of your game, and request it to prove a theorem or desired outcome. Each proof-step can be used to perhaps update a state-machine, or update the game state; or in more advanced implementations, its proof-steps can be utilized by helper A.I. that aim to achieve the proof-step as a desired outcome in your game. This feeding process is repeated until the game state reflects the final proof, as a desired outcome (i.e., the theorem is proven). The prover can also produce a path of proof-steps using the available Expand- and Reduce- solvers.

To compile the boost library, in for example Visual Studio, go to Project > [ProjectName] Properties > VC++ Directories > General > Include Directories > and include one of the macros: `$(Local[ProjectName]WorkingDirectory)` or `$(ProjectDirectory)`. If you do not have the latest boost library in your local directory, you must first download the boost library from https://www.boost.org/ and extract it to the local directory. 

Compiler flags: ```/permissive- /Yu"pch.h" /ifcOutput "x64\Release\" /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /Ob2 /sdl /Fd"x64\Release\vc143.pdb" /Zc:inline /fp:precise /D "NDEBUG" /D "EUCLIDPROVERLIBDLL_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /std:c17 /Gd /Oy /Oi /MD /std:c++20 /FC /Fa"x64\Release\" /EHsc /nologo /Fo"x64\Release\" /Ot /Fp"x64\Release\EuclidProverLibDLL.pch" /diagnostics:column```

## Formatting

{ } = Brackets

[ ] = Brackets

( ) = General Use Brackets

{ variable }

[ variable ]

( variable )

Usage Example pseudo code: 

```
  // Axioms
  { PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver } // Current Game State
  { PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
  { PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
  { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
  { PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
  .
  . Other available but non-relavant Game States, in the current environment, for the PlayerCharacterSideKick to choose from
  .
  { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } 

  // Lemmas
  { PlayerCharacterSideKick } IsIn { StyxBoat } ==> { StyxBoat } IsNotIn { StyxRiver } // These are connectives, and axiom rewrite helpers
  { PlayerCharacterSideKick } IsOn { Vehicle } <== { VehicleDriveDisabled }
  { PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } ==> { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle }
  { PlayerCharacterSideKick } IsNotIn { StyxBoat } <==> { StyxBoat } IsNotIn { StyxRiver }

  // Theorem to prove
  Prove { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } // Goal Game State

  // Proof-Step
  { PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver }
  { PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
  { PlayerCharacterSideKick } IsNotIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
  { PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
  { PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
  { PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
  { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
  { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }
```

Usage Example C++20 Code:

```c++
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <Euclid.h>

int main()
{
  using EuclidProverClass = 

  Euclid_Prover::EuclidProver<
  Euclid_Prover::BracketType::CurlyBraces>;

  // Instantiate Prover (module)
  EuclidProverClass Euclid;

  Euclid.Axioms
  (
    {
      // Axiom_1
      {
        {"1", "+", "1"}, // (lhs) Prime Composite: 8303 //
        {"2"} // (rhs) Prime Composite: 31 //
      },

      // Axiom_2
      {
        {"2", "+", "2"}, // (lhs) Prime Composite: 22103 //
        {"4"} // (rhs) Prime Composite: 29 //
      }
    }
  );

  /*

  // OR...

  Euclid.Axiom
  (
    // Axiom_1
    {
      {"1", "+", "1"}, // (lhs) Prime Composite: 8303 //
      {"2"} // (rhs) Prime Composite: 31 //
    },
  );

  Euclid.Axiom
  (
    // Axiom_2
    {
      {"2", "+", "2"}, // (lhs) Prime Composite: 22103 //
      {"4"} // (rhs) Prime Composite: 29 //
    }
  );
  */

  std::vector<
  std::vector<
  std::vector<
  std::vector<
  std::string>>>>

  // Instantiate ProofStep_4DStdStrVec[proof][step][lhs/rhs][token]
  ProofStep_4DStdStrVec;

  std::vector<
  std::vector<
  std::string>>

  // Instantiate AxiomCommitLog_StdStrVec[proof][step]
  AxiomCommitLog_StdStrVec;

  const auto start_time_chrono = std::chrono::high_resolution_clock::now();

  Euclid.Prove
  (
    {
      {"1", "+", "1", "+", "1", "+", "1"}, // (lhs) Prime Composite: 1585615607 //
      {"4"}, // (rhs) Prime Composite: 29 //
    },

    ProofStep_4DStdStrVec,
    AxiomCommitLog_StdStrVec
  );

  
  while (!Euclid.StatusReady)
  {
    //std::cout << "Performing some other work..." << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  if (Euclid.ProofFound_Flag)
  {
    std::cout << "Proof Found." << std::endl;
    ProofStep_4DStdStrVec;
    AxiomCommitLog_StdStrVec;
  } else if (ProofStep_4DStdStrVec.size()) {
    std::cout << "Partial Proof Found." << std::endl;
    ProofStep_4DStdStrVec;
    AxiomCommitLog_StdStrVec;
  } else {
    std::cout << "No Proof Found." << std::endl;
  }

  std::cout << std::endl;
  const auto end_time_chrono = std::chrono::high_resolution_clock::now();
  const auto duration_chrono = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_chrono - start_time_chrono).count();
  std::cout << "Total Duration (nanoseconds): " << duration_chrono << std::endl;

  // Hold for user-input //
  std::string inChar;
  std::cin >> inChar;

  return EXIT_SUCCESS;
}

/*
Output:

ProofStep_4DStdStrVec:
{
  {
    // Step 1 (Original).
    {
      {"1", "+", "1", "+", "1", "+", "1"}, // (lhs) Prime Composite: 1585615607 //
      {"4"} // (rhs) Prime Composite: 29 //
    },

    // Step 2. (rhs_expand via Axiom_2)
    {
      {"1", "+", "1", "+", "1", "+", "1"}, // (lhs) Prime Composite: 1585615607 //
      {"2", "+", "2"} // (rhs) Prime Composite: 22103 //
    },
    
    // Step 3. (rhs_expand via Axiom_1)
    {
      {"1", "+", "1", "+", "1", "+", "1"}, // (lhs) Prime Composite: 1585615607 //
      {"1", "+", "1", "+", "2"} // (rhs) Prime Composite: 5920039 //
    },
    
    // Step 4. (rhs_expand via Axiom_1)
    {
      {"1", "+", "1", "+", "1", "+", "1"}, // (lhs) Prime Composite: 1585615607 //
      {"1", "+", "1", "+", "1", "+", "1"} // (rhs) Prime Composite: 1585615607 (QED) //
    }
  }
}

AxiomCommitLog_StdStrVec:
{
  {
    "rhs_expand via Axiom_2",
    "rhs_expand via Axiom_1",
    "rhs_expand via Axiom_1"
  }
}
*/

```

Usage Example C++20 code

```c++
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <Euclid.h>

using EuclidProverClass = 

Euclid_Prover::EuclidProver<
Euclid_Prover::BracketType::CurlyBraces>;

void SkipTaskForNow
(
  const
  EuclidProverClass&
  EuclidConstRefObj,
  
  const
  auto&
  InProofStep_4DStdStrVec,
  
  const
  auto&
  InAxiomCommitLog_StdStrVec
)
{
  // Perform some other work... //
  std::this_thread::sleep_for(std::chrono::seconds(1));

  while (!EuclidConstRefObj.StatusReady)
  {
    //std::cout << "Performing some other work..." << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  if (EuclidConstRefObj.ProofFound_Flag)
  {
    std::cout << "Proof Found." << std::endl;
    InProofStep_4DStdStrVec;
    InAxiomCommitLog_StdStrVec;
  } else if (ProofStep_4DStdStrVec.size()) {
    std::cout << "Partial Proof Found." << std::endl;
    InProofStep_4DStdStrVec;
    InAxiomCommitLog_StdStrVec;
  } else {
    std::cout << "No Proof Found." << std::endl;
  }
}

int main()
{
  // Instantiate Prover (module)
  EuclidProverClass Euclid;

  Euclid.Axioms
  (
    {
      // Axiom_1
      {
        {
          "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "StyxBoat", "}" 
        }, // lhs //
        
        {
          "{", "StyxBoat", "}", "IsIn", "{", "StyxRiver", "}" 
        } // rhs //
      }, 
    .
    .
    }
  );
  
  Euclid.Lemmas
  (
    {
      // Lemma_1
      {
        {
          "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "StyxBoat", "}" 
        }, // lhs //
      
        {
          "{", "StyxBoat", "}", "IsNotIn", "{", "StyxRiver", "}" // These are connectives, and axiom helpers
        } // rhs //
      },
    .
    .
    }
  );

  Euclid.Prove
  (
    {
      {
        "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}"
      }, // lhs//
    
      {
        "{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}"
      } // rhs //
    }

    ProofStep_4DStdStrVec,
    AxiomCommitLog_StdStrVec
  );
  .
  .
  if (Euclid.StatusReady)
  {
    if (Euclid.ProofFound_Flag)
    {
      std::cout << "Proof Found." << std::endl;
      ProofStep_4DStdStrVec;
      AxiomCommitLog_StdStrVec;
    } else if (ProofStep_4DStdStrVec.size()) {
      std::cout << "Partial Proof Found." << std::endl;
      ProofStep_4DStdStrVec;
      AxiomCommitLog_StdStrVec;
    } else {
      std::cout << "No Proof Found." << std::endl;
    }
  } else {
    SkipTaskForNow(Euclid, ProofStep_4DStdStrVec, AxiomCommitLog_StdStrVec); // recheck later //
  }

  return EXIT_SUCCESS;
}
```
## Format

The required format for the expressions is as follows:

{ LHS }... = { RHS }..., where LHS and RHS are properly-formed sets of expressions.

The "=" is the equality operator, its required, and should be interpreted as a connective, used to delimit and or separate the LHS and RHS. 
The equality operator is the only builtin operator that is reserved. All other symbols may be used in your expressions.

The equality operator can also be used to separate multiple expressions, as shown:

{ LHS }... = { RHS_0000 }... = { RHS_0001 }... = { RHS_N }..., where LHS and RHS_N are properly-formed expressions.

If you have any questions, please contact me at: https://github.com/Seagat2011/UnrealEngine-Seagat2011-2023

Compatibility: C++20 (Windows x86 i64)

STYLEGUIDE

	POOR FORMATTING

		TEST CASE: RENDER [PASS], PROOF [FAIL]
		({a}plus{b})raised{2}={{c}raised{2}}plus{2ab}
		{{a}raised{2}}plus{2ab}plus{b raised{2}}<==({a}plus{b})raised{2}
		({a}plus{b})raised{2}minus{2ab}={c}raised{2}<==({a}plus{b})raised{2}={{c}raised{2}}plus{2ab}
		{{a}raised{2}}plus{2ab}minus{2ab}plus{b raised{2}}==>{{a}raised{2}}plus{{b}raised{2}}
		Prove {{a}raised{2}}plus{{b}raised{2}}={c}raised{2}

	GOOD FORMATTING

		TEST CASE [PASS]

		// Axioms
		( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }

		// Lemmas
		{ { a } raised { 2 } } plus { 2ab } plus { b raised { 2 } } <== ( { a } plus { b } ) raised { 2 }
		( { a } plus { b } ) raised { 2 } minus { 2ab } = { c } raised { 2 } <== ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
		{ { a } raised { 2 } } plus { 2ab } minus { 2ab } plus { b raised { 2 } } ==> { { a } raised { 2 } } plus { { b } raised { 2 } }

		Prove { { a } raised { 2 } } plus { { b } raised { 2 } } = { c } raised { 2 }

		TEST CASE [PASS]
		primes = { a } raised { 2 } + { b } raised { 2 } , where (a,b) in setz
		{ 1 } mod { 4 } = { a } raised { 2 } + { b } raised { 2 }
		Prove primes = { 1 } mod { 4 }

		TEST CASE [PASS]
		{ 1 } { + } { 1 } = { 2 }
		{ 2 } { + } { 2 } = { 4 }
		{ 4 } { + } { 2 } = { 6 }
		{ 1 } { + } { 1 } { + } { 1 } { + } { 1 } { + } { 1 } { + } { 1 } = { 2 }
		Prove { 1 } { + } { 2 } { + } { 2 } { + } { 1 } = { 6 }

		TEST CASE [PASS]
		( { a } + { b } ) ^ { 2 } = { { c } ^ { 2 } } + { 2ab }
		{ { a } ^ { 2 } } + { 2ab } + { b ^ { 2 } } = ( { a } + { b } ) ^ { 2 }
		( { a } + { b } ) ^ { 2 } - { 2ab } = { c } ^ { 2 }
		{ { a } ^ { 2 } } + { 2ab } + { b ^ { 2 } } - { 2ab } = { { a } ^ { 2 } } + { { b } ^ { 2 } }
		Prove { { a } ^ { 2 } } + { { b } ^ { 2 } } = { c } ^ { 2 }

	SYMBOL LIBRARY

		ref: https://documentation.libreoffice.org

PROOFGUIDE

	AXIOM FORMAT

		{ LHS... }... = { RHS... }... [ = { RHS_N... }... ]*
		.
		.

	LEMMA SUBSTITUTION FORMAT

		{ LHS... }... <== or <==> or ==> { RHS... }... [ <== or <==> or ==> { RHS_N... }... ]*
		.
		.

	PROOF FORMAT
		Prove { LHS... }... = { RHS... }... [ = { RHS_N... }... ]*

	QUICK FORMAT

		TEST CASE [PASS]
		primes = a raised 2 + b raised 2
		1 mod 4 = a raised 2 + b raised 2
		Prove primes = 1 mod 4

	EFFICIENT SCOPED FORMAT

		TEST CASE [PASS]
		{ a } raised { 2 } + { b } raised { 2 } = primes
		{ a } raised { 2 } + { b } raised { 2 } = { 1 } mod { 4 }
		Prove { 1 } mod { 4 } = primes
