# WALKSAT- Resolution Prover

These are 2 C++ algoirthms that can output if a list of k-CNFs are satisfiable or unsatisfiable. WALKSAT uses a local search algorithm and will choose a random clause and flip depending on a certain probability, a random or a optimized variable. Resolution Proving uses a rule of inference (A v B, -B = A) to eliminate variables in clauses until either a contradiction is met or no new clauses can be inferred.

## CNF.txt

This file is needed, with exactly this name, for the k-CNF-generator.py algorithm which will randomly generate up to m clauses with n symbols to choose from without replacement within each clause). No two clauses will be the same, and the output will be written to the CNF.txt file in a standard minisat input format.

## Usage

To run either WALKSAT or resolution proving, similar to minisat, you need to provide a file containing clauses in CNF format suitable for minisat (CNF.txt file is populated with the k-CNF-generator.py)

```text
 c N=2 queens problem
 p cnf 4 12
 1  2 0
-1 -2 0
 3  4 0
-3 -4 0
 1  3 0
-1 -3 0
 2  4 0
-2 -4 0
 1  4 0
-1 -4 0
 2  3 0
-2 -3 0
```