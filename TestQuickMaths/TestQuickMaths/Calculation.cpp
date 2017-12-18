#include "Calculation.h"

bool isNumber(char rawInput, int & output)
{
	output = rawInput - '0';
	if(output >=0 && output <10)
		cout << "got number: " << output << endl;
	return (output >= 0 & output <= 9);
}

FunctionOperator getOperator(char input) 
{
	switch (input) {
		case FunctionOperator::Add:
			cout << "got operator +" << endl;
			return FunctionOperator::Add;
		case FunctionOperator::Subtract:
			cout << "got operator -" << endl;
			return FunctionOperator::Subtract;
		default:
			return FunctionOperator::NotImplemented;
	}
}

void getStructure(vector<char> rawInput, Function & functionParam)
{
	int state = 0;
	functionParam.firstNumber = functionParam.lastNumber = 0;
	for (char c : rawInput) {
		int number;
		if (isNumber(c, number)) {
			if (state == 0) {
				functionParam.firstNumber *= 10;
				functionParam.firstNumber += number;
			}
			else if (state == 1) {
				functionParam.lastNumber *= 10;
				functionParam.lastNumber += number;
			}
		}
		else {
			state++;
			functionParam.fo = getOperator(c);
		}
	}
}

bool calculate(Function function, int & output)
{
	switch (function.fo) {
	case FunctionOperator::NotImplemented:
			return false;
	case FunctionOperator::Add:
		output = function.firstNumber + function.lastNumber;
		break;
	case FunctionOperator::Subtract:
		output = function.firstNumber - function.lastNumber;
		break;
	}
	return true;
}