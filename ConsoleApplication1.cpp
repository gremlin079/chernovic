#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


class Expression
{
protected:
	std::string rawExpression;
	std::string normolizedExpression;
	std::string type;

public:
	Expression(std::string& rawExpression, std::string& type)
	{
		this->rawExpression = rawExpression;
		this->type = type;
	}

	std::string getRawExpression()
	{
		return rawExpression;
	}
	
	std::string getNormolizedExpression()
	{
		return normolizedExpression;
	}

	std::string getType()
	{
		return type;
	}

	void setNormolizedExpression(std::string & normolizedExpression)
	{
		this->normolizedExpression = normolizedExpression;
	}

	void normalaize();
};

class ArithmeticExpressionProcessor
{
	Expression& expr;

public:
	ArithmeticExpressionProcessor(Expression &expr) : expr(expr)
	{
	}
	
	void convertToStandartForm()
	{
		int colvo_x = 0;
		std::vector<int> coefficents;
		int i = 0;
		std::vector<int> poz_znak;
		std::vector<int> poz_scobka;
		std::vector<int> x_coeff {1};


		for (char x : expr.getRawExpression())
		{
			if (x == 'x')
			{
				colvo_x += 1;
			}

			if (x == '+' or x == '-')
			{
				poz_znak.push_back(i);
			}
			
			if (x == ')')
			{
				poz_scobka.push_back(i);
			}
			i += 1;
		}
		
		for (int i = 0; i < colvo_x; i++)
		{
			std::string Help_string;
			for (int k = poz_znak[i]; k < poz_scobka[i]; k++)
			{
				Help_string += expr.getRawExpression()[k];
			}
			
			coefficents.push_back(std::stoi(Help_string));
		}

		for (int i = 1; i < colvo_x; i++)
		{
			int n = coefficents.size();
			std::vector<bool> mask(n, false);
			int total_sum = 0;

			std::fill(mask.end() - i, mask.end(), true);

			do {
				int product = 1;
				for (int k = 0; k < n; ++k)
				{
					if (mask[k])
					{
						product *= coefficents[k];
					}
				}
				
				total_sum += product;
			} while (std::next_permutation(mask.begin(), mask.end()));
			
			x_coeff.push_back(total_sum);
		}
		std::string return_this;
		for (int i = 0; i < colvo_x; i++)
		{
			if (i == 0)
			{
				return_this += "x^" + std::to_string(colvo_x);
			}
			else
			{
				if (x_coeff[i] >= 0)
				{
					return_this += " + " + std::to_string(x_coeff[i]) + "x^" + std::to_string(colvo_x - i);
				}
				else
				{
					return_this += " - " + std::to_string(abs(x_coeff[i])) + "x^" + std::to_string(colvo_x - i);
				}
			}
		
		}
		
		int free_coef = 1;
		for (int el : coefficents)
		{
			free_coef *= el;
		}
		
		if (free_coef >= 0)
		{
			return_this += " + " + std::to_string(free_coef);
		}
		else
		{
			return_this += " - " + std::to_string(abs(free_coef));
		}

		expr.setNormolizedExpression(return_this);
	
}

	//void addConstant()
	//{
	//	
	//}
};

//class LogicalExpressionProcessor : public Expression
//{
//public:
//	LogicalExpressionProcessor(std::string& rawExpression, std::string& type) : Expression(rawExpression, type)
//	{
//	}
//	
//	void convertToStandartForm()
//	{
//	}
//
//	void addConstant()
//	{
//	}
//};

void Expression::normalaize() {
    if (this->type == "Арифметическое") {
        ArithmeticExpressionProcessor processor(*this);
        processor.convertToStandartForm();
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    std::string a = "(x+5)(x-4)(x+7)(x-8)(x+9)";
    std::string t = "Арифметическое";
    Expression c(a, t);
    c.normalaize();
    std::cout << "Стандартный вид: " << c.getNormolizedExpression() << std::endl;

    return 0;
}
