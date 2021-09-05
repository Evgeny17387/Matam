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
        double hour;
        int length;
        std::string link;


    public:
        ExamDetails(int course_id, int month, int day, double hour, int length, const std::string link = "");
        ~ExamDetails();

        ExamDetails(const ExamDetails& examDetails);

        ExamDetails& operator=(const ExamDetails& examDetails);

        std::string getLink() const;
        void setLink(std::string link);

        int operator-(const ExamDetails& examDetails) const;

        bool operator<(const ExamDetails& examDetails) const;

        friend ostream& operator<<(ostream& os, const ExamDetails& examDetails);

        static ExamDetails makeMatamExam();
    };
}
