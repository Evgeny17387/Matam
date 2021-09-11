#ifndef _EXAM_DETAILS
#define _EXAM_DETAILS

#include <stdexcept>
#include <iostream>

using std::string;
using std::ostream;
using std::exception;

namespace mtm
{
    class ExamDetails
    {
    private:

        int course_id;
        int month;
        int day;
        double time;
        int duration;
        string link;

        static void verifyDate(int month, int day);
        static void verifyTime(double time);

        static void splitTimeToHoursAndMinutes(double time, int& time_hours, int& time_minutes);

    public:

        ExamDetails(int course_id, int month, int day, double hour, int length, const string link = "");

        ~ExamDetails() = default;
        ExamDetails(const ExamDetails& examDetails) = default;
        ExamDetails& operator=(const ExamDetails& examDetails) = default;

        string getLink() const;
        void setLink(string link);

        int operator-(const ExamDetails& examDetails) const;

        bool operator<(const ExamDetails& examDetails) const;

        friend ostream& operator<<(ostream& os, const ExamDetails& examDetails);

        static ExamDetails makeMatamExam();

        class InvalidDateException: public exception{};
        class InvalidTimeException: public exception{};
    };
}

#endif //_EXAM_DETAILS
