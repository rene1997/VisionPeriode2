#include "Calculation.h"

bool isNumber(int rawInput)
{
	//if(rawInput >=0 && rawInput <10)
	//	cout << "got number: " << rawInput << endl;
	return (rawInput >= 0 & rawInput <= 9);
}

FunctionOperator getOperator(int input) 
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

void getStructure(vector<int> rawInput, Function & functionParam)
{
	int state = 0;
	functionParam.firstNumber = functionParam.lastNumber = 0;
	for (int c : rawInput) {
		if (isNumber(c)) {
			if (state == 0) {
				functionParam.firstNumber *= 10;
				functionParam.firstNumber += c;
			}
			else if (state == 1) {
				functionParam.lastNumber *= 10;
				functionParam.lastNumber += c;
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