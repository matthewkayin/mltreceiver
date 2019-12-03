#include <iostream>
#include <fstream>
#include <string>
#include "encode.hpp"
#include "serial.hpp"
#include "smaz.hpp"


int main(){

    std::cout << "Welcome to the thing!" << std::endl;
    std::string location = "";
    std::string base_location_name = "";
    #ifdef _WIN32
        base_location_name = "COM";
    #else
        base_location_name = "/dev/ttyACM";
    #endif

    int attempts = 0;

    while(location == "" && attempts < 5){

        attempts++;
        for(int i = 0; i < 256; i++){

            std::string check_name = base_location_name + std::to_string(i);
            std::ifstream serial_check(check_name.c_str());
            if(serial_check.good()){

                location = check_name;
            }
        }
    }

    if(location == ""){

        std::cout << "Error! Could not detect serial device!" << std::endl;;
        //return 0;

    }else{

        std::cout << "Success! Device detected at " + location << std::endl;
    }

    #ifdef _WIN32
    #else
        system(("stty -F " + location + " -hupcl").c_str());
    #endif

    /* std::ofstream serial_out;
    serial_out.open(location);
    std::string message = "hey there\n";
    encode(message, ); */
    std::ifstream serial_in;
    serial_in.open(location);

    char input = 'y';
    std::string message = "";
    while(input != 'n'){

        std::cout << "Reading... ";
        std::string data = "";
        char value;
        //while loop to finish message?
        while(serial_in.get(value)){
            
            if(value == '\n'){

                break;
            }

            data += value;
            message.append(decode(data));

        }
        //decode char into a string and add to message
        
        std::cout << "Got! message = " << message << std::endl;
        std::cout << "Read more? (y/n) ";
        std::cin >> input;
    }

    

    std::cout << "cya!" << std::endl;

    serial_in.close();
    #ifdef _WIN32
    #else
        system(("stty -F " + location + " hupcl").c_str());
    #endif

    return 0;
}
