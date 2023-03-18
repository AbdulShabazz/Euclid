# EuclidAutomatedTheoremProver_
C++ Edition

## Description

Euclid is a proof engine that operates symbolically and does not support library calls or built-in libraries. However, it can be utilized in game development to derive any achievable game-state, given the current state without pre-training, which is typically required for A.I. If successful, the prover will provide a path of proof-steps that prove the theorem, if it is provable, which can be used in your game.

Additionally, Euclid can be beneficial in debugging game logic.

## Instructions

To deploy the prover in your game logic, you provide it with axioms and lemmas related to the state of your game, and then request it to prove a theorem or desired outcome. Each proof-step can be used to perhaps update a state-machine, or update the game state; or in more advanced implementations, its proof-steps can be utilized by helper A.I. that aim to achieve the proof-step as a desired outcome in your game. This feeding process is repeated until the game state reflects the final proof, as a desired outcome (i.e., the theorem is proven). The prover can also produce a path of proof-steps using the available Expand- and Reduce- solvers.

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
	// Create empty ProofStep[lineNumber][proofStep] vector to store proof
	std::vector<std::vector<std::string>> ProofStep;

 	// Instantiate Prover (module)
	EuclidProver<BracketType::CurlyBraces> Euclid;

	// Axiom
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "StyxBoat", "}", "=", "{", "StyxBoat", "}", "IsIn", "{", "StyxRiver", "}" }); // Current Game State
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "IsIn", "{", "EuropaLand", "}", "and", "{", "Vehicle", "{", "QuadUtilityVehicle", "{", "VehicleDriveDisabled", "}", "}", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "EuropaLand", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "IsIn", "{", "EuropaLand", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "{", "VehicleDriveDisabled", "}", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsNotIn", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "=", "{", "Vehicle", "{", "QuadUtilityVehicle", "}", "}", "IsIn", "{", "EuropaLand", "}" });
	Euclid.Axiom({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", "=", "{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}" });

	// Lemma
	Euclid.Lemma({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "StyxBoat", "}", "=", "{", "StyxBoat", "}", "IsNotIn", "{", "StyxRiver", "}" }); // These are connectives, and axiom helpers

	// Theorem
	Euclid.Prove({ "{", "PlayerCharacterSideKick", "}", "IsIn", "{", "QuadUtilityVehicle", "}", "=", "{", "QuadUtilityVehicle", "}", "and", "{", "VehicleDriveDisabled", "}" });

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

## Format

The required format for the expressions is as follows:

{ LHS }... = { RHS }..., where LHS and RHS are properly-formed expressions.

The "=" is the equality operator, is required, and should be interpreted as a connective, used to delimit and or separate the LHS and RHS. 
The equality operator is the only builtin operator that is reserved. All other symbols may be used in your expressions.

The equality operator can also be used to separate multiple expressions, as shown:

{ LHS }... = { RHS_0000 }... = { RHS_0001 }... = { RHS_N }..., where LHS and RHS_N are properly-formed expressions.

If you have any questions, please contact me at: https://github.com/Seagat2011NOTES

Compatibility C++20 (Windows)

STYLEGUIDE

    POOR FORMATTING EXAMPLE

        TEST CASE: RENDER [PASS], PROOF [FAIL]
        {{a}raised{2}}plus{2ab}plus{b raised{2}}<==({a}plus{b})raised{2}
        ({a}plus{b})raised{2}minus{2ab}={c}raised{2}<==({a}plus{b})raised{2}={{c}raised{2}}plus{2ab}
        {{a}raised{2}}plus{2ab}minus{2ab}plus{b raised{2}}==>{{a}raised{2}}plus{{b}raised{2}}
        ({a}plus{b})raised{2}={{c}raised{2}}plus{2ab}
        Prove {{a}raised{2}}plus{{b}raised{2}}={c}raised{2}

    GOOD FORMATTING

        TEST CASE [PASS]
        { { a } raised { 2 } } plus { 2ab } plus { b raised { 2 } } <== ( { a } plus { b } ) raised { 2 }
        ( { a } plus { b } ) raised { 2 } minus { 2ab } = { c } raised { 2 } <== ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
        { { a } raised { 2 } } plus { 2ab } minus { 2ab } plus { b raised { 2 } } ==> { { a } raised { 2 } } plus { { b } raised { 2 } }
        ( { a } plus { b } ) raised { 2 } = { { c } raised { 2 } } plus { 2ab }
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

        { LHS... } = { RHS... } [ = { RHS_N... } ]+
        .
        .

    LEMMA SUBSTITUTION FORMAT

        { LHS... } <== or <==> or ==> { RHS... }
        .
        .

    PROOF FORMAT
        Prove { LHS... } = { RHS... } [ = { RHS_N... } ]+

    QUICK FORMAT

        TEST CASE [PASS]
        primes = a raised 2 + b raised 2
        1 mod 4 = a raised 2 + b raised 2
        Prove primes = 1 mod 4

    EFFICIENT SCOPED FORMAT

        TEST CASE [PASS]
        primes = { a } raised { 2 } + { b } raised { 2 }
        { 1 } mod { 4 } = { a } raised { 2 } + { b } raised { 2 }
        Prove primes = { 1 } mod { 4 }
