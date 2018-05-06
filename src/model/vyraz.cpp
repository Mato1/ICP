/**
 * @brief
 * Implementacia hlavickoveho suboru vyraz.h.
 * Obsahuje definicie niektorych
 * metod pre triedu Vyraz.
 *
 * @file vyraz.cpp
 * @author Libor Moravcik, Matej Mrazik
 * @date 2018-04-28
 */
#include "model/vyraz.h"
#include <stack>
#include <iostream>
#include <math.h>
#include <algorithm>

namespace icp
{
namespace model
{

Vyraz::Vyraz(std::string expression, std::string premenna)
    : infix(expression), premenna(premenna)
{
    postfix = parse();
}

int Vyraz::eval()
{
    std::stack<double> operands;
    double vysledok;

    for (char &c : postfix)
    {
        if (isalpha(c))
        {
            double * d =  input_port->get_data_type().get(std::string(1, c));

            if (d != nullptr)
            {
                operands.push(*d);
            }
            else
            {
                return -1;
            }
        }
        else
        {
            double second_op = operands.top();
            operands.pop();
            double first_op = operands.top();
            operands.pop();
            double result;

            if (c == '+')
            {
                result = first_op + second_op;
            }

            if (c == '-')
            {
                result = first_op - second_op;
            }

            if (c == '*')
            {
                result = first_op * second_op;
            }

            if (c == '/')
            {
                result = first_op / second_op;
            }

            if (c == '^')
            {
                result = pow(first_op, second_op);
            }

            operands.push(result);

        }
    }

    vysledok = operands.top();
    output_port->get_data_type().set(premenna, vysledok);

    return 0;
}

std::string Vyraz::parse()
{
    std::string result;
    std::stack<char> st;

    for (char &c : infix)
    {
        if (isalpha(c) || isdigit(c))
        {
            result += c;
        }
        else if (c == '(')
        {
            st.push(c);
        }
        else if (c == ')')
        {
            while (!st.empty())
            {
                char ctop = st.top();
                st.pop();

                if (ctop == '(')
                {
                    break;
                }

                result += ctop;

            }
        }
        else
        {
            while (!st.empty() && precedence(c) <= precedence(st.top()))
            {
                char ctop = st.top();
                st.pop();
                result += ctop;
            }

            st.push(c);
        }
    }

    while (!st.empty())
    {
        result += st.top();
        st.pop();
    }

    return result;
}

int Vyraz::precedence(char c)
{
    switch (c)
    {
    case '+':
    case '-':
        return  1;

    case '*':
    case '/':
        return  2;

    case '^':
        return  3;

    default:
        return -1;
    }

    return -1;
}

}
}