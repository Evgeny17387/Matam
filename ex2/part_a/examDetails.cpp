#include "examDetails.h"

using std::string;
using std::endl;
using std::ostream;

const int MONTH_DAYS = 30;

mtm::ExamDetails::ExamDetails(int course_id, int month, int day, double hour, int length, const std::string link)
{
    this->course_id = course_id;
    this->month = month;
    this->day = day;
    this->hour = hour;
    this->length = length;
    this->link = link;
}

mtm::ExamDetails::~ExamDetails()
{
}

mtm::ExamDetails::ExamDetails(const ExamDetails& examDetails)
{
    this->course_id = examDetails.course_id;
    this->month = examDetails.month;
    this->day = examDetails.day;
    this->hour = examDetails.hour;
    this->length = examDetails.length;
    this->link = examDetails.link;
}

mtm::ExamDetails& mtm::ExamDetails::operator=(const mtm::ExamDetails& examDetails)
{
    this->course_id = examDetails.course_id;
    this->month = examDetails.month;
    this->day = examDetails.day;
    this->hour = examDetails.hour;
    this->length = examDetails.length;
    this->link = examDetails.link;

    return *this;
}

std::string mtm::ExamDetails::getLink() const
{
    return this->link;
}

void mtm::ExamDetails::setLink(std::string link)
{
    this->link = link;
}

int mtm::ExamDetails::operator-(const mtm::ExamDetails& examDetails) const
{
    return (this->month - examDetails.month) * MONTH_DAYS + (this->day - examDetails.day);
}

bool mtm::ExamDetails::operator<(const mtm::ExamDetails& examDetails) const
{
    return *this - examDetails < 0 ? true : false;
}

namespace mtm
{
    ostream& operator<<(ostream& os, const mtm::ExamDetails& examDetails)
    {
        os << examDetails.course_id;
        return os;
    }
}

mtm::ExamDetails mtm::ExamDetails::makeMatamExam()
{
    mtm::ExamDetails examDetails(234124, 7, 28, 13.0, 3, "https://tinyurl.com/59hzps6m");

    return examDetails;
}
