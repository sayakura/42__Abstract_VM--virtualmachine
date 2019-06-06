#ifndef OPERAND_HPP
#define OPERAND_HPP
#include <iostream>
#include <stdint.h>
#include <exception>
#include <vector>
#include <functional>
#include <math.h>
#include "IOperand.hpp"
#include "Token.hpp"
#include <sstream>
#include "Factory.hpp"

enum eOP
{
    O_ADD,
    O_SUB,
    O_MUL,
    O_DIV,
    O_MOD,
}; 

static std::vector<std::function<double(double, double)>> _cal = {
    [](double a, double b){ return a + b;},
    [](double a, double b){ return a - b;},
    [](double a, double b){ return a * b;},
    [](double a, double b){ return a / b;},
    [](double a, double b){ return std::fmod(a,b);}};

template <typename T>
class Operand: public IOperand
{
    private:
        const T         _val;
        eOperandType    _type;
        std::string     _str;
        Operand<T>();
    public:
        Operand<T>(T val, eOperandType type): _val(val), _type(type), _str(std::to_string(val)){} ;
        ~Operand(void) {};
        uint64_t getVal(void) const{
            return static_cast<uint64_t>(_val);
        }
        eToken_type getTokenType(void) {
            return tOperand; }
        int getPrecision(void) const {
            return _type; }
        eOperandType getType(void) const {
            return _type; }
        IOperand const * oPerandFactory(IOperand const & rhs, eOP type) const {
            std::istringstream  ss(rhs.toString());
            double              num;
            eOperandType        prec = static_cast<eOperandType>(std::max(getPrecision(), rhs.getPrecision()));
            static Factory      _factory;

            ss >> num;
            auto ret = _cal[type](_val, num);
            if (type == O_DIV)
                throw std::runtime_error("division by zero");
            else if (type == O_MOD)
                throw std::runtime_error("modulo by zero");
            return _factory.createOperand(prec, std::to_string(ret));
        }
        IOperand const * operator+( IOperand const & rhs ) const {
            return oPerandFactory(rhs, O_ADD); }
        IOperand const * operator-( IOperand const & rhs ) const {
            return oPerandFactory(rhs, O_SUB); }
        IOperand const * operator*( IOperand const & rhs ) const {
            return oPerandFactory(rhs, O_MUL); }
        IOperand const * operator/( IOperand const & rhs ) const {
            return oPerandFactory(rhs, O_DIV); }
        IOperand const * operator%( IOperand const & rhs ) const {
            return oPerandFactory(rhs, O_MOD); }
        std::string const & toString( void ) const {
		    return _str;}; 
};

#endif