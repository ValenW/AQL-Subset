#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include "view.h"

using namespace std;

//1.计算数字的位数的函数 
int calDigit(int number) {
	int j = 1;

	while (1) {
		number /= 10;
		if (number == 0) break;
		j++;
	}

	return j;
}

//1.打印线条 的函数 
void printLine(int len[], int number) {

	for (int i = 0; i < number; i++) {
		
		cout << "+";
		for (int j = 0; j < len[i] + 2; j++) {
			cout << "-";
		}
	}

	cout << "+" << endl;
}

bool output(View v, string text, string alia = "") {
	
	
//注释：数字表示层级

	//1.把每个列的最大程度计算出来 

	int colNumber = v.colName.size(); //列数 
	//cout << "table size is " << colNumber << endl;
	int *maxLength = new int[colNumber]; //每列的最大长度 

	for (int i = 0; i < colNumber; i++) {

		int longestLocation = 0;  //单词（不含数字信息）的最大长度的位置 
		int longestLength = 0; 		//单词（不含数字信息）的最大长度的长度 
		int colSizes = v.table.size();  //行数 
		
		
		//2.如果行数为0则将最大长度设为列名； 
		if (colSizes == 0) {
			maxLength[i] = (v.colName)[i].length();
			continue;
		}

		for (int j = 0; j < colSizes; j++) {

			int tempLen = (v.table)[j][i * 2 + 1] - (v.table)[j][i * 2];
			if (tempLen >= longestLength) {
				longestLength = tempLen;
				longestLocation = j;
			}

		}

		//2.将最长单词和他的数字信息与最后一行做比较 

		int lastNumberA = calDigit((v.table)[colSizes - 1][i * 2 + 1]);
		int lastNumberB = calDigit((v.table)[colSizes - 1][i * 2]);
		int lastLength = (v.table)[colSizes - 1][i * 2 + 1] - (v.table)[colSizes - 1][i * 2];

		int longestNumberA = calDigit((v.table)[longestLocation][i * 2 + 1]);
		int longestNumberB = calDigit((v.table)[longestLocation][i * 2]);

		int lastL = lastNumberA + lastNumberB + lastLength;
		int longestL = longestNumberA + longestNumberB + longestLength;
		int maxL = lastL > longestL ? lastL : longestL;
		maxL += 4;
		int colNameSize = (v.colName)[i].length();

		maxLength[i] = maxL > colNameSize ? maxL : colNameSize;
	}

	//1.输出view的名字或别名 
	if (alia == "") {
		cout << "View: " << v.viewName << endl;
	}
	else {
		cout << "View: " << alia << endl;
	}
	
	//1.打印线条 
	printLine(maxLength, colNumber);
	//cout << maxLength[0] << endl; 
	
	//1.打印每列的列名 
	for (int i = 0; i < colNumber; i++) {

		cout << "| " << std::left << setw(maxLength[i]) << (v.colName)[i] << " ";

	}
	cout << "|" << endl;
	
	
	//1.打印线条 
	printLine(maxLength, colNumber);
	
	
	
	//1.打印每行的信息 
	int rowNumber = v.table.size();
	for (int i = 0; i < rowNumber; i++) {

		for (int j = 0; j < colNumber; j++) {

			int a = (v.table)[i][j * 2];
			int b = (v.table)[i][j * 2 + 1];
			int len = b - a;
			string output = "";
			string str = text.substr(a, len);
			stringstream s;
			s << a;
			output = str + ":(" + s.str() + ",";
			s.str("");
			s << b;
			output += s.str() + ")";
			s.str("");

			cout << "| " << std::left << setw(maxLength[j]) << output << " ";


		}
		cout << "|" << endl;
	}
	//1.打印线条 
	printLine(maxLength, colNumber);
	
	//1.打印行数信息 
	if (rowNumber == 0) {
		cout << "Empty set" << endl;
	}
	else {
		cout << rowNumber << " rows in set" << endl;
	}
	cout << endl;

	delete [] maxLength;

	return true;

}

#endif
