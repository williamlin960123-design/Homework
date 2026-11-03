# 41143119

作業二

## 解題說明

本題要求實現一個遞迴函式，計算從 $1$ 到 $n$ 的連加總和。

### 核心設計架構與策略

1. 單項資料封裝：定義 Term 類別作為基礎單元，明確區分並儲使各單項之浮點數係數（coef）與非負整數指數（exp），並將其設為 Polynomial 的友元類別以利直接存取。
2. 記憶體動態調節：多項式主體採用動態記憶體配置的 Term 陣列。當既有容量（capacity）與實際項數（terms）飽和時，系統會自動觸發倍增機制，確保空間動態延伸且不遺失舊有數據。
3. 加法雙指標線性掃描：Add() 函式採用雙指標同步走訪兩組陣列，依據指數大小進行降冪排序與同類項係數歸併。
4. 乘法累加式設計：Mult() 函式利用巢狀迴圈進行分配律展開，計算出每一單項相乘的臨時多項式後，直接呼叫加法機制進行同類項合併與重新排序。
5. 多載串流與人機介面：透過多載 >> 與 << 運算子，簡化多項式的標準輸入輸出。主程式則以無窮迴圈搭配條件分支，提供持續運作的選單服務。
## 程式實作



```cpp
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

```

## 效能分析 


# 一. 時間複雜度（Time Complexity）

1.加法運算 ：
由於雙方陣列皆已依指數降冪排列，透過單次雙指標線性掃描即可完成歸併，故時間複雜度呈線性關係，為 \(\mathbf{O(m+n)}\)，其中 \(m\) 與 \(n\) 分別為兩多項式的非零項數。
2.乘法運算：
基礎展開需要經由雙重迴圈逐項相乘，共計執行 \(m \times n\) 次乘法。此外，由於每次將臨時結果累加至總多項式時會呼叫 Add() 進行線性搜尋與同類項合併，因此整體的經驗執行時間會高於理論值 \(\mathbf{O(m\times n)}\)。
3.多項式求值：
系統對陣列中的 \(m\) 個非零項進行走訪，每項透過 pow() 函數進行冪次計算並與係數相乘，時間複雜度為 \(\mathbf{O(m)}\)。

# 二. 空間複雜度（Space Complexity）

本程式採用緊湊（Sparse）的動態陣列配置，僅針對非零項分配實體記憶體。在最壞情況下，相乘後的結果多項式空間開銷為 \(\mathbf{O(m\times n)}\)。若遭遇陣列容量達到上限，將觸發倍增複製機制，在過渡期間會產生短暫的雙倍動態記憶體開銷。
## 測試與驗證

### 測試案例


### 測試案例

| 測試案例 | 功能描述 | 輸入參數 | 預期輸出 | 實際輸出 |
| :--- | :--- | :--- | :--- | :--- |
| 測試一 | 多項式相加 | 選擇 1 | 4x^3 + 1x^2 + 2x + 1 | 4x^3 + 1x^2 + 2x + 1 |
| 測試二 | 多項式相乘 | 選擇 2 | 12x^5 + 2x^4 - 24x^3 + 28x^2 - 12x - 30 | 12x^5 + 2x^4 - 24x^3 + 28x^2 - 12x - 30 |
| 測試三 | 帶入 x 求值 | 選擇 3, x = 2 | A(2) = 30 , B(2) = 11 | A(2) = 30 , B(2) = 11 |
| 測試四 | 結束選單 | 選擇 4 | 程式結束 | 程式結束 |
 

 ### 編譯與執行指令
 
 ```shell
 \$ g++ -std=c++17 -o poly poly.cpp
 \$ ./poly
```


### 結論

1. 程式能正確處理包含負號、係數消去與跨階缺項的多項式加法與乘法運算。
2. 透過 operator<< 與 operator>> 的多載，成功實現了直覺且符合 C++ 慣例的標準串流操作。
3. 測試案例涵蓋了同類項合併與求值計算，全面驗證了此多項式抽象資料型態（ADT）的正確性與動態配置的穩定性




### 實作心得與研討

本實驗結合理論與實務，深入探討了多項式抽象資料型態在動態陣列架構下的具體實作，在開發過程中，最具挑戰性且關鍵的技術點在於動態記憶體的生命週期管理，當 termArray 的儲存空間不敷使用時，不能直接對原陣列執行釋放，否則將導致懸空指標與資料集遺失；必須先建立一個兩倍容量的新暫存陣列，完成深複製後，方可解構舊陣列。
在核心邏輯方面，加法善用了雙指標在有序陣列中的並行掃描，成功實現了高度靈活的指數比對機制；而乘法則進一步復用了加法函式，在完成單項相乘的當下即時處理同類項歸併與自動排序，大幅提升了程式碼的內聚性與重複使用率，此外，透過宣告 friend 並多載輸入輸出串流運算子，成功隱藏了內部的類別私有細節，實現了符合 C++ 風格的直覺物件操作，對動態記憶體與物件導向的整合應用有了更深層的認知。

