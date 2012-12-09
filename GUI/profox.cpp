#include "profox.h"
#include "ui_profox.h"
#include <QtCore/QFile>
#include<cstring>
#include <QtCore/QTextStream>
#include<network.h>
#include<mylynx.h>
#include<Gsearch.h>
#include<sstream>
#include<fstream>

profox::profox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::profox)
{
    ui->setupUi(this);

}

profox::~profox()
{
    delete ui;
}
void profox::printtxt()
 {
     QFile inputFile("input.txt");
     inputFile.open(QIODevice::ReadOnly);

     QTextStream in(&inputFile);
     QString line = in.readAll();
     inputFile.close();

     ui->textEdit->setPlainText(line);
     //QTextCursor cursor = ui->textEdit->textCursor();
     //cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
 }

void profox::on_gobutton_clicked()
{
    QString in;
        std::string inp;

        in=ui->lineEdit->text();
        inp = in.toStdString();
        network x;
        std::string temptext;
        x.setuinput(inp);
        x.operationtoggler();
        temptext = getalltext();
        prevlinks.push(inp);
        PrevPages.push(std::pair < std::string,std::vector<std::string> > (temptext , x.links));

        linkvec=x.links;
        printtxt();
    //    return 0;

}

void profox::on_backbutton_clicked()
{
    std::pair<std::string, std::vector<std::string> > TempPage;
    std::string link;

    if(prevlinks.size()==1){}
    else{
        nextPages.push(PrevPages.top());
        forwardlinks.push(prevlinks.top());
        PrevPages.pop();
        prevlinks.pop();
        TempPage = PrevPages.top();
        link = prevlinks.top();
        std::ofstream fileout;
        fileout.open("input.txt");
        fileout << TempPage.first;
        fileout.close();
        ui->lineEdit->clear();
        QString qstr(link.c_str());
        ui->lineEdit->insert(qstr);
        linkvec = TempPage.second;
        printtxt();
    }

}

void profox::on_forwardbutton_clicked()
{
    if(forwardlinks.size()==0){}
    else{

    std::pair<std::string, std::vector<std::string> > TempPage;
    std::string link;
    TempPage = nextPages.top();
    PrevPages.push(TempPage);
    link= forwardlinks.top();
    prevlinks.push(link);
    nextPages.pop();
    forwardlinks.pop();
    std::ofstream fileout;
    fileout.open("input.txt");
    fileout << TempPage.first;
    fileout.close();
    ui->lineEdit->clear();
    QString qstr(link.c_str());
    ui->lineEdit->insert(qstr);
    linkvec = TempPage.second;
    printtxt();
    }
}

void profox::on_linksbutton_clicked()
{
    QString in;
        std::string inp,link;
        std::string temptext;
        in=ui->linksinput->text();
        inp = in.toStdString();

        int value = atoi(inp.c_str());
        link=linkvec[value-1];
        network x;
        x.setuinput(link);
        x.operationtoggler();
        linkvec.clear();
        linkvec=x.links;
        ui->lineEdit->clear();
        QString qstr(link.c_str());
        ui->lineEdit->insert(qstr);
        ui->linksinput->clear();
        temptext = getalltext();

        PrevPages.push(std::pair < std::string,std::vector<std::string> > (temptext , x.links));
        prevlinks.push(link);
        printtxt();
}

std::string profox::getalltext(){

    std::string text;
    std::string line;

    std::ifstream filein;
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
