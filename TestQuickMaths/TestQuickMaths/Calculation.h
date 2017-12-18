#ifndef CALCULATION_H
#define CALCULATION_H

#include <vector>
#include <iostream>
using namespace std;

enum FunctionOperator {Add = '+', Subtract='-', NotImplemented};

struct Function {
	int firstNumber;
	FunctionOperator fo;
	int lastNumber;
};




void getStructure(vector<char> rawInput, Function & functionParam);
bool calculate(Function function, int & output);


#endif // !CALCULATION_H
