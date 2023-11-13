/*
 * Welcome to school management system project!
 * In the main function, mostly, user prompting is implemented.
 * Explanations and comments are provided as we move forward
 */


#include <iostream>
#include "school.h"



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
        std::string exit_button;


        User::create_student_database(students);  //Creating initial database from students declared above



        //Initial window. User has two options, either press "1" to quit or "2" to continue
        //If he/she presses something else more than 2 times, program will terminate
        while (true) {

            //We will jump to this label if user will want to quit from password entering section
            initial_menu_label:

            std::cout << "Hello, welcome to school management system! \n"
                         "To exit the system, press \"1\", otherwise press \"2\"" << std::endl;
            std::getline(std::cin,exit_button);
            std::cout << std::endl;
            static int check_initial_input = 0;

            if (exit_button == "1") {
                std::cout << "Session has ended! System is turning off\n\n\n" << std::endl;
                delete users["principal"];
                delete users["english_teacher"];
                delete users["math_teacher"];
                return 0;
            }

            //Prompting user to enter username

            else if (exit_button == "2") {

                std::cout << "Enter username: " << std::endl;
                std::getline(std::cin,username);

                int failed_attempts_username = 0;

                while (true) {

                    // If user decides to quit
                    if (username == "1") {
                        std::cout << "Session has ended!" << std::endl;
                        return 0;
                    }

                    // Check if the username exists in the map
                    auto it = users.find(username);

                    if (it != users.end()) {
                        int checkResult = it->second->check_username(username);
                        if (checkResult == 1)
                            break;
                    }
                    else {
                        failed_attempts_username++;
                        std::cout << "Wrong username, please write correct username or press \"1\" to quit: "<< std::endl;
                        if (failed_attempts_username==3){
                            std::cout << "Invalid attempts, program is terminating..."<< std::endl;
                            return 0;
                        }
                        std::getline(std::cin,username);
                    }
                }

                //If username was correct, we move forward to password section


                for (int i = 0; i < 15; i++)
                    std::cout << std::endl;

                std::cout << "Welcome back " << username << std::endl;
                std::cout << "Enter password or press \"1\" to go back to menu \n" << std::endl;
                std::getline(std::cin,password);

                do {
                    if (users[username]->failed_attempts_password == 2) {

                        // If user entered incorrect password 6 times, access will be denied
                        std::cout << "Access denied, suspicious behavior" << std::endl;
                        return 0;
                    }

                    if (password == "1") {
                        std::cout << "Session has ended! Going back to menu...\n" << std::endl;
                        goto initial_menu_label;   //Jump to top, where menu starts
                    }


                    //If login is successful, we get out of the loop and move forward
                    if (users[username]->check_password(password)) {
                        break;

                    } else {
                        std::cout << "Incorrect password, input correct or press \"1\" to quit " << std::endl;
                        std::getline(std::cin,password);
                    }

                } while (true);

            }


                //Handling initial incorrect input values, if there will be more than 2 incorrect inputs, program will exit
            else {
                check_initial_input++;
                if (check_initial_input >= 3) {
                    std::cout << "Invalid attempts! Program is terminating\n";
                    return 0;
                }
                std::cout << R"(Please write "1" or "2", otherwise program will exit after three incorrect inputs)"<< std::endl;
                std::cout<<std::endl;

                continue;
            }

            //shift previous console messages
            for (int i = 0; i < 15; i++)
                std::cout << std::endl;

            std::string successful_login_input; //If login was successful, we move forward
            std::cout << "Login successful!" << std::endl;



            //If the username is principal, we follow this branch
            if (username == "principal") {
                while (true) {
                    auto principal = new Principal("principal","12345");
                    std::cout << "Welcome to the school management system " << users[username]->username << std::endl;

                    std::cout << R"(press "1" to quit, press "2" to add student to database, press "3" to view database)" << std::endl;



                    std::getline(std::cin, successful_login_input);


                    if (successful_login_input == "1") {
                        std::cout << "Session has ended!\n\n\n" << std::endl;
                        delete principal;

                        break;
                    }


                    // If principal wants to add student to a database, he/she presses "2"
                    // then he writes new student's name, which is this case is a string without space, so a single name
                    // after adding student to a new database, database is viewed automatically
                    else if (successful_login_input == "2") {

                        std::string new_student_name;
                        std::cout<<"Please write student's name, so he/she can be added to the database\n";
                        std::getline(std::cin, new_student_name);

                        if (!Principal::check_student_name(new_student_name)){
                            std::cout<<"Invalid student name, please only use alphabetic characters and spaces between names!\n";
                            continue;
                        }
                        Principal::add_student(students,new_student_name);
                        Principal::create_student_database(students);
                        std::cout << std::endl;
                        std::cout << "\nStudent has been added, viewing changed database and returning to menu\n\n"<< std::endl;
                        Principal::view_student_database();
                        std::cout<<std::endl;
                        continue;

                    } else if (successful_login_input == "3") {
                        std::cout << "Viewing database and returning to menu\n" << std::endl;
                        Principal::view_student_database();
                        std::cout << std::endl;
                    }
                    else{
                        std::cout<<"Invalid inputs, please input correct value. Returning to menu..\n";
                    }
                }
            }

            /*
             *If the username is math teacher, program will prompt to press "1" to quit, "2" to change/insert math grade
             * or press "3" to view database
             */
            else if (username == "math_teacher") {

                while (true) {

                    Invalid_grade_label_math:

                    std::cout << "Welcome to the school management system " << users[username]->username << std::endl;

                    std::cout << R"(press "1" to quit, press "2" to insert/change math grade, press "3" to view database)" << std::endl;


                    std::getline(std::cin,successful_login_input);


                    if (successful_login_input == "1") {
                        std::cout << "Session has ended!\n\n\n" << std::endl;
                        break;
                    }

                    if (successful_login_input == "3") {
                        std::cout << "Viewing database:\n";
                        Math_teacher::view_student_database();
                        std::cout << std::endl;
                        std::cout << "Returning to menu\n";
                        continue;
                    }

                    else if (successful_login_input == "2") {
                        auto math_teacher = new Math_teacher("math_teacher","123");
                        std::cout << std::endl;
                        std::cout<< "Which student's grade would you like to update/change? Please key-in corresponding ID" << std::endl;

                        int ID;
                        std::string ID_string;

                        std::getline(std::cin,ID_string);

                        //Check if ID is valid, if yes, we move forward, otherwise go back to menu
                        if (!math_teacher->check_ID(ID_string)){
                            std::cout<<"Invalid ID, please input numbers only. Returning to menu..\n";
                            continue;
                        }
                        ID = stoi(ID_string);


                        float changed_grade;
                        std::string changed_grade_string;
                        std::cout << "Now, please key in grade: " << std::endl;
                        std::getline(std::cin,changed_grade_string);


                        //Check if grade is valid. if yes, we convert it to float and call change_math_grade function
                        int dot_count = 0;

                        for (char c : changed_grade_string) {
                            if(c == '.') {
                                dot_count++;
                            }
                            if ( !(std::isdigit(c) or c=='.') or dot_count>1) {
                                std::cout << "Invalid input for grade!\n";
                                goto Invalid_grade_label_math;
                            }
                        }

                        changed_grade = std::stof(changed_grade_string);

                        int result = Math_teacher::change_math_grade(students, ID, changed_grade);

                        if (result == -2) {
                            std::cout << "\nNo matching ID, returning to menu\n\n";
                            continue;
                        }

                        else if (result == 0) {
                            std::cout << "Change aborted, returning to menu" << std::endl;
                            continue;
                        }

                        else if (result == 1) {
                            std::cout << "\nChange is made, viewing changed database and returning to menu\n\n"<< std::endl;
                            Math_teacher::create_student_database(students);
                            Math_teacher::view_student_database();
                            std::cout << std::endl << std::endl;
                            continue;
                        }

                        else if (result == -1) {
                            std::cout << "\nInvalid input, please input correct value. Returning to menu\n" << std::endl;
                            continue;
                        }
                    }
                    else{
                        std::cout << "\nInvalid input, please input correct value. Returning to menu\n" << std::endl;
                    }

                }
            }

                /*
                 *If the username is math teacher, program will prompt to press "1" to quit, "2" to change/insert math grade
                 * or press "3" to view database
                 */

            else if (username == "english_teacher") {

                while (true) {

                    Invalid_grade_label_english:

                    std::cout << "Welcome to the school management system " << users[username]->username << std::endl;
                    std::cout << R"(press "1" to quit, press "2" to insert/change english grade, press "3" to view database)" << std::endl;

                    std::getline(std::cin,successful_login_input);

                    if (successful_login_input == "1") {
                        std::cout << "Session has ended!\n\n\n" << std::endl;
                        break;
                    }

                    if (successful_login_input == "3") {
                        std::cout << "Viewing database:\n";
                        English_teacher::view_student_database();
                        std::cout << std::endl;
                        std::cout << "Returning to menu\n";
                        continue;
                    }
                    else if (successful_login_input == "2") {
                        auto english_teacher = new English_teacher("english_teacher","123456");
                        std::cout << std::endl;
                        std::cout<< "Which student's grade would you like to update/change? Please key-in corresponding ID"<< std::endl;

                        int ID;
                        std::string ID_string;

                        std::getline(std::cin,ID_string);

                        //Check if ID is valid, if yes, we move forward, otherwise go back to menu
                        if (!english_teacher->check_ID(ID_string)){
                            std::cout<<"Invalid ID, please input numbers only. Returning to menu..\n";
                            continue;
                        }
                        ID = stoi(ID_string);


                        float changed_grade;
                        std::string changed_grade_string;
                        std::cout << "Now, please key in grade: " << std::endl;
                        std::getline(std::cin,changed_grade_string);


                        //Check if grade is valid. if yes, we convert it to float and call change_math_grade function
                        int dot_count = 0;

                        for (char c : changed_grade_string) {
                            if(c == '.') {
                                dot_count++;
                            }
                            if ( !(std::isdigit(c) or c=='.') or dot_count>1) {
                                std::cout << "Invalid input for grade!\n";
                                goto Invalid_grade_label_english;
                            }
                        }

                        changed_grade = std::stof(changed_grade_string);

                        int result = English_teacher::change_english_grade(students, ID, changed_grade);


                        if (result == -2) {
                            std::cout << "\nNo matching ID, returning to menu\n\n";
                            continue;
                        }

                        else if (result == 0) {
                            std::cout << "Change aborted" << std::endl;
                        }

                        else if (result == 1) {
                            std::cout << "\nChange is made, viewing changed database and returning to menu\n"
                                      << std::endl;
                            English_teacher::create_student_database(students);
                            English_teacher::view_student_database();
                            std::cout << std::endl << std::endl;
                            continue;
                        }

                        else if (result == -1) {
                            std::cout << "\ninvalid input for change, input correct value. Returning to menu\n" << std::endl;
                            continue;
                        }
                    }

                    else {
                        std::cout << "\nInvalid input, please input correct value. Returning to menu\n" << std::endl;
                    }
                }
            }
        }
    }

