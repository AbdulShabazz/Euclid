# EuclidAutomatedTheoremProver_
C++ Edition

Examples

```c++
/**
	// Create empty path[lineNumber][proofStep] to store proof
	std::vector<std::vector<char>> path;

 	// Instantiate Prover (module)
	EuclidProver<BracketType::CurlyBraces> Euclid;

	// Add axioms
	Euclid.Axiom({'{', '1', '}','+','{', '1', '}','=','{', '2', '}'}); // axiom_0
	Euclid.Axiom({ '{', '2', '}','+','{', '2', '}','=','4' }); // axiom_1

	// Add supporting lemmas
	Euclid.Lemma({ '{', '1', '}','+','{', '0', '}','=','{', '1', '}' }); // lemma_0

	// Theorem to Prove
	const std::vector<char> ProofTarget = { '{', '4', '}', '=', '{', '1', '}','+','{', '1', '}','+','{', '1', '}', '+', '{', '1', '}' };

	if (Euclid.Prove(ProofTarget, path))
	{
		std::cout << "Proof found:\n";
		Euclid.PrintPath(path);
	}
	else
	{
		std::cout << "Proof failed\n";
	}

	// Optional Solver: Expand
	std::vector<std::vector<char>> ExpandPathSolution;
	if (Euclid.ProveViaExpand(ProofTarget, ExpandPathSolution))
	{
		std::cout << "Proof via Expand:\n";
		Euclid.PrintPath(ExpandPathSolution);
	}
	else
	{
		std::cout << "Proof via Expand failed\n";
	}

	// Optional Solver: Reduce
	std::vector<std::vector<char>> ReducePathSolution;
	if (Euclid.ProveViaReduce(ProofTarget, ReducePathSolution))
	{
		std::cout << "Proof via Reduce:\n";
		Euclid.PrintPath(ReducePathSolution);
	}
	else
	{
		std::cout << "Proof via Reduce failed\n";
	}
	*/
```
