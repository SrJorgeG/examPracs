#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

class bigint
{
	private:
		std::string	_str;
	public:
	
		// ORTHODOX CANONNICAL FORM 

		bigint();
		bigint(unsigned int n);
		bigint(const bigint& other);
		bigint&	operator=(const bigint& other);
		~bigint();
		
		// METHODS

		std::string	get_str() const;
		void	validate();

		// PRE & POST INCREMENT OPERATORS

		bigint&	operator++();
		bigint	operator++(int);
		
		// SUM OPERATORS

		bigint&	operator+=(const bigint& other);
		bigint	operator+(const bigint& other) const;
		
		// BIT OPS OPERATORS

		bigint&	operator>>=(const bigint& obj);
		bigint&	operator<<=(unsigned int n);
		bigint	operator<<(unsigned int n) const;

		// COMPARATION OPERATORS 

		bool	operator==(const bigint& other) const;
		bool	operator!=(const bigint& other) const;
		bool	operator<(const bigint& other) const;
		bool	operator>(const bigint& other) const;
		bool	operator<=(const bigint& other) const;
		bool	operator>=(const bigint& other) const;
};

// INSERTION OPERTATOR

std::ostream&	operator<<(std::ostream& os, const bigint& toPrint);