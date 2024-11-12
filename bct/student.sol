// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract StudentData {
    // Structure definition for Student
    struct Student {
        uint256 studentId;
        string name;
        uint256 age;
        string major;
        bool isActive;
    }

    // Array to store multiple students
    Student[] public students;
    
    // Mapping for quick student lookup using studentId
    mapping(uint256 => uint256) private studentIndex;
    
    // Contract owner
    address public owner;
    
    // Events
    event StudentAdded(uint256 indexed studentId, string name);
    event StudentUpdated(uint256 indexed studentId);
    event StudentDeactivated(uint256 indexed studentId);
    
    // Constructor
    constructor() {
        owner = msg.sender;
    }
    
    // Modifier to check if sender is owner
    modifier onlyOwner() {
        require(msg.sender == owner, "Only owner can call this function");
        _;
    }
    
    // Function to add a new student
    function addStudent(
        uint256 _studentId,
        string memory _name,
        uint256 _age,
        string memory _major
    ) public onlyOwner {
        require(_studentId > 0, "Student ID must be greater than 0");
        require(bytes(_name).length > 0, "Name cannot be empty");
        require(_age > 0, "Age must be greater than 0");
        
        // Create new student
        Student memory newStudent = Student({
            studentId: _studentId,
            name: _name,
            age: _age,
            major: _major,
            isActive: true
        });
        
        // Add to array and update mapping
        students.push(newStudent);
        studentIndex[_studentId] = students.length - 1;
        
        emit StudentAdded(_studentId, _name);
    }
    
    // Function to get student details
    function getStudent(uint256 _studentId) public view returns (
        string memory name,
        uint256 age,
        string memory major,
        bool isActive
    ) {
        uint256 index = studentIndex[_studentId];
        require(index < students.length, "Student not found");
        
        Student memory student = students[index];
        return (student.name, student.age, student.major, student.isActive);
    }
    
    // Function to update student details
    function updateStudent(
        uint256 _studentId,
        string memory _name,
        uint256 _age,
        string memory _major
    ) public onlyOwner {
        uint256 index = studentIndex[_studentId];
        require(index < students.length, "Student not found");
        
        students[index].name = _name;
        students[index].age = _age;
        students[index].major = _major;
        
        emit StudentUpdated(_studentId);
    }
    
    // Function to deactivate a student
    function deactivateStudent(uint256 _studentId) public onlyOwner {
        uint256 index = studentIndex[_studentId];
        require(index < students.length, "Student not found");
        
        students[index].isActive = false;
        
        emit StudentDeactivated(_studentId);
    }
    
    // Function to get total number of students
    function getTotalStudents() public view returns (uint256) {
        return students.length;
    }
    
    // Fallback function
    fallback() external payable {
        revert("Direct payments not accepted");
    }
    
    // Receive function
    receive() external payable {
        revert("Direct payments not accepted");
    }
}