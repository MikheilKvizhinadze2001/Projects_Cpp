#include <iostream>
#include "school.h"
#include <map>
#include <random>
#include <iomanip>
#include <fstream>
#include <string>
#include <utility>



Student::Student(std::string name):name(std::move(name)){
    GPA = -1;
    math_grade = -1.f;
    english_grade = -1.f;

    //creating random value for ID, range is 0 to 100
    int low = 0, high = 100;

    student_ID = (rand() % (low - high + 1));


}




User:: User(std::string username, std::string password) : username(std::move(username)), password(std::move(password)),
failed_attempts(0),faild_attempts_password(0),failed_attempts_username(0){}





int User::checkUsername(const std::string& enteredUsername){
    if(enteredUsername != username){
        failed_attempts_username++;
        return false;
    }
    else{
        failed_attempts_username = 0; // Reset failed attempts when username is correct
        return true;
    }
}


    bool User::checkPassword(const std::string& enteredPassword) {
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

        if (enteredPassword == password) {
            return true;
        }

        else {
            failed_attempts++;
            if (failed_attempts >= 2) {
                lockout_time = std::chrono::system_clock::now();
                faild_attempts_password++;
            }

            return false;
        }
    }

void User::view_student_database() {

    std::ifstream file("student_data.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}

void User::create_student_database(std::vector<Student> &students) {


   std::ofstream file("student_data.txt");
    file << std::left << std::setw(15) << "ID" << std::left << std::setw(15) << "Name" << std::left << std::setw(15)
         << "Math grade" << std::left << std::setw(15) << "English grade"
         << std::left << std::setw(15) << "GPA" << "\n";


    file << std::flush;  // Flush the buffer

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




Principal:: Principal(std::string username, std::string password) : User(username, password){}

void  Principal::add_student(std::vector<Student> &students,const std::string &name){
    Student student(name);
    students.push_back(student);
}


Math_teacher::Math_teacher(std::string username,std::string password): User(username,password){}

int Math_teacher::change_math_grade(std::vector<Student> &students, const int ID, const float &changed_math_grade) {
    User::view_student_database();

    for (Student& student : students) {
        std::cout<<student.student_ID<<std::endl;
        if (student.student_ID == ID) {
            std::cout<<"You are about to change " << student.name <<"'s math grade, which is"
            <<student.math_grade<<" to "<<changed_math_grade<<std::endl;
            std::cout<<"Please verify change. Key-in 'y' or 'n':"<<std::endl;
            char verify;
            std::cin>>verify;
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


int Math_teacher::change_math_grade_success(const char &verify) {
    if (verify == 'y')
        return 1;
    else if (verify == 'n')
        return 0;
    else
        return -1;

}



English_teacher::English_teacher(std::string username, std::string password): User(username,password) {}

int English_teacher::change_english_grade_success(const char &verify) {
    if (verify == 'y')
        return 1;
    else if (verify == 'n')
        return 0;
    else
        return -1;

}

int English_teacher::change_english_grade(std::vector<Student> &students, const int ID, const float &changed_english_grade) {
    User::view_student_database();

    for (Student& student : students) {
        if (student.student_ID == ID) {
            std::cout<<"You are about to change " << student.name <<"'s Englsih grade, which is"
                     <<student.english_grade<<" to "<<changed_english_grade<<std::endl;
            std::cout<<"Please verify change. Key-in 'y' or 'n':"<<std::endl;
            char verify;
            std::cin>>verify;
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

