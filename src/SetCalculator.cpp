#include "SetCalculator.h"

#include "Union.h"
#include "Intersection.h"
#include "Difference.h"
#include "Product.h"
#include "Comp.h"
#include "Identity.h"

#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace rng = std::ranges;

SetCalculator::SetCalculator(std::istream& istr, std::ostream& ostr)
	: m_actions(createActions()), m_operations(createOperations()), m_istr(istr), m_ostr(ostr), m_input(&m_istr)
{
	readMaxCommands("Enter max number of commands: ");
}

void SetCalculator::run()
{
	do
	{
		m_ostr << '\n';
		printOperations();
		m_ostr << "Max commands the program can save: " << m_numOfCommands << '\n'
			   << "Enter command ('help' for the list of available commands): ";

		try 
		{
			getInput();
		}
		catch (...) 
		{
			std::cerr << "command failed" << '\n';
			continue;
		}

	} while (m_running);
}

void SetCalculator::eval()
{
	if (auto index = readOperationIndex(); index)
	{
		const auto& operation = m_operations[*index];
		auto inputs = std::vector<Set>();

		for (auto i = 0; i < operation->inputCount(); ++i)
		{
			try 
			{
				inputs.push_back(Set(m_dataInput.values));
			}
			catch (std::invalid_argument& e) 
			{
				std::cout << e.what();
				throw;
			}
		}
		operation->print(m_ostr, inputs);
		m_ostr << " = " << operation->compute(inputs) << '\n';
	}
}

void SetCalculator::del()
{
	if (auto i = readOperationIndex(); i)
	{
		m_operations.erase(m_operations.begin() + *i);
	}
}

void SetCalculator::resize()
{
	readMaxCommands("");
}

void SetCalculator::getInput()
{
	readData();
	const auto action = readAction();
	runAction(action);
}

void SetCalculator::readMaxCommands(std::string strMsg) try
{
	std::string input;
	int max;

	m_ostr << strMsg;

	if (m_dataInput.values.str() == "" || strMsg != "")
		*m_input >> input;
	else
		m_dataInput.values >> input;

	max = std::stoi(input);

	if (max < 3 || max > 100)
		throw std::out_of_range("Out of range. Please try again\n");

	if (m_numOfCommands > max)
		resizeOptions(max);
	else
		m_numOfCommands = max;

}
catch (const std::out_of_range& e)
{
	std::cerr << e.what() << '\n';
	if (!m_numOfCommands)
		readMaxCommands("Enter max number of commands: ");
}
catch (const std::exception& e) 
{
	std::cerr << "Max number of commands should be a number. Please try again\n\n";
	if (!m_numOfCommands)
		readMaxCommands("Enter max number of commands: ");
}

void SetCalculator::resizeOptions(int newMax) try
{
	int selection;

	m_ostr << "You typed size smaller then current.\n"
		<< "To cancel resize command - typed 1\n"
		<< "To resize and the program delete some commands - typed 2\n\n";

	std::cin >> selection;

	if (!(selection == 1 || selection == 2)) throw std::out_of_range("You need to typed 1 or 2");
	if (selection == 2)
	{
		for (int i = m_operations.size() - 1; i >= newMax; --i)
			m_operations.erase(m_operations.begin() + *std::optional<int>(i));

		m_numOfCommands = newMax;
	}
}
catch (const std::out_of_range& e)
{
	std::cerr << e.what() << '\n';
}
catch (const std::exception& e) 
{
	std::cerr << "Should be a number.";
}

void SetCalculator::help()
{
	m_ostr << "The available commands are:\n";
	for (const auto& action : m_actions)
	{
		m_ostr << "* " << action.command << action.description << '\n';
	}
	m_ostr << '\n';
}

void SetCalculator::exit()
{
	m_ostr << "Goodbye!\n";
	m_running = false;
}

void SetCalculator::printOperations() const
{
	m_ostr << "List of available set operations:\n";
	for (decltype(m_operations.size()) i = 0; i < m_operations.size(); ++i)
	{
		m_ostr << i << ".\t";
		auto gen = NameGenerator();
		m_operations[i]->print(m_ostr, gen);
		m_ostr << '\n';
	}
	m_ostr << '\n';
}

std::optional<int> SetCalculator::readOperationIndex()
{
	auto i = readNumber<int>();

	if (i >= m_operations.size() || i < 0)
		throw std::out_of_range("You asked for unexist operation. Please try again");

	return i;
}

void SetCalculator::read()
{
	std::string filePath;
	m_isReadngFromFile = true;
	m_dataInput.values >> filePath;

	try 
	{
		m_opFile.open(filePath);

		if (!m_opFile)
			throw std::system_error(errno, std::system_category(), "failed to open file\n");
	}
	catch (std::system_error& e)
	{
		m_ostr << e.what();

		run();
	}

	m_input = &m_opFile;
	readFromFile();
	m_opFile.close();
	m_input = &m_istr;
	m_isReadngFromFile = false;
}
//
void SetCalculator::readFromFile()
{
	while (!m_opFile.fail() && m_running)
	{
		m_ostr << '\n';
		printOperations();
		m_ostr << "Max commands the program can save: " << m_numOfCommands << '\n'
			<< "Enter command ('help' for the list of available commands): ";
		try {
			getInput();
		}
		//catching all problem , and checkig if the file is ended or its a problem within the file 
		catch (...) { 
			if (m_opFile.fail())
				std::cout << "\n file has ended , whoud you like to exit ? ";
			else
				m_ostr << "The operation  in the file failed.\n"
				"would you like to stop read from the file? ";
			yesOrNo();
		}
	}
}

//checking for yes or no from user regarding the file system
void SetCalculator::yesOrNo() try
{  	
	std::string choice;
	std::cin >> choice;
	if (choice == "yes")
	{
		if (m_opFile.fail())
			exit();
	}
	else if (choice == "no") 
	{
		return;
	}
	else
		throw std::invalid_argument("plese enter yes or no\n");
}
catch (std::invalid_argument& e)
{
	std::cout << e.what();
	yesOrNo();
}

//reading data from user / file (we dont support read in read)
void SetCalculator::readData()
{
	m_dataInput.command.clear();
	m_dataInput.values.clear();
	m_dataInput.values.str(std::string());
	*m_input >> m_dataInput.command;
	std::string line;
	std::getline(*m_input, line);
	m_dataInput.values << line;
}

SetCalculator::Action SetCalculator::readAction() const
{
	auto action = m_dataInput.command;

	const auto i = std::ranges::find(m_actions, action, &ActionDetails::command);

	if (i == m_actions.end()) throw std::out_of_range("Command not exist");

	if (i != m_actions.end())
	{
		if (m_isReadngFromFile)
			std::cout << action << std::endl;
		
		return i->action;
	}
}

void SetCalculator::runAction(Action action)
{
	switch (action)
	{
		case Action::Eval:			eval();                     break;
		case Action::Union:			binaryFunc<Union>();        break;
		case Action::Intersection:	binaryFunc<Intersection>(); break;
		case Action::Difference:	binaryFunc<Difference>();   break;
		case Action::Product:		binaryFunc<Product>();      break;
		case Action::Comp:			binaryFunc<Comp>();         break;
		case Action::Resize:		resize();                   break;
		case Action::Read:			read();                     break;
		case Action::Del:			del();                      break;
		case Action::Help:			help();                     break;
		case Action::Exit:			exit();                     break;
	}
}

SetCalculator::ActionMap SetCalculator::createActions()
{
	return ActionMap
	{
		{
			"eval",
			"(uate) num ... - compute the result of function #num on the "
			"following set(s); each set is prefixed with the count of numbers to"
			" read",
			Action::Eval
		},
		{
			"uni",
			"(on) num1 num2 - Creates an operation that is the union of "
			"operation #num1 and operation #num2",
			Action::Union
		},
		{
			"inter",
			"(section) num1 num2 - Creates an operation that is the "
			"intersection of operation #num1 and operation #num2",
			Action::Intersection
		},
		{
			"diff",
			"(erence) num1 num2 - Creates an operation that is the "
			"difference of operation #num1 and operation #num2",
			Action::Difference
		},
		{
			"prod",
			"(uct) num1 num2 - Creates an operation that returns the product of"
			" the items from the results of operation #num1 and operation #num2",
			Action::Product
		},

		{
			"comp",
			"(osite) num1 num2 - creates an operation that is the composition "
			"of operation #num1 and operation #num2",
			Action::Comp
		},
		{
			"del",
			"(ete) num - delete operation #num from the operation list",
			Action::Del
		},
		{
			"resize",
			" num - resize max commands the program can save",
			Action::Resize
		},
		  {
			"read",
			" path to file - read commands from file and executes them",
			Action::Read
		},
		{
			"help",
			" - print this command list",
			Action::Help
		},
		{
			"exit",
			" - exit the program",
			Action::Exit
		}
	};
}

SetCalculator::OperationList SetCalculator::createOperations()
{
	return OperationList {
		std::make_shared<Union>(std::make_shared<Identity>(), std::make_shared<Identity>()),
		std::make_shared<Intersection>(std::make_shared<Identity>(), std::make_shared<Identity>()),
		std::make_shared<Difference>(std::make_shared<Identity>(), std::make_shared<Identity>())
	};
}
