# 41443119

作業一

## problem1

## 解題說明

本題目標為計算 Ackermann 函數 $A(m, n)$，並分別以遞迴（Recursive）**與**非遞迴（Non-recursive / Iterative）兩種方式進行實作。

Ackermann 函數的算術規則如下：

* 當 $m = 0$ 時：$A(m, n) = n + 1$

* 當 $m > 0$ 且 $n = 0$ 時：$A(m, n) = A(m - 1, 1)$

* 當 $m > 0$ 且 $n > 0$ 時：$A(m, n) = A(m - 1, A(m, n - 1))$

## 程式實作

以下為 C++ 實作程式碼，採用現代 C++ 風格並改用 `std::vector` 來作為非遞迴版本的堆疊模擬：

```cpp
#include <iostream>
#include <vector>

unsigned long long ackermannRecursive(unsigned long long m, unsigned long long n) {
    if (m == 0) {
        return n + 1;
    } 
    if (n == 0) {
        return ackermannRecursive(m - 1, 1);
    } 
    return ackermannRecursive(m - 1, ackermannRecursive(m, n - 1));
}

unsigned long long ackermannIterative(unsigned long long m, unsigned long long n) {
    std::vector<unsigned long long> st;
    st.push_back(m);

    while (!st.empty()) {
        m = st.back();
        st.pop_back();

        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            n = 1;
            st.push_back(m - 1);
        } else {
            st.push_back(m - 1);
            st.push_back(m);
            n = n - 1;
        }
    }
    return n;
}

int main() {
    unsigned long long m = 2, n = 2;
    std::cout << "Recursive Output: " << ackermannRecursive(m, n) << std::endl;
    std::cout << "Iterative Output: " << ackermannIterative(m, n) << std::endl;
    return 0;
}
```

## 效能分析

* **時間複雜度**：由於 Ackermann 函數增長速度極快，其時間開銷取決於總呼叫次數，時間複雜度界定為 $\mathcal{O}(A(m, n))$。

* **空間複雜度**：

  * 遞迴版本：空間消耗來自系統Call Stack，最大深度為 $\mathcal{O}(A(m, n))$。

  * 非遞迴版本：顯式堆疊（Stack）的大小最高亦會達到 $\mathcal{O}(A(m, n))$。

## 測試與驗證

| 測試案例 | 輸入條件 $(m, n)$ | 預期與實際輸出 | 
| ----- | ----- | ----- | 
| Case 1 | $A(0, 0)$ | `1` | 
| Case 2 | $A(1, 2)$ | `4` | 
| Case 3 | $A(2, 2)$ | `7` | 
| Case 4 | $A(3, 1)$ | `13` | 
| Case 5 | $A(3, 3)$ | `61` | 

### 編譯結果

```text
g++ -std=c++17 -O2 src/problem1.cpp -o problem1
./problem1
Recursive Output: 7
Iterative Output: 7
```

## 申論及開發報告

遞迴版本是直接依照 Ackermann 函式的數學定義進行實作，透過函式自己呼叫自己的方式完成計算，遞迴程式的結構較為簡潔，也容易理解，但當遞迴層數過深時，會消耗較多的記憶體，且可能降低執行效率，甚至造成 Stack Overflow，非遞迴版本則不使用函式自行呼叫的方式，而是利用 Stack 來模擬遞迴的執行過程。由於 Stack 具有後進先出（LIFO）的特性，可以將尚未完成的計算工作暫時保存，並依照正確的順序取出處理，因此能夠模擬原本遞迴函式的執行方式。

## problem2

## 解題說明

本題目標為計算一個集合的 Powerset（冪集），亦即找出該集合所有可能的子集合，並以遞迴方式完成。

例如：
$S = \{a, b, c\}$

每一個元素都有「選擇」和「不選擇」兩種情況：

1. 不選擇目前的元素。

2. 選擇目前的元素。

透過遞迴將每個元素分成這兩種情況，就可以找出所有可能的子集合。當所有元素都處理完時，就將目前的結果輸出。

因為每個元素都有 $2$ 種選擇，所以如果集合有 $n$ 個元素，總共有 $2^n$ 個子集合。

## 程式實作

以下採用 C++ 實作，利用傳址與回溯（Backtracking）概念產生冪集：

```cpp
#include <iostream>
#include <vector>
#include <string>

void generatePowerset(const std::vector<std::string>& set, size_t index, std::vector<std::string>& current) {
    if (index == set.size()) {
        std::cout << "{ ";
        for (size_t i = 0; i < current.size(); ++i) {
            std::cout << current[i] << (i + 1 < current.size() ? ", " : " ");
        }
        std::cout << "}\n";
        return;
    }

    generatePowerset(set, index + 1, current);

    current.push_back(set[index]);
    generatePowerset(set, index + 1, current);
    
    current.pop_back();
}

int main() {
    std::vector<std::string> S = {"a", "b", "c"};
    std::vector<std::string> current;
    generatePowerset(S, 0, current);
    return 0;
}
```

## 效能分析

* **時間複雜度**：產生 $n$ 個元素的所有子集合共需要走訪 $2^n$ 個狀態，因此時間複雜度為 $\mathcal{O}(2^n)$。

* **空間複雜度**：遞迴樹的最大深度等於集合大小 $n$，故系統 Stack 與暫存容器之空間複雜度皆為 $\mathcal{O}(n)$。

## 測試與驗證

輸入集合：$S = \{a, b, c\}$

輸出證明：

```text
{ }
{ c }
{ b }
{ b, c }
{ a }
{ a, c }
{ a, b }
{ a, b, c }
```

### 編譯結果

```text
g++ -std=c++17 -O2 src/problem2.cpp -o problem2
./problem2
{ }
{ c }
{ b }
{ b, c }
{ a }
{ a, c }
{ a, b }
{ a, b, c }
```

## 申論及開發報告

這題是利用遞迴的方式來產生集合的所有子集合。每處理一個元素時，都會分成「選擇」和「不選擇」兩種情況，再繼續處理下一個元素。當所有元素都處理完成後，就會將目前產生的子集合輸出，因為每個元素都有兩種選擇，所以一個有 $n$ 個元素的集合，最後會產生 $2^n$ 個子集合，在開發過程中，我使用 vector 來存放原本的集合以及目前產生的子集合，並利用 index 判斷目前處理到哪一個元素，透過遞迴不斷處理「選擇」與「不選擇」的情況，最後成功產生所有可能的子集合，透過這次作業，我更加了解遞迴的使用方式，也了解到冪集的數量會隨者集合元素增加而快速增加。
