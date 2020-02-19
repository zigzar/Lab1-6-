//TODO чтение даты
#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <ctime>
#include <fstream>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

string outputFile = "students.txt";
string databaseFile = "db.txt"; // Технический файл с неотформатированными данными

struct Student {
	string creationTime;
	int id;
	int groupNumber;
	int groupPosition;
	string studyForm;
	string name;
	string sex;
	string itExamMark;
	string aigExamMark;
	string mathanExamMark;
	string historyPassMark;
	string engPassMark;
	string codePassMark;
	string codeCourseMark;
};	 

Student* database;
int studentQuantity;

void inputStudent(Student &currentStudent);
void outputAll(Student& currentStudent);

void newFile();
void transaction();
void loadDatabase();

void menu();
void menuLocked();
int ans();
int ansLocked();

int main()	
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	SetConsoleTextAttribute(h, (WORD)((0 << 4) | 15));

	loadDatabase();
	if (studentQuantity == 0) menuLocked();
	else menu();
}

Student readStudents() {
	Student currentStudent;
	ifstream fin;
	string line;
	fin.open(databaseFile);
	getline(fin, line);
	getline(fin, currentStudent.creationTime);
	getline(fin, line);
	currentStudent.id = stoi(line);
	getline(fin, currentStudent.name);
	getline(fin, currentStudent.sex);
	getline(fin, line);
	currentStudent.groupNumber = stoi(line);
	getline(fin, line);
	currentStudent.groupPosition = stoi(line);
	getline(fin, currentStudent.studyForm);
	getline(fin, currentStudent.itExamMark);
	getline(fin, currentStudent.aigExamMark);
	getline(fin, currentStudent.mathanExamMark);
	getline(fin, currentStudent.historyPassMark);
	getline(fin, currentStudent.engPassMark);
	getline(fin, currentStudent.codePassMark);
	getline(fin, currentStudent.codeCourseMark);
	fin.close();
	return currentStudent;
}

void loadDatabase() {
	string line;
	ifstream fin;
	fin.open(databaseFile);
	getline(fin, line);
	fin.close();
	try
	{
		studentQuantity = stoi(line);
		database = new Student[studentQuantity];
		for (int i = 0; i < studentQuantity; i++)
		{
			database[i] = readStudents();
			outputAll(database[i]);
		};
	}
	catch (const exception&)
	{
		cout << "Ошибка чтения файла. Файл поврежден или изменен. Будет создан новый файл." << endl;
		studentQuantity = 0;
		newFile();
		cout << "\nСоздан новый файл базы данных!\n";
		Sleep(2000);
	}
}

void pushBack(const Student& value) { // Увеличить массив студентов и добавить добавить в него нового студента
	Student* newArray = new Student[studentQuantity+1];

	for (int i = 0; i < studentQuantity; i++)
	{
		newArray[i] = database[i];
	}

	delete[] database;

	newArray[studentQuantity++] = value;
	
	database = newArray;
}

string getCurrentDate() {
	struct tm newtime;
	__time64_t long_time;
	char buffer[25];

	_time64(&long_time); // Получить время как 64-битный integer
	_localtime64_s(&newtime, &long_time); // Конвертировать в структуру tm (локальное время)
	strftime(buffer, 25, "%d.%m.%Y %X" , &newtime); // Перевод в ACSII
	string time = buffer;
	return time;
}

void inputStudent(Student& currentStudent) {
	currentStudent.creationTime = getCurrentDate();
	currentStudent.id = rand();
	cout << "Введите ФИО студента: "; cin.ignore(cin.rdbuf()->in_avail()); getline(cin, currentStudent.name);
	system("CLS");
	cout << "Введите пол студента: "; cin >> currentStudent.sex;
	system("CLS");
	cout << "Введите номер группы студента: "; cin >> currentStudent.groupNumber;
	system("CLS");
	cout << "Введите место в группе студента: "; cin >> currentStudent.groupPosition;
	system("CLS");
	cout << "Выберите форму обучения: "; cin >> currentStudent.studyForm;
	system("CLS");
	cout << "Введите оценку за экзамен по Вв в ИТ: "; cin >> currentStudent.itExamMark;
	system("CLS");
	cout << "Введите оценку за экзамен по АиГу: "; cin >> currentStudent.aigExamMark;
	system("CLS");
	cout << "Введите оценку за экзамен по МатАнанализу: "; cin >> currentStudent.mathanExamMark;
	system("CLS");
	cout << "Введите оценку за дифзачет по истории: "; cin >> currentStudent.historyPassMark;
	system("CLS");
	cout << "Введите оценку за дифзачет по английскому: "; cin >> currentStudent.engPassMark;
	system("CLS");
	cout << "Введите оценку за дифзачет по программированию: "; cin >> currentStudent.codePassMark;
	system("CLS");
	cout << "Введите оценку за курсовую по программированию: "; cin >> currentStudent.codeCourseMark;
	system("CLS");
}

void outputAll(Student& currentStudent) { // Вывод информации о текущем студенте
	cout << "Время создания: " << currentStudent.creationTime << endl;
	cout << "ID студента: " << currentStudent.id << endl;
	cout << "ФИО студента: " << currentStudent.name << endl;
	cout << "Пол студента: " << currentStudent.sex << endl;
	cout << "Номер группы студента: " << currentStudent.groupNumber << endl;
	cout << "Место в группе студента: " << currentStudent.groupPosition << endl;
	cout << "Форма обучения: " << currentStudent.groupPosition << endl;
	cout << "Оценка за экзамен по Вв в ИТ: " << currentStudent.itExamMark << endl;
	cout << "Оценка за экзамен по АиГу: " << currentStudent.aigExamMark << endl;
	cout << "Оценка за экзамен по МатАнанализу: " << currentStudent.mathanExamMark << endl;
	cout << "Оценка за дифзачет по истории: " << currentStudent.historyPassMark << endl;
	cout << "Оценка за дифзачет по английскому: " << currentStudent.engPassMark << endl;
	cout << "Оценка за дифзачет по программированию: " << currentStudent.codePassMark << endl;
	cout << "Оценка за курсовую по программированию: " << currentStudent.codeCourseMark << endl;
}

void newFile() { // Создать новый файл базы данных с базовой разметкой и очистить файл структур
	ofstream fout;
	fout.open(outputFile);
	fout << "Количество студентов в базе данных:\n" << studentQuantity << endl;
	fout.close();
	fout.open(databaseFile);
	fout << studentQuantity << endl;
	fout.close();
}

void newStudent() {
	Student newStudent;
	inputStudent(newStudent);
	pushBack(newStudent);
	outputAll(database[studentQuantity-1]);
	system("pause");
	transaction();
}

void transaction() {
	newFile();
	ofstream fout;
	fout.open(outputFile, ofstream::app);
	for (int i = 0; i < studentQuantity; i++)
	{
		fout << "Время создания: " << database[i].creationTime << endl;
		fout << "ID студента: " << database[i].id << endl;
		fout << "ФИО студента: " << database[i].name << endl;
		fout << "Пол студента: " << database[i].sex << endl;
		fout << "Номер группы студента: " << database[i].groupNumber << endl;
		fout << "Место в группе студента: " << database[i].groupPosition << endl;
		fout << "Форма обучения: " << database[i].studyForm << endl;
		fout << "Оценка за экзамен по Вв в ИТ: " << database[i].itExamMark << endl;
		fout << "Оценка за экзамен по АиГу: " << database[i].aigExamMark << endl;
		fout << "Оценка за экзамен по МатАнанализу: " << database[i].mathanExamMark << endl;
		fout << "Оценка за дифзачет по истории: " << database[i].historyPassMark << endl;
		fout << "Оценка за дифзачет по английскому: " << database[i].engPassMark << endl;
		fout << "Оценка за дифзачет по программированию: " << database[i].codePassMark << endl;
		fout << "Оценка за курсовую по программированию: " << database[i].codeCourseMark << endl;
		fout << "_______________________________________________" << endl;
	}
	fout.close();
	fout.open(databaseFile, ofstream::app);
	for (int i = 0; i < studentQuantity; i++)
	{
		fout << database[i].creationTime << endl;
		fout << database[i].id << endl;
		fout << database[i].name << endl;
		fout << database[i].sex << endl;
		fout << database[i].groupNumber << endl;
		fout << database[i].groupPosition << endl;
		fout << database[i].studyForm << endl;
		fout << database[i].itExamMark << endl;
		fout << database[i].aigExamMark << endl;
		fout << database[i].mathanExamMark << endl;
		fout << database[i].historyPassMark << endl;
		fout << database[i].engPassMark << endl;
		fout << database[i].codePassMark << endl;
		fout << database[i].codeCourseMark << endl;
	};
	fout.close();
	cout << "Транзакция базы данных прошла успешно!\n";
	Sleep(1500);
}
// МЕНЮ
// Отображает меню в консоли
int ans() {
	int choice = 0;
	int options = 11;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

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

		if (choice == 5) cout << "-> Вывод количества парней и девушек" << endl;
		else  cout << "   Вывод количества парней и девушек" << endl;

		if (choice == 6) cout << "-> Вывод студентов, получающих стипендию" << endl;
		else  cout << "   Вывод студентов, получающих стипендию" << endl;

		if (choice == 7) cout << "-> Вывод студентов, не получающих стипендию" << endl;
		else  cout << "   Вывод студентов, не получающих стипендию" << endl;

		if (choice == 8) cout << "-> Вывод студентов по времени" << endl;
		else  cout << "   Вывод студентов по времени" << endl;

		if (choice == 9) cout << "-> Вывод студентов с k-м номеров в списке" << endl;
		else  cout << "   Вывод студентов с k-м номеров в списке" << endl;

		if (choice == 10) cout << "-> Выход" << endl;
		else  cout << "   Выход" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}

int ansLocked() {
	int choice = 0;
	int options = 2;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) cout << "-> Новый студент" << endl;
		else  cout << "   Новый студент" << endl;

		SetConsoleTextAttribute(h, (WORD)((0 << 4) | 8));

		cout << "   Редактирование студента" << endl;

		cout << "   Вывод всех студентов" << endl;

		cout << "   Вывод студентов группы N" << endl;

		cout << "   ТОП студентов" << endl;

		cout << "   Вывод количества парней и девушек" << endl;

		cout << "   Вывод студентов, получающих стипендию" << endl;

		cout << "   Вывод студентов, не получающих стипендию" << endl;

		cout << "   Вывод студентов по времени" << endl;

		cout << "   Вывод студентов с k-м номеров в списке" << endl;

		SetConsoleTextAttribute(h, (WORD)((0 << 4) | 15));

		if (choice == 1) cout << "-> Выход" << endl;
		else  cout << "   Выход" << endl;

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();
			if (ch == 80) choice++;
			if (ch == 72) choice--;
		}
		if (ch == 119) choice--;
		if (ch == 115) choice++;
		if (ch == 13) break;
	}
	system("cls");
	return choice;
}


// Выбирает действие в зависимости от выбора пользователя
void menu() {
	int answer;
	do {
		answer = ans();
		switch (answer)
		{
		case 0:
			newStudent();
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
		case 5:

			break;
		case 6:

			break;
		case 7:

			break;
		case 8:

			break;
		case 9:

			break;
		case 10:
			exit(0);
			break;
		}
	} while (answer == -1);
}

void menuLocked() {
	int answer;
	do {
		answer = ansLocked();
		switch (answer)
		{
		case 0:
			newStudent();
			outputAll(database[0]);
			main();
			break;
		case 10:
			exit(0);
			break;
		}
	} while (answer == -1);
}