#include "pch.h"
#include "../Greibach/Greibach.h"
#include "../Greibach/tinyxml2.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace tinyxml2;

Rhs::Rhs() : rhs(vector<pair<bool, string>>()) { }

void Rhs::insert_symbol(bool sym_type, string symbol)
{
	rhs.push_back(pair<bool, string>(sym_type, symbol));
}

vector<pair<bool, string>> Rhs::get_rhs()
{
	return rhs;
}

Rhs::~Rhs()
{
	for (int i = 0; i < rhs.size(); i++)
		rhs[i].second.clear();
	rhs.clear();
}

bool operator<(const Rhs & left, const Rhs & right)
{
	if (left.rhs.size() < right.rhs.size())
		return true;
	if (left.rhs.size() > right.rhs.size())
		return false;
	for (int i = 0; i < left.rhs.size(); i++)
	{
		if (left.rhs[i].first < right.rhs[i].first)
			return true;
		if (left.rhs[i].first > right.rhs[i].first)
			return false;
		if (left.rhs[i].second < right.rhs[i].second)
			return true;
		if (left.rhs[i].second > right.rhs[i].second)
			return false;
	}
	return false;
}

bool Grammar::start_from_termianl(set<Rhs> chain)
{
	for (Rhs r : chain)
		if (!r.get_rhs()[0].first)
			return false;
	return true;
}

Grammar::Grammar() : nonterminalsymbols(set<string>()),
terminalsymbols(map<string, string>()),
rules(map<string, set<Rhs>>()),
startsymbol("") { }

void Grammar::insert_nonterminalsymbol(string nonterminalsymbol)
{
	nonterminalsymbols.insert(nonterminalsymbol);
}

void Grammar::insert_terminalsymbol(string name, string spell)
{
	terminalsymbols.insert(pair<string, string>(name, spell));
}

void Grammar::insert_rule(string nonterminalsymbol, Rhs rhs)
{
	if (rules.find(nonterminalsymbol) == rules.end())
		rules.insert(pair<string, set<Rhs>>(nonterminalsymbol, set<Rhs>()));
	rules[nonterminalsymbol].insert(rhs);
}

void Grammar::set_rules(map<string, set<Rhs>> rules)
{
	this->rules = rules;
}

void Grammar::set_startsymbol(string startsymbol)
{
	this->startsymbol = startsymbol;
}

set<string> Grammar::get_nonterminalsymbols()
{
	return nonterminalsymbols;
}

map<string, string> Grammar::get_terminalsymbols()
{
	return terminalsymbols;
}

map<string, set<Rhs>> Grammar::get_rules()
{
	return rules;
}

string Grammar::get_startsymbol()
{
	return startsymbol;
}

void Grammar::greibach_normal_form()
{
	Grammar g;

	for (string nonterminal : nonterminalsymbols)
	{
		if (rules.count(nonterminal) == 0)
			continue;
		while (!start_from_termianl(rules[nonterminal]))
		{
			set<Rhs> new_rhs;
			for (Rhs rhs : rules[nonterminal])
				if (rhs.get_rhs()[0].first == true)
					new_rhs.insert(rhs);
				else
				{

					string sym = rhs.get_rhs()[0].second;
					for (Rhs rhs2 : rules[sym])
					{
						Rhs r;
						r = rhs2;
						int i = 0;
						for (pair<bool, string> s : rhs.get_rhs())
							if (i++ != 0)
								r.insert_symbol(s.first, s.second);
						new_rhs.insert(r);
					}

				}
			rules[nonterminal].clear();
			rules[nonterminal] = new_rhs;
		}
	}
	set<string> additional_nonterminals;
	map<string, set<Rhs>> additional_rules;
	for (string nonterminal : nonterminalsymbols)
	{
		if (rules.count(nonterminal) == 0)
			continue;
		set<Rhs> new_rhs;
		for (Rhs rhs : rules[nonterminal])
		{
			Rhs r;
			r.insert_symbol(rhs.get_rhs()[0].first, rhs.get_rhs()[0].second);
			for (int i = 1; i < rhs.get_rhs().size(); i++)
			{
				if (!rhs.get_rhs()[i].first)
					r.insert_symbol(rhs.get_rhs()[i].first, rhs.get_rhs()[i].second);
				else
				{
					string sym;
					bool b = false;
					for (pair<string, set<Rhs>> pr : additional_rules)
					{
						for (Rhs rr : pr.second) // 1 элемент
							if (rr.get_rhs()[0].second == rhs.get_rhs()[i].second)
							{
								sym = pr.first;
								b = true;
							}
						if (b)
							break;
					}
					if (!b)
					{
						int k = 0;
						sym = rhs.get_rhs()[i].second;
						while (nonterminalsymbols.find(sym + to_string(++k)) != nonterminalsymbols.end() ||
							additional_nonterminals.find(sym + to_string(k)) != additional_nonterminals.end());
						sym = sym + to_string(k);
						additional_nonterminals.insert(sym);
						Rhs t;
						set<Rhs> st;
						t.insert_symbol(true, rhs.get_rhs()[i].second);
						st.insert(t);
						additional_rules.insert(pair<string, set<Rhs>>(sym, st));
					}
					r.insert_symbol(false, sym);
				}
			}
			new_rhs.insert(r);
		}
		rules[nonterminal].clear();
		rules[nonterminal] = new_rhs;
	}
	for (string s : additional_nonterminals)
		nonterminalsymbols.insert(s);
	for (pair<string, set<Rhs>> p : additional_rules)
		rules.insert(p);
}

Grammar::~Grammar()
{
	nonterminalsymbols.clear();
	terminalsymbols.clear();
	rules.clear();
	startsymbol.clear();
}

Grammar XmlHandler::read_xml(string in_file)
{
	Grammar grammar;
	XMLDocument doc;
	doc.LoadFile(in_file.c_str());
	if (doc.ErrorID())
		throw runtime_error("Не удалось обработать XML файл\n");
	XMLHandle docHandle(&doc);
	XMLHandle root = docHandle.FirstChildElement("grammar");
	XMLElement* terminalsymbols = root.FirstChildElement("terminalsymbols").ToElement();
	XMLElement* term;
	while (terminalsymbols)
	{
		term = terminalsymbols->FirstChildElement("term");
		while (term) {
			grammar.insert_terminalsymbol(term->Attribute("name"), term->Attribute("spell"));
			term = term->NextSiblingElement();
		}
		terminalsymbols = terminalsymbols->NextSiblingElement();
	}
	XMLElement* nonterminalsymbols = root.FirstChildElement("nonterminalsymbols").ToElement();
	XMLElement* nonterm;
	while (nonterminalsymbols)
	{
		nonterm = nonterminalsymbols->FirstChildElement("nonterm");
		while (nonterm) {
			grammar.insert_nonterminalsymbol(nonterm->Attribute("name"));
			nonterm = nonterm->NextSiblingElement();
		}
		nonterminalsymbols = nonterminalsymbols->NextSiblingElement();
	}
	XMLElement* startsymbol = root.FirstChildElement("startsymbol").ToElement();
	if (!startsymbol)
		throw runtime_error("Отсутствует стартовый символ\n");
	grammar.set_startsymbol(startsymbol->Attribute("name"));
	set<string> nonterminals = grammar.get_nonterminalsymbols();
	if (nonterminals.find(startsymbol->Attribute("name")) == nonterminals.end())
		throw runtime_error("Cтартовый символ не найден в списке нетерминалов\n");
	map<string, string> terminals = grammar.get_terminalsymbols();
	XMLElement* productions = root.FirstChildElement("productions").ToElement();
	XMLElement* production, *lhs, *rhs;
	while (productions)
	{
		production = productions->FirstChildElement("production");
		while (production)
		{
			lhs = production->FirstChildElement("lhs");
			if (!lhs)
				throw runtime_error("Одна из продукций не имеет левой части\n");
			rhs = production->FirstChildElement("rhs");
			if (!rhs)
				throw runtime_error("Одна из продукций не имеет правой части\n");
			string lhs_name;
			lhs_name = lhs->Attribute("name");
			if (nonterminals.find(lhs_name) == nonterminals.end())
				throw runtime_error("В списке правил обнаружен неизвестный символ\n");
			XMLElement* symbol = rhs->FirstChildElement("symbol");
			Rhs rhs;
			bool sym_type;
			string sym;
			while (symbol)
			{
				if (string(symbol->Attribute("type")) == "nonterm")
					sym_type = false;
				else if (string(symbol->Attribute("type")) == "term")
					sym_type = true;
				else
					throw runtime_error("Неизвестный тип элемента в правой части продукции\n");
				sym = symbol->Attribute("name");
				if (sym_type)
				{
					if (!terminals.count(sym))
						throw runtime_error("Неизвестный терминал в правой части продукции\n");
				}
				else
				{
					if (nonterminals.find(sym) == nonterminals.end())
						throw runtime_error("Неизвестный нетерминал в правой части продукции\n");
				}
				rhs.insert_symbol(sym_type, sym);
				symbol = symbol->NextSiblingElement();
			}
			grammar.insert_rule(lhs_name, rhs);
			production = production->NextSiblingElement();
		}
		productions = productions->NextSiblingElement();
	}
	return grammar;
}

void XmlHandler::write_xml(std::string out_file, Grammar grammar)
{
	static const char* xml =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<grammar name=\"G1\">"
		"	<terminalsymbols/>"
		"	<nonterminalsymbols/>"
		"	<productions/>"
		"	<startsymbol/>"
		"</grammar>";
	XMLDocument doc;
	doc.Parse(xml);
	XMLElement* terminalsymbols = doc.FirstChildElement("grammar")->FirstChildElement("terminalsymbols");
	map<string, string> terminals = grammar.get_terminalsymbols();
	for (pair<string, string> term : terminals)
	{
		XMLElement* termelement = terminalsymbols->InsertNewChildElement("term");
		termelement->SetAttribute("name", term.first.c_str());
		termelement->SetAttribute("spell", term.second.c_str());
	}
	XMLElement* nonterminalsymbols = doc.FirstChildElement("grammar")->FirstChildElement("nonterminalsymbols");
	set<string> nonterminals = grammar.get_nonterminalsymbols();
	for (string nonterm : nonterminals)
		nonterminalsymbols->InsertNewChildElement("nonterm")->SetAttribute("name", nonterm.c_str());
	map<string, set<Rhs>> rules = grammar.get_rules();
	XMLElement* productions = doc.FirstChildElement("grammar")->FirstChildElement("productions");
	for (pair<string, set<Rhs>> term : rules)
		for (Rhs rhs : term.second)
		{
			XMLElement* production = productions->InsertNewChildElement("production");
			production->InsertNewChildElement("lhs")->SetAttribute("name", term.first.c_str());
			XMLElement* rhselement = production->InsertNewChildElement("rhs");
			vector<pair<bool, string>> chain = rhs.get_rhs();
			for (pair<bool, string> sym : chain)
			{
				XMLElement* symbolelement = rhselement->InsertNewChildElement("symbol");
				if (sym.first)
					symbolelement->SetAttribute("type", "term");
				else
					symbolelement->SetAttribute("type", "nonterm");
				symbolelement->SetAttribute("name", sym.second.c_str());
			}
		}
	doc.FirstChildElement("grammar")->FirstChildElement("startsymbol")->SetAttribute("name", grammar.get_startsymbol().c_str());
	doc.SaveFile(out_file.c_str());
}
