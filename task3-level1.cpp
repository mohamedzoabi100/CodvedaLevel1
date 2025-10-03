///String Manipulation
#include <iostream>
using std::cout;
using std::endl;

char* reverseString(char* str) {
    if (str == nullptr) return nullptr;

    char* start = str;
    char* end = str;

    // Move 'end' to the last character of the string
    while (*end != '\0') {
        end++;
    }
    end--; // Set 'end' to the last valid character

    // Swap characters from start and end until they meet in the middle
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    return str;
}

int countingVowels(const char* str) {
    if (str == nullptr) return 0;

    int count = 0;
    while (*str != '\0') {
        char ch = *str;
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
            ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') {
            count++;
        }
        str++;
    }
    return count;
}

bool isPalindrome(const char* str) {
    if (str == nullptr) return false;

    const char* start = str;
    const char* end = str;

    // Move 'end' to the last character of the string
    while (*end != '\0') {
        end++;
    }
    end--; // Set 'end' to the last valid character

    // Compare characters from start and end until they meet in the middle
    while (start < end) {
        if (*start != *end) {
            return false;
        }
        start++;
        end--;
    }

    return true;
}
int main() {
    char str1[] = "Hello, World!";
    cout << "Original String: " << str1 << endl;
    cout << "Reversed String: " << reverseString(str1) << endl;

    const char* str2 = "This is a test string.";
    cout << "Number of vowels in \"" << str2 << "\": " << countingVowels(str2) << endl;

    const char* str3 = "racecar";
    cout << "\"" << str3 << "\" is palindrome: " << (isPalindrome(str3) ? "true" : "false") << endl;

    const char* str4 = "hello";
    cout << "\"" << str4 << "\" is palindrome: " << (isPalindrome(str4) ? "true" : "false") << endl;

    return 0;
}