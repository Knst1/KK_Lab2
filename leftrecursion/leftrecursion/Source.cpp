#include "leftrecursion.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	XmlHandler xml_handler;
	try {
		Grammar grammar = xml_handler.read_xml(argv[1]);
		grammar = Grammar::remove_left_recursion(grammar);
		grammar = Grammar::left_factorization(grammar);
		xml_handler.write_xml(argv[2], grammar);
	}
	catch (const exception& err) {
		cerr << err.what() << endl;
	}
	return 0;
}
