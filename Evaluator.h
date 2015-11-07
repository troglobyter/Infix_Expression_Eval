/**
* Evaluates infix expressions that include arithmetic, comparison and logical operators.
*
* @author Josh Johnson, JP McDermott, Daniel Mitchell
* @file Evaluator.h
*/
#ifndef EVALUATOR
#define EVALUATOR
#include <stack>
#include <vector>
using namespace std;

class Evaluator
{
public:
	/**
	* Computes the result of an infix expression.
	* Reads each character of a string and
	* determines whether it is an operator or a digit
	* @param expression A string that is a mathematical/logical expression.
	* @return The answer to the expression.
	* @throw Syntax_Error If a character is neither an operator or a digit,
	* an unmatched open parentheses is encountered, there are too few operators
	* or there are too few operands.
	*/
	int eval(string infix_literal);


	/**
	* The original expression that is evaluated by the Evaluator object.
	*/
	string infix_expression;

	// Data Fields
	/** Keeps track of each character being processed.*/
	int position = 0;

private:
	/**
	* Searches supported operators for a match to the given value.
	* @param op An operator to search for.
	* @return The index where the operator is located or -1 if not found.
	*/
	int get_operator(string op);

	/**
	* Searches for an operators corresponding precedence value.
	* @param op An operator to find the precedence of.
	* @return The precedence of the operator.
	*/
	int precedence(string op);

	/**
	* Searches supported operators for a match to the given value.
	* @param op A string that may be a supported operator.
	* @return True if operator was found, false otherwise.
	*/
	bool is_operator(string op);

	/**
	* Converts a character into a string then searches 
	* supported operators for a match to the given value.
	* @param A character that may be a supported operator.
	* @return True if operator was found, false otherwise.
	*/
	bool is_operator(char ch);

	/**
	* Converts two characters into a string then searches 
	* supported operators for a match to the given value.
	* @param A character that may be part of a comparison/logic operator.
	* @param A character that may be part of a comparison/logic operator.
	* @return True if operator was found, false otherwise.
	*/
	bool is_operator(char ch1, char ch2);

	/**
	*Checks for membership of BINARY_OPERATORS vector.
	*/
	bool is_bin_op(string op);

	/**
	*Checks for membership of UNARY_OPERATORS
	*/
	bool is_un_op(string op);

	/**
	* Executes the given mathematical/logic operation using values
	* from the operand_stack.
	* @param op A mathematical/logic operation to execute.
	* @return The result of a mathematical/logic operation
	* @throw Syntax_Error If an invalid operator was passed or
	* if there are not enough operands to calculate the expression.
	*/
	int eval_op(string op);

	/**
	* Determines whether a given operator should be pushed onto the
	* operator_stack, discarded or used to evaluate a part of the expression. 
	* @param op An operator to evaluate.
	* @throw Syntax_Error If an unmatched parentheses is encountered.
	*/
	void process_operator(string op);

	

	/** Holds all of the supported unary operators to compare with given values. */
	static const vector<string> UNARY_OPERATORS;

	/** Holds all of the supported binary operators to compare with given values. */
	static const vector<string> BINARY_OPERATORS;

	/** Holds all of the supported parenthesis operators to compare with given values. */
	static const vector<string> PARENS;

	/** 
	* Holds the precedence of each supported operator with an index
	* corresponding to a particular operator. 
	*/
	static const int PRECEDENCE[];

	/** Holds operators parsed from the expression. */
	stack<string> operator_stack;
	 
	/** Holds operands parsed from the expression. */
	stack<int> operand_stack;
};

#endif
