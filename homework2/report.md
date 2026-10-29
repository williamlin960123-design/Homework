#include<iostream>
#include <algorithm>
using namespace std;

class Polynomial;

class Term {
	friend Polynomial;
private:
	float coef;
	int exp;
};

class Polynomial {
private:
	Term* termarray;
	int capacity;
	int terms;
public:
	Polynomial() :capacity(2), terms(0) {
		termarray = new Term[capacity];
	}
    void newTerm(const float theCoef, const int theExp);
    Polynomial Add(const Polynomial& b);
};

void Polynomial::newTerm(const float theCoef, const int theExp) {
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termarray, termarray + terms, temp);
        delete[]termarray;
        termarray = temp;
    }
    termarray[terms].coef = theCoef;
    termarray[terms++].exp = theExp;
}

Polynomial Polynomial::Add(const Polynomial& b) {
    Polynomial c;
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < b.terms)) {
        if (termarray[aPos].exp == b.termarray[bPos].exp) {
            float t = termarray[aPos].coef + b.termarray[bPos].coef;
            if (t) c.newTerm(t, termarray[aPos].exp);
            aPos++; bPos++;
        }
        else if (termarray[aPos].exp < b.termarray[bPos].exp) {
            c.newTerm(b.termarray[bPos].coef, b.termarray[bPos].exp);
            bPos++;
        }
        else {
            c.newTerm(termarray[aPos].coef, termarray[aPos].exp);
            aPos++;
        }
    }

    for (; aPos < terms; aPos++)
        c.newTerm(termarray[aPos].coef, termarray[aPos].exp);
    for (; bPos < b.terms; bPos++)
        c.newTerm(b.termarray[bPos].coef, b.termarray[bPos].exp);

    return c;
}

int main() {
    return 0;
}
