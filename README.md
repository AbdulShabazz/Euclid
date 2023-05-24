# EuclidAutomatedTheoremProver_
C++23 Edition

## Description

Euclid is a proof engine that operates symbolically and does not support library calls or have built-in libraries. However, it can be utilized in game development to derive any achievable game-state, given the current state without pre-training, which is typically required for A.I. If successful, the prover will provide a path of proof-steps that prove the theorem, if it is provable, which can be used in your game.

Additionally, Euclid can be beneficial in debugging game logic.

## Instructions

To deploy the prover in your game logic, you must provide it with axioms and lemmas related to the current state of your game, and request it to prove a theorem or desired outcome. Each proof-step can be used to perhaps invalidate a state-machine, or update the game state; or in more advanced implementations, its proof-steps can be utilized by helper A.I. that aim to achieve the proof-step as a desired outcome in your game. This feeding process is repeated until the game state reflects the final proofstep in the game (i.e. the theorem is proven). The prover can also produce a path of proof-steps using the available Expand- and Reduce- solvers.

To compile the boost library, in for example Visual Studio, go to Project > [ProjectName] Properties > VC++ Directories > General > Include Directories > and include one of the macros: `$(Local[ProjectName]WorkingDirectory)` or `$(ProjectDirectory)`. If you do not have the latest boost library in your local directory, you must first download the boost library from https://www.boost.org/ and extract it to the local directory. 

Compiler flags: ```/permissive- /Yu"pch.h" /ifcOutput "x64\Release\" /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /Ob2 /sdl /Fd"x64\Release\vc143.pdb" /Zc:inline /fp:precise /D "NDEBUG" /D "EUCLIDPROVERLIBDLL_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /std:c17 /Gd /Oy /Oi /MD /std:c++20 /FC /Fa"x64\Release\" /EHsc /nologo /Fo"x64\Release\" /Ot /Fp"x64\Release\EuclidProverLibDLL.pch" /diagnostics:column```

You won't need Euclid.cpp until you intend to compile the header file as part of a .DLL, in which case you will need to add both files to your .DLL project, then enable the EUCLIDPROVERLIBDLL_EXPORTS preprocessor directive in your project, and uncomment all occurences of the API_EXPORT directive in the header file.

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
#include <chrono>
#include <thread>
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

    const auto start_time_chrono = std::chrono::high_resolution_clock::now();

    Euclid.Prove
    (
        {
          {"1", "+", "1", "+", "1", "+", "1"}, // (lhs) Prime Composite: 1585615607 //
          {"4"}, // (rhs) Prime Composite: 29 //
        }
    );

    std::cout << "Performing some other task..." << std::endl; // std::this_thread::sleep_for(std::chrono::seconds(1));

    /*
    if (Euclid.StatusReady())
    {
        if (Euclid.ProofFoundFlag)
        {
            std::cout << "Proof Found. (QED)" << std::endl;
            Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
            Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
        } else if (Euclid.ProofStep3DStdStrVec.size()) {
            std::cout << "Partial Proof Found." << std::endl;
            Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
            Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
        } else {
            std::cout << "No Proof Found." << std::endl;
        }
    } else {
        std::cout << "No Proof Found." << std::endl;
    }
    */

    while (!Euclid.StatusReadyFlag)
    {
        std::this_thread::yield();
    }

    if (Euclid.ProofFoundFlag)
    {
        std::cout << "Proof Found. (QED)" << std::endl;
        Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
        Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
    } else if (Euclid.ProofStep3DStdStrVec.size()) {
        std::cout << "Partial Proof Found." << std::endl;
        Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
        Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
    } else {
        std::cout << "No Proof Found." << std::endl;
    }

    // Suspend a proof for current (GUID)
    const BigInt128_t guid = Euclid.Suspend();
    std::cout << "Proof suspended for: guid_" << guid << std::endl;

    // Resume a proof for (GUID)
    if(Euclid.Resume(guid))
    {
        std::cout << "Proof resumed for: guid_" << guid << std::endl;
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
      {"{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}"}, // lhs//
    
      {"{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}"} // rhs //
    }
  );

    std::cout << "Performing some other task..." << std::endl; // std::this_thread::sleep_for(std::chrono::seconds(1));

    /*
    if (Euclid.StatusReady())
    {
        if (Euclid.ProofFoundFlag)
        {
            std::cout << "Proof Found. (QED)" << std::endl;
            Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
            Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
        } else if (Euclid.ProofStep3DStdStrVec.size()) {
            std::cout << "Partial Proof Found." << std::endl;
            Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
            Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
        } else {
            std::cout << "No Proof Found." << std::endl;
        }
    } else {
        std::cout << "No Proof Found." << std::endl;
    }
    */

    while (!Euclid.StatusReadyFlag)
    {
        std::this_thread::yield();
    }
    
    if (Euclid.ProofFoundFlag)
    {
        std::cout << "Proof Found. (QED)" << std::endl;
        Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
        Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
    } else if (Euclid.ProofStep3DStdStrVec.size()) {
        std::cout << "Partial Proof Found." << std::endl;
        Euclid.PrintPath(Euclid.ProofStep3DStdStrVec);
        Euclid.PrintPath(Euclid.AxiomCommitLogStdStrVecRef);
    } else {
        std::cout << "No Proof Found." << std::endl;
    }
  
  return EXIT_SUCCESS;
}
```
