// a2_test.cpp

#include "Stringlist.h"
#include <cassert>

using namespace std;

int main() {
    Stringlist lst;
    Stringlist lst2;
    lst.insert_before(0,"four");
    lst.insert_before(1,"one");
    lst.insert_before(1,"two");
    lst.insert_before(1,"three");
    cout<<lst<<endl;  

    lst2.insert_before(0,"four");
    lst2.insert_before(1,"one");
    lst2.insert_before(1,"two");
    lst2.insert_before(1,"a");
    cout<<lst2<<endl;

    lst = lst2;
    cout<<lst2<<endl; 
    cout<<lst<<endl;

    lst.undo(); 
    cout<<lst2<<endl;   
    cout<<lst <<endl;
  
    lst.remove_all();
    cout<<lst<<endl;

    lst.undo();
    cout<<lst<<endl;
 
   
    


}
