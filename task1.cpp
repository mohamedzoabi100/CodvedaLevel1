/// <task1: Basic Calculator>
#include <iostream>
/// @brief I assume that the operations are binary operations
class BasicOperation {
    public:
    double arg1, arg2;
    char op;
    double result;
    BasicOperation(double a, char op, double b): arg1(a), op(op) , arg2(b) {
        switch(op) {
            case '+': result = arg1 + arg2; break;
            case '-': result = arg1 - arg2; break;
            case '*': result = arg1 * arg2; break;
            case '/': 
                if(arg2 != 0) {
                    result = arg1 / arg2; 
                } else {
                    throw std::runtime_error("Division by zero");
                }
                break;
            default:
                throw std::runtime_error("Invalid operator");
        }
    }

};
int main() {
    char continueChoice;
    
    do {
        try {
            double num1, num2;
            char op;
            
            std::cout << "Enter first number: ";
            std::cin >> num1;
            
            std::cout << "Enter operator (+, -, *, /): ";
            std::cin >> op;
            
            std::cout << "Enter second number: ";
            std::cin >> num2;
            
            BasicOperation calc(num1, op, num2);
            std::cout << "Result: " << calc.result << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            std::cout << "Please try again." << std::endl;
        }
        
        std::cout << "Do another calculation? (y/n): ";
        std::cin >> continueChoice;
        
    } while (continueChoice == 'y' || continueChoice == 'Y');
    
    return 0;
}

