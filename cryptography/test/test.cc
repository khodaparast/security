#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>


#include <bits/stdc++.h> 



using namespace std;
typedef unsigned char       BYTE;

 char *str_to_hex(string str){
char *result[str.length()+1];
int i=0;
	for(i;i<str.length();i++){
		//cout<<"hex " << str.at(i)<<" : "<<hex<<int(str.at(i))<<"\n";
	result[i]=str.at(i);
	//cout << result[i]<<"\n";
	}
return *result;
}

int main()
{
	int i=0;
	string str="hello";
	char *result2;
	result2=str_to_hex(str);
	for(i;i<str.length();i++){

		result2[i];
}
	cout<<"-------> "<<result2[0];
	free(result2);
	char ch = str.at(2);
	char ch2='A';
	
	
	return(0);
}












