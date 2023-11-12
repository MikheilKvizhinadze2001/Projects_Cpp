#ifndef SCHOOL_H
#define SCHOOL_H
#include <string>
#include <thread>
#include <map>
#include <vector>

class Student{

public:
    float GPA;
    int student_ID;
    friend class User;
    friend class Math_teacher;
    friend class English_teacher;
    const std::string name;
    float english_grade;
    float math_grade;

    explicit Student(std::string name);
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
    static void view_student_database();
    int checkUsername(const std::string& enteredUsername);
    bool checkPassword(const std::string& enteredPassword);
    void create_student_database(std::vector<Student> &students);


};



class Principal : public User {

public:
    Principal(std::string username, std::string password);
    void add_student(std::vector<Student> &students,const std::string &name);

};



class Math_teacher : public User{
public:
    Math_teacher(std::string username,std::string password);
    int change_math_grade (std::vector<Student> &students, const int ID, const float &changed_math_grade);
    int change_math_grade_success(const char &verify);
};



class English_teacher : public User{
public:
    English_teacher(std::string username, std::string password);
    int change_english_grade (std::vector<Student> &students, const int ID,const float &changed_english_grade);

    int change_english_grade_success(const char &verify);

};



#endif //SCHOOL_H
