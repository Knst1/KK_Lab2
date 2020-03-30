#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>

class Rhs {
	// true - òåðìèíàë; false - íåòåðìèíàë
	std::vector<std::pair<bool, std::string>> rhs;
public:
	Rhs();
	void insert_symbol(bool sym_type, std::string symbol);
	std::vector<std::pair<bool, std::string>> get_rhs();
	friend bool operator<(const Rhs& left, const Rhs& right);
	~Rhs();
};

class Grammar {
	std::set<std::string> nonterminalsymbols;
	std::map<std::string, std::string> terminalsymbols;
	std::map<std::string, std::set<Rhs>> rules;
	std::string startsymbol;
	bool start_from_termianl(std::set<Rhs> chain);
public:
	Grammar();
	void insert_nonterminalsymbol(std::string nonterminalsymbol);
	void insert_terminalsymbol(std::string name, std::string spell);
	void insert_rule(std::string nonterminalsymbol, Rhs rhs);
	void set_rules(std::map<std::string, std::set<Rhs>> rules);
	void set_startsymbol(std::string startsymbol);
	std::set<std::string> get_nonterminalsymbols();
	std::map<std::string, std::string> get_terminalsymbols();
	std::map<std::string, std::set<Rhs>> get_rules();
	std::string get_startsymbol();
	void greibach_normal_form();
	~Grammar();
};

class XmlHandler {
public:
	Grammar read_xml(std::string in_file);
	void write_xml(std::string out_file, Grammar grammar);
};
