//
// Created by SKryem on 26/08/2021.
//

#ifndef PROJECT_EXAMDETAILS_H
#define PROJECT_EXAMDETAILS_H



#include <string>
#include <iostream>


namespace mtm{


    class ExamDetails {

        int course_id;
        int month;
        int day;
        double hour;
        int duration;
        std::string link;

        static bool validDateCheck(int month, int day);
        static bool validHourCheck(double hour);
        
    public:

        ExamDetails(int course_id, int month, int day, double hour, int duration, const std::string link = "");
        ExamDetails& operator=(const ExamDetails& src) = default;
        ExamDetails(const ExamDetails& src) = default;
        ~ExamDetails() = default;


        std::string getLink() const;

        void setLink(const std::string new_link);

        int operator-(const ExamDetails &exam) const;

        bool operator<(const ExamDetails &exam) const;

        friend std::ostream &operator<<(std::ostream & os, const ExamDetails &exam);

        static  ExamDetails makeMatamExam();
        class InvalidDateException : public std::exception{};
        class InvalidTimeException : public std::exception{};

    };


#endif //PROJECT_EXAMDETAILS_H


}// name space