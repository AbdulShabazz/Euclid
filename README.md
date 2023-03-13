# EuclidAutomatedTheoremProver_
C++ Edition

## Description

Euclid is a proof engine that operates symbolically and does not support library calls or built-in libraries. However, it can be utilized in game development to derive any achievable game-state, given the current state without pre-training, which is typically required for A.I. If successful, the prover will provide a path of proof-steps that prove the theorem, if it is provable, which can be used in your game.

Additionally, Euclid can be beneficial in debugging game logic.

## Instructions

To deploy the prover in your game logic, you provide it with axioms and lemmas related to the state of your game, and then request it to prove a theorem or desired outcome. Each proof-step can be used to perhaps update a state-machine, or update the game state; or in more advanced implementations, its proof-steps can be utilized by helper A.I. that aim to achieve the proof-step as a desired outcome in your game. This feeding process is repeated until the game state reflects the final proof, as a desired outcome (i.e., the theorem is proven). The prover can also produce a path of proof-steps using the available Expand- and Reduce- solvers.

Example expressed as C++ Code

```c++

	// Create empty ProofStep[lineNumber][proofStep] vector to store proof
	std::vector<std::vector<std::string>> ProofStep;

 	// Instantiate Prover (module)
	EuclidProver<BracketType::CurlyBraces> Euclid;

	// Use chars, as opposed to std::strings, for efficiency and minor performance gains
	/* char */ std::string PlayerCharacterSideKick = "1";
	/* char */ std::string QuadUtilityVehicle = "1";
	/* char */ std::string VehicleDriveDisabled = "1";
	/* char */ std::string EuropaLAnd = "1";
	/* char */ std::string StyxRiver = "2";
	/* char */ std::string NotInEuropaLand = "2";
	/* char */ std::string OutOfStyxBoat = "4";

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

	// Theorem to prove
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } // Game State goal

	// Proof-Step
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsIn { StyxRiver }
	{ PlayerCharacterSideKick } IsIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
	{ PlayerCharacterSideKick } IsNotIn { StyxBoat } = { StyxBoat } IsNotIn { StyxRiver }
	{ PlayerCharacterSideKick } IsIn { EuropaLand } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsNotIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand }
	{ PlayerCharacterSideKick } IsIn { Vehicle { QuadUtilityVehicle } } = { Vehicle { QuadUtilityVehicle } } IsIn { EuropaLand } and { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { Vehicle { QuadUtilityVehicle { VehicleDriveDisabled } } }
	{ PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled } 

	// Theorem
	Prove = { PlayerCharacterSideKick } IsIn { QuadUtilityVehicle } = { QuadUtilityVehicle } and { VehicleDriveDisabled }
```

Example expressed as C++ Code:

```c++
	// Axiom
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "StyxBoat", "}", "=", "{", "StyxBoat", "}", "IsIn", "{", "StyxRiver", "}" }); // Current Game State
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "IsIn", "{", "EuropaLand", "}", "and", "{", "Vehicle", "{", "QuadUtilityVehicle", "{", "VehicleDriveDisabled", "}", "}", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "EuropaLand", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "IsIn", "{", "EuropaLand", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "{", "VehicleDriveDisabled", "}", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsNotIn", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "IsIn", "{", "EuropaLand", "}" });
	//.
	//. Other available Game States, in the current environment, for the PlayerCharacterSideKick to choose from
	//.
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", "=", "{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}" });

	// Lemma
	Euclid.Lemma({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "StyxBoat", "}", "=", "{", "StyxBoat", "}", "IsNotIn", "{", "StyxRiver", "}" }); // These are connectives, and axiom helpers

	// Theorem
	Euclid.Prove({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", "=", "{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}" });
	
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
