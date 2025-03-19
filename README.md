# Task Management System

## Description
This is a simple console-based Task Management System implemented in C++. The program allows users to create, update, remove, and display tasks categorized into four groups: 

1. **My Day**
2. **Assigned To Me**
3. **Important**
4. **Planned**

Each task consists of a **title**, **category**, **deadline**, **note**, and **priority** level (High, Medium, or Low).

## Features
- **Add Tasks:** Users can add new tasks by providing the required details.
- **Remove Tasks:** Users can delete existing tasks based on their title.
- **Update Tasks:** Users can modify task attributes such as title, category, deadline, note, or priority.
- **Display Tasks:** Users can view tasks based on categories or display all tasks in the database.
- **Persistent Storage:** Tasks are stored in text files (`MyDay.txt`, `AssignedToMe.txt`, `Important.txt`, `Planned.txt`) for data persistence.

## Compilation and Execution

### Compilation:
To compile the program, use the following command in a terminal:

g++ -o TaskManager main.cpp

### Execution:
Run the compiled executable:

./TaskManager


## Usage Instructions

1. Upon starting the program, you will be prompted to select a task category.
2. Inside a category, you can choose to **Add**, **Remove**, **Display**, or **Update** tasks.
3. When adding a task, follow the required format:
   - **Deadline Format:** `mm/dd/yyyy`
   - **Priority:** `1` for High, `2` for Medium, `3` for Low`
4. To update or remove a task, you need to provide the correct task title.
5. Selecting **Exit** will terminate the program, saving changes to respective files.

## File Storage Format
Each task is stored in a category-based text file in the format:

TaskTitle;TaskCategory;TaskDeadline;TaskNote;TaskPriority

Example (`MyDay.txt`):

Complete Project;My Day;04/10/2025;Work on the final project submission;1 Buy Groceries;My Day;04/12/2025;Buy milk, eggs, and bread;3


## Error Handling
- Invalid inputs are handled to prevent crashes.
- Users are prompted to re-enter values if they input incorrect formats.
- The system prevents duplicate task titles in a category.

## Dependencies
- Standard C++ libraries (`iostream`, `fstream`, `string`, `vector`, `sstream`, `regex`, etc.)
- No external dependencies required.

## Possible Future Enhancements
- Implement a **graphical user interface (GUI)** for better user experience.
- Support for **recurring tasks** and **notifications**.
- Improve **search functionality** within tasks.
