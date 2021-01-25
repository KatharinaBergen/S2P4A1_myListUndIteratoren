#include <exception>
#include <iostream>
#include "MyList.h"

int main() try
{
	MyList<int> mine{};
	mine.push_back(1);
	mine.push_back(2);
	mine.push_back(3);
	mine.push_back(4);
	mine.push_back(5);

	MyList<int>::MyIterator p{ mine.begin() };
	p++;
	p++;
	p++;
	p = mine.insert(42, p);
	p = mine.remove(p);

	mine.push_front(9);
	mine.pop_front();

	MyNode<int>* i{mine.begin().get_curr()->suc->suc};
	mine.ins(42, i);
	i = i->pre;
	mine.rem(i);

	return 0;
}
catch (const std::exception& e)
{
	std::cerr << e.what(); 
	return -1;
}
catch (...)
{
	std::cerr << "unknown error";
	return -2;
}