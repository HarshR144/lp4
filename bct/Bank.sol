// SPDX-License-Identifier: Unlicensed
pragma solidity ^0.8.0; //tells us that i wan to use solidity version this or grater

contract MyBank  //just like we create class in oop here we create contract
{
    mapping(address=> uint ) private _balances;
    address public owner;
    event LogDepositeMade(address accountHoder, uint amount );

    constructor () public   //just like class we can write constr here too
     {
         owner=msg.sender;  // jo koi first time contract deploy karega use owner bana do
         emit LogDepositeMade(msg.sender, 1000);
     }


    function deposite() public payable  returns (uint)
    {

    require ((_balances[msg.sender] + msg.value) >  _balances[msg.sender] && msg.sender!=address(0));
    _balances[msg.sender] += msg.value;
    emit LogDepositeMade(msg.sender , msg.value);
    return _balances[msg.sender];
    } 


    function withdraw (uint withdrawAmount) public  returns (uint)
    {

            require (_balances[msg.sender] >= withdrawAmount);
            require(msg.sender!=address(0));
            require (_balances[msg.sender] > 0);
            _balances[msg.sender]-= withdrawAmount;
            payable(msg.sender).transfer(withdrawAmount);
            emit LogDepositeMade(msg.sender , withdrawAmount);
            return _balances[msg.sender];

    }


    function viewBalance() public view returns (uint)
    {
        return _balances[msg.sender];
   }

}