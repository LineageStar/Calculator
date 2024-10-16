#ifndef _BIG_INTEGER_H_
#define _BIG_INTEGER_H_

#include <iostream>
#include <vector>
// #include <bitset>
#include <string>

using i64 = long long;
const int MOD = 10;

class Big_Integer {
    public:
        Big_Integer() {
            num_len = 1;
            r_num.assign(1, '0');
        }
        Big_Integer(const std::string& number) {
            num_len = number.size();
            r_num.assign(number.rbegin(), number.rend());
        }
        template<typename T>
        Big_Integer(T number) {
            while (number) {
                r_num.push_back(num % MOD - '0');
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
        Big_Integer& operator=(Big_Integer&& other) {
            num_len = other.num_len;
            r_num = std::move(other.r_num);
            return *this;
        }
        Big_Integer operator+(const Big_Integer& other) const {
            Big_Integer result;
            int carry = 0;
            for (int i = 0; i < std::max(num_len, other.num_len) || carry; ++i) {
                if (i == result.num_len) {
                    result.r_num.push_back(0);
                    ++result.num_len;
                }
                result.r_num[i] += carry + (i < num_len ? r_num[i] : 0) + (i < other.num_len ? other.r_num[i] : 0);
                carry = result.r_num[i] >= MOD;
                if (carry) {
                    result.r_num[i] -= MOD;
                }
            }
            return result;
        }
        Big_Integer operator-(const Big_Integer& other) const {
            Big_Integer result;
            int carry = 0;
            for (int i = 0; i < num_len || carry; ++i) {
                result.r_num.push_back(0);
                result.r_num[i] = r_num[i] - carry - (i < other.num_len ? other.r_num[i] : 0);
                carry = result.r_num[i] < 0;
                if (carry) {
                    result.r_num[i] += MOD;
                }
            }
            while (result.num_len > 1 && result.r_num.back() == 0) {
                result.r_num.pop_back();
                --result.num_len;
            }
            return result;
        }
        Big_Integer operator*(const Big_Integer& other) const {
            Big_Integer result;
            result.num_len = num_len + other.num_len;
            result.r_num.assign(result.num_len, 0);
            for (int i = 0; i < num_len; ++i) {
                for (int j = 0, carry = 0; j < other.num_len || carry; ++j) {
                    long long cur = result.r_num[i + j] + r_num[i] * 1ll * (j < other.num_len ? other.r_num[j] : 0) + carry;
                    result.r_num[i + j] = cur % MOD;
                    carry = cur / MOD;
                }
            }
            while (result.num_len > 1 && result.r_num.back() == 0) {
                result.r_num.pop_back();
                --result.num_len;
            }
            return result;
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
                result.r_num.insert(result.r_num.begin(), x);
                result.num_len = result.r_num.size();
                current = current - other * x;
            }
            while (result.num_len > 1 && result.r_num.back() == 0) {
                result.r_num.pop_back();
                --result.num_len;
            }
            return result;
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
                result.r_num.insert(result.r_num.begin(), x);
                result.num_len = result.r_num.size();
                current = current - other * x;
            }
            return current;
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
        bool operator>(const Big_Integer& other) const {
            return other < *this;
        }
        bool operator<=(const Big_Integer& other) const {
            return !(other < *this);
        }
        bool operator>=(const Big_Integer& other) const {
            return !(*this < other);
        }
        bool operator==(const Big_Integer& other) const {
            return !(*this < other) && !(other < *this);
        }
        bool operator!=(const Big_Integer& other) const {
            return *this < other || other < *this;
        }
        friend std::ostream& operator<<(std::ostream& out, const Big_Integer& number);
        friend std::istream& operator>>(std::istream& in, Big_Integer& number);
    private:
        int num_len;
        std::vector<char> r_num;
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
