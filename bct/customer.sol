// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract CustomerData {
    
    struct Customer {
        uint256 id;
        string name;
        string email;
        uint256 age;
    }
    
    Customer[] public customers;
    uint256 public customerCount;

    event CustomerAdded(uint256 customerId, string name, string email, uint256 age);
    
    // Function to add a new customer
    function addCustomer(string memory _name, string memory _email, uint256 _age) public {
        customerCount++;
        customers.push(Customer(customerCount, _name, _email, _age));
        emit CustomerAdded(customerCount, _name, _email, _age);
    }

    // Function to retrieve a specific customer by ID
    function getCustomer(uint256 _customerId) public view returns (uint256, string memory, string memory, uint256) {
        require(_customerId > 0 && _customerId <= customerCount, "Customer ID does not exist.");
        Customer memory customer = customers[_customerId - 1];
        return (customer.id, customer.name, customer.email, customer.age);
    }

    // Function to retrieve all customers
    function getAllCustomers() public view returns (Customer[] memory) {
        return customers;
    }
    
    // Fallback function to handle unexpected Ether sent to the contract
    fallback() external payable {
        // You can add custom logic here or simply reject the transaction if necessary.
        revert("This contract does not accept Ether.");
    }
    
    // Receive function to receive Ether directly
    receive() external payable {
        revert("Ether not accepted.");
    }
}
