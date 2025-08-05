### Meeting 1 ( 19/8/2024 )

##### Questions
1. Why in the example given is "hile" not accepted by the oracle?
2. What does it mean gramma?
	- Set of instruction that describe how to write valid statements in that language.
	- Given as a set of rules of how characters / words can be put together 1 after the other to form valid sentences. 
	- Grammar is just about syntax, and will not tell you if the statement makes sense.
3. What is a maximal generalisation?
4. What does it mean by context free grammar?
5. Why did we choose C?
		- 
1. What could I write in the background and Methodology?
2. In the Methodology section, m I just suppose describe ARVADA and how it works.
3. What is the expected timeline of the study?


What is LL{1}?
- Left to right, left most derivation, and the "1 " indicating it will look ahead 1 symbol. 
What is LR{1}
- Left to right, right most derivation, and the "1 " indicating it will look ahead 1 symbol. 

Regular Grammars?
- A subset of context free grammars, either has to be right or left linear. Hence, it cannot have not terminal on both sides.
General Context free grammars?
 A set of rules for each or a set of statement that is accepted by the language. Parser will try to build the statement using the grammar and is only accept if it can do so.
- Why can't regular expression handle palindromes? 

### Meeting 6 
Look to improve, to expand, or find limitation. 
Attempt it, document and track against the paper. 

-> Write up a problem statement ( view)

Due the unknowns and gaps in the paper published by University of California, which are the uncertainties of ARVADA and the testing. The lack of guarantee on the kind of grammars ARVADA can handle, the complexity of the grammars it can access, the alphabet of the grammar is can successfully tackle, as well as the lack of clarity on the way the test was performed in the study. Hence aim to replicate this study, and the problem becoming:

Replicating ARVADA algorithm on a clean room with the intention of expanding its evaluation to include diverse grammars and identify its characteristics.

The re-implementation will be conducted in a clean room environment, meaning it will be done form starch on an empty canvas, to also know if what is provided in the paper is enough for a complete re-implementation or if it is lacking details, and if so, what are the missing details.

Although, the intention of the study is to re-implement and expand on the paper, it may be a case this study does not discover any new knowledge. Which is trivial as it would just re-confirm the previous study with no net gain, however performing replication studies to enhance the trustworthiness of empirical results are a central piece in the scientific method, therefore standing as reason that this study is useful in computer science.

The current version of ARVADA also valid  has some issues:
- maximal generalisation assumes that all oracles used are context-free so there is not indication of what will happen in the case of context-sensitive grammars.
- Compares with only GLADE and there is no guarantee it will generalise to all benchmarks.

Intention with the re-implementation: 
- Read the input in a file format
- Build a simple parse tree with each character tokenised.
- For each tokenised character in the tree, attempt to merge.
- If merge is accepted by the oracle, build a new tree and add it as a subtree. Else Discard.
- Continually do this until there are no valid trees, and the end resulting tree is the grammar.
Things to consider:
- How is the replace algorithm is going to work?
- How are you going to merge?
- How are you going to check?
- How are you going to update the tree?
- What is the order of checking the merges? 

### Meeting 10
- 	For this week, I don't have any questions. I have implemented the bubbling, and will now continue onto parsing and refactoring my code. 

### Meeting 11
Concerns: The way I have implemented bubbling differs from the implementation of the paper.
- The Implementation of the paper does all the bubbling at once, while I have implemented doing 1 at a time. As the paper looks are way of exploration of the bubbles later, getting all the bubble at once makes sense. 
- Hence the plan is to change the implementation to get all the bubble at once, even though it will be very memory intensive. 
