#include "examDetails.h"

#include <iomanip>

using std::string;
using std::endl;
using std::ostream;

using namespace mtm;

const int MONTH_FIRT = 1;
const int MONTH_LAST = 12;

const int MONTH_FIRT_DAY = 1;
const int MONTH_LAST_DAY = 30;
const int MONTH_DAYS = MONTH_LAST_DAY - MONTH_FIRT_DAY + 1;

const double TIME_INTERVAL = 0.5;
const double TIME_INTERVAL_MAX_DIFFERENCE = 0.000001;

const int MINUTES_IN_HOUR = 60;

const int MATAM_COURSE_ID = 234124;
const int MATAM_EXAM_DATE_MONTH = 7;
const int MATAM_EXAM_DATE_DAY = 28;
const double MATAM_EXAM_TIME = 13.0;
const int MATAM_EXAM_DURAION = 3;
const string MATAM_EXAM_LINK = "https://tinyurl.com/59hzps6m";

void ExamDetails::VerifyDate(int month, int day)
{
    if ((month < MONTH_FIRT) || (month > MONTH_LAST))
    {
        throw InvalidDateException();
    }

    if ((day < MONTH_FIRT_DAY) || (day > MONTH_LAST_DAY))
    {
        throw InvalidDateException();
    }
}

void ExamDetails::VerifyTime(double time)
{
    double time_interval_index = time / TIME_INTERVAL;
    double time_interval_index_difference = time_interval_index - (int)time_interval_index;

    if (time_interval_index_difference > TIME_INTERVAL_MAX_DIFFERENCE)
    {
        throw InvalidTimeException();
    }
}

ExamDetails::ExamDetails(int course_id, int month, int day, double time, int duration, const string link)
{
    VerifyDate(month, day);
    VerifyTime(time);

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
        int time_minutes = (int)((examDetails.time - time_hours) * MINUTES_IN_HOUR);
        os << "Time: " << examDetails.day << "." << examDetails.month << " at " << time_hours << ":" << std::setw(2) << std::setfill('0') << time_minutes << endl;
        
        int duration_hours = (int)(examDetails.duration);
        int duration_minutes = (int)((examDetails.duration - duration_hours) * MINUTES_IN_HOUR);
        os << "Duration: " << duration_hours << ':' << std::setw(2) << std::setfill('0') << duration_minutes << endl;
        
        os << "Zoom Link: " << examDetails.link << endl;

        return os;
    }
}

ExamDetails ExamDetails::makeMatamExam()
{
    ExamDetails examDetails(MATAM_COURSE_ID, MATAM_EXAM_DATE_MONTH, MATAM_EXAM_DATE_DAY, MATAM_EXAM_TIME, MATAM_EXAM_DURAION, MATAM_EXAM_LINK);

    return examDetails;
}
