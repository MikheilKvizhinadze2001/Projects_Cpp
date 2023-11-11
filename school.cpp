#include <iostream>
#include "school.h"
#include <map>
#include <cstdlib>   //for random number
#include <iomanip>
#include <chrono>
#include <fstream>
#include <string>

Student::Student(std::string name,int age):name(std::move(name)),age(age){
    student_ID = 0;
    GPA = -1;
    math_grade = -1.f;
    english_grade = -1.f;

}



User:: User(std::string username, std::string password) : username(username), password(password),
failed_attempts(0),faild_attempts_password(0),failed_attempts_username(0){}




std::string User::getUsername() const {

    return username;

}


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


void User::save_students(const std::vector<Student> &students) {

    std::ofstream file("student_data.txt");


    file << std::left << std::setw(15) << "Student ID" << std::setw(15) << "Name" <<
    std::setw(15) << "Math grade" << std::setw(15) << "English Grade" << std::setw(15) << "GPA";

    std::cout<<std::endl;

    for (const Student& student : students){

        if (student.english_grade!=-1 and student.math_grade !=-1) {
            file << std::left << std::setw(15) << student.student_ID << std::setw(15) << student.name << std::setw(15)<<
                 student.math_grade << std::setw(15) << student.english_grade << std::setw(15) << "NA";
            std::cout<<std::endl;
        }

        else if (student.english_grade==-1 and student.math_grade !=-1) {

            file << std::left << std::setw(15) << student.student_ID << std::setw(15) << student.name << std::setw(15)<<
            student.math_grade << std::setw(15) << "NA" << std::setw(15) << "NA";
            std::cout<<std::endl;
        }

        else if (student.math_grade==-1 and student.english_grade!=-1) {

            file << std::left << std::setw(15) << student.student_ID << std::setw(15) << student.name << std::setw(15)<<
            "NA" << std::setw(15) << student.english_grade << std::setw(15) << "NA";
            std::cout<<std::endl;

        }
        else {
            file << std::left << std::setw(15) << student.student_ID << std::setw(15) << student.name << std::setw(15)<<
            "NA" << std::setw(15) << "NA" << std::setw(15) << "NA";
            std::cout<<std::endl;
        }

    }
    file.close();
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
        } else {
            failed_attempts++;
            if (failed_attempts >= 2) {
                lockout_time = std::chrono::system_clock::now();
                faild_attempts_password++;
            }

            return false;
        }
    }

void User::view_student_database() {


}


Principal:: Principal(std::string username, std::string password) : User(username, password){}



Math_teacher::Math_teacher(std::string username,std::string password): User(username,password){}



English_teacher::English_teacher(std::string username, std::string password): User(username,password) {}


void English_teacher::change_english_grade(std::vector<Student> &students) {
    User::view_student_database();


}