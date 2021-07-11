//
//  main.cpp
//  catalogue
//
//  Created by Максим Крыжановский on 11.07.2021.
//

#include <iostream>
#include <iostream>
#include <fstream>

#include "transport_catalogue.hpp"
#include "json_reader.hpp"
#include "request_handler.hpp"


using namespace std;
using namespace json;
using namespace transport_catalogue;
using namespace transport_catalogue::detail;
using namespace transport_catalogue::request_handler;
using namespace transport_catalogue::request_handler::detail;
using namespace transport_catalogue::json_reader;

int main(int argc, const char * argv[]) {
    
    string test1 = "/Users/makskryzhanovskiy/Desktop/Projects/catalogue/Tests/test1.txt";
    string TestStopCommand = "/Users/makskryzhanovskiy/Desktop/Projects/catalogue/Tests/TestStopCommand.txt";
    
    std::fstream ifs;
    ifs.open("/Users/makskryzhanovskiy/Desktop/Projects/do IT/Tests/test1.txt");
    
    if(!ifs.is_open()) {
        std::cout << "Something went wrong" << std::endl;
    } else {
        JsonReader jr;
        jr.ReadJson(ifs);
        jr.Print(std::cout);
    }
    
    return 0;
}
