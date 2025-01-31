#include <bits/stdc++.h>

struct Field{
    std::string name;
    uint8_t type;
    uint32_t value;   
};

struct Car_Types{
    std::string type_name;
    uint32_t can_id;
    std::unordered_map<std::string, Field> fields;
};

std::unordered_map<std::string, Car_Types> car_types;

int deserialize(std::string file_name){
    std::ifstream file(file_name);

    if(!file.is_open()){
        std::cerr << "Error: Could not open file " << file_name << "\n";
        return 1;
    }

    int line_number = 0;
    Car_Types *current_type = nullptr;
    std::string line;

    while(std::getline(file, line)){
        line_number++;
        std::istringstream stream(line);
        std::string first_word;
        stream >> first_word;

        if(first_word == ""){
            continue;
        }

        if(first_word == ">"){
            std::string type_name;
            std::string can_id_str;
            stream >> type_name >> can_id_str;
            uint32_t can_id = std::stoul(can_id_str, nullptr, 16);
            std::unordered_map<std::string, Field> fields;
            car_types[type_name] = {type_name, can_id, fields};
            current_type = &car_types[type_name];
            continue;
        }


        if(first_word.substr(0, 2) != ">>" || first_word.length() < 3){
            std::cerr << "Error: Invalid syntax at line " + std::to_string(line_number) +"\n";
            return 1;
        }

        if(current_type == nullptr){
            std::cerr << "Error: Field not in a type at line " + std::to_string(line_number) + "\n";
            return 1;
        }

        Field field;
        field.type = first_word[2];
        stream >> field.name;  
        stream >> field.value;
        current_type->fields[field.name] = field;

    }

    for(auto &car_type : car_types){
        std::cout << "Type: " << car_type.second.type_name << "\n";
        std::cout << "  CAN ID: " << car_type.second.can_id << "\n";
        for(auto &field : car_type.second.fields){
            std::cout << "  Field: " << field.second.name << "\n";
            std::cout << "      Type: " << field.second.type << "\n";
            std::cout << "      Value: " << field.second.value << "\n";
       }
       std::cout << "----------------\n";
    }
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2 || argc > 2){
        std::cout << "Usage: " << argv[0] << " <file_name>\n";
        return 1;
    }

    std::string file_name = argv[1];
    deserialize(file_name);
    return 0;
}
