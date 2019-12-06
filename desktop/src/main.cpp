#include "encode.hpp"

#include <iostream>
#include <fstream>

int binary_to_int(std::string bitstring);
std::string validate(std::string input);

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
        return 0;

    }else{

        std::cout << "Success! Device detected at " + location << std::endl;
    }

    #ifdef _WIN32
    #else
        system(("stty -F " + location + " -hupcl").c_str());
    #endif

    std::ifstream serial_in;
    serial_in.open(location);

    char input = 'y';
    std::string current = "";
    while(input != 'n'){

        std::cout << "Reading... ";
        std::string data = "";
        char value;
        while(serial_in.get(value)){

            std::cout << "READ! " << value << std::endl;
            if(value == '\n'){

                break;
            }

            data += byte_to_binary(value);
        }
        //std::cout << "Got! Data = " << data << std::endl;
        std::string potential = validate(data);
        if(potential != "" && current != potential){

            current = potential;
            std::cout << current << std::endl;
        }

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

int binary_to_int(std::string bitstring){

    return (int)(binary_to_byte(bitstring)) + 128;
}

std::string validate(std::string input){

    std::cout << "Received input: " << input << std::endl;
    if(input.length() <= 16){

        std::cout << "Input length is too short" << std::endl;
        return "";
    }

    int byte_count = binary_to_int(input.substr(0, 8)) / 8;
    int chunk_size = binary_to_int(input.substr(8, 8));
    std::cout << "byte count = " << byte_count << std::endl;
    std::cout << "chunk count = " << chunk_size << std::endl;

    double remaining_bytes = (input.length() - 16.0) / 8.0;
    if(remaining_bytes != (int)remaining_bytes){

        // we return false here because we've received partial bytes
        std::cout << "Detected partial bytes" << std::endl;
        return "";
    }

    if((int)remaining_bytes != byte_count){

        // we return false here because we've received a different number than expected
        std::cout << "Byte count inconsistent with header" << std::endl;
        return "";
    }

    return decode(input.substr(16, chunk_size * 7));
}
