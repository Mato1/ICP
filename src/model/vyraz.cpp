/**
 * @brief 
 * Implementacia hlavickoveho suboru vypocet.h.
 * Obsahuje definicie niektorych
 * metod pre triedu Vyraz.
 * 
 * @file vypocet.cpp
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
        Vyraz::Vyraz(std::string expression)
            : infix(expression)
        {
            postfix = parse();
        }

        Vyraz::Vyraz(std::string expression, std::string premenna)
            : infix(expression), premenna(premenna)
        {
            postfix = parse();
        }

        double Vyraz::eval()
        {  
            std::stack<double> operands;

            fill_expected_type();

            for (char &c : postfix)
            {
                if (isalpha(c))
                {
                    operands.push(*expected_type.data[std::to_string(c)]);
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
            return operands.top();
        }

        void Vyraz::fill_expected_type()
        {
           for (auto it = expected_type.data.begin(); it != expected_type.data.end(); it++)
           {
               for(auto port : input_ports)
               {
                   if (port->get_data_type().has_name(it->first))
                   {
                       it->second = port->get_data_type().get(it->first);
                       break;
                   }
               }
           }
        }

        void Vyraz::assign_input_port(Port * port)
        {
            input_ports.push_back(port);
        }

        void Vyraz::assign_output_port(Port * port)
        {
            output_ports.push_back(port);
        }

        std::string Vyraz::parse()
        {
            std::string result;
            std::stack<char> st;

            for(char& c : infix)
            {
                if (isalpha(c) || isdigit(c))
                {
                    //Operand
                    if (isalpha(c))
                    {
                        expected_type.data.insert(std::pair<std::string, double *>(std::to_string(c), nullptr));
                    }

                    result += c;
                } 
                else if (c == '(')
                {
                    st.push(c);
                }
                else if (c == ')')
                {
                    while(!st.empty())
                    {
                        char ctop = st.top();
                        st.pop();
                        if (ctop == '(') { break; }
                        result += ctop;
                        
                    }
                }
                else 
                {
                    while(!st.empty() && precedence(c) <= precedence(st.top()))
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
                case '+': case '-':                      return  1;
                case '*': case '/':                      return  2;
                case '^':                                return  3;
                default:                                 return -1;
            }

            return -1;
        }
    }
}