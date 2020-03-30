#include "leftrecursion.h"
#include "tinyxml2.h"
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

pair<int, vector<pair<int, int>>> Grammar::max_elements(vector<vector<int>> matr)
{
	pair<int, vector<pair<int, int>>> positions;
	int maxv = 0, maxx = -1, maxy = -1, size = matr.size();
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (matr[i][j] > maxv)
			{
				maxv = matr[i][j];
				maxx = i;
				maxy = j;
			}
	if (maxv == 0)
		return positions;
	for (int i = 0; i < size; i++)
		if (matr[maxx][i] == maxv)
			positions.second.push_back(pair<int, int >(maxx, i));
	positions.first = maxv;
	return positions;
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

Grammar Grammar::remove_left_recursion(Grammar grammar)
{
	Grammar g;
	map<string, set<Rhs>> rules = grammar.get_rules();
	set<string> nonterminals = grammar.get_nonterminalsymbols();
	map<string, string> terminals = grammar.get_terminalsymbols();
	set<string> new_nonterminals = set<string>();
	for (string i : nonterminals)
	{
		if (rules.count(i) == 0)
			continue;
		for (string j : nonterminals)
		{
			if (i <= j)
				break;
			if (rules.count(j) == 0)
				continue;
			set<Rhs> new_rhs;
			for (Rhs r : rules[i])
			{
				if (!(r.get_rhs().begin()->first) && r.get_rhs().begin()->second == j)
				{
					for (Rhs r1 : rules[j])
					{
						Rhs rhs;
						for (pair<bool, string> pr : r1.get_rhs())
							rhs.insert_symbol(pr.first, pr.second);
						for (int k = 1; k < r.get_rhs().size(); k++)
							rhs.insert_symbol(r.get_rhs()[k].first, r.get_rhs()[k].second);
						new_rhs.insert(rhs);
					}
				}
				else
				{
					Rhs rhs;
					for (pair<bool, string> pr : r.get_rhs())
						rhs.insert_symbol(pr.first, pr.second);
					new_rhs.insert(rhs);
				}
			}
			rules[i].clear();
			rules[i] = new_rhs;
		}
		string nonterminal = "";
		for (Rhs r : rules[i])
		{
			if (!(r.get_rhs().begin()->first) && r.get_rhs().begin()->second == i)
			{
				if (nonterminal == "")
				{
					unsigned int k = 0;
					while (nonterminals.find(i + to_string(++k)) != nonterminals.end() ||
						new_nonterminals.find(i + to_string(k)) != new_nonterminals.end());
					nonterminal = i + to_string(k);
					new_nonterminals.insert(nonterminal);
					//terminals.insert(pair<string, string>("EPSILON", ""));
					rules.insert(pair<string, set<Rhs>>(nonterminal, set<Rhs>()));
					//Rhs rhs;
					//rhs.insert_symbol(true, "EPSILON");
					//rules[nonterminal].insert(rhs);
					break;
				}
			}
		}
		if (nonterminal != "")
		{
			set<Rhs> chains;
			for (Rhs r : rules[i])
			{
				Rhs rhs;
				if (!(r.get_rhs().begin()->first) && r.get_rhs().begin()->second == i)
				{
					for (int i = 1; i < r.get_rhs().size(); i++)
						rhs.insert_symbol(r.get_rhs()[i].first, r.get_rhs()[i].second);
					rules[nonterminal].insert(rhs); // Ликвидация эпсилон-переходов
					rhs.insert_symbol(false, nonterminal);
					rules[nonterminal].insert(rhs);
				}
				else
				{
					for (pair<bool, string> p : r.get_rhs())
						rhs.insert_symbol(p.first, p.second);
					chains.insert(rhs); // Ликвидация эпсилон-переходов
					rhs.insert_symbol(false, nonterminal);
					chains.insert(rhs);
				}
			}
			rules[i].clear();
			rules[i] = chains;
		}
	}
	for (pair<string, string> term : terminals)
		g.insert_terminalsymbol(term.first, term.second);
	for (string nonterm : nonterminals)
		g.insert_nonterminalsymbol(nonterm);
	for (string nonterm : new_nonterminals)
		g.insert_nonterminalsymbol(nonterm);
	g.set_startsymbol(grammar.get_startsymbol());
	g.set_rules(rules);
	return g;
}

Grammar Grammar::left_factorization(Grammar grammar)
{
	Grammar g;
	map<string, set<Rhs>> rules = grammar.get_rules();
	set<string> nonterminals = grammar.get_nonterminalsymbols();
	map<string, string> terminals = grammar.get_terminalsymbols();
	set<string> new_nonterminals = set<string>();
	for (string nonterminal : nonterminals)
	{
		if (rules.count(nonterminal) == 0)
			continue;
		while (true)
		{
			vector<vector<int>> matr;
			vector<Rhs> rs;
			for (Rhs r : rules[nonterminal])
			{
				rs.push_back(r);
				vector<int> str;
				for (Rhs r1 : rules[nonterminal])
				{
					if (!(r < r1) && !(r1 < r))
					{
						str.push_back(-1);
						continue;
					}
					int minsize = min(r.get_rhs().size(), r1.get_rhs().size());
					bool b = true;
					for (int i = 0; i < minsize; i++)
					{
						if (r.get_rhs()[i].first != r1.get_rhs()[i].first || r.get_rhs()[i].second != r1.get_rhs()[i].second)
						{
							str.push_back(i);
							b = false;
							break;
						}
					}
					if (b)
						str.push_back(minsize);
				}
				matr.push_back(str);
			}

			pair<int, vector<pair<int, int>>> maxelements = max_elements(matr);
			if (maxelements.first < 1)
				break;
			// Создание нового нетерминала и добавление его в список правил
			string new_nonterminal;
			unsigned int k = 0;
			while (nonterminals.find(nonterminal + to_string(++k)) != nonterminals.end() ||
				new_nonterminals.find(nonterminal + to_string(k)) != new_nonterminals.end());
			new_nonterminal = nonterminal + to_string(k);
			new_nonterminals.insert(new_nonterminal);
			rules.insert(pair<string, set<Rhs>>(new_nonterminal, set<Rhs>()));
			// Добавление всех правил для нового нетерминала
			Rhs rhs;
			for (int j = maxelements.first; j < rs[maxelements.second[0].first].get_rhs().size(); j++)
				rhs.insert_symbol(rs[maxelements.second[0].first].get_rhs()[j].first, rs[maxelements.second[0].first].get_rhs()[j].second);
			if (rhs.get_rhs().size() == 0)
			{
				terminals.insert(pair<string, string>("EPSOLON", ""));
				rhs.insert_symbol(true, "EPSOLON");
			}
			rules[new_nonterminal].insert(rhs);
			rhs.~Rhs();
			for (pair<int, int> p : maxelements.second)
			{
				for (int j = maxelements.first; j < rs[p.second].get_rhs().size(); j++)
					rhs.insert_symbol(rs[p.second].get_rhs()[j].first, rs[p.second].get_rhs()[j].second);
				if (rhs.get_rhs().size() == 0)
				{
					terminals.insert(pair<string, string>("EPSOLON", ""));
					rhs.insert_symbol(true, "EPSOLON");
				}
				rules[new_nonterminal].insert(rhs);
				rhs.~Rhs();
			}
			// Изменение правил для текущего нетерминала
			set<Rhs> srhs;
			set<int> v;
			v.insert(maxelements.second[0].first);
			for (pair<int, int> p : maxelements.second)
				v.insert(p.second);
			// Изменившаяся правая часть
			for (int j = 0; j < maxelements.first; j++)
				rhs.insert_symbol(rs[maxelements.second[0].first].get_rhs()[j].first, rs[maxelements.second[0].first].get_rhs()[j].second);
			rhs.insert_symbol(false, new_nonterminal);
			srhs.insert(rhs);
			rhs.~Rhs();
			// Остальное без изменений
			for (int j = 0; j < rs.size(); j++)
			{
				if (v.find(j) != v.end())
					continue;
				for (pair<bool, string> p : rs[j].get_rhs())
				{
					rhs.insert_symbol(p.first, p.second);
				}
				srhs.insert(rhs);
				rhs.~Rhs();
			}
			rules[nonterminal].clear();
			rules[nonterminal] = srhs;
		}
	}
	for (pair<string, string> term : terminals)
		g.insert_terminalsymbol(term.first, term.second);
	for (string nonterm : nonterminals)
		g.insert_nonterminalsymbol(nonterm);
	for (string nonterm : new_nonterminals)
		g.insert_nonterminalsymbol(nonterm);
	g.set_startsymbol(grammar.get_startsymbol());
	g.set_rules(rules);
	return g;
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
