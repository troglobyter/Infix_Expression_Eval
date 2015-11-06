//Main.cpp
//Josh Johnson, JP McDermott, Danny Mitchell

#include "Evaluator.h"
#include <cstring>
#include <iostream>
#include <vector>

#include "Syntax_Error.h"
using namespace std;

int main()
{
		Evaluator eval;
		vector<string> expression = { "1+2*3","2+2^2*3" ,"1==2", "1+3 > 2", "(4>=4) && 0", "(1+2)*3" }; //<--expressions we wish to evaluate go here
		vector<string>::iterator exprItr;

		for (exprItr = expression.begin();
		exprItr != expression.end();
			exprItr++)
		{
			try
			{
				int result = eval.eval(*exprItr);

				cout << eval.infix_expression.c_str() << " = " << result << endl << endl;
			}
			catch (Syntax_Error e)
			{
				cout << eval.infix_expression.c_str() << endl << e.what() << " " << "@ char: " << eval.position << endl << endl;
			}
		}

		system("pause");
	return 0;
}