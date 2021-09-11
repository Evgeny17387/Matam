#include "examDetails.h"

#include <iomanip>
#include <math.h>

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

ExamDetails::ExamDetails(int course_id, int month, int day, double time, int duration, const string link): course_id(course_id), month(month), day(day), time(time), duration(duration), link(link)
{
    verifyDate(month, day);
    verifyTime(time);
}

string ExamDetails::getLink() const
{
    return this->link;
}

void ExamDetails::setLink(string link)
{
    this->link = link;
}

// ToDo: should it be friend ?
int ExamDetails::operator-(const ExamDetails& examDetails) const
{
    return (this->month - examDetails.month) * MONTH_DAYS + (this->day - examDetails.day);
}

// ToDo: should it be friend ?
bool ExamDetails::operator<(const ExamDetails& examDetails) const
{
    return *this - examDetails < 0 ? true : false;
}

void ExamDetails::splitTimeToHoursAndMinutes(double time, int& time_hours, int& time_minutes)
{
    double hours;
    double minutes;

    minutes = modf(time, &hours);

    time_hours = (int)hours;
    time_minutes = (int)(minutes * MINUTES_IN_HOUR);
}

// ToDo: why does it have to be declared this way ?
namespace mtm
{
    // ToDo: why it has to be friend ?
    ostream& operator<<(ostream& os, const ExamDetails& examDetails)
    {
        os << "Course Number: " << examDetails.course_id << endl;
        
        int hours;
        int minutes;

        ExamDetails::splitTimeToHoursAndMinutes(examDetails.time, hours, minutes);
        os << "Time: " << examDetails.day << "." << examDetails.month << " at " << hours << ":" << std::setw(2) << std::setfill('0') << minutes << endl;

        ExamDetails::splitTimeToHoursAndMinutes(examDetails.duration, hours, minutes);
        os << "Duration: " << hours << ':' << std::setw(2) << std::setfill('0') << minutes << endl;
        
        os << "Zoom Link: " << examDetails.link << endl;

        return os;
    }
}

ExamDetails ExamDetails::makeMatamExam()
{
    ExamDetails examDetails(MATAM_COURSE_ID, MATAM_EXAM_DATE_MONTH, MATAM_EXAM_DATE_DAY, MATAM_EXAM_TIME, MATAM_EXAM_DURAION, MATAM_EXAM_LINK);

    return examDetails;
}

void ExamDetails::verifyDate(int month, int day)
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

void ExamDetails::verifyTime(double time)
{
    double time_interval_index = time / TIME_INTERVAL;
    double time_interval_index_difference = time_interval_index - (int)time_interval_index;

    if (time_interval_index_difference > TIME_INTERVAL_MAX_DIFFERENCE)
    {
        throw InvalidTimeException();
    }
}
