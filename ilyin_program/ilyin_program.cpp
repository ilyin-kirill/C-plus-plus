// ilyin_program.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <stdlib.h>
#include "Zad.h"
#include "Bil.h"
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <clocale>
#include <set>
#include <algorithm>
int numofeasy(const vector<Zad>& qst)
{
	int k = 0;
	for (int i = 0; i < qst.size(); i++)
		if (qst[i].get_d() == false)
			k++;
	return k;
}
template<typename T>
bool check_length(const vector<T>& qst)
{
	if (qst.size() == 0) {
		cout << "Сначала считайте вопросы/билеты из файла или создайте их." << endl;
		return false;
	}
	else return true;
}
string print_mark(const Zad& q)
{
	ostringstream oss;
	if (q.get_d() == 1)
		oss << "(*)" << endl;
	else oss << endl;
	return oss.str();
}
typedef string(*DoFormat)(Zad);
string format1(Zad q)
{
	ostringstream oss;
	oss << "Раздел дисциплины - " << q;
	return oss.str();
}
string format2(Zad q)
{
	ostringstream oss;
	oss << q.get_text() << endl;
	return oss.str();
}
string format3(Zad q)
{
	ostringstream oss;
	oss << "\n" << q << "$@$@$@$" << endl; 
	oss << q.get_d();
	return oss.str();
}
DoFormat PrintFormats[3] = { format1, format2, format3 };
string path_file()
{
	string a;
	cout << "Укажите путь до файла. Для считывания базы из файла по умолчанию (in.txt)." << endl;
	cin.clear();
	cin.ignore();
	getline(cin, a);
	return a;
}
template<typename T>
void check_open_file(T& f, string& a)
{
	f.open(a);
	while (!f.is_open())
	{
		cout << "Файл не найден!" << endl;
		cin.clear();
		getline(cin, a);
		f.open(a);
	}
}
Zad input1q()
{
	Zad q;
	cin >> q;	
	return q;
}
void in(vector<Zad>& qst)
{
	do {
		qst.push_back(input1q());
		cout << "Продолжить вводить вопросы? Да - 1, Нет - 0." << endl;
	} while (check(false, true));
	cout << "Не забудьте сохранить базу в файл. Все несохраненные данные будут утеряны." << endl;
}
void out(const vector<Zad>& qst)
{
	if(check_length(qst) == false)
		return;
	for (int i = 0; i < qst.size(); i++) {
		cout << "Вопрос №" << i + 1 << print_mark(qst[i]);
		cout << "Раздел дисциплины - " << qst[i];
	}
}
void out_to_file(vector<Zad>& qst)
{
	if (check_length(qst) == false)
		return;
	string a;
	ofstream f;
	cout << "WARNING: Если Вы сохраняете полную базу, сохраните ее НЕ в исходный файл!!!" << endl;
	a = path_file();
	f.open(a, ios_base::app);
	for (int i = 0; i < qst.size(); i++) {
		f << "\n" << qst[i].get_qt() << endl;
		f << qst[i].get_text() << endl;
		f << "$@$@$@$" << endl;
		f << qst[i].get_d();
	}
	f.close();
	qst.clear();
	cout << "База сохранена в файл." << endl;
}
void input_file(vector<Zad>& qst)
{
	string a;
	ifstream f;
	a = path_file();
	check_open_file(f, a);
	Zad q;
	qst.clear();
	f.ignore(numeric_limits<streamsize>::max(), '\n');
	while (!f.eof())
	{
		q.set_zad(q, f);
		qst.push_back(q);
		f.peek();
	}
	f.close();
	cout << "База считана из файла." << endl;
}
template <typename T>
void sort(vector<Zad>& qst, T Zad::* p)
{
	Zad x;
	for (int i = 0; i < qst.size() - 1; i++) {
		for (int j = 0; j < qst.size() - i - 1; j++) {
			if ((qst[j].*p)() > (qst[j + 1].*p)()) {
				x = qst[j];
				qst[j] = qst[j + 1];
				qst[j + 1] = x;
			}
		}
	}
	cout << "База отсортирована." << endl;
}
template<typename Iter, typename T>
Iter findkey(Iter begin, Iter end, const T& v)
{
	bool check = false;
	for (; begin != end; ++begin)
		if (begin->first == v) {
			return begin;
			bool check = true;
		}
	if (check == false)
		return end;
}
void delete_hard_qst(const vector<Zad>& qst, vector<pair<int, int>>& qstns)
{
	for (int i = 0; i < qstns.size(); i++)
		if (qst[qstns[i].second].get_d() == 1)
			qstns.erase(qstns.begin() + i);
}
void gen_onebil(const vector<Zad>& qst, Bil& ask,const int& numofqst)
{
	ask.Qs.clear();
	vector<pair<int, int>>qstns;
	set<int>section;
	int t = 0;
	int num;
	auto k = qstns.end();
	for (int i = 0; i < qst.size(); i++) {
		qstns.push_back(pair<int, int>{qst[i].get_qt(), i});
		section.insert(qst[i].get_qt());
	}
	random_shuffle(qstns.begin(), qstns.end());
	while (ask.Qs.size() != numofqst) {
		for (auto i = section.begin(); i != section.end() ;i++) {
			if (ask.Qs.size() != numofqst) {
				auto it = findkey(qstns.begin(), qstns.end(), *i);
				if (it != qstns.end()) {
					if (qst[it->second].get_d() == 0) {
						ask.Qs.push_back(qst[it->second]);
						qstns.erase(it);
					}
					else {
						t++;
						if (t == 1) {
							ask.Qs.push_back(qst[it->second]);
							delete_hard_qst(qst, qstns);
						}
					}
				}
			}
			else break;
		}
	}
}
void print_bil(ostream& f, const Bil& ask, int format)
{
	for (int i = 0; i < ask.Qs.size(); i++) {
		if (format == 1)
			f << "Вопрос №" << i + 1 << print_mark(ask.Qs[i]);
		f << PrintFormats[format](ask.Qs[i]);
	}
}
void gen_bil(const vector<Zad>& qst, vector<Bil>& bil)
{
	if (check_length(qst) == false)
		return;
	Bil ask;
	cout << "Всего вопросов - " << qst.size() << endl;
	cout << "Всего легких вопросов - " << numofeasy(qst) << endl;
	cout << "Введите желаемое количество билетов." << endl;
	int numofbil = check(1, numeric_limits<int>::max());
	cout << "Введите желаемое количество вопросов в билете." << endl;
	int numofqst = check(1, numofeasy(qst) + 1);
	for (int i = 0; i < numofbil; i++) {
		ask.n = bil.size() + 1;
		gen_onebil(qst, ask, numofqst);
		bil.push_back(ask);
	}
}
string to_lower(string str)
{
	transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}
vector<int> find_qst(const vector<Zad>& qst)
{
	vector<int>findqstvec;
	string word;
	cout << "Введите слово, по которому хотите найти вопросы." << endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, word);
	word = to_lower(word);
	bool check = false;
	for (int i = 0; i < qst.size(); i++) {
		if (to_lower(qst[i].get_text()).find(word) != -1) {
			findqstvec.push_back(i);
			check = true;
		}
	}
	if (check == false)
		cout << "Поиск не дал результатов." << endl;
	else cout << "Было найдено вопросов - " << findqstvec.size() << endl;
	return findqstvec;
}
void print_find_qst(const vector<int>& findqstvec, const vector<Zad>& qst)
{
	for (auto i : findqstvec) {
		cout << "Вопрос №" << i + 1 << print_mark(qst[i]);
		cout << "Раздел дисциплины - " << qst[i];
	}
}
int check_section(const vector<Zad>& qst)
{
	set<int>section;
	for (auto i : qst)
		section.insert(i.get_qt());
	cout << "Введите желаемый раздел. Если желаете просмотреть все разделы - нажмите 0." << endl;
	int qt = check(0, numeric_limits<int>::max());
	while (qt != 0 && section.find(qt) == section.end()) {
		cout << "Такого раздела не найдено. Повторите ввод." << endl;
		qt = check(0, numeric_limits<int>::max());
	}
	return qt;
}
int check_numofask(const vector<int>& findqstvec, int begin)
{
	cout << "Введите номер вопроса" << endl;
	int numofask = check(begin, numeric_limits<int>::max());
	while (numofask != 0 && find(findqstvec.begin(), findqstvec.end(), numofask - 1) == findqstvec.end()) {
		cout << "Повторите ввод." << endl;
		numofask = check(0, numeric_limits<int>::max());
	}
	return numofask;
}
void view_quest(const vector<Zad>& qst)
{
	if (check_length(qst) == false)
		return;
	int qt = check_section(qst);
	cout << "Введите сложность желаемых вопросов. 0 - легкие, 1 - сложные, 2 - все вопросы." << endl;
	int d = check(0, 2);
	for (auto i : qst) 
		if ((i.get_qt() == qt || qt == 0) && (i.get_d() == d || d == 2))
			cout << "Раздел дисциплины - " << i << "\n" << endl;
}
void edit_the_bil(Bil& bil, const vector<Zad>& qst)
{
	vector<int> selection_of_qst;
	cout << "Введите номер вопроса, подлежащего замене." << endl;
	int numofask = check<int>(1, bil.Qs.size());
	cout << "Вопросы из аналогичной категории: " << endl;
	for (int i = 0; i < qst.size(); i++)
		if (qst[i].get_qt() == bil.Qs[numofask - 1].get_qt()) {
			selection_of_qst.push_back(i);
			print_find_qst(selection_of_qst, qst);
		}
	if (selection_of_qst.empty())
		cout << "Вопросов из аналогичной категории не найдено." << endl;
	else {
		cout << "Выберите вопрос." << endl;
		int numofnewask = check_numofask(selection_of_qst, 1);
		bil.Qs[numofask - 1] = qst[numofnewask - 1];
		cout << "Вопрос заменен." << endl;
	}
}
void edit_the_full_bil(vector<Bil>& bil, const vector<Zad>& qst)
{
	if (check_length(bil) == false)
		return;
	cout << "Билетов, доступных для редактирования - " << bil.size() << endl;
	cout << "Введите номер билета, который хотите изменить." << endl;
	int numofbil = check<int>(1, bil.size());
	print_bil(cout, bil[numofbil - 1], 0);
	edit_the_bil(bil[numofbil - 1], qst);
}
void input_bil(vector<Bil>& bil)
{
	vector<Zad>qst;
	ifstream f;
	string check;
	string a = path_file();
	check_open_file(f, a);
	Zad q;
	Bil ask;
	int i = 1;
	f.ignore(numeric_limits<streamsize>::max(), '\n');
	while (!f.eof()) {
		check = "";
		while (check != "------")
		{
			q.set_zad(q, f);
			qst.push_back(q);
			streampos position = f.tellg();
			getline(f, check);
			if (check != "------")
				f.seekg(position);
			f.peek();
		}
		ask.n = i;
		i = i + 1;
		ask.Qs = qst;
		qst.clear();
		bil.push_back(ask);
	}
	f.close();
	cout << "Билеты считаны из файла." << endl;
}
void menu()
{
	cout << "\n"
		<< "1. Добавить вопрос в базу\n"
		<< "2. Вывести базу вопросов\n"
		<< "3. Отредактировать вопрос\n"
		<< "4. Сгенерировать билет\n"
		<< "5. Сохранить базу в файл\n"
		<< "6. Считать базу из файла\n"
		<< "7. Сортировать базу по параметрам\n"
		<< "8. Вывести сгенерированные билеты\n"
		<< "9. Поиск вопроса по слову\n"
		<< "10. Вывод вопросов по категории и сложности\n"
		<< "11. Вывести найденные вопросы\n"
		<< "12. Редактировать билет\n"
		<< "13. Считать билеты из файла\n"
		<< "0. Выход из программы\n" << endl;
}
int main()
{
	Zad q;
	ofstream f;
	setlocale(LC_CTYPE, "rus");
	vector<Zad>qst;
	vector<Bil>bil;
	vector<int>findqstvec;
	string filename;
	int numofask;
	srand(time(0));
	cout << "Выберите вариант\n" << endl;
	menu();
	while (1) {
		switch (check(0, 13)) {
		case 0:
			return 0;
			break;
		case 1:
			in(qst);
			menu();
			break;
		case 2:
			out(qst);
			menu();
			break;
		case 3:
			if (qst.size() != 0) {
				cout << "Всего вопросов, доступных для редактирования - " << qst.size() << endl;
				cout << "Для поиска вопросов по номеру, нажмите - 0. По ключевой фразе - нажмите 1." << endl;
				int mark = check(0, 1);
				if (mark == 0)
					numofask = check<int>(1, qst.size());
				else {
					findqstvec = find_qst(qst);
					if (findqstvec.size() != 0) {
						print_find_qst(findqstvec, qst);
						cout << "Для редактирования всех вопросов - нажмите 0." << endl;
						numofask = check_numofask(findqstvec, 0);
					}
				}
				if (numofask == 0)
					for (auto i : findqstvec)
						q.rewrite(qst[i]);
				else q.rewrite(qst[numofask - 1]);
				cout << "Сложность изменена." << endl;
				cout << "Не забудьте сохранить измененную базу в файл, иначе все несохраненные данные будут утеряны." << endl;
			}
			else cout << "Сначала считайте вопросы из файла или создайте их." << endl;
			menu();
			break;
		case 4:
			gen_bil(qst, bil);
			menu();
			break;
		case 5:
			out_to_file(qst);
			menu();
			break;
		case 6:
			input_file(qst);
			menu();
			break;
		case 7:
			if (qst.size() != 0) {
				cout << "Введите параметр сортировки (1 - по разделам, 2 - по сложности)." << endl;
				if (check(1, 2) == 1)
					sort(qst, &Zad::get_qt);
				else sort(qst, &Zad::get_d);
			} else cout << "Сначала считайте вопросы из файла или создайте их." << endl;
			menu();
			break;
		case 8:
			if (bil.size() != 0) {
			cout << "Введите способ вывода билетов (0 - полный, 1 - для печати)." << endl;
			int format = check(0, 1);
			cout << "Введите область вывода (0 - консоль, 1 - файл)." << endl;
			int output = check(0, 1);
			if (output == 1) {
				filename = path_file();
				f.open(filename, ios_base::app);
			}
			for (int i = 0; i < bil.size(); i++)
				if (output == 0) {
					cout << "Билет №" << i + 1 << endl;
					print_bil(cout, bil[i], format);
				}
				else {
					if (format == 0) {
						print_bil(f, bil[i], 2);
						f << "\n------";
					}
					else {
						f << "Билет №" << i + 1 << endl;
						print_bil(f, bil[i], format);
					}
					cout << "Билет сохранен в файл." << endl;
				}
			f.close();
			} else cout << "Сначала считайте билеты из файла или создайте их." << endl;
			menu();
			break;
		case 9: 
			if (qst.size() != 0)
				findqstvec = find_qst(qst);
			else cout << "Сначала считайте вопросы из файла или создайте их." << endl;
			menu();
			break; 
		case 10: 
			view_quest(qst);
			menu();
			break;
		case 11:
			print_find_qst(findqstvec, qst);
			menu();
			break;
		case 12:
			edit_the_full_bil(bil, qst);
			menu();
			break;
		case 13:
			input_bil(bil);
			menu();
			break;
		}
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
