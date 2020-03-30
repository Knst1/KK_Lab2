#include "pch.h"
#include "CppUnitTest.h"
#include "../leftrecursion/leftrecursion.h"
#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(Factorization1)
		{
			setlocale(LC_ALL, "Russian");
			try {
				// Входная грамматика
				const char* inp = "..\\inps\\inp0.xml";
				// Преобразованная грамматика
				const char* outp = "..\\outps\\outp0.xml";
				// Эталонная грамматика
				const char* standard = "..\\standards\\standard0.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if(fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Factorization2)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp5.xml";
				const char* outp = "..\\outps\\outp5.xml";
				const char* standard = "..\\standards\\standard5.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if (fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Recursion1)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp1.xml";
				const char* outp = "..\\outps\\outp1.xml";
				const char* standard = "..\\standards\\standard1.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if (fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Recursion2)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp2.xml";
				const char* outp = "..\\outps\\outp2.xml";
				const char* standard = "..\\standards\\standard2.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if (fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Recursion3)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp3.xml";
				const char* outp = "..\\outps\\outp3.xml";
				const char* standard = "..\\standards\\standard3.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if (fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Recursion4)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp4.xml";
				const char* outp = "..\\outps\\outp4.xml";
				const char* standard = "..\\standards\\standard4.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if (fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}

		TEST_METHOD(Recursion5)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp6.xml";
				const char* outp = "..\\outps\\outp6.xml";
				const char* standard = "..\\standards\\standard6.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar = Grammar::remove_left_recursion(grammar);
				grammar = Grammar::left_factorization(grammar);
				xml_handler.write_xml(outp, grammar);
				std::fstream fin1(outp, std::ios::in | std::ios::ate | std::ios::binary);
				std::fstream fin2(standard, std::ios::in | std::ios::ate | std::ios::binary);
				if (!fin1.is_open())
					throw std::runtime_error("Ошибка открытия тестируемого файла\n");
				if (!fin2.is_open())
					throw std::runtime_error("Ошибка открытия эталонного файла\n");
				if (fin1.tellg() != fin2.tellg())
					Assert::IsTrue(false);
				fin1.seekg(0);
				fin2.seekg(0);
				bool result = true;
				char ch1, ch2;
				while (fin1.get(ch1) && fin2.get(ch2))
					if (ch1 != ch2)
					{
						result = false;
						break;
					}
				fin1.close();
				fin2.close();
				Assert::IsTrue(result);
			}
			catch (const std::exception& err) {
				std::cerr << err.what() << std::endl;
				Assert::IsTrue(false);
			}
		}
	};
}
