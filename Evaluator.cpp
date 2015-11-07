/**
* Implementation of the Evaluator class
* @author Josh Johnson, JP McDermott, Daniel Mitchell.
* @file Evaluator.cpp
*/
#include "Evaluator.h"
#include "Syntax_Error.h"
#include <sstream>


using namespace std;

const vector<string> Evaluator::UNARY_OPERATORS = { "!", "++", "--" };

const vector<string> Evaluator::BINARY_OPERATORS = { "==", "!=", "&&", "||", ">", ">=", "<", "<=", "+", "-", "*", "/", "%", "^" };

const vector<string> Evaluator::PARENS = { "(", ")", "[", "]", "{", "}" };

const int Evaluator::PRECEDENCE[] = { 8, 8, 8, 3, 3, 2, 2, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, -1, -1, -1, -1, -1, -1 };

int Evaluator::get_operator(string op)
{
	for (size_t i = 0; i < UNARY_OPERATORS.size(); i++)
		if (UNARY_OPERATORS[i] == op)
			return i;
	for (size_t i = 0; i < BINARY_OPERATORS.size(); i++)
		if (BINARY_OPERATORS[i] == op)
			return ((UNARY_OPERATORS.size() -1) + i);
	for (size_t i = 0; i < PARENS.size(); i++)
		if (PARENS[i] == op)
			return ((BINARY_OPERATORS.size() -1) + (UNARY_OPERATORS.size() -1) + i);
	return -1;
} // end get_operator

int Evaluator::precedence(string op)
{
	return PRECEDENCE[get_operator(op)];
} // end precedence

bool Evaluator::is_operator(string op)
{
	return (get_operator(op) >= 0);
} // end is_operator(string)

bool Evaluator::is_operator(char ch)
{
	string str;
	str += ch;
	return is_operator(str);
} // end is_operator(char)

bool Evaluator::is_operator(char ch1, char ch2)
{
	string str;
	str += ch1;
	str += ch2;
	return is_operator(str);
} // end is_operator(char, char)

bool Evaluator::is_bin_op(string op){
	for (int i = 0; i < BINARY_OPERATORS.size(); i++)
		if (BINARY_OPERATORS[i] == op)
			return true;
	return false;
}

bool Evaluator::is_un_op(string op){
	for (int i = 0; i < UNARY_OPERATORS.size(); i++)
		if (UNARY_OPERATORS[i] == op)
			return true;
	return false;
}

int Evaluator::eval_op(string op)
{
	
	if (operand_stack.empty())
	{
		// Operand should have come before last character processed
		throw Syntax_Error("There are no more operands.");
	}
	

	int rhs = operand_stack.top();
	operand_stack.pop();
	

	if (operand_stack.empty() || op == "!" || op == "++" || op == "--")
	{
		switch (get_operator(op))
		{
		case 0: return !rhs;
			break;
		case 1: return ++rhs;
			break;
		case 2: return --rhs;
			break;
		}
		
	}
	else
	{
		int lhs = operand_stack.top();
		operand_stack.pop();

		switch (get_operator(op))
		{
		case 3: return lhs == rhs;
			break;
		case 4: return lhs != rhs;
			break;
		case 5: return lhs && rhs;
			break;
		case 6: return lhs || rhs;
			break;
		case 7: return lhs > rhs;
			break;
		case 8: return lhs >= rhs;
			break;
		case 9: return lhs < rhs;
			break;
		case 10: return lhs <= rhs;
			break;
		case 11: return lhs + rhs;
			break;
		case 12: return lhs - rhs;
			break;
		case 13: return lhs * rhs;
			break;
		case 14: if (rhs != 0)
			return lhs / rhs;
				 else
					 throw Syntax_Error("Cannot divide by zero.");
			break;
		case 15: return lhs % rhs;
			break;
		case 16: return pow(lhs, rhs);
		}
	}
} // end eval_op


void Evaluator::process_operator(string op)
{
	if (operator_stack.empty() || (op == "(") || (op == "[") || (op == "{"))
	{
		if (op == ")" || op == "]" || op == "}")
			throw Syntax_Error("Expression can't start with a closing parenthesis.");
		if (op == ">" || op == "<" || op == ">=" || op == "<=" || op == "==" || op == "!=" || op == "&&" || op == "||")
			throw Syntax_Error("Expression can't start with a logical operator.");

		operator_stack.push(op);
	}
	else
	{
		if (precedence(op) > precedence(operator_stack.top()))
			operator_stack.push(op);
			
		else
		{
			// Pop all stacked operators with equal or higher precedence than op
			while (!operator_stack.empty()
				&& (operator_stack.top() != "(")
				&& (operator_stack.top() != "[")
				&& (operator_stack.top() != "{")
				&& (precedence(op) <= precedence(operator_stack.top())))
			{
				string current_op = operator_stack.top();
				if (is_bin_op(current_op) && is_bin_op(op))
					throw Syntax_Error("Cannot process two binary operators consecutively.");
				if (is_un_op(current_op) && is_bin_op(op ))
					throw Syntax_Error("Unary operator cannot be followed by a binary operator.");

				int result = eval_op(current_op);
				operand_stack.push(result);
				operator_stack.pop();
			}

			//	operator_stack is empty or top of stack is '(' or current
			//  operator precedence > top of stack operator precedence
			if (op == ")")
				if (!operator_stack.empty() && (operator_stack.top() == "("))
					operator_stack.pop();
				else
					throw Syntax_Error("Unmatched closing parentheses");
			else if (op == "]")
				if (!operator_stack.empty() && (operator_stack.top() == "["))
					operator_stack.pop();	
				else
					throw Syntax_Error("Unmatched closing parentheses");
			else if (op == "}")
				if (!operator_stack.empty() && (operator_stack.top() == "{"))
					operator_stack.pop();
				else
					throw Syntax_Error("Unmatched closing parentheses");
			else
				operator_stack.push(op);
		}
	}
} // end process_operator

int Evaluator::eval(string infix_literal)
{
	// Save the literal expression as a member variable object.
	infix_expression = infix_literal;

	// Make sure stack is empty
	while (!operator_stack.empty())
		operator_stack.pop();
	
	istringstream infix_tokens(infix_literal);
	char next_token;

	while (infix_tokens >> next_token)
	{
		// Increase position for each character that is numeric.
		if (isdigit(next_token))
		{
			infix_tokens.putback(next_token);
			string operand_string = "";
			do{
				++position;
				infix_tokens >> next_token;
				operand_string += next_token;
			}
			while (isdigit(next_token));

			int operand_integer = stoi(operand_string);
			operand_stack.push(operand_integer);
		}
		else if (is_operator(next_token))
		{
			char look_next;
			infix_tokens >> look_next;
			++position;

			string op;
			op += next_token;

			// Checks for two character comparison or logical operator
			if (is_operator(next_token, look_next)) 
				op += look_next;
			else{
				infix_tokens.putback(look_next);
				--position;
			}

			process_operator(op);
		}
		else
			throw Syntax_Error("Unexpected Character Encountered");
		
	}	// end while

	// Pop any remaining operators and evaluate using remaining operands
	while (!operator_stack.empty())
	{
		string op = operator_stack.top();
		operator_stack.pop();
		
		if (op == "(" || op == "[" || op == "}")
			throw Syntax_Error("Unmatched open parenthesis");

		int result = eval_op(op);
		operand_stack.push(result);
	}

	// The last item in the operand stack should be the answer.
	if (!operand_stack.empty()) 
	{
		int answer = operand_stack.top();
		operand_stack.pop();

		if (operand_stack.empty())
		{
			position = 0;
			return answer;
		}
		else
		{
			--position; // Remaining operands would come before current character
			throw Syntax_Error("There are more operands than operators.");
		}
	}
	else 
		throw Syntax_Error("There are not enough operands.");
} // end eval