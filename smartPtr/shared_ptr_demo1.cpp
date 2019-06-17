#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;


int main()
{
    shared_ptr<string> pNico {new string("nico")};
    shared_ptr<string> pJutta (new string("jutta"));
    shared_ptr<string> pNico(new string("nico"), [](string* p) {cout << "delete" << *p << endl;});
    
    vector<shared_ptr<string> >whoMade;
    whoMade.push_back(pJutta);
    whoMade.push_back(pJutta);
    whoMade.push_back(pNico);
    whoMade.push_back(pNico);

    *pNico = "NICO";

    cout << whoMade[2].use_count() << endl;
    for (auto ptr : whoMade) {
        cout << *ptr << endl;
    }

    return 0;

}