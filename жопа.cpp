#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <thread>


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

	void setNormolizedExpression(std::string& normolizedExpression)
	{
		this->normolizedExpression = normolizedExpression;
	}

	void normalaize();

	void addConst(int);
};

class ArithmeticExpressionProcessor
{
	Expression& expr;

public:
	ArithmeticExpressionProcessor(Expression& expr) : expr(expr)
	{
	}

	void convertToStandartForm()
	{
		int colvo_x = 0;
		std::vector<int> coefficents;
		int i = 0;
		std::vector<int> poz_znak;
		std::vector<int> poz_scobka;
		std::vector<int> x_coeff{ 1 };


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

	void addConst(int constant)
	{
		std::string now_expression = expr.getNormolizedExpression();
		std::string changeme;
		std::vector<int> poz_znak;
		std::string help_str;
		bool flag_polozh = false;
		for (int i = 0; i < now_expression.size(); i++)
		{
			if (now_expression[i] == '+' or now_expression[i] == '-')
			{
				poz_znak.push_back(i);

			}
		}
		
		if (now_expression[poz_znak[poz_znak.size() - 1]] == '+')
		{
			flag_polozh = true;
		}
		
		for (int i = poz_znak[poz_znak.size() - 1]; i < now_expression.size(); i++)
		{
			if (now_expression[i] != ' ')
			{
				help_str += now_expression[i];
			}
		}
	
		for (int i = 0; i < poz_znak[poz_znak.size() - 1]; i++)
		{
			changeme += now_expression[i];
		}

		if ((std::stoi(help_str) + constant) >= 0)
		{
			changeme += "+ " + std::to_string(std::stoi(help_str) + constant);
		}
		else
		{
			changeme += "- " + std::to_string(abs(std::stoi(help_str) + constant));
		}
	
		expr.setNormolizedExpression(changeme);
	}
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

void Expression::addConst(int constanta)
{
	if (this->type == "Арифметическое") 
	{
		ArithmeticExpressionProcessor processor(*this);
		processor.addConst(constanta);
	}

}

class ExpressionEditor {
private:
	std::string filename;
	Expression expr;
	std::vector<int> constants = {0};

public:
    ExpressionEditor(const std::string& filename) : filename(filename), expr(*(new std::string("ю")), *(new std::string("й"))) {}

	void loadFromFile() {
		std::ifstream file(filename);
		if (!file) {
			std::cerr << "Ошибка: не удалось открыть файл!" << std::endl;
			exit(1);
		}

		std::string type, expression;
		std::getline(file, type);
		std::getline(file, expression);

		expr = Expression(expression, type); // Обновляем после загрузки данных!
		expr.normalaize();
		file.close();
	}

	void showMenu() {
		while (true) {
			system("cls");
			std::cout << "Окно редактирования:\n\n";
			std::cout << "1) Показать введенные данные\n";
			std::cout << "2) Добавить константу\n";
			std::cout << "3) Показать результат\n";
			std::cout << "4) Выход\n";
			std::cout << "Выбор: ";

			int choice;
			std::cin >> choice;
			std::cin.ignore(); // Фикс ввода
			handleUserChoice(choice);
		}
	}

	void handleUserChoice(int choice) {
		switch (choice) {
		case 1:
			displayExpression();
			break;
		case 2:
			addConstant();
			break;
		case 3:
			showResult();
			break;
		case 4:
			exit(0);
		default:
			std::cout << "Ошибка: неправильный ввод!" << std::endl;
			break;
		}
	}

	void displayExpression() {
		system("cls");
		std::cout << "Тип выражения: " << expr.getType() << std::endl;
		std::cout << "Выражение: " << expr.getRawExpression() << std::endl;
		std::cout << "Нажмите любую клавишу для выхода...";
		std::cin.get();
	}

	void addConstant() {
		system("cls");
		std::cout << "Введите константу: ";
		int c;
		std::cin >> c;
		std::cin.ignore(); // Фикс ввода
		constants.push_back(c);
		expr.addConst(c); // Применяем сразу!

		std::cout << "Константа добавлена. Нажмите любую клавишу...";
		std::cin.get();
	}

	void showResult() {
		system("cls");
		std::cout << "Обработка...\n";
		std::this_thread::sleep_for(std::chrono::seconds(2));

		expr.normalaize();
		std::cout << "Упрощенное выражение: " << expr.getNormolizedExpression() << std::endl;

		for (int c : constants) {
			expr.addConst(c);
		}
		std::cout << "С добавленными константами: " << expr.getNormolizedExpression() << std::endl;

		std::cout << "Нажмите любую клавишу для выхода...";
		std::cin.get();
	}
};


int main() {
	setlocale(LC_ALL, "Russian");

	std::cout << "Введите имя файла: ";
	std::string filename;
	std::cin >> filename;

	ExpressionEditor editor(filename);
	editor.loadFromFile();
	editor.showMenu();

	return 0;
}


//int main() {
//	setlocale(LC_ALL, "Russian");
//	for (int i = 0; i <= 100; i += 1) {
//		std::cout << "\rЗагрузка: " << i << "%";
//		std::cout.flush();
//		std::this_thread::sleep_for(std::chrono::milliseconds(15));
//	}
//	system("cls");
//	std::cout << "-Редактор выражений-\n\n";
//	std::cout << "Введите имя файла для работы(с расширением):" << std::endl;
//	
//	short flag1;
//	std::string filename;
//	std::string type;
//	std::string expression;
//	
//	std::cin >> filename;
//	
//	std::ifstream ss(filename);
//	std::string line;
//
//	int i = 0;
//	std::vector<long long> constants = {0};
//	
//	while (std::getline(ss, line))
//	{
//		if (i == 0)
//		{
//			type = line;
//		}
//		else if (i == 1)
//		{
//			expression = line;
//		}
//		
//		i += 1;
//	}
//	
//	std::cout << "\n\n";
//	std::cout << "Прочитанные данные" << std::endl;
//	std::cout << "Тип выражения: " << type << std::endl;
//	std::cout << "Выражение: " << expression << std::endl;
//	std::cout << "\n\n\nВведите '1', если желаете перейти к редактированию, и '0', если выйти: ...";
//	std::cin >> flag1;
//	if (flag1 == 1)
//	{
//		while (true) {
//
//		
//			system("cls");
//			std::cout << "Окно редактирования:\n\n\n\n";
//			std::cout << "Выберете один из пунктов и введите его цифру:\n\n";
//			std::cout << "1) Показать введенные дынные" << std::endl;
//			std::cout << "2) Добавить константу" << std::endl;
//			std::cout << "3) Перейти к результату упрощения выражения" << std::endl;
//			std::cout << "4) Окончание работы программы" << std::endl;
//				
//			short flag2;
//			std::cout << "Ввод: ";
//			std::cin >> flag2;
//
//			if (flag2 == 1)
//			{
//				std::string flag3;
//				system("cls");
//				std::cout << "Тип выражения: " << type << std::endl;
//				std::cout << "Выражение: " << expression << std::endl;
//				std::cout << "\n\nДля выхода введите любой символ: ";
//				std::cin >> flag3;
//			}
//
//			if (flag2 == 2)
//			{
//				long long c;
//				std::string flag4;
//				system("cls");
//				std::cout << "Введите константу:\n";
//				std::cout << "Ввод: ";
//				std::cin >> c;
//				std::cout << "\n\nПринято, введите любой символ для выхода: ";
//				std::cin >> flag4;
//				constants.push_back(c);
//			}
//		
//			if (flag2 == 3)
//			{
//				for (int i = 0; i <= 100; i += 1) {
//					std::cout << "\rЗагрузка: " << i << "%";
//					std::cout.flush();
//					if (i == 98 or i == 99)
//					{
//						std::this_thread::sleep_for(std::chrono::milliseconds(50));
//
//					}
//					else {
//
//						std::this_thread::sleep_for(std::chrono::milliseconds(10));
//					}
//					
//				}
//				short flag5;
//				system("cls");
//				std::cout << "Упрощенное выражение без добавления констант:\n";
//				Expression result(expression, type);
//				result.normalaize();
//				std::cout << result.getNormolizedExpression();
//				std::cout << "\n\nУпрощенное выражение с добавлением констант\n";
//				for (int i = 0; i < constants.size(); i++)
//				{
//					result.addConst(constants[i]);
//				}
//				std::cout << result.getNormolizedExpression();
//
//				std::cout << "\n\nДля прекращения работы программы введите '0', для возврата '1'\n";
//				std::cout << "Ввод: ";
//				std::cin >> flag5;
//				if (flag5 == 0)
//				{
//					break;
//				}
//			}
//		
//			if (flag2 == 4)
//			{
//				break;
//			}
//		}
//	}
//	else if (flag1 == 0)
//	{
//		std::cout << "\n\n";
//		std::cout << "Досвидания!\n";
//		return 0;
//	}
//	else
//	{
//		std::cout << "\n\n";
//		std::cout << "Неправильный ввод, аварийное завершение работы!!!";
//		return 0;
//	}
//
//
//	/*std::string a = "(x+5)(x-4)(x+7)(x-8)(x+9)";
//	std::string t = "Арифметическое";
//	Expression c(a, t);
//	c.normalaize();
//	c.addConst(20000);
//	std::cout << "Стандартный вид: " << c.getNormolizedExpression() << std::endl;*/
//
//	return 0;
//}
