#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#ifdef _MSC_VER // Memory leak check
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;

const int NUM_GRADES = 5;

int main(int argc, char* argv[]){
    VS_MEM_CHECK               // Enable memory leak check

    if (argc < 3){
        cerr << "Please provide name of input and output files.";
        return 1;
    }
    cout << "Input file: " << argv[1] << endl;
    ifstream in(argv[1]);
    if (!in.is_open()){
        cerr << "Unable to open " << argv[1] << " for input.";
        return 2;
    }
    cout << "Output file: " << argv[2] << endl;
    ofstream out(argv[2]);
    if (!out){
        in.close();
        cerr << "Unable to open " << argv[2] << " for output.";
        return 3;
    }

    int num_students;
    int num_exams;
    in >> num_students >> num_exams;
    in.ignore(std::numeric_limits<int>::max(), '\n');

    string* students = new string[num_students];
    int** scores = new int* [num_students];

    for (int i = 0; i < num_students; ++i){
        scores[i] = new int[num_exams];
    }

    //parse through input file

    for (int i = 0; i < num_students; ++i){
        string line;
        string name;
        getline(in, line);
        size_t p = 0;
        while (!isdigit(line[p])) ++p;

        name = line.substr(0, p - 1);
        students[i] = name;
        line = line.substr(p);
        istringstream iss(line);
        for (int j = 0; j < num_exams; ++j){
            int score;
            iss >> score;
            scores[i][j] = score;
        }
    }
    //close input file:
    in.close();

    //part 1:

    out << "Student Scores: " << endl;
    for (int i = 0; i < num_students; ++i){
        if (i != 0) out << endl;
        out << setw(20) << students[i] << " ";
        for (int j = 0; j < num_exams; ++j){
            out << setw(6) << scores[i][j];
        }
    }
    out << endl;

    //part 2:

    out << "Exam Averages: " << endl;
    double* avg = new double[num_exams];

    for (int i = 0; i < num_exams; ++i){
        double sum = 0.0;
        for (int j = 0; j < num_students; ++j){
            sum += scores[j][i];
        }

        double average = sum / num_students;
        avg[i] = average;

        out << setw(9) << "Exam " << i + 1 << " Average =";
        out << setw(7) << fixed << setprecision(1) << avg[i] << '\n';
    }

    //part 3:

    out << "Student Exam Grades: " << endl;
    int** grade_count = new int* [num_exams];
    for (int i = 0; i < num_exams; ++i){
        grade_count[i] = new int[NUM_GRADES];
        for (int j = 0; j < NUM_GRADES; ++j){
            grade_count[i][j] = 0;
        }
    }

    for (int i = 0; i < num_students; ++i){
        if (i != 0) out << endl;
        out << setw(21) << students[i];
        for (int j = 0; j < num_exams; ++j){
            string grade = "_";
            if (scores[i][j] - avg[j] > 5.0 && scores[i][j] - avg[j] < 15.0){
                grade = "B";
                ++grade_count[j][1];
            }
            else if(scores[i][j] - avg[j] > 15.0){
                grade = "A";
                ++grade_count[j][0];
            }
            else if (scores[i][j] - avg[j] < -5.0 && scores[i][j] - avg[j] > -15.0){
                grade = "D";
                ++grade_count[j][3];
            }
            else if (scores[i][j] - avg[j] < -15.0){
                grade = "E";
                ++grade_count[j][4];
            }
            else{
                grade = "C";
                ++grade_count[j][2];
            }
            out << setw(5) << scores[i][j] << "(" << grade << ")";
        }
    }

    //part 4:

    out << endl << "Exam Grades: " << endl;
    for (int i = 0; i < num_exams; ++i){
        out << setw(8) << "Exam" << setw(3) << i + 1;
        out << setw(5) << grade_count[i][0] << "(A)";
        out << setw(5) << grade_count[i][1] << "(B)";
        out << setw(5) << grade_count[i][2] << "(C)";
        out << setw(5) << grade_count[i][3] << "(D)";
        out << setw(5) << grade_count[i][4] << "(E)";
        out << endl;
    }

    //part 5:

    out << "Student Final Grades: " << endl;
    double* total_grade = new double[num_students];

    double classSumGrade = 0.0;
    for (int i = 0; i < num_students; ++i){
        double sumGrade = 0.0;
        for (int j = 0; j < num_exams; ++j){
            sumGrade = sumGrade + scores[i][j];
            total_grade[i] = sumGrade / num_exams;
        }
        classSumGrade = classSumGrade + total_grade[i];
    }

    double classAverage = classSumGrade / num_students;
    for (int i = 0; i < num_students; ++i){
        string grade = "_";
        if (total_grade[i] - classAverage > 5.0 && total_grade[i] - classAverage < 15.0){
            grade = "B";
        }
        else if (total_grade[i] - classAverage > 15.0){
            grade = "A";
        }
        else if (total_grade[i] - classAverage < -5.0 && total_grade[i] - classAverage > -15.0){
            grade = "D";
        }
        else if (total_grade[i] - classAverage < -15.0){
            grade = "E";
        }
        else{
            grade = "C";
        }
        out << setw(20) << students[i] << setw(6) << total_grade[i] << "(" << grade << ")" << endl;
    }
    out << "Class Average Score = " << classAverage;

    out.close();

    //Delete arrays to avoid memory leak:
    delete[] students;
    delete[] total_grade;
    delete[] avg;
    for (int i = 0; i < num_students; ++i){
        delete[] scores[i];
    }
    delete[] scores;
    for (int i = 0; i < num_exams; ++i){
        delete[] grade_count[i];
    }
    delete[] grade_count;

    return 0;
}