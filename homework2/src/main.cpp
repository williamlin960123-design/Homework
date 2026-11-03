#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& p);
    friend istream& operator>>(istream& in, Polynomial& p);
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    Polynomial(const Polynomial& other) {
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        std::copy(other.termArray, other.termArray + other.terms, termArray);
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            delete[] termArray;
            capacity = other.capacity;
            terms = other.terms;
            termArray = new Term[capacity];
            std::copy(other.termArray, other.termArray + other.terms, termArray);
        }
        return *this;
    }

    void newTerm(const float theCoef, const int theExp) {
        if (theCoef == 0) return;
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            std::copy(termArray, termArray + terms, temp);
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = theCoef;
        termArray[terms++].exp = theExp;
    }

    Polynomial Add(const Polynomial& b) {
        Polynomial c;
        int aPos = 0, bPos = 0;
        while ((aPos < terms) && (bPos < b.terms)) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float t = termArray[aPos].coef + b.termArray[bPos].coef;
                if (t != 0) c.newTerm(t, termArray[aPos].exp);
                aPos++; bPos++;
            } else if (termArray[aPos].exp < b.termArray[bPos].exp) {
                c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            } else {
                c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
        }
        for (; aPos < terms; aPos++) c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < b.terms; bPos++) c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
        return c;
    }

    Polynomial Mult(const Polynomial& b) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            Polynomial temp;
            for (int j = 0; j < b.terms; j++) {
                float newCoef = termArray[i].coef * b.termArray[j].coef;
                int newExp = termArray[i].exp + b.termArray[j].exp;
                temp.newTerm(newCoef, newExp);
            }
            result = result.Add(temp);
        }
        return result;
    }

    float Eval(float x) {
        float total = 0;
        for (int i = 0; i < terms; i++) {
            total += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return total;
    }

    friend ostream& operator<<(ostream& out, const Polynomial& p);
    friend istream& operator>>(istream& in, Polynomial& p);
};

ostream& operator<<(ostream& out, const Polynomial& p) {
    if (p.terms == 0) { out << "0"; return out; }
    for (int i = 0; i < p.terms; i++) {
        if (i > 0 && p.termArray[i].coef > 0) out << " + ";
        else if (p.termArray[i].coef < 0) out << " - ";
        float absCoef = abs(p.termArray[i].coef);
        if (p.termArray[i].exp == 0) out << absCoef;
        else if (p.termArray[i].exp == 1) out << absCoef << "x";
        else out << absCoef << "x^" << p.termArray[i].exp;
    }
    return out;
}

istream& operator>>(istream& in, Polynomial& p) {
    int count;
    cout << "請輸入多項式的非零項個數: ";
    in >> count;
    for (int i = 0; i < count; i++) {
        float c;
        int e;
        cout << "請輸入第 " << i + 1 << " 項的係數與指數 (以空格隔開): ";
        in >> c >> e;
        p.newTerm(c, e);
    }
    return in;
}

int main() {
    Polynomial A, B;
    cout << "建立多項式 A:" << endl;
    cin >> A;
    cout << "建立多項式 B:" << endl;
    cin >> B;

    while (true) {
        cout << "\n功能選單:" << endl;
        cout << "1. 多項式相加" << endl;
        cout << "2. 多項式相乘" << endl;
        cout << "3. 帶入 x 求值" << endl;
        cout << "4. 結束程式" << endl;
        cout << "請選擇功能: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            Polynomial C = A.Add(B);
            cout << "A + B = " << C << endl;
        } else if (choice == 2) {
            Polynomial D = A.Mult(B);
            cout << "A * B = " << D << endl;
        } else if (choice == 3) {
            float x;
            cout << "請輸入 x 的值: ";
            cin >> x;
            cout << "A(" << x << ") = " << A.Eval(x) << " , B(" << x << ") = " << B.Eval(x) << endl;
        } else if (choice == 4) {
            break;
        }
    }
    return 0;
}

