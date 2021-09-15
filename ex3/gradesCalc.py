#### PART 1 ####
# final_grade: Calculates the final grade for each student, and writes the output (while eliminating illegal
# rows from the input file) into the file in `output_path`. Returns the average of the grades.
#   input_path: Path to the file that contains the input
#   output_path: Path to the file that will contain the output


ID_INDEX_INPUT_FILE = 0
NAME_INDEX_INPUT_FILE = 1
SEMESTER_INDEX_INPUT_FILE = 2
HW_AVERAGE_INDEX_INPUT_FILE = 3

ID_LENGTH = 8

WHITE_SPACE = ' '

HW_AVEREGA_MIN_VALID_GRADE = 51
HW_AVEREGA_MAX_VALID_GRADE = 100

SEMESTER_MIN_VALID = 1

HW_AVERAGE_INDEX_STUDENTS_DICTIONARY = 2


def final_grade(input_path: str, output_path: str) -> int:

    students = {}

    parse_students_data(input_path, students)

    write_students_grades(output_path, students)

    return calculate_average_grage(students)


def calculate_average_grage(students: dict) -> int:
    
    average = 0

    for student in students:

        average += int(students[student][HW_AVERAGE_INDEX_STUDENTS_DICTIONARY])

    average /= len(students.keys())

    return average


def parse_students_data(input_path:str, students: dict) -> None:

    input_file = open(input_path, 'r')

    for line in input_file:

        id = line.split(',')[ID_INDEX_INPUT_FILE].strip()
        name = line.split(',')[NAME_INDEX_INPUT_FILE].strip()
        semester = line.split(',')[SEMESTER_INDEX_INPUT_FILE].strip()
        hw_averega = line.split(',')[HW_AVERAGE_INDEX_INPUT_FILE].strip()

        is_student_parameter_valid = is_id_valid(id) and is_name_valid(name) and is_semester_valid(semester) and \
            is_hw_average_valid(hw_averega)

        if is_student_parameter_valid:
            students[id] = (id, hw_averega, str(calculate_final_grage(id, hw_averega)))

    input_file.close()


def is_id_valid(id: str) -> bool:

    if id[0] == '0':
        return False

    if len(id) != ID_LENGTH:
        return False

    return True


def is_name_valid(name: str) -> bool:

    for char in name:

        if (char != WHITE_SPACE) and not char.isalpha():
            return False

    return True


def is_semester_valid(semester: str) -> bool:

    if int(semester) < SEMESTER_MIN_VALID:
        return False

    return True


def is_hw_average_valid(hw_averega: str) -> bool:

    hw_averega_int = int(hw_averega)

    if (hw_averega_int < HW_AVEREGA_MIN_VALID_GRADE) or (hw_averega_int > HW_AVEREGA_MAX_VALID_GRADE):
        return False

    return True


def calculate_final_grage(id: str, hw_average: str) -> int:

    id_grade = int(id[-2:])
    hw_average_int = int(hw_average)

    final_grade = int((id_grade + hw_average_int) / 2)

    return final_grade


def write_students_grades(output_path: str, students: dict) -> None:

    output_file = open(output_path, 'w')

    for id in sorted(students):

        line = (", ").join(students[id])

        output_file.write(line + "\n")

    output_file.close()


#### PART 2 ####
# check_strings: Checks if `s1` can be constructed from `s2`'s characters.
#   s1: The string that we want to check if it can be constructed
#   s2: The string that we want to construct s1 from
def check_strings(s1: str, s2: str) -> bool:
    
    for char_code in range(ord('a'),ord('z') + 1):

        char = chr(char_code)

        print(char)

        if s1.lower().count(char) > s2.lower().count(char):
            return False

    return True
