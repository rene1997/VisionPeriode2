#ifndef CALCULATION_H
#define CALCULATION_H

#include <vector>
#include <iostream>
using namespace std;

enum FunctionOperator {Add = 15, Subtract=14, NotImplemented=13};

struct Function {
	int firstNumber;
	FunctionOperator fo;
	int lastNumber;
};

void getStructure(vector<int> rawInput, Function & functionParam);
bool calculate(Function function, int & output);


#endif // !CALCULATION_H
