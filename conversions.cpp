#include "iostream"
#include "iomanip"
#include "sstream"
using namespace std;

int main()
{
    // Converting the content of a char array or a string to a double variable
    double d;
    string S;
    S = "4.5";
    istringstream(S) >> d;
    cout << "\nThe value of the double variable d is " << d << endl;
    istringstream("9.87654") >> d;
    cout << "\nNow the value of the double variable d is " << d << endl;



    // Converting a double to string with formatting restrictions
    double D=3.771238567;

    ostringstream Q;
    // Q.fill('#');
    Q << setprecision(6) << setw(20) << D;
    S = Q.str(); // formatted converted double is now in string

    cout << "\nThe value of the string variable S is " << S << endl;

    istringstream(S) >> d;
    cout << "\nThe value of the Q->S  is " << d << endl;
    return 0;
}
