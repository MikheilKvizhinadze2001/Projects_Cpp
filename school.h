//Function declarations

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
    int failed_attempts_password;
    std::string username;
    User(std::string username, std::string password);
    static void view_student_database();
    int check_username(const std::string& enteredUsername) const;
    bool check_password(const std::string& enteredPassword);
    static void create_student_database(std::vector<Student> &students);

};



class Principal : public User {

public:
    Principal(std::string username, std::string password);
    static void add_student(std::vector<Student> &students,const std::string &name);
    static int check_student_name(std::string &string);
};



class Math_teacher : public User{
public:
    Math_teacher(std::string username,std::string password);
    static int change_math_grade (std::vector<Student> &students, int ID, const float &changed_math_grade);
    static int change_math_grade_success(const std::string &verify);
     bool check_ID(const std::string& ID);
};



class English_teacher : public User{
public:
    English_teacher(std::string username, std::string password);
    static int change_english_grade (std::vector<Student> &students, int ID,const float &changed_english_grade);

    static int change_english_grade_success(const std::string &verify);
    bool check_ID(const std::string& ID);
};



#endif //SCHOOL_H
