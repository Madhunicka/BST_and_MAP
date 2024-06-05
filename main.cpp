#include <iostream>
#include <fstream>
#include "DIFF.H"
#include <string>




const std::string input = "Others/data_source.txt";


int main()
{
    WindlogType data;



    std::ifstream file(input);
    if (!file.is_open()) {
        std::cout << "File: " << input << " could not open!" << std::endl;
        return 1; // Exit with error code
    }

    std::string txt;
    while (getline(file, txt)) {
        std::ifstream in(txt);
        if (in.is_open()) {
            in >> data; // Directly read into data
        }
        else {
            std::cout << "File: " << txt << " could not open!";
            // Handle file open error if needed
        }
    }




    display(data);
    return 0;
}
