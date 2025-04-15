#include <iostream>
#include "type_detector.hpp"

/*
$ g++ const_ref.cpp -DCPP17 -o const_ref
$ ./const_ref
0
1
0
decltype(ret) is int
0
decltype(cref) is const int&
*/

struct myclass{
	int i = 0;
	const int & get_data()const{return i;}
};

int main(){
	myclass c;

	auto ret = c.get_data();
	std::cout<<ret<<std::endl;
	//even though get_data returns a const ref, we can modify ret
	ret = 1;
	std::cout<<ret<<std::endl;
	//but this won't affect the class object member
	std::cout<<c.i<<std::endl;

	//indeed, ret is not a const ref
	std::cout << "decltype(ret) is " <<
	type_name<decltype(ret)>() << std::endl;

	//if we enforce it to be a const ref,
	const int & cref = c.get_data();
	std::cout<<cref<<std::endl;
	//then cref = 1 will rise compile-time error:
	//assignment of read-only reference ‘cref’.
	//un-comment to verify
	//cref = 1;
	std::cout << "decltype(cref) is " <<
	type_name<decltype(cref)>() << std::endl;
	
	//a workaround is to use decltype auto
	//BUT you should instead use auto and add by hand
	decltype (auto) da = c.get_data ();
	std::cout<<da<<std::endl;
	std::cout << "decltype(da) is " <<
	type_name<decltype(da)>() << std::endl;
	
	//So, you should
	const auto & acref = c.get_data ();
	std::cout<<acref<<std::endl;
	std::cout << "decltype(acref) is " <<
	type_name<decltype(acref)>() << std::endl;
	return 0;
}

