#ifndef _BIG_INTEGER_H_
#define _BIG_INTEGER_H_

#include <iostream>
#include <vector>
// #include <bitset>
#include <string>

using i64 = long long;
using i128 = __int128;

class Big_Integer {
public:
    Big_Integer() {
        num_len = 1;
        r_num.assign(1, '0');
    }
    Big_Integer(int len, char ch) {
        r_num.resize(len, ch);
    }
    Big_Integer(const std::string& number) {
        num_len = number.size();
        r_num.assign(number.rbegin(), number.rend());
    }
    Big_Integer(const char* number) {
        num_len = strlen(number);
        r_num.assign(number + num_len - 1, number - 1);
    }
    template<typename T>
    Big_Integer(T number) {
        while (number) {
            r_num.push_back(number % MOD + '0');
            number /= MOD;
        }
    }
    Big_Integer(const Big_Integer& other) {
        num_len = other.num_len;
        r_num = other.r_num;
    }
    Big_Integer(Big_Integer&& other) {
        num_len = other.num_len;
        r_num = std::move(other.r_num);
    }
    Big_Integer& operator=(const Big_Integer& other) {
        num_len = other.num_len;
        r_num = other.r_num;
        return *this;
    }
    template<typename T>
    Big_Integer& operator=(T number) {
        r_num.clear();
        while (number) {
            r_num.push_back(number % MOD + '0');
            number /= MOD;
        }
        return *this;
    }
    Big_Integer operator+(const Big_Integer &other) {
        int len1 = r_num.size();
        int len2 = other.r_num.size();
        int maxlen = std::max(len1, len2);
        Big_Integer temp_num;
        temp_num.r_num.resize(maxlen + 1, '0');
        int offset = 0, temp_offset;
        for (int i = 0; i < maxlen; i++) {
            int cur_offset = r_num[i] - '0' + other.r_num[i] - '0';
            temp_offset = offset + cur_offset;
            temp_num.r_num[i] = temp_offset % MOD + '0';
            offset = temp_offset / MOD;
        }
        if (offset) {
            temp_num.r_num[maxlen] = offset + '0';
        } else {
            temp_num.r_num.pop_back();
        }
        return temp_num;
    }
    template<typename T>
    Big_Integer operator+(T number) const {
        return *this + Big_Integer(number);
    }
    Big_Integer operator-(const Big_Integer& other) const {
        Big_Integer result;
        int carry = 0;
        for (int i = 0; i < num_len || carry; ++i) {
            result.r_num.push_back('0');
            result.r_num[i] = r_num[i] - carry - (i < other.num_len ? other.r_num[i] : '0');
            carry = result.r_num[i] < '0';
            if (carry) {
                result.r_num[i] += MOD;
            }
        }
        while (result.num_len > 1 && result.r_num.back() == '0') {
            result.r_num.pop_back();
            --result.num_len;
        }
        return result;
    }
    template<typename T>
    Big_Integer operator-(T number) const {
        return *this - Big_Integer(number);
    }
    Big_Integer operator*(const Big_Integer &other) {
        int len1 = this->r_num.size();
        int len2 = other.r_num.size();
        int re_len = len1 + len2;
        Big_Integer temp_num(re_len, '0');
        int offset = 0, temp_offset;
        for (int i = 0; i < len1; i++) {
            for (int j = 0; j < len2; j++) {
                int cur_offset = (this->r_num[i] - '0') * (other.r_num[j] - '0');
                temp_offset = cur_offset + offset + temp_num.r_num[i + j] - '0';
                temp_num.r_num[i + j] = temp_offset % MOD + '0';
                offset = temp_offset / MOD;
            }
            if (offset) {
                temp_num.r_num[i + len2] = temp_num.r_num[i + len2] - '0' + offset + '0';
                offset = 0;
            }
        }
        while (*(temp_num.r_num.end() - 1) == '0' && temp_num.r_num.end() - 1 != temp_num.r_num.begin()) {
            temp_num.r_num.pop_back();
        }
        return temp_num;
    }
    template<typename T>
    Big_Integer operator*(T number) const {
        return *this * Big_Integer(number);
    }
    Big_Integer operator/(const Big_Integer& other) const {
        Big_Integer result;
        Big_Integer current;
        for (int i = num_len - 1; i >= 0; --i) {
            current.r_num.insert(current.r_num.begin(), r_num[i]);
            current.num_len = current.r_num.size();
            int x = 0, l = 0, r = MOD;
            while (l <= r) {
                int m = (l + r) >> 1;
                Big_Integer cur = other * m;
                if (cur <= current) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            result.r_num.insert(result.r_num.begin(), x + '0');
            result.num_len = result.r_num.size();
            current = current - other * x;
        }
        while (result.num_len > 1 && result.r_num.back() == '0') {
            result.r_num.pop_back();
            --result.num_len;
        }
        return result;
    }
    template<typename T>
    Big_Integer operator/(T number) const {
        return *this / Big_Integer(number);
    }
    Big_Integer operator%(const Big_Integer& other) const {
        Big_Integer result;
        Big_Integer current;
        for (int i = num_len - 1; i >= 0; --i) {
            current.r_num.insert(current.r_num.begin(), r_num[i]);
            current.num_len = current.r_num.size();
            int x = 0, l = 0, r = MOD;
            while (l <= r) {
                int m = (l + r) >> 1;
                Big_Integer cur = other * m;
                if (cur <= current) {
                    x = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
            result.r_num.insert(result.r_num.begin(), x + '0');
            result.num_len = result.r_num.size();
            current = current - other * x;
        }
        return current;
    }
    template<typename T>
    Big_Integer operator%(T number) const {
        return *this % Big_Integer(number);
    }
    bool operator<(const Big_Integer& other) const {
        if (num_len != other.num_len) {
            return num_len < other.num_len;
        }
        for (int i = num_len - 1; i >= 0; --i) {
            if (r_num[i] != other.r_num[i]) {
                return r_num[i] < other.r_num[i];
            }
        }
        return false;
    }
    template<typename T>
    bool operator<(T number) const {
        return *this < Big_Integer(number);
    }
    bool operator>(const Big_Integer& other) const {
        return other < *this;
    }
    template<typename T>
    bool operator>(T number) const {
        return *this > Big_Integer(number);
    }
    bool operator<=(const Big_Integer& other) const {
        return !(other < *this);
    }
    template<typename T>
    bool operator<=(T number) const {
        return *this <= Big_Integer(number);
    }
    bool operator>=(const Big_Integer& other) const {
        return !(*this < other);
    }
    template<typename T>
    bool operator>=(T number) const {
        return *this >= Big_Integer(number);
    }
    bool operator==(const Big_Integer& other) const {
        return !(*this < other) && !(other < *this);
    }
    template<typename T>
    bool operator==(T number) const {
        return *this == Big_Integer(number);
    }
    bool operator!=(const Big_Integer& other) const {
        return *this < other || other < *this;
    }
    template<typename T>
    bool operator!=(T number) const {
        return *this != Big_Integer(number);
    }
    friend std::ostream& operator<<(std::ostream& out, const Big_Integer& number);
    friend std::istream& operator>>(std::istream& in, Big_Integer& number);
private:
    int num_len;
    std::vector<char> r_num;
    static constexpr int MOD = 10;
};

std::ostream& operator<<(std::ostream& out, const Big_Integer& number) {
    for (int i = number.num_len - 1; i >= 0; --i) {
        out << number.r_num[i];
    }
    return out;
}
std::istream& operator>>(std::istream& in, Big_Integer& number) {
    std::string s;
    in >> s;
    number = Big_Integer(s);
    return in;
}
#endif //  _BIG_INTEGER_H_