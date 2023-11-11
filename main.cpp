#include <iostream>
#include "school.h"
#include <conio.h>
#include <cstdlib>   //for random number
#include <iomanip>
#include <chrono>


int main() {



        std::map<std::string, User *> users;


        users["principal"] = new Principal("principal", "12345");
        users["english_teacher"] = new English_teacher("english_teacher", "123456");
        users["math_teacher"] = new Math_teacher("math_teacher", "123");
        std::string username, password;
        char exit_button;


        std::cout << "Hello, welcome to school management system, \n"
                     "to exit the system, press \"1\", otherwise press \"2\"" << std::endl;
        std::cin >> exit_button;
        std::cout << std::endl;
        int check_initial_input = 0;


        while (true) {


            if (exit_button == '1') {
                std::cout << "Session has ended!" << std::endl;
                return 0;
            } else if (exit_button == '2') {

                std::cout << "Enter username: " << std::endl;
                std::cin >> username;


                int failed_attempts_username = 0;

                while (true) {
                    if (username == "1") {
                        std::cout << "Session has ended!" << std::endl;
                        return 0;
                    }

                    if (failed_attempts_username >= 2) {
                        std::cout << "Access denied, suspicious behavior" << std::endl;
                        return 0;
                    }


                    // Check if the username exists in the map
                    auto it = users.find(username);

                    if (it != users.end()) {
                        int checkResult = it->second->checkUsername(username);
                        if (checkResult == -1)
                            return 0;

                        if (checkResult == 1)
                            break;
                    } else {
                        failed_attempts_username++;
                        std::cout << "Wrong username, please write correct username or press \"1\" to quit: "
                                  << std::endl;
                        std::cin >> username;
                    }
                }


                for (int i = 0; i < 15; i++)
                    std::cout << std::endl;

                std::cout << "Welcome back \n" << username << std::endl;
                std::cout << "Enter password: or press \"1\" to quit \n" << std::endl;
                std::cin >> password;


                do {

                    if (users[username]->faild_attempts_password == 2) {

                        // If user entered incorrect password 6 times, access will be denied
                        std::cout << "Access denied, suspicious behavior" << std::endl;
                        return 0;
                    }

                    if (password == "1") {
                        std::cout << "Session has ended!" << std::endl;
                        return 0;
                    }


                    //If login is successful, we get out of the loop and move forward
                    if (users[username]->checkPassword(password)) {
                        break;

                    } else {
                        std::cout << "Incorrect password, input correct or press \"1\" to quit " << std::endl;
                        std::cin >> password;
                    }

                } while (true);

                break;

            }



                //Handling initial incorrect input values, if there will be more than 2 incorrect inputs, program will exit
            else {
                check_initial_input++;
                std::cout << R"(Please write "1" or "2",otherwise program will exit after two incorrect inputs)"
                          << std::endl;
                std::cin >> exit_button;

                if (check_initial_input >= 2) {
                    std::cout << "Invalid attempts!";
                    return 0;
                }

            }


        }

        //shift previous console messages
        for (int i = 0; i < 15; i++)
            std::cout << std::endl;

        char b; // character for successful login inputs

        std::cout << "Login successful!" << std::endl;


        if (username == "principal") {

            auto *principal = new Principal("principal", "12345");

            std::cout << "Welcome back " << users[username]->username << std::endl;

            std::cout << R"(press "1" to quit, press "2" to create student database,
 press "3" to view database)" << std::endl;
            std::cin >> b;
            if (b == '1') {
                std::cout << "Session has ended!" << std::endl;
                return 0;
            } else if (b == '2') {

                //Creating student database

                Student student1("John", 15);
                Student student2("Jane", 16);

                std::vector<Student> students = {student1, student2};

                principal->create_student_database(students);
            } else if (b == '3') {

            }
        }


        return 0;
    }

