#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <ctime>
#include <fstream>

using namespace std;

string databaseFile = "students.txt";

struct Student {
	int id;
	string name;
	string sex;
	int groupNumber;
	int groupPosition;
	string itExamMark;
	string aigExamMark;
	string mathanExamMark;
	string historyPassMark;
	string engPassMark;
	string codePassMark;
	string codeCourseMark;
};

void input(Student &currentStudent);
void outputAll(Student& currentStudent);
void menu(Student* database);
int ans();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
    
	int studentQuantity;

	ofstream fout;
	ifstream fin;
	fstream file(databaseFile);
	if (!file.is_open() || file.peek() == EOF) {	// Если файл отсутствует или он пустой, то создать новый файл
		fout.open(databaseFile);
		studentQuantity = 0;
		fout << "Количество студентов в базе данных:\n" << studentQuantity << endl;
	}
	else {
		// TODO: прочесть количество студентов из файла
	}
	system("pause");

	Student *database = new Student [studentQuantity];

	menu(database);

	delete[] database;

	system("pause");
}

void input(Student& currentStudent) {
	currentStudent.id = rand();
	cout << "Введите ФИО студента: "; cin >> currentStudent.name;
	cout << "Введите пол студента: "; cin >> currentStudent.sex;
	cout << "Введите номер группы студента: "; cin >> currentStudent.groupNumber;
	cout << "Введите место в группе студента: "; cin >> currentStudent.groupPosition;
	cout << "Введите оценку за экзамен по Вв в ИТ: "; cin >> currentStudent.itExamMark;
	cout << "Введите оценку за экзамен по АиГу: "; cin >> currentStudent.aigExamMark;
	cout << "Введите оценку за экзамен по МатАнанализу: "; cin >> currentStudent.mathanExamMark;
	cout << "Введите оценку за дифзачет по истории: "; cin >> currentStudent.historyPassMark;
	cout << "Введите оценку за дифзачет по английскому: "; cin >> currentStudent.engPassMark;
	cout << "Введите оценку за дифзачет по программированию: "; cin >> currentStudent.codePassMark;
	cout << "Введите оценку за курсовую по программированию: "; cin >> currentStudent.codeCourseMark;
}

void outputAll(Student& currentStudent) {
	cout << "ID студента: " << currentStudent.id << endl;
	cout << "ФИО студента: " << currentStudent.name << endl;
	cout << "Пол студента: " << currentStudent.sex << endl;
	cout << "Номер группы студента: " << currentStudent.groupNumber << endl;
	cout << "Место в группе студента: " << currentStudent.groupPosition << endl;
	cout << "Оценка за экзамен по Вв в ИТ: " << currentStudent.itExamMark << endl;
	cout << "Оценка за экзамен по АиГу: " << currentStudent.aigExamMark << endl;
	cout << "Оценка за экзамен по МатАнанализу: " << currentStudent.mathanExamMark << endl;
	cout << "Оценка за дифзачет по истории: " << currentStudent.historyPassMark << endl;
	cout << "Оценка за дифзачет по английскому: " << currentStudent.engPassMark << endl;
	cout << "Оценка за дифзачет по программированию: " << currentStudent.codePassMark << endl;
	cout << "Оценка за курсовую по программированию: " << currentStudent.codeCourseMark << endl;
}

void newStudent(Student* database) {
	input(database[0]);
}
// МЕНЮ
// Отображает меню в консоли
int ans() {
	int choice = 0;
	int options = 5;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl;

		if (choice == 0) cout << "-> Новый студент" << endl;
		else  cout << "   Новый студент" << endl;

		if (choice == 1) cout << "-> Редактирование студента" << endl;
		else  cout << "   Редактирование студента" << endl;

		if (choice == 2) cout << "-> Вывод всех студентов" << endl;
		else  cout << "   Вывод всех студентов" << endl;

		if (choice == 3) cout << "-> Вывод студентов группы N" << endl;
		else  cout << "   Вывод студентов группы N" << endl;

		if (choice == 4) cout << "-> ТОП студентов" << endl;
		else  cout << "   ТОП студентов" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119)
		{
			choice--;
		}
		if (ch == 115)
		{
			choice++;
		}
	}
	system("cls");
	return choice;
}

// Выбирает действие в зависимости от выбора пользователя
void menu(Student *database) {
	int answer;
	do {
		answer = ans();
		switch (answer)
		{
		case 0:
			newStudent(database);
			outputAll(database[0]);
			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:
			break;
		}
	} while (answer == -1);
}