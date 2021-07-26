#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
template <typename T>
T check(T min, T max)
{
	T a;
	cin >> a;
	while (cin.fail() || a<min || a>max) {
		cout << "Неверное значение. Повторите ввод." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> a;
	}
	return a;
}
class Zad
{
public:
	void set_zad(Zad& q, istream& f)
	{
		string a;
		bool dif;
		getline(f, a);
		q.qt = stoi(a);
		getline(f, a);
		q.text.clear();
		while (a != "$@$@$@$") {
			q.text = q.text + a + "\n";
			getline(f, a);
		}
		q.text.erase(q.text.end()-1);
		getline(f, a);
		istringstream(a) >> dif;
		q.d = dif;
	}
	void rewrite(Zad& q) { q.d = !q.d; }
	int get_qt() const { return qt; }
	string get_text() const { return text; }
	bool get_d() const { return d; }
	friend ostream& operator<<(ostream& out, const Zad& q)
	{
		out << q.qt << endl;
		out << q.text << endl;
		return out;
	}
	friend istream& operator>>(istream& in, Zad& q)
	{
		string text;
		cout << "Введите раздел дисциплины." << endl;
		q.qt = (check(1, numeric_limits<int>::max()));
		cout << "Введите текст вопроса." << endl;
		in.clear();
		in.ignore();
		getline(in, q.text);
		cout << "Вопрос относится к сложным? Да - 1, Нет - 0." << endl;
		q.d = (check(false, true));
		return in;
	}
private:
	int qt;
	string text;
	bool d;
};

