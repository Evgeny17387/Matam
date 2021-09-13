#ifndef _EXAM_DETAILS
#define _EXAM_DETAILS

#include <stdexcept>
#include <iostream>

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
        std::string link;

        static void verifyDate(int month, int day);
        static void verifyTime(double time);

        static void splitTimeToHoursAndMinutes(double time, int& time_hours, int& time_minutes);

    public:

        ExamDetails(int course_id, int month, int day, double hour, int length, const std::string link = "");

        ~ExamDetails() = default;
        ExamDetails(const ExamDetails& examDetails) = default;
        ExamDetails& operator=(const ExamDetails& examDetails) = default;

        std::string getLink() const;
        void setLink(std::string link);

        int operator-(const ExamDetails& examDetails) const;

        bool operator<(const ExamDetails& examDetails) const;

        friend std::ostream& operator<<(std::ostream& os,
            const ExamDetails& examDetails);

        static ExamDetails makeMatamExam();

        class InvalidDateException: public std::exception{};
        class InvalidTimeException: public std::exception{};
    };
}

#endif //_EXAM_DETAILS
