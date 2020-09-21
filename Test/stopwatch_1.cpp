#include <iostream>
#include <ctime>

int main()
{
    unsigned int start = clock();
    std::cout << "waiting for keyhit";
    std::cin.ignore();    
    std::cout << "Time taken in millisecs: " << clock()-start;
    std::cin.ignore();   
    return 0;
}