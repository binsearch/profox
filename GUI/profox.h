#ifndef PROFOX_H
#define PROFOX_H

#include <QMainWindow>
#include<cstring>
#include<vector>
#include<stack>
using namespace std;
namespace Ui {
class profox;
}

class profox : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit profox(QWidget *parent = 0);
    ~profox();
    
private slots:
    void on_gobutton_clicked();



    void on_backbutton_clicked();

    void on_forwardbutton_clicked();

    void on_linksbutton_clicked();

private:
    Ui::profox *ui;
    std::vector<std::string> linkvec;
    void printtxt();
    std::string getalltext();
    std::stack < std::pair < std::string,std::vector<std::string> > > PrevPages;
    std::stack < std::pair < std::string,std::vector<std::string> > > nextPages;
    std::stack<std::string> prevlinks;
    std::stack<std::string> forwardlinks;
};

#endif // PROFOX_H
