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
    map<string, float> courses; // �γ̱�źͳɼ�

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
            cout << "�γ̲����ڣ��޷����³ɼ���" << endl;
        }
    }

    void deleteCourse(string courseID) {
        if (courses.find(courseID) != courses.end()) {
            courses.erase(courseID);
        }
        else {
            cout << "�γ̲����ڣ��޷�ɾ����" << endl;
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
        cout << "ѧ������: " << name << ", ѧ��: " << studentID
            << ", �Ա�: " << gender << ", �꼶: " << grade << endl;
        cout << "�γ̳ɼ�: " << endl;
        for (const auto& course : courses) {
            cout << "�γ̱��: " << course.first << ", �ɼ�: " << course.second << endl;
        }
    }
    int getId() const {
        return stoi(studentID); // ����ѧ���ǿ���ת��Ϊ������
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
    int courseCode; // �γ̱��
    string courseName; // �γ�����
    string examType; // ���Է�ʽ

public:
    Course(int code, string name, string examType)
        : courseCode(code), courseName(name), examType(examType) {}
    int getCourseCode() const {
        return courseCode;
    }

    void displayInfo() const {
        cout << "�γ̱��: " << courseCode
            << ", �γ�����: " << courseName
            << ", ���Է�ʽ: " << examType << endl;
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
    map<int, map<int, float>> grades; // ѧ��ID -> (�γ̴��� -> �ɼ�)

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
            cout << "ѧ�� " << student->getName() << " ���ܳɼ�: " << totalScore << endl;
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
    // ���ض�ѧ���б������
    const vector<shared_ptr<Student>>& getStudents() const {
        return students;
    }

    // ���ضԿγ��б������
    const vector<Course>& getCourses() const {
        return courses;
    }

    // ������Ҫ�ķ�������ɾ��ѧ�����γ̣�����ѧ����
};
void saveToFile(const StudentManagementSystem& sms, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ļ�����д��" << endl;
        return;
    }

    // д��ѧ����Ϣ
    for (const auto& student : sms.getStudents()) {
        file << "Student," << student->getId() << "," << student->getName() << "," << student->getGender() << "," << student->getGrade();
        for (const auto& course : student->getCourses()) {
            file << "," << course.first << ":" << course.second;
        }
        file << endl;
    }

    // д��γ���Ϣ
    for (const auto& course : sms.getCourses()) {
        file << "Course," << course.getCourseCode() << "," << course.getCourseName() << "," << course.getExamType() << endl;
    }

    file.close();
}

void addStudentToSystem(StudentManagementSystem& sms) {
    string studentID, name, gender, grade;
    cout << "������ѧ��ID: ";
    cin >> studentID;
    cout << "������ѧ������: ";
    cin >> name;
    cout << "������ѧ���Ա�: ";
    cin >> gender;
    cout << "������ѧ���꼶: ";
    cin >> grade;

    auto student = make_shared<Student>(studentID, name, gender, grade);
    sms.addStudent(student);
    cout << "ѧ���ѳɹ���ӡ�" << endl;
}
void addCourseToSystem(StudentManagementSystem& sms) {
    int courseCode;
    string courseName, examType;
    cout << "������γ̴���: ";
    cin >> courseCode;
    cout << "������γ�����: ";
    cin.ignore(); // ����֮ǰ�Ļ��з�
    getline(cin, courseName);
    cout << "�����뿼�Է�ʽ: ";
    getline(cin, examType);

    Course course(courseCode, courseName, examType);
    sms.addCourse(course);
    cout << "�γ��ѳɹ���ӡ�" << endl;
}
void loadFromFile(StudentManagementSystem& sms, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ļ����ж�ȡ" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        getline(ss, type, ',');

        if (type == "Student") {
            // ����ѧ������
            string studentID, name, gender, grade;
            getline(ss, studentID, ',');
            getline(ss, name, ',');
            getline(ss, gender, ',');
            getline(ss, grade, ',');

            auto student = make_shared<Student>(studentID, name, gender, grade);

            // �����γ̳ɼ�
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
            // �����γ�����
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

    // �Զ����ļ���������
    loadFromFile(sms, "data.txt");

    int choice;
    do {
        cout << "1. ���ѧ��" << endl;
        cout << "2. ��ӿγ�" << endl;
        cout << "3. ��ʾ����ѧ��" << endl;
        cout << "4. ��ʾ���пγ�" << endl;
        cout << "5. �ֶ����浽�ļ�" << endl;
        cout << "6. ���ļ�����" << endl;
        cout << "0. �˳�" << endl;
        cout << "��ѡ�����: ";
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
            cout << "�������ֶ����浽�ļ���" << endl;
            break;
        case 6:
            loadFromFile(sms, "data.txt");
            cout << "�����Ѵ��ļ����ء�" << endl;
            break;
        case 0:
            cout << "�˳����򲢱�������..." << endl;
            saveToFile(sms, "data.txt");
            break;
        default:
            cout << "��Ч��ѡ������������" << endl;
        }
    } while (choice != 0);

    return 0;
}
