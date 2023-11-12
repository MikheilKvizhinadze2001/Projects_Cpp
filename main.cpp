#include <iostream>
#include "school.h"
#include <conio.h>
#include <iomanip>



int main() {

    //Creating dummy database
    Student student1("John");
    Student student2("Jane");
    Student student3("Zaza");

    std::vector<Student> students = {student1, student2, student3};

        std::map<std::string, User *> users;


        users["principal"] = new Principal("principal", "12345");
        users["english_teacher"] = new English_teacher("english_teacher", "123456");
        users["math_teacher"] = new Math_teacher("math_teacher", "123");
        std::string username, password;
        char exit_button;


        users["principal"]->create_student_database(students);  //Creating initial database from students declared above

        
        while (true) {
            std::cout << "Hello, welcome to school management system! \n"
                         "To exit the system, press \"1\", otherwise press \"2\"" << std::endl;
            std::cin >> exit_button;
            std::cout << std::endl;
            int check_initial_input = 0;

            if (exit_button == '1') {
                std::cout << "Session has ended! System is turning off\n\n\n" << std::endl;
                delete users["principal"];
                delete users["english_teacher"];
                delete users["math_teacher"];
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

                std::cout << "Welcome back " << username << std::endl;
                std::cout << "Enter password or press \"1\" to go back to menu \n" << std::endl;
                std::cin >> password;


                do {

                    if (users[username]->faild_attempts_password == 2) {

                        // If user entered incorrect password 6 times, access will be denied
                        std::cout << "Access denied, suspicious behavior" << std::endl;
                        return 0;
                    }

                    if (password == "1") {
                        std::cout << "Session has ended!" << std::endl;
                        continue;   //Jump to top, where menu starts
                    }


                    //If login is successful, we get out of the loop and move forward
                    if (users[username]->checkPassword(password)) {
                        break;

                    } else {
                        std::cout << "Incorrect password, input correct or press \"1\" to quit " << std::endl;
                        std::cin >> password;
                    }

                } while (true);

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




            //shift previous console messages
            for (int i = 0; i < 15; i++)
                std::cout << std::endl;

            char b; //  successful login inputs

            std::cout << "Login successful!" << std::endl;



            if (username == "principal") {
                while (true) {
                    auto *principal = new Principal("principal", "12345");

                    std::cout << "Welcome to the school management system " << users[username]->username << std::endl;

                    std::cout << R"(press "1" to quit, press "2" to add student to database, press "3" to view database)" << std::endl;

                    std::cin >> b;
                    if (b == '1') {
                        std::cout << "Session has ended!\n\n\n" << std::endl;
                        break;
                    } else if (b == '2') {
                        std::string new_student_name;
                        std::cout<<"Please write student's name, so he/she can be added to the database\n";
                        std::cin>>new_student_name;
                        principal->add_student(students,new_student_name);
                        principal->create_student_database(students);
                        std::cout << std::endl;
                        std::cout << "\nStudent has been added, viewing changed database and returning to menu\n\n"<< std::endl;
                        Principal::view_student_database();
                        std::cout<<std::endl;
                        continue;

                    } else if (b == '3') {
                        std::cout << "Viewing database\n" << std::endl;
                        Principal::view_student_database();
                        std::cout << std::endl;

                    }
                }
            } else if (username == "math_teacher") {

                while (true) {
                    auto *math_teacher = new Math_teacher("math_teacher", "123");

                    std::cout << "Welcome to the school management system " << users[username]->username << std::endl;

                    std::cout << R"(press "1" to quit, press "2" to insert/change math grade, press "3" to view database)" << std::endl;

                    std::cin >> b;

                    if (b == '1') {
                        std::cout << "Session has ended!\n\n\n" << std::endl;
                        delete math_teacher;
                        break;
                    }

                    if (b == '3') {
                        std::cout << "Viewing database:\n";
                        math_teacher->view_student_database();
                        std::cout << std::endl;
                        std::cout << "Returning to menu\n";
                        continue;
                    } else if (b == '2') {
                        std::cout << std::endl;
                        std::cout<< "Which student's grade would you like to update/change? Please key-in corresponding ID" << std::endl;

                        int ID;
                        float changed_grade;

                        std::cin >> ID;

                        std::cout << "Now, please key in grade: " << std::endl;
                        std::cin >> changed_grade;

                        int result = math_teacher->change_math_grade(students, ID, changed_grade);

                        if (result == -2) {
                            std::cout << "\nNo matching ID, returning to menu\n\n";
                            continue;
                        } else if (result == 0) {
                            std::cout << "Change aborted" << std::endl;
                        } else if (result == 1) {
                            std::cout << "\nChange is made, viewing changed database and returning to menu\n\n"
                                      << std::endl;
                            math_teacher->create_student_database(students);
                            math_teacher->view_student_database();
                            std::cout << std::endl << std::endl;
                            continue;
                        } else if (result == -1) {
                            std::cout << "\nInvalid input, please input correct value. Returning to menu\n" << std::endl;
                            continue;
                        }
                    }

                }
            } else if (username == "english_teacher") {

                while (true) {
                    auto *english_teacher = new English_teacher("english_teacher", "123");

                    std::cout << "Welcome to the school management system " << users[username]->username << std::endl;

                    std::cout << R"(press "1" to quit, press "2" to insert/change english grade, press "3" to view database)" << std::endl;

                    std::cin >> b;

                    if (b == '1') {
                        std::cout << "Session has ended!\n\n\n" << std::endl;
                        delete english_teacher;
                        break;
                    }

                    if (b == '3') {
                        std::cout << "Viewing database:\n";
                        english_teacher->view_student_database();
                        std::cout << std::endl;
                        std::cout << "Returning to menu\n";
                        continue;
                    } else if (b == '2') {
                        std::cout << std::endl;
                        std::cout<< "Which student's grade would you like to update/change? Please key-in corresponding ID"<< std::endl;

                        int ID;
                        float changed_grade;

                        std::cin >> ID;

                        std::cout << "Now, please key in grade: " << std::endl;
                        std::cin >> changed_grade;

                        int result = english_teacher->change_english_grade(students, ID, changed_grade);

                        std::cout<<"Result is:: \n"<<result;
                        if (result == -2) {
                            std::cout << "\nNo matching ID, returning to menu\n\n";
                            continue;
                        } else if (result == 0) {
                            std::cout << "Change aborted" << std::endl;
                        } else if (result == 1) {
                            std::cout << "\nChange is made, viewing changed database and returning to menu\n"
                                      << std::endl;
                            english_teacher->create_student_database(students);
                            english_teacher->view_student_database();
                            std::cout << std::endl << std::endl;
                            continue;
                        } else if (result == -1) {
                            std::cout << "\nInvalid input, please input correct value. Returning to menu\n" << std::endl;
                            continue;
                        }
                    }

                }
            }
        }

    }

