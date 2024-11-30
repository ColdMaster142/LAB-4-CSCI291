#include <iostream>
#include <iomanip>
using namespace std;

// Function to print a floating-point number
// Takes the number, total space to display, and decimal precision as inputs
void print_real(float number, int fieldspace, int precision) {
    cout << fixed << setprecision(precision) << setw(fieldspace) << number << endl;
}

// Template function to modify two variables based on a scale
// The function updates the values of m1 and m2 by applying some calculations
// m3 is an optional scaling factor, defaulting to 10 if not provided
template <typename T>
void update_scale(T &m1, T &m2, T m3 = 10) {
    T original_m1 = m1; // Save the original value of m1
    T original_m2 = m2; // Save the original value of m2

    // Update m1: Add m1 and m2, then multiply by the scaling factor
    m1 = (original_m1 + original_m2) * m3;

    // Update m2: Subtract m2 from m1, then multiply by the scaling factor
    m2 = (original_m1 - original_m2) * m3;
}

int main() {
    float a, b; // Declare variables to store user inputs
    cout << "Please input two real numbers: ";
    cin >> a >> b; // Get two numbers from the user

    // Show the original values of a and b
    print_real(a, 7, 3); // Print 'a' with a width of 7 and 3 decimal places
    print_real(b, 7, 3); // Print 'b' with a width of 7 and 3 decimal places

    // Call the function to modify a and b
    update_scale(a, b);

    // Show the new values of a and b after the update
    print_real(a, 7, 3); // Print the updated 'a'
    print_real(b, 7, 3); // Print the updated 'b'

    return 0; // End of the program
}
