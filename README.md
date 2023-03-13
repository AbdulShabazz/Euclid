# EuclidAutomatedTheoremProver_
C++ Edition

## Description

Euclid is a proof engine that operates symbolically and does not support library calls or built-in libraries. However, it can be utilized in game development to derive any achievable game state from the current state without pre-training, which is typically necessary for A.I. Additionally, it can be beneficial in debugging game logic.

## Instructions

You feed the prover axioms and lemmas, pertaining to the state of your game, and then ask it to prove a theorem or desired outcome. Each proof-step can then be used to update the state of your game, 
or in advanced implementations, can be fed into a helper engine A.I. which attempts to achieve the proof-step as a desried outcome in your game. The feeding process is repeated until the game state represents the desired outcome (ie. the theorem is proven). When successful, The prover will return a path of proof-steps that prove the theorem, if it can be proven. NOTE: The prover can also return a path of proof steps that prove the theorem, using the Expand and Reduce optional solvers.

Examples

```c++

	// Create empty ProofStep[lineNumber][proofStep] vector to store proof
	std::vector<std::vector<std::string>> ProofStep;

 	// Instantiate Prover (module)
	EuclidProver<BracketType::CurlyBraces> Euclid;

	// Add axioms
	Euclid.Axiom({"{", "1", "}","+","{", "1", "}","=","{", "2", "}"}); // axiom_0
	Euclid.Axiom({ "{", "2", "}","+","{", "2", "}","=","4" }); // axiom_1

	// Add supporting lemmas
	Euclid.Lemma({ "{", "1", "}","+","{", "0", "}","=","{", "1", "}" }); // lemma_0

	// Theorem to Prove
	const std::vector<std::string> Prove = { "{", "4", "}", "=", "{", "1", "}","+","{", "1", "}","+","{", "1", "}", "+", "{", "1", "}" };

	if (Euclid.Prove(Prove, ProofStep))
	{
		std::cout << "Proof found:\n";
		Euclid.PrintPath(ProofStep);
	}
	else
	{
		std::cout << "Proof failed\n";
	}

	// Optional Solver: Expand
	std::vector<std::vector<std::string>> ProofStep;
	if (Euclid.ProveViaExpand(Prove, ProofStep))
	{
		std::cout << "Proof via Expand:\n";
		Euclid.PrintPath(ProofStep);
	}
	else
	{
		std::cout << "Proof via Expand failed\n";
	}

	// Optional Solver: Reduce
	std::vector<std::vector<std::string>> ProofStep;
	if (Euclid.ProveViaReduce(Prove, ProofStep))
	{
		std::cout << "Proof via Reduce:\n";
		Euclid.PrintPath(ProofStep);
	}
	else
	{
		std::cout << "Proof via Reduce failed\n";
	}

	
```

## Formatting

{ } = Brackets

[ ] = Brackets

( ) = Brackets

{ variable } = Variable

Example expressed as plain text: 

```c++
	// Axiom
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver } // Current Game State
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	.
	. Other available Game States, in the current environment, for the PlayerCharacterSideKick to choose from
	.
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } 

	// Lemma
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver } // These are connectives, and axiom helpers
	{ PlayerCharacterSideKick } IsOn { Vehicle } = { VehicleDriveDisabled }
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle }
	{ PlayerCharacterSideKick } IsNotIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }

	// Theorem
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } // Goal State

	// Proof-Step
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver }
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
	{ PlayerCharacterSideKick } IsNotIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
	{ PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } 

	// Theorem expressed as C++ Code
	const std::vector<std::string> Prove = { "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}","=","{", "QuadUtilityVehicle", "}","and","{", "VehicleDriveDisabled" }; 
```

## Format

The required format for the expressions is as follows:

{ LHS }... = { RHS }..., where LHS and RHS are properly-formed expressions.

The "=" is the equality operator, which is required, and should be interpreted 
as a connective to mean "and-", "therefore-", "then-" used to delimit and or separate the LHS and RHS. 
The equality operator is the only builtin operator that is supported and reserved. All other symbols may be used in your expressions.

The equality operator can also be used to separate multiple expressions, as shown:

{ LHS }... = { RHS_0000 }... = { RHS_0001 }... = { RHS_N }...,, where LHS and RHS_N are properly-formed expressions.

If you have any questions, please contact me at: https://github.com/Seagat2011
