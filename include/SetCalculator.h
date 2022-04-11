#pragma once

#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>
#include <iostream>
#include <sstream>

class Operation;

class SetCalculator
{
public:
    SetCalculator(std::istream& istr, std::ostream& ostr);
    void run();

private:
    void eval();
    void del();
    void resize();
    void help();
    void exit();

    void readMaxCommands(std::string);
    void resizeOptions(int);

    template <typename FuncType>
    void binaryFunc() try
    {

        if (auto f0 = readOperationIndex(), f1 = readOperationIndex(); f0 && f1)
        {
            if (m_operations.size() == m_numOfCommands) 
                throw std::exception("\nThe program can not save more commands");

            m_operations.push_back(std::make_shared<FuncType>(m_operations[*f0], m_operations[*f1]));
        }
    }    
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }    

    void printOperations() const;
    void readData();
    enum class Action
    {
        Invalid,
        Eval,
        Union,
        Intersection,
        Difference,
        Product,
        Comp,
        Del,
        Resize, 
        Read,
        Help,
        Exit,
    };

    struct ActionDetails
    {
        std::string command;
        std::string description;
        Action action;
    };
    struct Data {
        std::string command;
        std::stringstream values;
    };

    Data m_dataInput;
    using ActionMap = std::vector<ActionDetails>;
    using OperationList = std::vector<std::shared_ptr<Operation>>;

    const ActionMap m_actions;
    OperationList m_operations;
    bool m_running = true;
    std::istream& m_istr;
    std::ostream& m_ostr;
    void readFromFile();
    void read();
    std::optional<int> readOperationIndex() const;
    Action readAction() const;
    void runAction(Action action);
    bool m_isReadngFromFile=false;
    std::ifstream m_opFile;

    static ActionMap createActions();
    static OperationList createOperations();
    std::istream* m_input;
    int m_numOfCommands = 0;
};
