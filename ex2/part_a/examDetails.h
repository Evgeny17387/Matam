#ifndef _EXAM_DETAILS
#define _EXAM_DETAILS

#include <iostream>

using std::string;
using std::ostream;

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

        void VerifyDate(int month, int day);
        void VerifyTime(double time);
        void VerifyCourseId(int course_id);

    public:

        ExamDetails(int course_id, int month, int day, double hour, int length, const string link = "");
        ~ExamDetails();

        ExamDetails(const ExamDetails& examDetails);

        ExamDetails& operator=(const ExamDetails& examDetails);

        string getLink() const;
        void setLink(string link);

        int operator-(const ExamDetails& examDetails) const;

        bool operator<(const ExamDetails& examDetails) const;

        friend ostream& operator<<(ostream& os, const ExamDetails& examDetails);

        static ExamDetails makeMatamExam();

        class InvalidDateException{};
        class InvalidTimeException{};
    };
}

#endif //_EXAM_DETAILS
