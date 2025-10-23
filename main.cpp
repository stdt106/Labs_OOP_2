#include <iostream>
#include <string>
#include <stdexcept>

class Four {
private:
    size_t size;  
    unsigned char* digits; 
             
public:
    Four() : digits(nullptr), size(0) {
        //std::cout << "Created Octal by default." << std::endl;
    }

    Four(const size_t &n, const unsigned char v = 0) : size(n) {
        digits = new unsigned char[size];
        for (size_t i = 0; i < size; i++) {
            digits[i] = v;
        }
    }

    Four(const std::initializer_list<unsigned char> &v) {
        size = v.size();
        if (size == 0) {
            digits = nullptr;
            return;
        }
        digits = new unsigned char[size];
        size_t ind = 0;
        for (unsigned char dig : v) {
            if (dig > 3) {
                delete[] digits;
                throw std::invalid_argument(
                    "Invalid octal digit: " + std::to_string(dig) + 
                    ". Must be between 0 and 3."
                );
            }
            digits[ind++] = dig;
        }
    }

    Four(const std::string &str) {
        size = str.length();
        if (size == 0) {
            digits = nullptr;
            return;
        }
        digits = new unsigned char[size];
        for (size_t i = 0; i < size; i++) {
            char c = str[i];
            if (c < '0' || c > '3') {
                delete[] digits;
                throw std::invalid_argument(
                    "Invalid four digit: " + std::string(1, c) + 
                    ". Must be between 0 and 3."
                );
            }
            digits[i] = c - '0';
        }
    }

    Four(const Four &other) {
        if (other.size == 0) {
            digits = nullptr;
            size = 0;
            return;
        }
        digits = new unsigned char[other.size];
        size = other.size;
        for (size_t i = 0; i < size; i++) {
            digits[i] = other.digits[i];
        }
    }

    Four& operator=(const Four &other) {
        if (this == &other) {
            return *this;
        }
        delete[] digits;
        if (other.size == 0) {
            digits = nullptr;
            size = 0;
            return *this;
        } else {
            digits = new unsigned char[other.size];
            size = other.size;
            for (size_t i = 0; i < size; i++) {
                digits[i] = other.digits[i];
            }
        }
        return *this;
    }

    Four(Four&& other) noexcept : digits(other.digits), size(other.size) {
        other.digits = nullptr;
        other.size = 0;
    }

    virtual ~Four() noexcept {
        delete[] digits;
    }

    std::string toString() const {
        if (isEmpty()) {
            return "0";
        }
        std::string result;
        for (size_t i = 0; i < size; i++) {
            result += std::to_string(digits[i]);
        }
        return result;
    }

    bool isEmpty() const { return size == 0 || digits == nullptr; }

    Four copy() const { return Four(*this); }

    size_t getSize() const { return size; }

    unsigned char getDigit(size_t index) const {
        if (index >= size) { throw std::out_of_range("Out of range."); }
        return digits[index];
    } 

    void print() const {
        std::cout << "Your Four number: ";
        for (size_t i = 0; i < size; i++) {
            std::cout << static_cast<int>(digits[i]);
        }
        std::cout << std::endl;
    }

    Four removeLeadingZeros() const {
        if (isEmpty()) { return Four(); }
        size_t start = 0;
        while (start < size && digits[start] == 0) {
            start++;
        }
        if (start == size) {
            return Four(1, 0);
        }
        size_t new_size = size - start;
        Four result(new_size);
        for (size_t i = 0; i < new_size; i++) {
            result.digits[i] = digits[start + i];
        }
        return result;
    }

Four add(const Four& other) const {
    if (isEmpty()) { return other.copy(); }
    if (other.isEmpty()) { return copy(); }
    
    int max_size = (size > other.size) ? (int)size : (int)other.size;
    Four res(max_size + 1);
    int next_dig = 0;
    
    for (int i = 0; i < max_size; i++) {
        int idx1 = (int)size - 1 - i;
        int idx2 = (int)other.size - 1 - i;
        
        unsigned char d1 = (idx1 >= 0) ? digits[idx1] : 0;
        unsigned char d2 = (idx2 >= 0) ? other.digits[idx2] : 0;
        
        int sum = d1 + d2 + next_dig;
        res.digits[res.size - 1 - i] = sum % 4;
        next_dig = sum / 4;
    }
    
    if (next_dig > 0) {
        res.digits[0] = next_dig;
    } else {
        return res.removeLeadingZeros();
    }
    return res;
}

    Four subtract(const Four& other) const {
        if (isEmpty()) {
            throw std::invalid_argument("Result would be negative");
        }
        if (other.isEmpty()) {
            return this->copy();
        }
    
        if (this->lessThan(other)) {
            throw std::invalid_argument("Result would be negative");
        }
    
        int max_size = (size > other.size) ? (int)size : (int)other.size;
        Four result(max_size, 0);
        int borrow = 0;
    
        for (int i = 0; i < max_size; i++) {
            int idx1 = (int)size - 1 - i;
            int idx2 = (int)other.size - 1 - i;
            int res_idx = max_size - 1 - i;
        
            int digit1 = (idx1 >= 0) ? digits[idx1] : 0;
            int digit2 = (idx2 >= 0) ? other.digits[idx2] : 0;
        
            int diff = digit1 - digit2 - borrow;
        
            if (diff < 0) {
                diff += 4;
                borrow = 1;
            } else {
                borrow = 0;
            }
        
            result.digits[res_idx] = diff;
        }
    
        if (borrow > 0) {
            throw std::runtime_error("Unexpected borrow after subtraction");
        }
        return result.removeLeadingZeros();
    }

    bool lessThan(const Four& other) const {
        return compare(other) < 0;
    }

    bool greaterThan(const Four& other) const {
        return compare(other) > 0;
    }

    bool equals(const Four& other) const {
        return compare(other) == 0;
    }

    int compare(const Four& other) const {
        Four a = this->removeLeadingZeros();
        Four b = other.removeLeadingZeros();
    
        if (a.size > b.size) return 1;
        if (a.size < b.size) return -1;
    
        for (size_t i = 0; i < a.size; i++) {
            if (a.digits[i] > b.digits[i]) return 1;
            if (a.digits[i] < b.digits[i]) return -1;
        } 
        return 0; 
    }
};