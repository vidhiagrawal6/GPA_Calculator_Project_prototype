#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;
struct Course {
    string name;
    string grade;
    int credits;
    double gradePoint;
};

struct Semester {
    int semesterNumber;
    vector<Course> courses;
    double sgpa;
    int totalCredits;
};

class CGPACalculator {
private:
    vector<Semester> semesters;
    double cgpa;
    
    
    double gradeToPoint(const string& grade) {
        if (grade == "A+" || grade == "a+") return 10.0;
        if (grade == "A" || grade == "a") return 9.0;
        if (grade == "B+" || grade == "b+") return 8.0;
        if (grade == "B" || grade == "b") return 7.0;
        if (grade == "C+" || grade == "c+") return 6.0;
        if (grade == "C" || grade == "c") return 5.0;
        if (grade == "D" || grade == "d") return 4.0;
        if (grade == "F" || grade == "f") return 0.0;
        return -1.0; // for Invalid grade, there are less chances of it though 
    }
    
    
    double calculateSGPA(const Semester& sem) {
        double totalPoints = 0.0;
        int totalCredits = 0;
        
        for (const auto& course : sem.courses) {
            totalPoints += course.gradePoint * course.credits;
            totalCredits += course.credits;
        }
        
        return (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0;
    }
    
    // try to calculate overall CGPA
    void calculateCGPA() {
        double totalPoints = 0.0;
        int totalCredits = 0;
        
        for (const auto& sem : semesters) {
            for (const auto& course : sem.courses) {
                totalPoints += course.gradePoint * course.credits;
                totalCredits += course.credits;
            }
        }
        
        cgpa = (totalCredits > 0) ? (totalPoints / totalCredits) : 0.0;
    }

public:
    CGPACalculator() : cgpa(0.0) {}
    
    // to add new semester
    void addSemester() {
        Semester newSem;
        int numCourses;
        
        cout << "\n=== Add New Semester ===\n";
        cout << "Enter semester number: ";
        cin >> newSem.semesterNumber;
        
        // check of pre exists semester
        for (const auto& sem : semesters) {
            if (sem.semesterNumber == newSem.semesterNumber) {
                cout << "Semester " << newSem.semesterNumber << " already exists!\n";
                return;
            }
        }
        
        cout << "Enter number of courses: ";
        cin >> numCourses;
        
        if (numCourses <= 0) {
            cout << "Invalid number of courses!\n";
            return;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        for (int i = 0; i < numCourses; i++) {
            Course course;
            
            cout << "\nCourse " << (i + 1) << ":\n";
            cout << "Enter course name: ";
            getline(cin, course.name);
            
            cout << "Enter grade (A+, A, B+, B, C+, C, D, F): ";
            cin >> course.grade;
            
            course.gradePoint = gradeToPoint(course.grade);
            if (course.gradePoint < 0) {
                cout << "Invalid grade! Please try again.\n";
                i--;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            cout << "Enter credits: ";
            cin >> course.credits;
            
            if (course.credits <= 0) {
                cout << "Invalid credits! Please try again.\n";
                i--;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            newSem.courses.push_back(course);
        }
        
        newSem.sgpa = calculateSGPA(newSem);
        newSem.totalCredits = 0;
        for (const auto& course : newSem.courses) {
            newSem.totalCredits += course.credits;
        }
        
        semesters.push_back(newSem);
        calculateCGPA();
        
        cout << "\nSemester added successfully!\n";
        cout << "SGPA for Semester " << newSem.semesterNumber << ": " 
             << fixed << setprecision(2) << newSem.sgpa << "\n";
    }
    
    // display for all semesters
    void displayAllSemesters() {
        if (semesters.empty()) {
            cout << "\nNo semesters added yet!\n";
            return;
        }
        
        cout << "\n" << string(80, '=') << "\n";
        cout << "                        ACADEMIC RECORD\n";
        cout << string(80, '=') << "\n";
        
        for (const auto& sem : semesters) {
            cout << "\nSEMESTER " << sem.semesterNumber << "\n";
            cout << string(80, '-') << "\n";
            cout << left << setw(30) << "Course Name" 
                 << setw(10) << "Grade" 
                 << setw(10) << "Credits" 
                 << setw(15) << "Grade Points\n";
            cout << string(80, '-') << "\n";
            
            for (const auto& course : sem.courses) {
                cout << left << setw(30) << course.name
                     << setw(10) << course.grade
                     << setw(10) << course.credits
                     << setw(15) << fixed << setprecision(2) << course.gradePoint << "\n";
            }
            
            cout << string(80, '-') << "\n";
            cout << "Total Credits: " << sem.totalCredits << "\n";
            cout << "SGPA: " << fixed << setprecision(2) << sem.sgpa << "\n";
        }
        
        cout << "\n" << string(80, '=') << "\n";
        cout << "CUMULATIVE CGPA: " << fixed << setprecision(2) << cgpa << "\n";
        cout << string(80, '=') << "\n";
    }
    
    // display of current CGPA
    void displayCGPA() {
        if (semesters.empty()) {
            cout << "\nNo data available. Please add semesters first.\n";
            return;
        }
        
        cout << "\n" << string(50, '=') << "\n";
        cout << "Current CGPA: " << fixed << setprecision(2) << cgpa << "\n";
        cout << string(50, '=') << "\n";
    }
    
    // Save data to file
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        
        if (!outFile) {
            cout << "Error: Could not open file for writing!\n";
            return;
        }
        
        outFile << semesters.size() << "\n";
        
        for (const auto& sem : semesters) {
            outFile << sem.semesterNumber << " " << sem.courses.size() << "\n";
            for (const auto& course : sem.courses) {
                outFile << course.name << "\n";
                outFile << course.grade << " " << course.credits << "\n";
            }
        }
        
        outFile.close();
        cout << "\nData saved successfully to " << filename << "!\n";
    }
    
    // to load data from file
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        
        if (!inFile) {
            cout << "No previous data found. Starting fresh.\n";
            return;
        }
        
        semesters.clear();
        int numSemesters;
        inFile >> numSemesters;
        inFile.ignore();
        
        for (int i = 0; i < numSemesters; i++) {
            Semester sem;
            int numCourses;
            inFile >> sem.semesterNumber >> numCourses;
            inFile.ignore();
            
            for (int j = 0; j < numCourses; j++) {
                Course course;
                getline(inFile, course.name);
                inFile >> course.grade >> course.credits;
                inFile.ignore();
                course.gradePoint = gradeToPoint(course.grade);
                sem.courses.push_back(course);
            }
            
            sem.sgpa = calculateSGPA(sem);
            sem.totalCredits = 0;
            for (const auto& course : sem.courses) {
                sem.totalCredits += course.credits;
            }
            
            semesters.push_back(sem);
        }
        
        calculateCGPA();
        inFile.close();
        cout << "\nData loaded successfully from " << filename << "!\n";
    }
    
    // delete a semester
    void deleteSemester() {
        if (semesters.empty()) {
            cout << "\nNo semesters to delete!\n";
            return;
        }
        
        int semNum;
        cout << "\nEnter semester number to delete: ";
        cin >> semNum;
        
        auto it = remove_if(semesters.begin(), semesters.end(),
                           [semNum](const Semester& s) { return s.semesterNumber == semNum; });
        
        if (it != semesters.end()) {
            semesters.erase(it, semesters.end());
            calculateCGPA();
            cout << "Semester " << semNum << " deleted successfully!\n";
        } else {
            cout << "Semester " << semNum << " not found!\n";
        }
    }
};

void displayMenu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "          CGPA CALCULATOR - MAIN MENU\n";
    cout << string(50, '=') << "\n";
    cout << "1. Add New Semester\n";
    cout << "2. Display All Semesters\n";
    cout << "3. Display Current CGPA\n";
    cout << "4. Delete a Semester\n";
    cout << "5. Save Data to File\n";
    cout << "6. Load Data from File\n";
    cout << "7. View Sample Test Cases\n";
    cout << "8. Exit\n";
    cout << string(50, '=') << "\n";
    cout << "Enter your choice: ";
}

void displayTestCases() {
    cout << "\n" << string(70, '=') << "\n";
    cout << "                    SAMPLE TEST CASES\n";
    cout << string(70, '=') << "\n";
    
    cout << "\n--- TEST CASE 1: Simple Semester ---\n";
    cout << "Semester Number: 1\n";
    cout << "Number of courses: 3\n\n";
    cout << "Course 1:\n";
    cout << "  Name: Data Structures\n";
    cout << "  Grade: A+\n";
    cout << "  Credits: 4\n\n";
    cout << "Course 2:\n";
    cout << "  Name: Mathematics\n";
    cout << "  Grade: A\n";
    cout << "  Credits: 3\n\n";
    cout << "Course 3:\n";
    cout << "  Name: Physics\n";
    cout << "  Grade: B+\n";
    cout << "  Credits: 3\n";
    cout << "\nExpected SGPA: 9.20\n";
    
    cout << "\n--- TEST CASE 2: Multiple Semesters ---\n";
    cout << "Add Semester 2 after Semester 1:\n";
    cout << "Number of courses: 4\n\n";
    cout << "Course 1: Algorithms | Grade: A+ | Credits: 4\n";
    cout << "Course 2: Database Systems | Grade: A | Credits: 3\n";
    cout << "Course 3: Operating Systems | Grade: A | Credits: 4\n";
    cout << "Course 4: Computer Networks | Grade: B+ | Credits: 3\n";
    cout << "\nExpected Semester 2 SGPA: 9.29\n";
    cout << "Expected Cumulative CGPA: 9.24\n";
    
    cout << "\n--- TEST CASE 3: Mixed Grades ---\n";
    cout << "Semester 3 with varied performance:\n";
    cout << "Course 1: Machine Learning | Grade: A+ | Credits: 4\n";
    cout << "Course 2: Web Development | Grade: B | Credits: 3\n";
    cout << "Course 3: Software Engineering | Grade: C+ | Credits: 3\n";
    cout << "Course 4: Theory of Computation | Grade: A | Credits: 2\n";
    cout << "\nExpected Semester 3 SGPA: 7.83\n";
    
    cout << "\n--- TEST CASE 4: Error Handling ---\n";
    cout << "Test invalid inputs:\n";
    cout << "  - Invalid grade: 'X' (should reject)\n";
    cout << "  - Negative credits: -3 (should reject)\n";
    cout << "  - Zero credits: 0 (should reject)\n";
    cout << "  - Duplicate semester: Add semester 1 again (should reject)\n";
    
    cout << "\n--- TEST CASE 5: Edge Case - All A+ ---\n";
    cout << "Semester 4 with perfect grades:\n";
    cout << "Course 1: Artificial Intelligence | Grade: A+ | Credits: 4\n";
    cout << "Course 2: Cloud Computing | Grade: A+ | Credits: 3\n";
    cout << "Course 3: Blockchain | Grade: A+ | Credits: 3\n";
    cout << "\nExpected SGPA: 10.00\n";
    
    cout << "\n--- TEST CASE 6: File I/O Test ---\n";
    cout << "1. Add some semesters\n";
    cout << "2. Select option 5 to save\n";
    cout << "3. Exit program (option 7)\n";
    cout << "4. Run program again\n";
    cout << "5. Select option 6 to load\n";
    cout << "Expected: All previous data should be restored\n";
    
    cout << "\n" << string(70, '=') << "\n";
}

int main() {
    CGPACalculator calculator;
    int choice;
    string filename = "cgpa_data.txt";
    
    cout << "\n" << string(50, '*') << "\n";
    cout << "      WELCOME TO CGPA CALCULATOR\n";
    cout << string(50, '*') << "\n";
    
    // try to load existing data
    calculator.loadFromFile(filename);
    
    while (true) {
        displayMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                calculator.addSemester();
                break;
            case 2:
                calculator.displayAllSemesters();
                break;
            case 3:
                calculator.displayCGPA();
                break;
            case 4:
                calculator.deleteSemester();
                break;
            case 5:
                calculator.saveToFile(filename);
                break;
            case 6:
                calculator.loadFromFile(filename);
                break;
            case 7:
                displayTestCases();
                break;
            case 8:
                cout << "\nSaving data before exit...\n";
                calculator.saveToFile(filename);
                cout << "Thank you for using CGPA Calculator!\n";
                return 0;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    }
    
    return 0;
}
