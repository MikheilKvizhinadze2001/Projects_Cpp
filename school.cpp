/*
 * This is a source file for school management project. Most necessary functions are implemented below
 * Functions' prototypes are declared in the header file
 */


#include <iostream>
#include "school.h"
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <utility>


//Initialize "Student" class
//***
Student::Student(std::string name):name(std::move(name)){
    GPA = -1;
    math_grade = -1.f;
    english_grade = -1.f;

    // Creating random value for ID, range is 0 to 100
    // Since the program is small, and number of students are assumed to be little, we can use rand() function to create
    // ID values for each student, as soon as Student's objects will be created
    int low = 0, high = 1000;

    student_ID = (rand() % (low - high + 1));


}
User:: User(std::string username, std::string password) : username(std::move(username)), password(std::move(password)),
failed_attempts(0),failed_attempts_password(0){}
//***


//This function checks username. If mapped username's username is same the value that actual user prompted
//it will return true, otherwise it is false

int User::check_username(const std::string& entered_username) const{
    if(entered_username != username){
        return false;
    }
    else{
        return true;
    }
}


//Implement password checking functionality
//Access will be blocked for 1 minute after entering three incorrect password
//If you enter three incorrect passwords in a row, "faild_attempts_password" will increment to 2,
//Prompting the program to terminate for security reasons
//***
bool User::check_password(const std::string& entered_password) {
    if (failed_attempts ==2) {

        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - lockout_time);

        if (duration.count() < 1) {
            std::cout << "Access blocked for 1 minutes due to 3 failed attempts." << std::endl;
            return false;
        }

        else {
            failed_attempts = 0; // Reset failed attempts after 1 minutes
        }
    }

    if (entered_password == password) {
        return true;
    }

    else {
        failed_attempts++;
        if (failed_attempts >= 2) {
            lockout_time = std::chrono::system_clock::now();
            failed_attempts_password++;
        }

        return false;
    }
}
//***




//***  Function below is used to create student database, and write data in a text file
void User::create_student_database(std::vector<Student> &students) {

   std::ofstream file("student_data.txt");
    file << std::left << std::setw(15) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15)
         << "Math grade" << std::left << std::setw(15) << "English grade"
         << std::left << std::setw(15) << "GPA" << "\n";


    file << std::flush;  // Flush the buffer

    // GPA is calculated as the average of math_grade and english_grade, with default value being -1
    //If any of them, math_grade or english_grade is -1, it means that grade has not been updated yet, so value for that
    //course is "NA" and value for GPA is "NA"
    for (Student& student: students){
        if (student.english_grade != -1 and student.math_grade != -1) {
            student.GPA = (student.english_grade + student.math_grade) / 2.f;
            file << std::left << std::setw(15) << student.student_ID << std::left << std::setw(15) << student.name
                 << std::left << std::setw(15) << student.math_grade << std::left << std::setw(15) << student.english_grade
                 << std::left << std::setw(15) << student.GPA << "\n";
            file << std::flush;  // Flush the buffer
        } else if (student.english_grade == -1 and student.math_grade != -1) {

            file << std::left << std::setw(15) << student.student_ID << std::left << std::setw(15) << student.name
                 << std::left << std::setw(15) << student.math_grade << std::left << std::setw(15) << "NA"
                 << std::left << std::setw(15) << "NA" << "\n";
            file << std::flush;  // Flush the buffer
        } else if (student.math_grade == -1 and student.english_grade != -1) {

            file << std::left << std::setw(15) << student.student_ID << std::left << std::setw(15) << student.name
                 << std::left << std::setw(15) << "NA" << std::left << std::setw(15) << student.english_grade
                 << std::left << std::setw(15) << "NA" << "\n";
            file << std::flush;  // Flush the buffer
        } else if (student.english_grade == -1 and student.math_grade == -1) {

            file << std::left << std::setw(15) << student.student_ID << std::left << std::setw(15) << student.name
                 << std::left << std::setw(15) << "NA" << std::left << std::setw(15) << "NA"
                 << std::left << std::setw(15) << "NA" << "\n";
            file << std::flush;  // Flush the buffer
        }

    }
    file.close();
}
//***

//This functions views student database by reading data from file where the database was created
void User::view_student_database() {

    std::ifstream file("student_data.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}


//Initializing "Principal" class, which is subclass of User
Principal:: Principal(std::string username, std::string password) : User(std::move(username), std::move(password)){}

//This function adds student to student vector. After adding and then using
void  Principal::add_student(std::vector<Student> &students,const std::string &name){
    Student student(name);
    students.push_back(student);
}

//This function checks if student name begins with space, or if it contains character other than alphabetic character
//or spaces in between (so spaces between first name and last name is acceptable)
//***
int Principal::check_student_name(std::string &string) {

    if (isspace(string[0]))
        return false;
    for (char c : string){
        if (!isalpha(c) && !isspace(c))
            return false;
    }
    return true;
}
//***


//Initializing "Math_teacher" class, which is subclass of User
//Math_teacher class can view student database and update math_grade
Math_teacher::Math_teacher(std::string username,std::string password): User(std::move(username),std::move(password)){}


/*
 * This function is used to change math grade, and it is unique to math_teacher
 * It detects student by his/her ID, then makes sure that change needs to be made
 */
int Math_teacher::change_math_grade(std::vector<Student> &students, const int ID, const float &changed_math_grade) {
    User::view_student_database();

    for (Student& student : students) {
        if (student.student_ID == ID) {
            std::cout<<"You are about to change " << student.name <<"'s math grade, which is "
            <<student.math_grade<<" to "<<changed_math_grade<<std::endl;
            std::cout<<"Please verify change. Key-in 'y' or 'n':"<<std::endl;
            std::string verify;
            std::getline(std::cin,verify);
            int verify_output_value = change_math_grade_success(verify);
            if (verify_output_value == 1) {
                student.math_grade = changed_math_grade;
                return 1;
            }
            else if (verify_output_value == 0){

                return 0;
            }
            else {

                return -1;
            }

        }

    }
        return -2;

}


bool Math_teacher::check_ID(const std::string& ID) {
    for (char c : ID) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}
//This function checks if user is sure that he/she wants to update math grade
int Math_teacher::change_math_grade_success(const std::string &verify) {
    if (verify == "y")
        return 1;
    else if (verify == "n")
        return 0;
    else
        return -1;

}


//Declaring english_teacher class, which is subclass of "User" class.
//English teacher is able to change English grade and view database, with similar functionality as math teacher
English_teacher::English_teacher(std::string username, std::string password): User(std::move(username),std::move(password)) {}

int English_teacher::change_english_grade_success(const std::string &verify) {
    if (verify == "y")
        return 1;
    else if (verify == "n")
        return 0;
    else
        return -1;

}

bool English_teacher::check_ID(const std::string& ID) {
    for (char c : ID) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

int English_teacher::change_english_grade(std::vector<Student> &students, const int ID, const float &changed_english_grade) {
    User::view_student_database();

    for (Student& student : students) {
        if (student.student_ID == ID) {
            std::cout<<"You are about to change " << student.name <<"'s English grade, which is "
                     <<student.english_grade<<" to "<<changed_english_grade<<std::endl;
            std::cout<<"Please verify change. Key-in 'y' or 'n':"<<std::endl;
            std::string verify;
            std::getline(std::cin,verify);
            int verify_output_value = change_english_grade_success(verify);
            if (verify_output_value == 1) {
                student.english_grade = changed_english_grade;
                return 1;
            }
            else if (verify_output_value == 0){

                return 0;
            }
            else {

                return -1;
            }

        }


    }
    return -2;

}

