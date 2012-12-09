#ifndef GSEARCH_H
#define GSEARCH_H
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

struct combo {
	
	string name;
	string link;  

};

class Gsearch {
	
	vector<combo> links;
	vector<string> results;
	string source;
	public:
	Gsearch();
	void mainrender();
	int gettags(int m);
	void print();
	vector<string> getResults();

};





/***************************************************************************************************************************************************************
                                               GSEARCH Func. Defns.
***************************************************************************************************************************************************************/


#include <fstream>
#include "Gsearch.h"
using namespace std;

/*default constructor*/

//the function opens search results page and gets all html code in to the string "source".

Gsearch::Gsearch(){
	
	string text;
	ifstream filein;
  string line;
  filein.open("tmp.html");
  if(filein.is_open())
  {
	  while(filein.good())
	  {
		  getline(filein,line);
		  text.append(line);
		  
	  }
	filein.close();
	source = text;
	}

}


/*this function gets result link and string displayed corresponding to the link.
	then the link and string are made into a structure and structure is pushed in 
	to a vector which contains all the search results.**/
	
int Gsearch::gettags(int m){
	string ttext;
	string tlink;
	int i,j;
	i = m + 3;
	
	while(1){
	
		//checking if end of the link.
		if (source.substr(i,5) == "</h3>"){break;}
		
		//checking for the start of link.
		if (source.substr(i,4) == "href"){
			
			while(1)
			{  
				if(source[i] == '"'){
					i++;
					
					//checking for the end of link.
					while(source.substr(i,4) != "&amp"){
						tlink.push_back(source[i]);
					
						i++;
					}
					
					break;
				}
				i++;
			}
			
			while(source[i] != '>'){i++;}
			i++;
			continue;
		}
		
		if(source[i] == '<'){
			while(source[i] != '>'){
				i++;
			}
			
			i++;
			continue;
		}
		
		//checking for tags like &nsb.. and removing them.
		
		if (source[i] == '&'){
	    j = 0;
	    while(j < 10){
	      i++;
	      if (source[i] == ';'){break;}
	      j++;
	    }
	    i++;
	    continue;
	  }
		
		//pushing the text displayed along with the link into a vector.
		ttext.push_back(source[i]);
		i++;
	}
	
	combo temp;
	tlink = tlink.substr(7);
	temp.link = tlink;
	temp.name = ttext;
	
	//pushing back every link into the vector links.
	links.push_back(temp);
	return i+4;
	
}
				
//func for main rendering of page.		

void Gsearch::mainrender(){
	
	int i = 0;
	while(source[i] != '\0'){
		if(source.substr(i,10) == "<h3 class="){
			i = gettags(i+14);
			continue;
		}
		
		i++;
	
	}
}

//func that prints all links onto the terminal.

void Gsearch::print(){
	
	ofstream fileout;
	fileout.open("input.txt");
	int i = 0;
	
	while(i < links.size()){
		
		fileout << endl;
		fileout << "***" << " " << i+1 << "  " << "***" <<"  " <<  links[i].name << endl;
		i++;
	
	}
	
	return;

}

//function that returns a vector containing links of all results. 

vector<string>  Gsearch::getResults(){
 	
  int i = 0;
  
  for(i = 0; i <links.size() ; i++){
    
    results.push_back(links[i].link);
  }
  
  return results;

}
	

#endif
