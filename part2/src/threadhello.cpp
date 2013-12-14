 //  g++ -std=c++11 threadhello.cpp -lpthread -lboost_system-mt

#include <iostream>
#include <string>
#include <sstream>
#include "threadhello.h"
#include <stdio.h>

using namespace std;


//class implementation
void Arduino::set_parameters(std::string parameters){

  //cout << "check parameters length: " << parameters.length() << endl;
  int aux1=0;

  if(parameters.length() != 11) {
    cout << "length " << parameters.length() << " is not 11" << endl;
    return;
  }

  sendReady = parameters;
  LL = stoi(parameters.substr(0,2));

  PP = stoi(parameters.substr(2,2));

  TT = stoi(parameters.substr(4,2));

  aux1 = stoi(parameters.substr(6,2), NULL, 16);
  CC = aux1*100/255;

  DD = stoi(parameters.substr(8,2), NULL, 16);

  return;

}

//TODO we must use a map function to convert the values of duty cycle from 0-255 to 0-100%

void Arduino::print(){
  cout << "LDR:" << LL << " proximity:" << PP << " temperature:" << TT << " fan control duty cycle:" << CC << " LED duty cycle" << DD << endl;
}

string Arduino::getString(){
 // return to_string(LL) + "," + to_string(PP) + "," + to_string(TT) + "," + to_string(CC) + "," + to_string(DD);
	return sendReady;
}

int Arduino::getLDR(){
  return LL;
}

int Arduino::getPresence(){
  if (PP <= 50) return 1;
  else return 0;
}
