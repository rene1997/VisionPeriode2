#include <iostream>
#include <string>
#include <vector>
#include "Calculation.h"

using namespace std;


int main() 
{
	while (1) {
		cout << "insert function end with enter" << endl;
		string input;
		getline(cin, input);
		vector<char> inputList(input.begin(), input.end());
		Function f;
		getStructure(inputList, f);
		cout << "got structure, going to calculate" << endl;
		int answer;
		bool success = calculate(f, answer);
		if (success) {
			cout << "answer is: " << answer << endl;
		}
		else {
			cout << "an error occured" << endl;
		}

	}
}