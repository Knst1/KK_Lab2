#include "pch.h"
#include "CppUnitTest.h"
#include "../Greibach/Greibach.h"
#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(Greibach1)
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
				grammar.greibach_normal_form();
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

		TEST_METHOD(Greibach2)
		{
			setlocale(LC_ALL, "Russian");
			try {
				const char* inp = "..\\inps\\inp1.xml";
				const char* outp = "..\\outps\\outp1.xml";
				const char* standard = "..\\standards\\standard1.xml";
				XmlHandler xml_handler;
				Grammar grammar = xml_handler.read_xml(inp);
				grammar.greibach_normal_form();
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
