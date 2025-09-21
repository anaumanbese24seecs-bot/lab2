#include <iostream>
#include <cstring>  // For strlen

int main() {
    // Step 1: Get input string length from user
    std::cout << "Enter the length of the string: ";
    int len;
    std::cin >> len;

    // Validate input
    if (len <= 0) {
        std::cout << "Length must be positive. Exiting.\n";
        return 1;
    }

    // Step 2: Dynamically allocate memory for the string (including null terminator)
    char* str = new char[len + 1]; // +1 for null terminator

    // Step 3: Get input string from user
    std::cout << "Enter the string: ";
    std::cin.ignore(); // Clear the newline from previous input
    std::cin.getline(str, len + 1); // Read up to len characters

    // Step 4: Print the string in reverse order (without modifying the original)
    std::cout << "Reversed string: ";
    int length = std::strlen(str); // Get actual length of input
    for (int i = length - 1; i >= 0; i--) {
        std::cout << str[i];
    }
    std::cout << std::endl;

    // Step 5: Display the original string (to show it remains unchanged)
    std::cout << "Original string: " << str << std::endl;

    // Step 6: Free the allocated memory
    delete[] str;
    std::cout << "Memory deallocated successfully.\n";

    return 0;
}