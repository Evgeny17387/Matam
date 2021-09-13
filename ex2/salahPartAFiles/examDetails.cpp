//
// Created by SKryem on 26/08/2021.
//


#include <string>
#include <cmath>
#include <iostream>
using std::modf;
using std::string;
using std::ostream;
using std::endl;
#include "examDetails.h"

namespace mtm {
const int MAX_MONTH= 12;
const int DAYS_IN_MONTH= 30;
const int MAX_HOUR= 23;
const int MATAM_COURSE_NUMBER= 234124;
const int MATAM_MONTH=7 ;
const int MATAM_DAY=28 ;
const double MATAM_HOUR=13 ;
const int MATAM_DURATION=3 ;
const string MATAM_LINK= "https://tinyurl.com/59hzps6m";
const double PRECISION=0.000001; // 10^-6

     bool ExamDetails::validDateCheck(int month, int day) // return 1 if valid, 0 if invalid
    {
        if(month<1 || month>MAX_MONTH) {
            return false;
        }
        if(day<1 || day>DAYS_IN_MONTH){
            return false;
        }
        return true;
    }
     bool ExamDetails::validHourCheck(double hour)
    {

        double whole; /// if hour is 7:30 >> whole =7, fraction=0.5
        double fraction=modf(hour,&whole);
        if(whole>MAX_HOUR || whole<0) // 0-23 are the allowed hours
        {
            return false;
        }
        bool cond1=std::fabs(fraction-0.5)  <=  PRECISION; // fraction is 0.5
        bool cond2=std::fabs(fraction)  <= PRECISION; // fraction is 0
        if( !(cond1 || cond2) ) // fraction is neither 0 nor 0.5
        {
            return false;
        }
        return true;
    }

    ExamDetails::ExamDetails(int course_id, int month, int day, double hour, int duration, const string link):
    course_id(course_id), month(month),day(day),hour(hour),duration(duration),link(link) {
        if(!validDateCheck(month,day))
        {
            throw ExamDetails::InvalidDateException();
        }
        if(!validHourCheck(hour))
        {
            throw ExamDetails::InvalidTimeException();
        }

    }




    string ExamDetails::getLink() const 
    {
        return this->link;
    }


    void ExamDetails::setLink(const string new_link)
    {
         this->link=new_link;
    }


    int ExamDetails::operator-(const ExamDetails &exam) const{
        int days1= this->day + this->month * DAYS_IN_MONTH;
        int days2= exam.day + exam.month * DAYS_IN_MONTH;
        return days1-days2;
    }


    bool ExamDetails::operator<(const ExamDetails &exam) const{
        if( *this-exam < 0) return true;
        else if(*this-exam>0) return false;
             else if( this->hour - exam.hour < -PRECISION ) return true;
                  else if( this->hour - exam.hour > PRECISION ) return false;
                       else return false; // equal
    }

    ostream &operator<<(ostream & os, const ExamDetails &exam) {
         double whole; /// if hour is 7:30 >> whole =7, fraction=0.5

         double fraction=modf(exam.hour,&whole);

         os << "Course Number: " << exam.course_id  << endl <<"Time: "  <<exam.day << "." << exam.month << " at ";

         if(std::fabs(fraction-0.5)  <= PRECISION)
         {
            os<<whole<<":30";
         }
         else
         {
            os<<whole<<":00";
         }

         return os<< endl <<"Duration: "<< exam.duration << ":00"<< endl<< "Zoom Link: "<<exam.link<<endl ;
    }

      ExamDetails ExamDetails::makeMatamExam()
     {
          ExamDetails matam_exam(MATAM_COURSE_NUMBER,MATAM_MONTH,MATAM_DAY,MATAM_HOUR,MATAM_DURATION,MATAM_LINK);

         return matam_exam;

    }

} // name space