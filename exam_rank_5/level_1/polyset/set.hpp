#include "searchable_bag.hpp"

class set
{
	private:
		searchable_bag&	_bag;
		set();
		set(const set& other);
		set&	operator=(const set& other);
	public:
		set(searchable_bag& s_bag);
		~set();
		
		const searchable_bag&	get_bag();
		bool	has(int value) const;
		
		void insert (int);
		void insert (int *, int);
		void print() const;
		void clear();
};