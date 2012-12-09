
/**************************************************************************************************************************************************************
                                           main program for implementing the user interface.
***************************************************************************************************************************************************************/

#include <stack>
#include "mylynx.h"
#include<iostream>
#include <string>
#include <sstream>
#include "network.h"
#include <fstream>
using namespace std;

//the function to dispaly parts of text on to the terminal.
// (functions similar to "more" command)

void printRoutine(){
	
	network crap;
	
	//clearing terminal so that main text can be diplayed.
	
	crap.getwebpage("clear");
	
	string line;
	int i = 0;
	string c;
	ifstream filein;
	
	//input.txt <- the file in which rendered output 
	//from html file is present.
	
	filein.open("input.txt");
	
	if(filein.is_open())
  {
	  
	  while(filein.good())
	  {
	  	
	  	//displaying 30 lines of text at a time.
	  	if( i == 28) {
	  		cout << "more text (y/n)" << endl;
	  		i = 0;
	  		getline (cin , c)	;
	  		
	  		//proceeding further depending on user input.
	  		
	  		if(c == "n"){cout << endl << endl;break;}
	  		if(c == "y"){cout << endl << endl;continue;}
	  	
	  	}
		  
		  getline(filein,line);
		  cout << line << endl;
		  i++;
	  }
	  
	  filein.close();
	}

return ;

}

//function that returns all the text present in the file "input.txt" in a string.

string getalltext(){
	
	string text;
	string line;
	
	ifstream filein;
	filein.open("input.txt");
  
  if(filein.is_open())
  {
	  while(filein.good())
	  {
		  getline(filein,line);
		  text.append(line);
		  text.append("\n");
	  }
	
		filein.close();
	}
	
	return text;
}


//************* the main program  *********************//


int main(){
  
  string input;
  network x;
  string temptext;
  
  //stack for storing previous webpage details to implement 'back' function.
  //the pair contains text and vector of links present in the previous webpage.
  
  stack < pair < string,vector<string> > > PrevPages;
  
  //set of instructions for usage.
  
  cout << endl << endl;
  cout <<"   " <<  "*****INSTRUCTIONS*******" << endl;
  cout << endl;
  cout<<"   " << "type quit to exit"<<endl;
  cout << endl;  
  cout<<"   " << "type a URL or a search parameter to browse"<<endl;
  cout << endl;
  cout <<"   " <<  "to visit a link present in a webpage type n press return and then enter the link number" << endl;
  cout << endl; 
  cout <<"   " <<  "type b and press return to go back to previous webpage." << endl;
  cout << endl;
  cout << endl;
  
  getline(cin,input);
  
  if(input == "quit") {return 0;}
  
  //setting input to given string then calling operation toggler to get text in to input.txt.
  //then calling print routine to print text onto the terminal.
  
  x.setuinput(input);
  x.operationtoggler();
  printRoutine();
  
  //pushing the text and links in to a stack called Prevpages to implement 'back' function.
  
  temptext = getalltext();
  PrevPages.push(pair < string,vector<string> > (temptext , x.links));
  
  string temp;
  int t;
  
  while(1){
  
  getline(cin,temp);
  if(temp == "quit")
  {
    return 0;
  }
  
  //if user wants to visit the previous page.
  
  if(temp == "b")
  {
  
  	//popping the information related to previous page 
  	//and displaying it on to the terminal
  	
  	if(PrevPages.size() == 1){continue;}
  	pair<string, vector<string> > TempPage;
  	PrevPages.pop();
  	
  	TempPage = PrevPages.top();
  	
  	//writing the text in webpage into input.txt
  	
  	ofstream fileout;
  	fileout.open("input.txt");
  	fileout << TempPage.first;
  	fileout.close();
  	printRoutine();
  	x.links = TempPage.second;
  	continue;
  
  }
  
  //if the user wants to visit a link present in the text.
  
  if(temp == "n")
  {
    
    getline(cin,temp);
    istringstream ( temp ) >> t;
    
    //all the links present in the content are stored in the vector links.
    //so by knowing the # of the link we can access it.
    
    temp = x.links[t-1];
    cout << temp << endl; 
    
    x.setuinput(temp);
    x.operationtoggler();
    printRoutine();
		temptext = getalltext();
  	PrevPages.push(pair <string, vector<string> > (temptext , x.links));
    continue;
  }    
  
    x.setuinput(temp);
    x.operationtoggler();
    printRoutine();
    temptext = getalltext();
  	PrevPages.push(pair <string, vector<string> > (temptext , x.links));
  
  }
  
  cout << endl;
  
  return 0;
}
