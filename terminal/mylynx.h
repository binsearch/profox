#ifndef MYLYNX_H
#define MYLYNX_H
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

class mylynx {

	vector<string> lines;
	vector<string> links;
	vector<float> TTR;
	vector<float> smoothTTR;
	float mean;
	float sd;
	
	
	public:
	
	mylynx(string filname);//starting text extraction.
	int no_of_tags(string str);
	int no_of_char(string str);
	void createTTR();
	void printpage();
	void smoothen();
	float smooth(int index);
	void setsd();
	string render(string str);
	void printmain();
	vector<string> getLinks();

};	



/***************************************************************************************************************************************************************
                                                         MYLYNX function defns.
****************************************************************************************************************************************************************/


#include "mylynx.h"
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <sstream>
using namespace std;

/*convert integer to string */

string convertInt(int number)
{
   
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream

}

/*function to remove whitespaces in a string */

string whiteSpace (string str)
{
	
	int len;
	while (true)
	{
		if (str[0] == ' ' || str[0] == '\t')
		{
			str = str.substr (1);
		}
		else {break;}
    }
    
    while (true)
    {	
		len = str.length();
		if (str[len-1] == ' ' || str[len-1] == '\t')
		{
			str = str.substr(0,len-1);
		}
		else 
		{break;}
	}
	return str;

}

/*function to check get the  start and end points of body of html file */

bool tag_present(string str){
	
	int i=0;
	string temp;
	while(i < str.length()){
		if (str[i] == '<'){
			temp = whiteSpace(str.substr(i+1));
			if (temp.substr(0,4) == "body" || temp.substr(0,5) == "/body") { return true;}}
		i++;
		}
	return false;

}

//func. to count number of non terminated script tags in a line.
// <script> considered as '+1' </script> as '-1'

int scriptcounter(string str){
	
	int i=0;
	int j = 0;
	int count = 0;
	string temp;
	while(i < str.length()){
		if (str[i] == '<'){
			temp = whiteSpace(str.substr(i+1));
			if (temp.substr(0,6) == "script") { j = 1; count++;}
			if (temp.substr(0,7) == "/script") { count--;}
		}
		i++;
		}
	if(count == 0 && j == 1){return -3;}	
	else {return count;}

}


//func. to count number of non terminated style tags in a line.
// <style> considered as '+1' </style> as '-1'

int stylecounter(string str){
	
	int i=0;
	int j = 0;
	int count = 0;
	string temp;
	while(i < str.length()){
		if (str[i] == '<'){
			temp = whiteSpace(str.substr(i+1));
			if (temp.substr(0,5) == "style") { j = 1; count++;}
			if (temp.substr(0,6) == "/style") { count--;}
		}
		i++;
		}
	if(count == 0 && j == 1){return -3;}	
	else {return count;}

}


//default constructor takes the name of html file to be displayed.
mylynx::mylynx(string filname){
 
	ifstream filein;
  string line;
  filein.open(filname.c_str());
  if(filein.is_open())
  { 
	  while(filein.good())
	  {
		  getline(filein,line);
		  line = whiteSpace(line);
		  if (tag_present(line)){ //checking if the body of html file has started. 
		  	
		  	if (scriptcounter(line) == 0){lines.push_back(line);}
		  	else if (scriptcounter(line) == 1){ //skipping the script lines.
		  		while(filein.good()){
		  			getline(filein,line);
		  			if(scriptcounter(line)== -1){break;}
		  		}
		  	}
		  	
		  	if (stylecounter(line) == 0){lines.push_back(line);}
		  	else if (stylecounter(line) == 1){ //skipping the style lines.
		  		while(filein.good()){
		  			getline(filein,line);
		  			if(stylecounter(line)== -1){break;}
		  		}
		  	}
		  	
		  	
		  	while(filein.good()){
		  		getline(filein,line);
		  		line = whiteSpace(line);
		  		if (scriptcounter(line) == -3){continue;} //skipping the lines which are script.
		  		if (scriptcounter(line) == 1){ //skipping the script lines.
		  			while(filein.good()){
		  				getline(filein,line);
		  				if(scriptcounter(line)== -1){break;}
		  			}
		  			continue;
		  		}
		  		
		  		if (stylecounter(line) == -3){continue;} //skipping the lines which are style.
		  		if (stylecounter(line) == 1){ //skipping the style lines.
		  			while(filein.good()){
		  				getline(filein,line);
		  				if(stylecounter(line)== -1){break;}
		  			}
		  			continue;
		  		}
		  		
		  		
		  		
					lines.push_back(line);
		  		if (tag_present(line)){break;}
		  	}
		  	break;
		  }
		}
	}
	return;	  	

}  	

/* func. ot find number of tags in a line */
		  	
int mylynx::no_of_tags(string str){

	int i = 0;
	int count = 0;
	while(str[i] != '\0'){
		if(str[i] == '>' || str[i] == '<'){count++;}
		i++;
	}
	count = count/2;
	return count;

}

/*function to find number of characters ina line */

int mylynx::no_of_char(string str){

	int i = 0;
	int count = 0;
	int cruc = 1;
	int count1 = 0;
	while(str[i] != '\0'){
		if (str[i] == '>' && count1 == 0) {count = 0; i++; continue;}		  	
		if (str[i] == '<') {
			count1++;
			while(1){
				if (str[i] == '>'){i++; break;}
				i++; 
			}
			continue;
		}
		count++;
		i++;
	}
	return count;

}

/*finding TTR for each line and creating an array with the obtained values */

void mylynx::createTTR(){

	int m,n;
	int i = 0;
	while(i < lines.size()){
		m = no_of_char(lines[i]);
		n = no_of_tags(lines[i]);
		if (n == 0){TTR.push_back(m);}
		else { TTR.push_back(m/n);}
		i++;
	}
	i = 0;
	while(i < TTR.size()){
		i++;
	}

}	

/*smoothening the ttr array to decrease the possibility of error
taking radius as 2 to smoothen TTR value of a particular liune.*/

float mylynx::smooth(int index){

	int i;
	float smooth1 = 0;
	i = index - 2;
	while(i < index+3){
		if (i > -1 && i < lines.size()){smooth1 = smooth1 + TTR[i];}
		i++;
	}
	return smooth1;

}

/*smoothening all the lines */

void mylynx::smoothen(){

	int i = 0;
	float sum = 0;
	while(i < lines.size()){
		smoothTTR.push_back(this->smooth(i));
		sum = sum + (this->smooth(i));//can improve this.
		i++;
	}
	mean = sum / (lines.size());

}

/*finding the standard deviation of TTR value for the lines*/

void mylynx::setsd(){

	int i = 0;
	float sum = 0;
	while(i < lines.size()){
		sum = sum + ((smoothTTR[i] - mean)*(smoothTTR[i] - mean));
		i++;
	}
	sd = sqrt(sum / (lines.size())) ;

}

/*rendering a text line in html*/

string mylynx::render(string str){

	int i = 0;
	int temp;
	int j ;
	int no_al = 0;
	string render1;
	int cruc = 1;
	int count1 = 0;
	string tlink;
	while(str[i] != '\0'){
		
		//to fit output into half screen.
		
		if ( str[i] == ' '  &&  no_al > 100) { no_al = 0; render1.push_back('\n');}
	  
	  //checking for tags like &nsb..
	  
	  if (str[i] == '&'){
	    j = 0;
	    while(j < 10){
	      i++;
	      if (str[i] == ';'){break;}
	      j++;
	    }
	    i++;
	    continue;
	  }
	  
	  //checking whether a non terminated tag has terminated in this line.
	  
		if (str[i] == '>' && count1 == 0) {render1.clear(); i++; continue;}		  	
		
		/*checking for presence of a link*/
		
		if (str[i] == '<') {
			count1++;
			if((str.substr(i)).size() > 7) {
				
				/*to extract links from line*/
				
				if(str.substr(i,7) == "<a href"){
					temp = i;
					i = i+3;
					while(1)
						{
							if(str[i] == '"'){
								i++;
								tlink.clear();
								while(str[i] != '"' && str.substr(i,4) != "&amp" ){
									tlink.push_back(str[i]);
									i++;
								}
								break;
							}
							i++;
						}
					while(str[i] != '>'){i++;}
				i = temp;
				
				//storing links in a vector named 'links'.
				
				links.push_back(tlink);
				render1.push_back('[');
				stringstream s;
				s << (links.size());
				string str = s.str();
				//str=itoa(links.size());
				render1.append(str);				
				render1.push_back(']');
			}
			}
						
		//checking for a paragraph break.	
			
		if(str.substr(i,2) == "<p" || str.substr(i,3) == "</p"){render1.push_back('\n');}
		
		// checking for the beginning of a tag and skipping it.
		
		while(1){
			if (str[i] == '\0') {break;}
			if (str[i] == '>'){i++;break;}
			i++; 
		}
		continue;
		}
		
		render1.push_back(str[i]);
		no_al++;
		i++;
	}
	
	render1.push_back(' ');
	return render1;

}

		
void mylynx::printmain(){
	
	ofstream fileout;
	string main;
	int i = 0;
	
	while(i < lines.size()){
	
		//checking whether the line belong to main content.
		
		if(smoothTTR[i] > sd) {main.append(this->render(lines[i]));}
		i++;
	}
	
	fileout.open("input.txt");
	fileout << endl;
	fileout << main << endl;
	
	//displaying the links so that user can select one.
	
	fileout << "************************links*******************" << endl;
	for(i = 0; i < links.size() ; i++){fileout << "  " << "**"  << " " << i+1 << " " << "**" << "    " << links[i] << endl;}	
	return;

}

//function that returns alllinkspresent in a webpage.

vector<string> mylynx::getLinks(){

  return links;

}


#endif
