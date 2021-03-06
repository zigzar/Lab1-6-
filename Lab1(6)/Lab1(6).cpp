﻿#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

string outputFile = "students.txt"; // Файл вывода
string databaseFile = "db.txt"; 	// Технический файл с неотформатированными данными

struct Student {
	string creationTime;			// Время создания/изменения
	int id;							// ID студента в БД
	string name;					// ФИО студента
	bool sex;						// Пол
	int age;						// Возраст
	string wearSize;				// Размер одежды
	int rank;						// Разряд по волейболу
	int courseNumber;				// Номер курса
	int groupNumber;				// Номер группы
	int groupPosition;				// Место в группе
	string studyForm;				// Форма обучения
	int itExamMark;					// Оценка по Вв в ИТ (экз.)
	int aigExamMark;				// Оценка по АиГу (экз.)
	int mathanExamMark;				// Оценка по МатАну (экз.)
	int historyPassMark;			// Оценка по истории (диф. з.)
	int engPassMark;				// Оценка по Ин. Яз. (диф. з.)
	int codePassMark;				// Оценка по прогр. (диф. з.)
	int codeCourseMark;				// Оценка по прогр. (курсовая)
	int markSum;					// Сумма баллов по всем предметам
};	 

Student* database;					// Указатель на массив студентов
int studentQuantity;				// Количество студентов в БД

// ДОБАВЛЕНИЕ СТУДЕНТА //
void newStudent();								// Создать студента
void pushBack(const Student& value);			// Увеличить массив студентов и добавить в него нового студента
void inputStudent(Student &currentStudent);		// Ввести информацию о студенте
string getCurrentDate();						// Получить текущие дату и время
bool has2(Student& currentStudent);				// Проверить на наличие неуд оценок
int countMarkSum(Student& currentStudent);		// Просуммировать оценки студента

// ОПЕРАЦИИ С ФАЙЛАМИ //
void newFile();									// Создать новый файл базы данных с базовой разметкой и очистить файл структур
void transaction();								// Произвести транзикцию БД в технический файл и файл вывода
void loadDatabase();							// Загрузить БД из технического файла

// МЕНЮ. ГРАФИЧЕСКАЯ ЧАСТЬ //
int ans();										// Выбрать пункт в главном меню
int ansLocked();								// Выбрать пункт в главном меню при пустой БД
int markAns(string message);					// Выбрать пункт в меню выбора оценки
int sexAns();									// Выбрать пункт в меню выбора пола
int wearAns();									// Выбрать пункт в меню выбора размера одежды
int formAns();									// Выбрать пункт в меню выбора формы обучения
int editMenuAns(int i);							// Выбрать пункт в меню редактирования студента
int notGrantedAns();							// Выбрать пункт в меню вывода студентов без стипендии
int outputByDayAns();							// Выбрать пункт в меню вывода студентов по дате

// МЕНЮ. ЛОГИЧЕСКАЯ ЧАСТЬ //
void menu();									// Главное меню
void menuLocked();								// Главное меню при пустой БД
void markMenu(string message, int& mark);		// Меню выбора оценки
void sexMenu(bool& sex);						// Меню выбора пола
void wearMenu(string& size);					// Меню выбора размера одежды
void formMenu(string& size);					// Меню выбора формы обучения
int editMenu(int i);							// Меню редактирования студента
int notGrantedMenu();							// Меню вывода студентов без стипендии
int outputByDayMenu(string date);				// Меню вывода студентов по дате

// РЕДАКТИРОВАНИЕ СТУДЕНТА //
void chooseStudent();							// Выбор студента для редактирования
void editStudent(int id);						// Отредактировать студента
int getIndex(int id);							// Получить индекс студента в массиве БД по его ID
string getValue();								// Получает значение для редактируемого поля структуры
void deleteStudent(int index);					// Удалить студента из БД

// ВЫВОД ОБЩИЙ //
void outputAllFields(Student& currentStudent);	// Вывести все поля студента
string formatSex(bool& sex);					// Отформатировать пол
string formatMark(int& mark);					// Отформатировать оценку

// ВЫВОД ОТФИЛЬТРОВАННЫЙ //
int getGroupNumber();							// Получить номер группы
void outputByGroup();							// Вывести по номеру группы

bool comp(Student a, Student b);				// Компаратор для функции sort();
void outputTop();								// Вывести топ студентов

void outputBySex();								// Вывести количество мужчин и женщин в БД

void outputWithGrant();							// Вывести студентов, получающих стипендию
bool isGranted(Student& currentStudent);		// Проверка на стипендию


bool is4(Student& currentStudent);				// Проверка только на хор
bool is4and5(Student& currentStudent);			// Проверка на хор и отл
bool is5(Student& currentStudent);				// Проверка только на отл
void notGranted_4();							// Проверка только на хор при отсутствии стипендии
void notGranted_4and5();						// Проверка на хор и отл при отсутствии стипендии
void notGranted_5();							// Проверка только на отл при отсутствии стипендии

void outputByDay();								// Вывести студентов по дате
void outputAnyTime(string date);				// В любое время
void outputAM(string date);						// До полудня
void outputPM(string date);						// После полудня

void outputByPosition();						// Вывести по месту в группе

void outputRanked();							// Вывести студентов с разрядом по волейболу

void outputFemaleTeam();						// Вывести студенток женской сборной

int getCourseNumber();							// Получить номер курса
void outputByCourse();							// Вывести студентов полученного курса

void outputMaleS();								// Вывести студентов мужского пола, чей размер одежды >S


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

void loadDatabase() {
	string line;
	ifstream fin;
	try
	{
		fin.open(databaseFile);
		getline(fin, line);
		studentQuantity = stoi(line);
		database = new Student[studentQuantity];
		for (int i = 0; i < studentQuantity; i++)
		{
			getline(fin, database[i].creationTime);
			getline(fin, line);
			database[i].id = stoi(line);
			getline(fin, database[i].name);
			getline(fin, line);
			database[i].sex = stoi(line);
			getline(fin, line);
			database[i].age = stoi(line);
			getline(fin, database[i].wearSize);
			getline(fin, line);
			database[i].rank = stoi(line);
			getline(fin, line);
			database[i].courseNumber = stoi(line);
			getline(fin, line);
			database[i].groupNumber = stoi(line);
			getline(fin, line);
			database[i].groupPosition = stoi(line);
			getline(fin, database[i].studyForm);
			getline(fin, line);
			database[i].itExamMark = stoi(line);
			getline(fin, line);
			database[i].aigExamMark = stoi(line);
			getline(fin, line);
			database[i].mathanExamMark = stoi(line);
			getline(fin, line);
			database[i].historyPassMark = stoi(line);
			getline(fin, line);
			database[i].engPassMark = stoi(line);
			getline(fin, line);
			database[i].codePassMark = stoi(line);
			getline(fin, line);
			database[i].codeCourseMark = stoi(line);
			getline(fin, line);
			database[i].markSum = stoi(line);
		};
		fin.close();
	}
	catch (const exception&)
	{
		fin.close();
		cout << "Ошибка чтения файла. Файл поврежден или изменен. Будет создан новый файл." << endl;
		studentQuantity = 0;
		newFile();
		cout << "\nСоздан новый файл базы данных!\n";
		Sleep(2000);
	}
}

void pushBack(const Student& value) {
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
	cout << "Введите пол студента: "; sexMenu(currentStudent.sex);
	system("CLS");
	cout << "Введите возраст студента: "; cin >> currentStudent.age;
	system("CLS");
	wearMenu(currentStudent.wearSize);
	system("CLS");
	cout << "Введите разряд по волейболу (0, если отсутствует): "; cin >> currentStudent.rank;
	system("CLS");
	cout << "Введите номер курса студента: "; cin >> currentStudent.courseNumber;
	system("CLS");
	cout << "Введите номер группы студента: "; cin >> currentStudent.groupNumber;
	system("CLS");
	cout << "Введите место в группе студента: "; cin >> currentStudent.groupPosition;
	system("CLS");
	formMenu(currentStudent.studyForm);
	system("CLS");
	markMenu("Введите оценку за экзамен по Вв в ИТ: ", currentStudent.itExamMark);
	system("CLS");
	markMenu("Введите оценку за экзамен по АиГу: ", currentStudent.aigExamMark);
	system("CLS");
	markMenu("Введите оценку за экзамен по МатАнанализу: ", currentStudent.mathanExamMark);
	system("CLS");
	markMenu("Введите оценку за дифзачет по истории: ", currentStudent.historyPassMark);
	system("CLS");
	markMenu("Введите оценку за дифзачет по английскому: ", currentStudent.engPassMark);
	system("CLS");
	markMenu("Введите оценку за дифзачет по программированию: ", currentStudent.codePassMark);
	system("CLS");
	markMenu("Введите оценку за курсовую по программированию: ", currentStudent.codeCourseMark);
	currentStudent.markSum = countMarkSum(currentStudent);
	system("CLS");
}

void outputAllFields(Student& currentStudent) { // Вывод информации о текущем студенте
	cout << "Время создания/изменения: " << currentStudent.creationTime << endl;
	cout << "ID студента: " << currentStudent.id << endl;
	cout << "ФИО студента: " << currentStudent.name << endl;
	cout << "Пол студента: " << formatSex(currentStudent.sex) << endl;
	cout << "Возраст студента: " << currentStudent.age << endl;
	cout << "Размер одежды студента: " << currentStudent.wearSize << endl;
	cout << "Разряд по волейболу (0, если отсутствует): " << currentStudent.rank << endl;
	cout << "Номер курса студента: " << currentStudent.courseNumber << endl;
	cout << "Номер группы студента: " << currentStudent.groupNumber << endl;
	cout << "Место в группе студента: " << currentStudent.groupPosition << endl;
	cout << "Форма обучения: " << currentStudent.studyForm << endl;
	cout << "Оценка за экзамен по Вв в ИТ: " << formatMark(currentStudent.itExamMark) << endl;
	cout << "Оценка за экзамен по АиГу: " << formatMark(currentStudent.aigExamMark) << endl;
	cout << "Оценка за экзамен по МатАнанализу: " << formatMark(currentStudent.mathanExamMark) << endl;
	cout << "Оценка за дифзачет по истории: " << formatMark(currentStudent.historyPassMark) << endl;
	cout << "Оценка за дифзачет по английскому: " << formatMark(currentStudent.engPassMark) << endl;
	cout << "Оценка за дифзачет по программированию: " << formatMark(currentStudent.codePassMark) << endl;
	cout << "Оценка за курсовую по программированию: " << formatMark(currentStudent.codeCourseMark) << endl;
	cout << "_______________________________________________" << endl << endl;
}

string formatSex(bool& sex)
{
	switch (sex)
	{
	case false:
		return "мужской";
		break;
	case true:
		return "женский";
		break;
	}
}

string formatMark(int& mark)
{
	switch (mark)
	{
	case 2:
		return "Неуд";
		break;
	case 3:
		return "Уд";
		break;
	case 4:
		return "Хор";
		break;
	case 5:
		return "Отл";
		break;
	}
}

bool has2(Student& currentStudent)
{
	if (currentStudent.itExamMark == 2 ||
		currentStudent.aigExamMark == 2 ||
		currentStudent.mathanExamMark == 2 ||
		currentStudent.historyPassMark == 2 ||
		currentStudent.engPassMark == 2 ||
		currentStudent.codePassMark == 2 ||
		currentStudent.codeCourseMark == 2)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

int countMarkSum(Student& currentStudent)
{
	int sum = 0;
	sum += currentStudent.itExamMark;
	sum += currentStudent.aigExamMark;
	sum += currentStudent.mathanExamMark;
	sum += currentStudent.historyPassMark;
	sum += currentStudent.engPassMark;
	sum += currentStudent.codePassMark;
	sum += currentStudent.codeCourseMark;
	return sum;
}

void markMenu(string message, int& mark)
{
	int answer;
	answer = markAns(message);
	switch (answer)
	{
	case 0:
		mark = 5;
		break;
	case 1:
		mark = 4;
		break;
	case 2:
		mark = 3;
		break;
	case 3:
		mark = 2;
		break;
	}
}

int sexAns()
{
	int choice = 0;
	int options = 2;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;
		cout << "Выберите пол студента:" << endl;

		if (choice == 0) cout << "-> Мужской" << endl;
		else  cout << "   Мужской" << endl;
		
		if (choice == 1) cout << "-> Женский" << endl;
		else  cout << "   Женский" << endl;

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

void sexMenu(bool& sex)
{
	int answer;
	answer = sexAns();
	switch (answer)
	{
	case 0:
		sex = 0;
		break;
	case 1:
		sex = 1;
		break;
	}
}

int wearAns()
{
	int choice = 0;
	int options = 5;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;
		cout << "Выберите размер одежды:" << endl;

		if (choice == 0) cout << "-> XS" << endl;
		else  cout << "   XS" << endl;

		if (choice == 1) cout << "-> S" << endl;
		else  cout << "   S" << endl;

		if (choice == 2) cout << "-> M" << endl;
		else  cout << "   M" << endl;

		if (choice == 3) cout << "-> L" << endl;
		else  cout << "   L" << endl;

		if (choice == 4) cout << "-> XL" << endl;
		else  cout << "   XL" << endl;

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

void wearMenu(string& size)
{
	int answer;
	answer = wearAns();
	switch (answer)
	{
	case 0:
		size = "XS";
		break;
	case 1:
		size = "S";
		break;
	case 2:
		size = "M";
		break;
	case 3:
		size = "L";
		break;
	case 4:
		size = "XL";
		break;
	}
}

int formAns()
{
	int choice = 0;
	int options = 5;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;
		cout << "Выберите форму обучения:" << endl;

		if (choice == 0) cout << "-> очная" << endl;
		else  cout << "   очная" << endl;

		if (choice == 1) cout << "-> очно-заочная" << endl;
		else  cout << "   очно-заочная" << endl;

		if (choice == 2) cout << "-> заочная" << endl;
		else  cout << "   заочная" << endl;

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

void formMenu(string& size)
{
	int answer;
	answer = formAns();
	switch (answer)
	{
	case 0:
		size = "очная";
		break;
	case 1:
		size = "очно-заочная";
		break;
	case 2:
		size = "заочная";
		break;
	}
}

int markAns(string message)
{
	int choice = 0;
	int options = 4;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и " << "вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;
		cout << message << endl;

		if (choice == 0) cout << "-> Отл" << endl;
		else  cout << "   Отл" << endl;

		if (choice == 1) cout << "-> Хор" << endl;
		else  cout << "   Хор" << endl;

		if (choice == 2) cout << "-> Уд" << endl;
		else  cout << "   Уд" << endl;

		if (choice == 3) cout << "-> Неуд" << endl;
		else  cout << "   Неуд" << endl;

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

void newFile() {
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
	try
	{
		inputStudent(newStudent);
	}
	catch (const exception&)
	{
		cout << "Ошибка! Неверный формат ввода. Пожалуйста, вводите числа, где это возможно." << endl;

	}

	if (!has2(newStudent)) {
		pushBack(newStudent);
		transaction();
	}
	else 
	{
		cout << "Внесение записи невозможно! Данный студент имеет 2 как минимум по одному предмету." << endl;
		system("pause");
	}

}

void transaction() {
	newFile();
	ofstream fout;
	fout.open(outputFile, ofstream::app);
	for (int i = 0; i < studentQuantity; i++)
	{
		fout << "Время создания/изменения: " << database[i].creationTime << endl;
		fout << "ID студента: " << database[i].id << endl;
		fout << "ФИО студента: " << database[i].name << endl;
		fout << "Пол студента: " << formatSex(database[i].sex) << endl;
		fout << "Возраст студента: " << database[i].age << endl;
		fout << "Размер одежды студента: " << database[i].wearSize << endl;
		fout << "Разряд по волейболу (0, если отсутствует): " << database[i].rank << endl;
		fout << "Номер курса студента: " << database[i].courseNumber << endl;
		fout << "Номер группы студента: " << database[i].groupNumber << endl;
		fout << "Место в группе студента: " << database[i].groupPosition << endl;
		fout << "Форма обучения: " << database[i].studyForm << endl;
		fout << "Оценка за экзамен по Вв в ИТ: " << formatMark(database[i].itExamMark) << endl;
		fout << "Оценка за экзамен по АиГу: " << formatMark(database[i].aigExamMark) << endl;
		fout << "Оценка за экзамен по МатАнанализу: " << formatMark(database[i].mathanExamMark) << endl;
		fout << "Оценка за дифзачет по истории: " << formatMark(database[i].historyPassMark) << endl;
		fout << "Оценка за дифзачет по английскому: " << formatMark(database[i].engPassMark) << endl;
		fout << "Оценка за дифзачет по программированию: " << formatMark(database[i].codePassMark) << endl;
		fout << "Оценка за курсовую по программированию: " << formatMark(database[i].codeCourseMark) << endl;
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
		fout << database[i].age << endl;
		fout << database[i].wearSize << endl;
		fout << database[i].rank << endl;
		fout << database[i].courseNumber << endl;
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
		fout << database[i].markSum << endl;
	};
	fout.close();
	cout << "Транзакция базы данных прошла успешно!\n";
	Sleep(1500);
}
// МЕНЮ
// Отображает меню в консоли
int ans() {
	int choice = 0;
	int options = 15;
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

		if (choice == 10) cout << "-> Вывод студентов с разрядом по волейболу" << endl;
		else  cout << "   Вывод студентов с разрядом по волейболу" << endl;

		if (choice == 11) cout << "-> Вывод студенток женской сборной по волейболу" << endl;
		else  cout << "   Вывод студенток женской сборной по волейболу" << endl;

		if (choice == 12) cout << "-> Вывод студентов k-го курса" << endl;
		else  cout << "   Вывод студентов k-го курса" << endl;

		if (choice == 13) cout << "-> Вывод студентов мужского пола, чей размер одежды больше, чем S" << endl;
		else  cout << "   Вывод студентов мужского пола, чей размер одежды больше, чем S" << endl;

		if (choice == 14) cout << "-> Выход" << endl;
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
		cout << "Вверх/w и вниз/s для перемещения" << endl;
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

		cout << "   Вывод студентов с разрядом по волейболу" << endl;

		cout << "   Вывод студенток женской сборной по волейболу" << endl;

		cout << "   Вывод студентов k-го курса" << endl;
		
		cout << "   Вывод студентов мужского пола, чей размер одежды больше, чем S" << endl;

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
			break;
		case 1:
			chooseStudent();
			break;
		case 2:
			for (int i = 0; i < studentQuantity; i++)
			{
				outputAllFields(database[i]);
			}
			system("pause");
			break;
		case 3:
			outputByGroup();
			break;
		case 4:
			outputTop();
			break;
		case 5:
			outputBySex();
			break;
		case 6:
			outputWithGrant();
			break;
		case 7:
			notGrantedMenu();
			break;
		case 8:
			outputByDay();
			break;
		case 9:
			outputByPosition();
			break;
		case 10:
			outputRanked();
			break;
		case 11:
			outputFemaleTeam();
			break;
		case 12:
			outputByCourse();
			break;
		case 13:
			outputMaleS();
			break;
		case 14:
			exit(0);
			break;
		}
	} while (true);
}

void menuLocked() {
	int answer = -1;
	do {
		answer = ansLocked();
		switch (answer)
		{
		case 0:
			newStudent();
			outputAllFields(database[0]);
			main();
			break;
		case 1:
			exit(0);
			break;
		}
	} while (answer != 0);
}

void chooseStudent()
{
	int id;
	bool exist = false;
	system("cls");

	for (int i = 0; i < studentQuantity; i++)
	{
		cout << "ID: " << database[i].id << "\t" << database[i].name << " (гр. " << database[i].groupNumber << ")" << endl;
	}
	cout << endl;
	cout << "Введите Id студента для редактирования (0 для выхода в меню): ";

	try
	{
		cin >> id;
		if (id == 0) return;

		for (int i = 0; i < studentQuantity; i++)
		{
			if (database[i].id == id)
			{
				exist = true;
				break;
			}
		}

		if (exist) editStudent(id);
		else throw invalid_argument("Неверный ID");
	}
	catch (const invalid_argument&)
	{
		cerr << "Студента с таким ID не существует. Пожалуйста, введите корректный ID." << endl;
		Sleep(2000);
		chooseStudent();
	}
	catch (const exception&)
	{
		cerr << "Неверная запись ID. Пожалуйста, введите ID числом, используя арабские цифры" << endl;
		Sleep(2000);
		chooseStudent();
	}
}

void editStudent(int id)
{
	int i = getIndex(id);
	editMenu(i);
	transaction();
}

int getIndex(int id)
{
	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].id == id) return i;
	}
}

int editMenu(int i)
{
	int answer;
	Student& currentStudent = database[i];
	string value;
	do {
		if (has2(currentStudent))
		{
			deleteStudent(i);
			return 0;
		}
		answer = editMenuAns(i);
		if (answer >= 0 && answer <= 7 && answer != 1 && answer != 3)
		{
			value = getValue();
			currentStudent.creationTime = getCurrentDate();
		}
		switch (answer)
		{
		case 0:
			currentStudent.name = value;
			break;
		case 1:
			sexMenu(currentStudent.sex);
			break;
		case 2:
			currentStudent.age = stoi(value);
			break;
		case 3:
			wearMenu(currentStudent.wearSize);
			break;
		case 4:
			currentStudent.rank = stoi(value);
			break;
		case 5:
			currentStudent.courseNumber = stoi(value);
			break;
		case 6:
			currentStudent.groupNumber = stoi(value);
			break;
		case 7:
			currentStudent.groupPosition = stoi(value);
			break;
		case 8:
			formMenu(currentStudent.studyForm);
			break;
		case 9:
			markMenu("Введите оценку за экзамен по Вв в ИТ: ", currentStudent.itExamMark);
			break;
		case 10:
			markMenu("Введите оценку за экзамен по АиГу: ", currentStudent.aigExamMark);
			break;
		case 11:
			markMenu("Введите оценку за экзамен по МатАнанализу: ", currentStudent.mathanExamMark);
			break;
		case 12:
			markMenu("Введите оценку за дифзачет по истории: ", currentStudent.historyPassMark);
			break;
		case 13:
			markMenu("Введите оценку за дифзачет по английскому: ", currentStudent.engPassMark);
			break;
		case 14:
			markMenu("Введите оценку за дифзачет по программированию: ", currentStudent.codePassMark);
			break;
		case 15:
			markMenu("Введите оценку за курсовую по программированию: ", currentStudent.codeCourseMark);
			break;
		case 16:
			return 0;
			break;
		}
		if (answer >= 9 && answer <= 15) countMarkSum(currentStudent); // Если оценку изменили, то пересчитать сумму оценок
	} while (true);
}

int editMenuAns(int i)
{
	int choice = 0;
	int options = 17;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) {
			cout << "-> ФИО студента: " << database[i].name << endl;
		}
		else  cout << "   ФИО студента: " << database[i].name << endl;

		if (choice == 1) {
			cout << "-> Пол студента: " << database[i].sex << endl;
		}
		else  cout << "   Пол студента: " << database[i].sex << endl;

		if (choice == 2) {
			cout << "-> Возраст студента: " << database[i].age << endl;
		}
		else  cout << "   Возраст студента: " << database[i].age << endl;

		if (choice == 3) {
			cout << "-> Размер одежды студента: " << database[i].wearSize << endl;
		}
		else  cout << "   Размер одежды студента: " << database[i].wearSize << endl;

		if (choice == 4) {
			cout << "-> Разряд по волейболу (0, если отсутствует): " << database[i].rank << endl;
		}
		else  cout << "   Разряд по волейболу (0, если отсутствует): " << database[i].rank << endl;

		if (choice == 5) {
			cout << "-> Номер курса студента: " << database[i].courseNumber << endl;
		}
		else  cout << "   Номер курса студента: " << database[i].courseNumber << endl;

		if (choice == 6) {
			cout << "-> Номер группы студента: " << database[i].groupNumber << endl;
		}
		else  cout << "   Номер группы студента: " << database[i].groupNumber << endl;

		if (choice == 7) {
			cout << "-> Место в группе студента: " << database[i].groupPosition << endl;
		}
		else  cout << "   Место в группе студента: " << database[i].groupPosition << endl;

		if (choice == 8) {
			cout << "-> Форма обучения: " << database[i].studyForm << endl;
		}
		else  cout << "   Форма обучения: " << database[i].studyForm << endl;

		if (choice == 9) {
			cout << "-> Оценка за экзамен по Вв в ИТ: " << database[i].itExamMark << endl;
		}
		else  cout << "   Оценка за экзамен по Вв в ИТ: " << database[i].itExamMark << endl;

		if (choice == 10) {
			cout << "-> Оценка за экзамен по АиГу: " << database[i].aigExamMark << endl;
		}
		else  cout << "   Оценка за экзамен по АиГу: " << database[i].aigExamMark << endl;

		if (choice == 11) {
			cout << "-> Оценка за экзамен по МатАнанализу: " << database[i].mathanExamMark << endl;
		}
		else  cout << "   Оценка за экзамен по МатАнанализу: " << database[i].mathanExamMark << endl;

		if (choice == 12) {
			cout << "-> Оценка за дифзачет по истории: " << database[i].historyPassMark << endl;
		}
		else  cout << "   Оценка за дифзачет по истории: " << database[i].historyPassMark << endl;

		if (choice == 13) {
			cout << "-> Оценка за дифзачет по английскому: " << database[i].engPassMark << endl;
		}
		else  cout << "   Оценка за дифзачет по английскому: " << database[i].engPassMark << endl;

		if (choice == 14) {
			cout << "-> Оценка за дифзачет по программированию: " << database[i].codePassMark << endl;
		}
		else  cout << "   Оценка за дифзачет по программированию: " << database[i].codePassMark << endl;

		if (choice == 15) {
			cout << "-> Оценка за курсовую по программированию: " << database[i].codeCourseMark << endl;
		}
		else  cout << "   Оценка за курсовую по программированию: " << database[i].codeCourseMark << endl;

		if (choice == 16) {
			cout << "-> Выход" << endl;
		}
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

string getValue()
{
	string value;
	system("cls");
	cout << "Введите новое значение: ";
	cin.ignore(cin.rdbuf()->in_avail()); // Игнор буфера потока ввода
	getline(cin, value);
	return value;
}

void deleteStudent(int index)
{
	for (int i = index; i < studentQuantity-1; i++)
	{
		database[i] = database[i + 1];
	}
	studentQuantity--;

	Student* newArray = new Student[studentQuantity];

	for (int i = 0; i < studentQuantity; i++)
	{
		newArray[i] = database[i];
	}

	delete[] database;

	database = newArray;

	cout << "Внимание! Студент имеет неуд как минимум по одному предмету. Запись о нем будет удалена" << endl;
	Sleep(2000);
}

int getGroupNumber() {
	int number;
	system("cls");
	cout << "Введите номер группы студентов: ";
	cin >> number;
	return number;
}

void outputByGroup() {
	int groupNumber = getGroupNumber();
	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].groupNumber == groupNumber)	outputAllFields(database[i]);
	}
	system("pause");
}

bool comp(Student a, Student b) {
	return a.markSum > b.markSum;
}

void outputTop()
{
	Student* topArray = new Student[studentQuantity];
	int topQuantity = 10;	// Количество позиций в ТОПе
	for (int i = 0; i < studentQuantity; i++)
	{
		topArray[i] = database[i];
	}

	sort(topArray, topArray + studentQuantity, comp);

	for (int i = 0; i < studentQuantity; i++)
	{
		cout << i+1 << ": " << topArray[i].name << " (гр. " << topArray[i].groupNumber << ")\t" 
		<< round(double(topArray[i].markSum) * 100 / 7) / 100 << endl; // Вывести краткую информацию о студенте и его средний балл

		if (i == studentQuantity - 1) break;	// Если студентов меньше, чем позиций в ТОПе, то закончить вывод
	}
	system("pause");
	delete[] topArray;
}

void outputBySex()
{
	int male = 0;
	int female = 0;

	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].sex) female++;
		else male++;
	}

	cout << "В базе данных " << male << " мужчин и " << female << " женщин" << endl;
	system("pause");
}

void outputWithGrant()
{
	cout << "Следующие студенты получат стипендию:" << endl;
	for (int i = 0; i < studentQuantity; i++)
	{
		bool granted = isGranted(database[i]);
		if (granted) cout << database[i].name << " (гр. " << database[i].groupNumber << ")" << endl;
	}
	system("pause");
}

bool isGranted(Student& currentStudent)
{
	if (currentStudent.studyForm == "очная" &&
		currentStudent.itExamMark > 3 &&
		currentStudent.aigExamMark > 3 &&
		currentStudent.mathanExamMark > 3 &&
		currentStudent.engPassMark > 3 &&
		currentStudent.historyPassMark > 3 &&
		currentStudent.codeCourseMark > 3 &&
		currentStudent.codePassMark > 3) return true;
	else return false;
}

int notGrantedMenu()
{
	int answer;
	string value;
	do {
		answer = notGrantedAns();
		switch (answer)
		{
		case 0:
			notGranted_4();
			system("pause");
			break;
		case 1:
			notGranted_4and5();
			system("pause");
			break;
		case 2:
			notGranted_5();
			system("pause");
			break;
		case 3:
			return 0;
			break;
		}
	} while (true);
}

int notGrantedAns()
{
	int choice = 0;
	int options = 4;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) {
			cout << "-> Учатся только на «хор»" << endl;
		}
		else  cout << "   Учатся только на «хор»" << endl;

		if (choice == 1) {
			cout << "-> Учатся на «хор» и «отл»" << endl;
		}
		else  cout << "   Учатся на «хор» и «отл»" << endl;

		if (choice == 2) {
			cout << "-> Учатся только на «отл»" << endl;
		}
		else  cout << "   Учатся только на «отл»" << endl;

		if (choice == 3) {
			cout << "-> Выход" << endl;
		}
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

bool is4(Student& currentStudent) {
	if (currentStudent.itExamMark == 4 &&
		currentStudent.aigExamMark == 4 &&
		currentStudent.mathanExamMark == 4 &&
		currentStudent.engPassMark == 4 &&
		currentStudent.historyPassMark == 4 &&
		currentStudent.codeCourseMark == 4 &&
		currentStudent.codePassMark == 4) return true;
	else return false;
}

bool is4and5(Student& currentStudent) {
	if (currentStudent.itExamMark > 3 &&
		currentStudent.aigExamMark > 3 &&
		currentStudent.mathanExamMark > 3 &&
		currentStudent.engPassMark > 3 &&
		currentStudent.historyPassMark > 3 &&
		currentStudent.codeCourseMark > 3 &&
		currentStudent.codePassMark > 3) return true;
	else return false;
}

bool is5(Student& currentStudent) {
	if (currentStudent.itExamMark == 5 &&
		currentStudent.aigExamMark == 5 &&
		currentStudent.mathanExamMark == 5 &&
		currentStudent.engPassMark == 5 &&
		currentStudent.historyPassMark == 5 &&
		currentStudent.codeCourseMark == 5 &&
		currentStudent.codePassMark == 5) return true;
	else return false;
}

void notGranted_4() {
	cout << "Следующие студенты не получат стипендию, но учатся только на 4:" << endl;
	for (int i = 0; i < studentQuantity; i++)
	{
		bool granted = isGranted(database[i]);
		bool only4 = is4(database[i]);
		if (!granted && only4) cout << database[i].name << " (гр. " << database[i].groupNumber << ")" << endl;
	}
}

void notGranted_4and5() {
	cout << "Следующие студенты не получат стипендию, но учатся на 4 и 5:" << endl;
	for (int i = 0; i < studentQuantity; i++)
	{
		bool granted = isGranted(database[i]);
		bool only4and5 = is4and5(database[i]);
		if (!granted && only4and5) cout << database[i].name << " (гр. " << database[i].groupNumber << ")" << endl;
	}
}

void notGranted_5() {
	cout << "Следующие студенты не получат стипендию, но учатся только на 5:" << endl;
	for (int i = 0; i < studentQuantity; i++)
	{
		bool granted = isGranted(database[i]);
		bool only5 = is5(database[i]);
		if (!granted && only5) cout << database[i].name << " (гр. " << database[i].groupNumber << ")" << endl;
	}
}

void outputByDay()
{
	string date;
	cout << "Введите дату в формате дд.мм.гггг: ";
	cin >> date;
	outputByDayMenu(date);
}

int outputByDayMenu(string date)
{
	int answer;
	string value;
	do {
		answer = outputByDayAns();
		switch (answer)
		{
		case 0:
			outputAnyTime(date);
			system("pause");
			break;
		case 1:
			outputAM(date);
			system("pause");
			break;
		case 2:
			outputPM(date);
			system("pause");
			break;
		case 3:
			return 0;
			break;
		}
	} while (true);
	return 0;
}

int outputByDayAns()
{
	int choice = 0;
	int options = 4;
	int ch;
	while (true) {
		system("cls");
		choice = (choice + options) % options;
		cout << "Вверх/w и вниз/s для перемещения" << endl;
		cout << "Enter для выбора" << endl << endl;

		if (choice == 0) {
			cout << "-> Данные за весь день" << endl;
		}
		else  cout << "   Данные за весь день" << endl;

		if (choice == 1) {
			cout << "-> Данные до полудня" << endl;
		}
		else  cout << "   Данные до полудня" << endl;

		if (choice == 2) {
			cout << "-> Данные после полудня" << endl;
		}
		else  cout << "   Данные после полудня" << endl;

		if (choice == 3) {
			cout << "-> Выход" << endl;
		}
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

void outputAnyTime(string date)
{
	string dataDate;
	for (int i = 0; i < studentQuantity; i++)
	{
		dataDate.assign(database[i].creationTime, 0, 10);
		if (date == dataDate) outputAllFields(database[i]);
	}
}

void outputAM(string date)
{
	string dataDate;
	string dataHour;
	for (int i = 0; i < studentQuantity; i++)
	{
		dataDate.assign(database[i].creationTime, 0, 10); // Записываем дату из времени создания
		dataHour.assign(database[i].creationTime, 11, 13); // Записываем часы из времени создания
		if (date == dataDate && stoi(dataHour) < 12) outputAllFields(database[i]);
	}
}

void outputPM(string date)
{
	string dataDate;
	string dataHour;
	for (int i = 0; i < studentQuantity; i++)
	{
		dataDate.assign(database[i].creationTime, 0, 10); // Записываем дату из времени создания
		dataHour.assign(database[i].creationTime, 11, 13); // Записываем часы из времени создания
		if (date == dataDate && stoi(dataHour) >= 12) outputAllFields(database[i]);
	}
}

void outputByPosition()
{
	int position;
	cout << "Введите номер студента в списке: ";
	cin >> position;

	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].groupPosition == position) cout << database[i].name << " (гр. " << database[i].groupNumber << ")" << endl;
	}
	system("pause");
}

void outputRanked()
{
	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].rank > 0) {
			outputAllFields(database[i]);
		}
	}
	system("pause");
}

void outputFemaleTeam()
{
	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].rank > 0 && database[i].sex) {
			outputAllFields(database[i]);
		}
	}
	system("pause");
}

int getCourseNumber() {
	int number;
	system("cls");
	cout << "Введите номер курса студентов: ";
	cin >> number;
	return number;
}

void outputByCourse() {
	int courseNumber = getCourseNumber();
	for (int i = 0; i < studentQuantity; i++)
	{
		if (database[i].courseNumber == courseNumber)	outputAllFields(database[i]);
	}
	system("pause");
}

void outputMaleS()
{
	for (int i = 0; i < studentQuantity; i++)
	{
		if (!database[i].sex && (database[i].wearSize == "M" || database[i].wearSize == "L" || database[i].wearSize == "XL")) {
			outputAllFields(database[i]);
		}
	}
	system("pause");
}