#include <iostream>

struct Rat
{
    long long numerator, denominator;
    Rat() : numerator(0), denominator(1) {}
    Rat(long long n) : numerator(n), denominator(1) {}
    Rat(long long top, long long bottom) : numerator(top), denominator(bottom) {};
    long long gcd(long long v1, long long v2)
    {
        long long ret = v2;
        for (v2 = v1 % ret; v2; v1 = ret, ret = v2, v2 = v1 % ret);
        return ret;
    }
    void red()
    {
        if (denominator < 0)
        {
            denominator = -denominator;
            numerator = -numerator;
        }
        long long g = gcd(abs(numerator), abs(denominator));
        numerator /= g;
        denominator /= g;
    }
    Rat operator+ (Rat a)
    {
        Rat ret(numerator * a.denominator + denominator * a.numerator, denominator * a.denominator);
        ret.red();
        return ret;
    }
    Rat operator- (Rat a)
    {
        Rat ret(numerator * a.denominator - denominator * a.numerator, denominator * a.denominator);
        ret.red();
        return ret;
    }
    Rat operator* (Rat a)
    {
        Rat ret(numerator * a.numerator, denominator * a.denominator);
        ret.red();
        return ret;
    }
    Rat operator/ (Rat a)
    {
        Rat ret(numerator * a.denominator, denominator * a.numerator);
        ret.red();
        return ret;
    }
};
bool operator== (Rat a, Rat b) {return a.numerator * b.denominator == a.denominator * b.numerator;}
bool operator!= (Rat a, Rat b) {return a.numerator * b.denominator != a.denominator * b.numerator;}
bool operator< (Rat a, Rat b) {return a.numerator * b.denominator < a.denominator * b.numerator;}
bool operator> (Rat a, Rat b) {return a.numerator * b.denominator > a.denominator * b.numerator;}
bool operator>= (Rat a, Rat b) {return a.numerator * b.denominator >= a.denominator * b.numerator;}
bool operator<= (Rat a, Rat b) {return a.numerator * b.denominator <= a.denominator * b.numerator;}

std::ostream& operator<< (std::ostream& stream, const Rat& a)
{
    stream << a.numerator << "/" << a.denominator;
    return stream;
}

using namespace std;

int main()
{
    Rat a(3, 5), b(147, 105), tmp;
    b.red();
    cout << a + b << endl << a * b << endl << a - b << a / b << endl;
}