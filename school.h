#ifndef SCHOOL_H
#define SCHOOL_H
#include <string>
#include <thread>
#include <map>
#include <vector>

class Student{

public:
    float GPA;
    static int student_ID;
    friend class User;
    friend class Math_teacher;
    friend class English_teacher;
    const std::string name;
    const int age;
    float english_grade;
    float math_grade;

    Student(std::string name,int age);
};



class User {
private:
    std::map<std::string,User*> users;
    std::string password;
    int failed_attempts;
    std::chrono::system_clock::time_point lockout_time;

public:
    int failed_attempts_username;
    int faild_attempts_password;
    std::string username;
    User(std::string username, std::string password);
    std::string getUsername() const;
    void view_student_database();
    int checkUsername(const std::string& enteredUsername);
    bool checkPassword(const std::string& enteredPassword);


};



class Principal : public User {

public:
    Principal(std::string username, std::string password);
    void create_student_database(const std::vector<Student> &students);


};



class Math_teacher : public User{
public:
    Math_teacher(std::string username,std::string password);


};



class English_teacher : public User{
public:
    English_teacher(std::string username, std::string password);
    void change_english_grade (std::vector<Student> &students);
};



#endif //SCHOOL_H
