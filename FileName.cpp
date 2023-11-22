#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
using namespace std;
class Student {
private:
    string studentID;
    string name;
    string gender;
    string grade;
    map<string, float> courses; // 课程编号和成绩

public:
    Student(string id, string name, string gender, string grade)
        : studentID(id), name(name), gender(gender), grade(grade) {}

    void addCourse(string courseID, float score) {
        courses[courseID] = score;
    }

    void updateCourse(string courseID, float score) {
        if (courses.find(courseID) != courses.end()) {
            courses[courseID] = score;
        }
        else {
            cout << "课程不存在，无法更新成绩。" << endl;
        }
    }

    void deleteCourse(string courseID) {
        if (courses.find(courseID) != courses.end()) {
            courses.erase(courseID);
        }
        else {
            cout << "课程不存在，无法删除。" << endl;
        }
    }

    float calculateTotalScore() const {
        float totalScore = 0;
        for (const auto& course : courses) {
            totalScore += course.second;
        }
        return totalScore;
    }

    void displayInfo() const {
        cout << "学生姓名: " << name << ", 学号: " << studentID
            << ", 性别: " << gender << ", 年级: " << grade << endl;
        cout << "课程成绩: " << endl;
        for (const auto& course : courses) {
            cout << "课程编号: " << course.first << ", 成绩: " << course.second << endl;
        }
    }
    int getId() const {
        return stoi(studentID); // 假设学号是可以转换为整数的
    }
    string getName() const {
        return name;
    }
    string getGender() const {
        return gender;
    }

    string getGrade() const {
        return grade;
    }

    const map<string, float>& getCourses() const {
        return courses;
    }
};

class Course {
private:
    int courseCode; // 课程编号
    string courseName; // 课程名称
    string examType; // 考试方式

public:
    Course(int code, string name, string examType)
        : courseCode(code), courseName(name), examType(examType) {}
    int getCourseCode() const {
        return courseCode;
    }

    void displayInfo() const {
        cout << "课程编号: " << courseCode
            << ", 课程名称: " << courseName
            << ", 考试方式: " << examType << endl;
    }
    string getCourseName() const {
        return courseName;
    }

    string getExamType() const {
        return examType;
    }
  
};
class StudentManagementSystem {
private:
    vector<shared_ptr<Student>> students;
    vector<Course> courses;
    map<int, map<int, float>> grades; // 学生ID -> (课程代码 -> 成绩)

public:
    void addStudent(const shared_ptr<Student>& student) {
        students.push_back(student);
    }

    void addCourse(const Course& course) {
        courses.push_back(course);
    }

    void setGrade(int studentId, int courseCode, float grade) {
        grades[studentId][courseCode] = grade;
    }

    shared_ptr<Student> findStudent(int studentId) {
        for (auto& student : students) {
            if (student->getId() == studentId) {
                return student;
            }
        }
        return nullptr;
    }

    Course* findCourse(int courseCode) {
        for (auto& course : courses) {
            if (course.getCourseCode() == courseCode) {
                return &course;
            }
        }
        return nullptr;
    }

    void displayAllStudents() {
        for (const auto& student : students) {
            student->displayInfo();
        }
    }

    void displayAllCourses() {
        for (const auto& course : courses) {
            course.displayInfo();
        }
    }

    void calculateAndDisplayGrades() {
        for (const auto& student : students) {
            float totalScore = 0;
            auto studentGrades = grades[student->getId()];
            for (const auto& grade : studentGrades) {
                totalScore += grade.second;
            }
            cout << "学生 " << student->getName() << " 的总成绩: " << totalScore << endl;
        }
    }
    void removeStudent(int studentId) {
        students.erase(
            remove_if(students.begin(), students.end(),
                [studentId](const shared_ptr<Student>& student) { return student->getId() == studentId; }),
            students.end());
    }

    void removeCourse(int courseCode) {
        courses.erase(
            remove_if(courses.begin(), courses.end(),
                [courseCode](const Course& course) { return course.getCourseCode() == courseCode; }),
            courses.end());
    }

    void sortStudentsByTotalScore() {
        sort(students.begin(), students.end(),
            [](const shared_ptr<Student>& a, const shared_ptr<Student>& b) {
                return a->calculateTotalScore() > b->calculateTotalScore();
            });
    }
    // 返回对学生列表的引用
    const vector<shared_ptr<Student>>& getStudents() const {
        return students;
    }

    // 返回对课程列表的引用
    const vector<Course>& getCourses() const {
        return courses;
    }

    // 其他必要的方法，如删除学生、课程，排序学生等
};
void saveToFile(const StudentManagementSystem& sms, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件进行写入" << endl;
        return;
    }

    // 写入学生信息
    for (const auto& student : sms.getStudents()) {
        file << "Student," << student->getId() << "," << student->getName() << "," << student->getGender() << "," << student->getGrade();
        for (const auto& course : student->getCourses()) {
            file << "," << course.first << ":" << course.second;
        }
        file << endl;
    }

    // 写入课程信息
    for (const auto& course : sms.getCourses()) {
        file << "Course," << course.getCourseCode() << "," << course.getCourseName() << "," << course.getExamType() << endl;
    }

    file.close();
}

void addStudentToSystem(StudentManagementSystem& sms) {
    string studentID, name, gender, grade;
    cout << "请输入学生ID: ";
    cin >> studentID;
    cout << "请输入学生姓名: ";
    cin >> name;
    cout << "请输入学生性别: ";
    cin >> gender;
    cout << "请输入学生年级: ";
    cin >> grade;

    auto student = make_shared<Student>(studentID, name, gender, grade);
    sms.addStudent(student);
    cout << "学生已成功添加。" << endl;
}
void addCourseToSystem(StudentManagementSystem& sms) {
    int courseCode;
    string courseName, examType;
    cout << "请输入课程代码: ";
    cin >> courseCode;
    cout << "请输入课程名称: ";
    cin.ignore(); // 忽略之前的换行符
    getline(cin, courseName);
    cout << "请输入考试方式: ";
    getline(cin, examType);

    Course course(courseCode, courseName, examType);
    sms.addCourse(course);
    cout << "课程已成功添加。" << endl;
}
void loadFromFile(StudentManagementSystem& sms, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件进行读取" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        getline(ss, type, ',');

        if (type == "Student") {
            // 解析学生数据
            string studentID, name, gender, grade;
            getline(ss, studentID, ',');
            getline(ss, name, ',');
            getline(ss, gender, ',');
            getline(ss, grade, ',');

            auto student = make_shared<Student>(studentID, name, gender, grade);

            // 解析课程成绩
            string courseItem;
            while (getline(ss, courseItem, ',')) {
                stringstream courseStream(courseItem);
                string courseID;
                float score;
                getline(courseStream, courseID, ':');
                courseStream >> score;
                student->addCourse(courseID, score);
            }

            sms.addStudent(student);
        }
        else if (type == "Course") {
            // 解析课程数据
            int courseCode;
            string courseName, examType;
            ss >> courseCode;
            getline(ss, courseName, ',');
            getline(ss, examType, ',');

            Course course(courseCode, courseName, examType);
            sms.addCourse(course);
        }
    }

    file.close();
}



int main() {
    StudentManagementSystem sms;

    // 自动从文件加载数据
    loadFromFile(sms, "data.txt");

    int choice;
    do {
        cout << "1. 添加学生" << endl;
        cout << "2. 添加课程" << endl;
        cout << "3. 显示所有学生" << endl;
        cout << "4. 显示所有课程" << endl;
        cout << "5. 手动保存到文件" << endl;
        cout << "6. 从文件加载" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addStudentToSystem(sms);
            break;
        case 2:
            addCourseToSystem(sms);
            break;
        case 3:
            sms.displayAllStudents();
            break;
        case 4:
            sms.displayAllCourses();
            break;
        case 5:
            saveToFile(sms, "data.txt");
            cout << "数据已手动保存到文件。" << endl;
            break;
        case 6:
            loadFromFile(sms, "data.txt");
            cout << "数据已从文件加载。" << endl;
            break;
        case 0:
            cout << "退出程序并保存数据..." << endl;
            saveToFile(sms, "data.txt");
            break;
        default:
            cout << "无效的选择，请重新输入" << endl;
        }
    } while (choice != 0);

    return 0;
}
