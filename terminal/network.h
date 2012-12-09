#ifndef NETWORK_H
#define NETWORK_H
#include <iostream>
#include <stdio.h>
#include<string>
#include"Gsearch.h"
#include"mylynx.h"
using namespace std;

class network
{
  
  private:
  string uinput;
	
	string output; 
  
	public: 
	int getwebpage(string); 
	vector<string> links;
	void setuinput(string);
	void operationtoggler();
  	
};	

/***************************************************************************************************************************************************************
                                                                    class Func. Defns.
***************************************************************************************************************************************************************/

//takes a webadress or a search string as input and sets it as uinput member of class.

void network::setuinput(string in)
{
  
  uinput=in;

}	

//the main function which creates Gsearch object or mylynx object according to input given

void network::operationtoggler()
{
  
  links.clear();
  
  while (true)
  {
		if (uinput[0] == ' ' || uinput[0] == '\t')
		{
			uinput = uinput.substr (1);
		}
		else {break;}
  }
  
  while (true)
  {
    
    size_t found;
    found=uinput.find(" ");
    
    if (found!=string::npos)
    {
      uinput.replace(found,1,"+");
    }
    
    else{break;}
  }
  
  //checking whether its a link or a search string.
  
  if(uinput.substr(0,4)=="http")
  {
  
    //building the shell comand to get the webpage into tmp.html
    
    string s1="wget ";
    string s2=" -O tmp.html";
    string shellcmd;
    shellcmd=s1+uinput+s2;
    getwebpage(shellcmd);
    
    //creating mylynx object and threby parsing and rendering 
    //the html file.
    
    mylynx temp ("tmp.html");
    temp.createTTR();
    temp.smoothen();
    temp.setsd();
    temp.printmain();
    links = temp.getLinks();
  
  }
  
  else
  {
  	
  	//building the shell command to get the webpage.
  	
  	string s1,s2,shellcmd,renamecmd;
  	s1= "wget -U \"mozilla\" \"https://www.google.com/search?q=";
  	shellcmd=s1+uinput+"\"";
  	getwebpage(shellcmd);
  	
  	//renaming the html file to tmp.html
  	
  	string a,b,c,d;
  	getwebpage("rm tmp.html");
  	a="mv ";
  	b="search?q=";
  	c=" tmp.html";
  	renamecmd=a+b+uinput+c;
  	getwebpage(renamecmd);
  	
  	//creating gsearch object and extracting search results.
  	
  	Gsearch gun;
	  gun.mainrender();
	  gun.print();
	  links = gun.getResults();
  }


}	
	
//program to execute a shell command.	
	
int network::getwebpage(string command) {
	
			//cout<<command<<endl;
			FILE *in;
	    	    
			char buff[512];
	 
	    if(!(in = popen(command.c_str(), "r"))){
	        return 1;
	    }
	 
	    while(fgets(buff, sizeof(buff), in)!=NULL){
	        cout << buff;
	    }
	    pclose(in);
	 
	    
	    return 0;
}
	
#endif	
