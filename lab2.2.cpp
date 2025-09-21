#include <iostream>
using namespace std;

void analyze_pointer(int *ptr) {
    // Print the memory location pointed to by the pointer
    cout << "Memory location: " << ptr << endl;
    // Print the value of the integer pointed to by the pointer
    cout << "Value: " << *ptr << endl;
}

int main() {
    // Part 2, Task 1: Allocate an int on the stack
    int iValue = 42; // Assign a value to the stack variable
    cout << "Analyzing stack-allocated integer:" << endl;
    analyze_pointer(&iValue); // Pass the address of iValue

    // Part 2, Task 2: Allocate an int on the heap
    int *heapValue = new int; // Allocate memory on the heap
    *heapValue = 100;         // Assign a value to the heap-allocated integer
    cout << "\nAnalyzing heap-allocated integer:" << endl;
    analyze_pointer(heapValue); // Pass the heap pointer

    // Clean up heap memory to prevent memory leak
    delete heapValue;

    return 0;
}