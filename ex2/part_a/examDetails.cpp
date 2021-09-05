#include "examDetails.h"

#include <iomanip>

using std::string;
using std::endl;
using std::ostream;

using namespace mtm;

const int MONTH_DAYS = 30;

ExamDetails::ExamDetails(int course_id, int month, int day, double time, int duration, const string link)
{
    this->course_id = course_id;
    this->month = month;
    this->day = day;
    this->time = time;
    this->duration = duration;
    this->link = link;
}

ExamDetails::~ExamDetails()
{
}

ExamDetails::ExamDetails(const ExamDetails& examDetails)
{
    this->course_id = examDetails.course_id;
    this->month = examDetails.month;
    this->day = examDetails.day;
    this->time = examDetails.time;
    this->duration = examDetails.duration;
    this->link = examDetails.link;
}

ExamDetails& ExamDetails::operator=(const ExamDetails& examDetails)
{
    this->course_id = examDetails.course_id;
    this->month = examDetails.month;
    this->day = examDetails.day;
    this->time = examDetails.time;
    this->duration = examDetails.duration;
    this->link = examDetails.link;

    return *this;
}

string ExamDetails::getLink() const
{
    return this->link;
}

void ExamDetails::setLink(string link)
{
    this->link = link;
}

int ExamDetails::operator-(const ExamDetails& examDetails) const
{
    return (this->month - examDetails.month) * MONTH_DAYS + (this->day - examDetails.day);
}

bool ExamDetails::operator<(const ExamDetails& examDetails) const
{
    return *this - examDetails < 0 ? true : false;
}

namespace mtm
{
    ostream& operator<<(ostream& os, const ExamDetails& examDetails)
    {
        os << "Course Number: " << examDetails.course_id << endl;

        // ToDo: put logic into private function
        int time_hours = (int)(examDetails.time);
        int time_minutes = (int)((examDetails.time - time_hours) * 60);
        os << "Time: " << examDetails.day << "." << examDetails.month << " at " << time_hours << ":" << std::setw(2) << std::setfill('0') << time_minutes << endl;
        
        int duration_hours = (int)(examDetails.duration);
        int duration_minutes = (int)((examDetails.duration - duration_hours) * 60);
        os << "Duration: " << duration_hours << ':' << std::setw(2) << std::setfill('0') << duration_minutes << endl;
        
        os << "Zoom Link: " << examDetails.link << endl;

        return os;
    }
}

ExamDetails ExamDetails::makeMatamExam()
{
    ExamDetails examDetails(234124, 7, 28, 13.0, 3, "https://tinyurl.com/59hzps6m");

    return examDetails;
}
