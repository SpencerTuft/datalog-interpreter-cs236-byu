# datalog-interpreter-cs236-byu

## Table of Contents
- [Project Description](#project-description)
- [Requirements](#requirements)
    + [Part 1](#part-1)
        * [union and join](#union-and-join)
    + [Part 2](#part-2)
        * [interpreting rules](#interpreting-rules)
- [Evaluating Rules](#evaluating-rules)
- [Assumptions](#assumptions)
- [Examples](#examples)
  * [Another Example](#another-example)
- [FAQ](#faq)

## Project Description
Extend the [relational database](https://wiki.cs.byu.edu/cs-236/relational-database) for Datalog to implement the rules in the input program. This means you need the union ( ∪ ) and join ( ⋈ ) operators. At the end of this project, your interpreter should be able to handle all aspects of the Datalog in this course. Please review the [project standards](https://wiki.cs.byu.edu/cs-236/project-standards) for the pass-off requirements as *the time-bound for running time in pass-off is strictly enforced for this project*.

As with the other projects, this project is divided into two parts: extending the relation class with union and join and then interpreting rules.

## Requirements
#### Part 1
###### union and join
Extend the relation class from the previous project to support union and natural join. Include at least 10 automatic test that validate the functionality of both operators. The majority of the test should cover natural join. Every test must be documented with at least one sentence justifying its existence. Be sure to cover corner cases (of which there are several).

Further, all the test must be automatic; automatic means the test run without any user input and reports to the console the status of every test: pass or fail.

- *Command line*: no command line arguments
- *Input*: none
- *Output*: A pass/fail report for each test. (see below for further details on these test)

The only text that needs to be outputted to the console is the test number and whether it passed or not (but you may definitely output more!). The TAs will look at your code to make sure the test cases do what you say they are doing in the comments. They will also make sure you implemented the union and join functions. *The test cases you come up with should be different that the examples provided by the wiki (i.e. use different schemes, facts, rules, etc.)*.

For this project, each test case you create should pass because the actual output of your program should equal the expected output. The pass/fail report of a test is based on a comparison between the expected result and the actual result of each test. The comparison must be made automatically by your tester. This means that your tester must have the expected output in the code to compare with the actual output in an if-else statement; the actual output is the relation that is created after performing the union and/or join operations, while the expected output is what you calculate the relation should be after performing the operations. Common ways to check if they are equal include creating a method that test if two relations have the same name, schema, and tuples; or creating a toString function for the relation class and comparing the strings for each relation. It is up to you how to do this, but what is not allowed is always outputting that each test passed without actually checking that the code does what it is supposed to do.

The pass-off is based on the quality of test and whether or not the solution passes. If a test does not automatically compare actual and expected out as described above, it won't be counted. If more than one test case test the same thing, only the first test case will be counted.

#### Part 2
###### interpreting rules
Implement a least-fixed-point algorithm to interpret rules from the Datalog input before interpreting the queries. The rules should add new facts to the relations in the database. By way of clarification, the processing of queries in this project is the same as for the [last project](https://wiki.cs.byu.edu/cs-236/relational-database) . The only thing new is the interpretation of the rules to generate new facts before answering the queries.

The output is the same as for the last project too except that there is an additional first statement: “Schemes populated after n passes through the Rules.” where n is the number of times the least-fixed-point algorithm processes the rule set. As was done in the other projects, for pass-off purposes, be sure your output matches precisely the examples below. The first line is the string “Schemes populated after n passes through the Rules.”, with n replaced by the number of iterations it takes to converge to a solution.)

As a reminder, the rules are repeatedly evaluated until no new facts are added to the database. The rules are evaluated in the order in which they are defined in the input file. If a solution interprets the rules in a different order, then the value of n may not match with the reference solution.

- *Command line*: a single argument indicating the input file
- *Input*: a valid datalog program
- *Output*: see the [output specifications](https://wiki.cs.byu.edu/cs-236/datalog-parser#output-specifications) and [examples](https://wiki.cs.byu.edu/cs-236/datalog-parser#examples)

Part 2 is scored on a set of 10 private test at submission.

## Evaluating Rules
There are several ways to evaluate rules. The most direct way is to use the mental model of an expression tree, where each predicate in the rule is evaluated as a query to return a relation, and that relation returned by the predicate is then natural joined with the relations for other predicates in the rule. This process is best understood as a simple traversal of the rule, evaluating each predicate as a query, and then gathering the results with a natural join. As a breakdown into steps, it might proceed as follows:

1. *Evaluate the predicates on the right-hand side of the rule*: For every predicate on the right hand side of a rule, evaluate that predicate in the same way queries are evaluated in the previous project. The result of the evaluation should be a relation. If there are n predicates on the right hand side of a rule, then there should be n intermediate relations from each predicate.
2. *Join the relations that result*: When there are two or more predicates on the right-hand side of a rule, join all the intermediate results to form the single result for Step 2. Thus, if p1, p2, and p3 are the intermediate results from step one; you should construct a relation: p1 |x| p2 |x| p3. If there is only a single predicate on the right hand side of the rule, no joins are necessary.
3. *Project columns that appear in head predicate*: It is possible for the predicate list in the original rule to have free variables (variables not defined in the head of the rule). Perform a project operation on the result from Step 2 to remove columns corresponding to free variables. That is, project on the result from Step 2 keeping only those columns whose attribute names appear in the head predicate of the rule.
4. *Rename relation to match the schema of the relation in the database*: Before taking the union in Step 5, we must first make the result of Step 3 be union compatible with the appropriate relation in the database. To make it union compatible: Get the relation r from the database whose name matches the name of the head predicate of the rule. For every variable v that appears in column i of the head predicate of the rule and column j of the result from Step 3, rename the attribute for column j of the result to the name of the attribute in column i of the schema of r.
5. *Union with the relation in the database*: Union the results of Step 4 with the relation in the database whose name is equal to the name of the head of the rule. In Step 4 we called this relation in the database r. Add tuples to relation r from the result of Step 4.

That is the algorithm for evaluating one rule. Each rule potentially adds new facts to a relation. The fixed-point algorithm repeatedly performs iterations on the rules adding new facts from each rule as the facts are generated. Each iteration may change the database by adding at least one new tuple to at least one relation in the database. The fixed-point algorithm terminates when an iteration of the rule expression set does not union a new tuple to any relation in the database.

## Assumptions
As a reminder, our goal is not to build an industrial strength interpreter. Additional issues arise, which you need not check. Without checking, you may assume:

- Attribute names in the schemes and variable names in the rules have distinct name spaces: no attribute name will appear as a variable in a rule.
- Only identifiers appear in a rule head, and they are unique; no two identifiers in a rule head are the same.
- Every identifier in the head will appear in at least one predicate in the body (the right-hand side) of the rule.

## Examples

The examples from the prior project are valid for this project. In addition, you might consider the following (remembering that these are not sufficient to test your code):

- [ex40.txt](http://students.cs.byu.edu/~egm/datalog/ex40.txt) with output in [out40.txt](http://students.cs.byu.edu/~egm/datalog/out40.txt)
- [ex41.txt](http://students.cs.byu.edu/~egm/datalog/ex41.txt) with output in [out41.txt](http://students.cs.byu.edu/~egm/datalog/out41.txt)
- [ex42.txt](http://students.cs.byu.edu/~egm/datalog/ex42.txt) with output in [out42.txt](http://students.cs.byu.edu/~egm/datalog/out42.txt)
- This last example is easily extendable to be very big or very small: [ex43.txt](http://students.cs.byu.edu/~egm/datalog/ex43.txt) with output in [out43.txt](http://students.cs.byu.edu/~egm/datalog/out43.txt)

Here is a large example if you want to test your running time. This example runs (unmodified) in 6-7 seconds on the reference solution. It is expected to run longer on the typical student solution but should not be excessively long (more than a few minutes). You can half the size with a simple change that is commented in the file: [ex44.txt](http://students.cs.byu.edu/~egm/datalog/ex44.txt) . No output it provided since this is intended for performance tuning and not correctness.

*You should not use these examples as your own test cases for part 1*. Part of creating your own test cases means coming up with your own relations using different schemes, facts, rules, and queries than those provided.

### Another Example
```
Schemes:
E(X,Y)
R(T,U)
Facts:
E('1','1').
E('1','2').
E('2','2').
E('2','3').
E('3','3').
E('3','4').
E('4','4').
E('4','1').
R('1','1').
Rules:
R(I,J) :- R(I,Z),E(Z,J).
Queries:
R('1','4')?
```
How would you edit this example to compute a full transitive closure?

## FAQ
*When should a rule update the associated relation?*

New facts should be added to the relation as soon as a rule associated with the relation is evaluated. As a example, if there are two rules, R1 and R2, for relation A, then R1 is evaluated, and any new facts it generates are added to A. Then R2 is evaluated using the updated relation A that contains new facts from R1, and any new facts R2 generates are added to A.

*When should the algorithm that iteratively evaluates rules terminate?*

The algorithm should evaluate rules, sequentially, until the number of facts in all the relations in the database does not change. At this point, the rules do not generate any new facts.

*What order should be followed in evaluating rules?*

Input order should be followed. So however the rules appear in the input file is the order of evaluation.

*Is there a time-bound for pass-off?*

Yes. Please review the project standards for the time-bound requirements.

*I thought my project is set up correctly, but the test cases are taking forever to run. What is happening?*

Are you running your program in Visual Studio? Visual Studio in debug mode tends to slow down the running time of your project by a huge factor. Try running the test cases either outside of Visual Studio (in the command line) or set up Visual Studio to run in release mode (here are instructions from Microsoft on how to do that).

Other things that may be influencing your run time include what computer you are running your program on (run the program on the lab computers for the best approximation of how it will run on the TA computers) and using a vector to store the tuples in each Relation instead of a set (if you have to sort the vector manually, that can really slow down run time).

[Back to top](#datalog-interpreter-cs236-byu)
