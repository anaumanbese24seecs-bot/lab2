#include <iostream>
using namespace std;

int main(void) {
    // Step 1: Dynamically allocate memory for 20 integers
    const int SIZE = 20;
    int* salary = new int[SIZE];  // Dynamically allocate array using pointer

    int* i = salary;  // Pointer to iterate through the array

    // Step 2: Input salaries using pointer arithmetic
    for (i = salary; i < salary + SIZE; ++i) {
        cout << "Enter Salary: ";
        cin >> *i;  // Dereference pointer to store input
    }

    // Step 3: Modify salaries using pointer arithmetic
    for (i = salary; i < salary + SIZE; ++i) {
        *i = *i + *i / (*i - salary + 1);  // Modify value using pointer
    }

    // Step 4: Free the allocated memory
    delete[] salary;  // Deallocate the dynamic memory
    cout << "Memory deallocated successfully.\n";

    return 0;
}