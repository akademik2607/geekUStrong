#include<iostream>
#include<fstream>
#include<optional>
#include<tuple>
#include<vector>
#include<string>
#include<algorithm>

struct Person
{
	std::string surname;
	std::string name;
	std::optional<std::string> patronymic;

	friend std::ostream& operator<<(std::ostream& o, const Person& person);
	bool operator<(const Person& person){
		return std::tie(surname, name, patronymic) < std::tie(person.surname, person.name, person.patronymic);
	}

	bool operator==(const Person& person){
		return std::tie(surname, name, patronymic) == std::tie(person.surname, person.name, person.patronymic);
	}
};

std::ostream& operator<<(std::ostream& o, const Person& person){
		o << person.surname << ' ' << person.name << ' ';
		if(person.patronymic) o << *person.patronymic << ' ';
		return o;
}


struct PhoneNumber {
	int country;
	int city;
	std::string number;
	std::optional<int> addNumber;

	friend std::ostream& operator<<(std::ostream& o, const PhoneNumber& number);
};

std::ostream& operator<<(std::ostream& o, const PhoneNumber& number){
	o << '+' << number.country << '(' << number.city << ')' << number.number;
	if (number.addNumber) o << ' ' << *number.addNumber;
	return o;
}


class PhoneBook {
private:
	std::vector<std::pair<Person, PhoneNumber>> items;

	size_t getNpos(std::string& str, size_t pos){
		size_t nPos = str.find(';', pos);
		return nPos;
	}

public:
	PhoneBook(std::ifstream& file){
		std::string str;
		if(file.is_open()){
			while(!file.eof()){
				getline(file, str);
				size_t pos = 0;
				size_t nPos = getNpos(str, pos);
				
				std::string surname = str.substr(pos, nPos - pos);

				pos = nPos + 1;
				nPos = getNpos(str, pos);

				std::string name = str.substr(pos, nPos - pos);

				pos = nPos + 1;
				nPos = getNpos(str, pos);

				std::optional<std::string> patronymic;
				if((nPos - pos) == 0){
					patronymic = std::nullopt;
				}
				else{
					patronymic = str.substr(pos, nPos - pos);
				
				}

				pos = nPos + 1;
				nPos = getNpos(str, pos);

				int country = std::stoi(str.substr(pos, nPos - pos));

				pos = nPos + 1;
				nPos = getNpos(str, pos);

				int city = std::stoi(str.substr(pos, nPos - pos));

				pos = nPos + 1;
				nPos = getNpos(str, pos);

				std::string number = str.substr(pos, nPos - pos);

				pos = nPos + 1;
				nPos = getNpos(str, pos);

				std::optional<int> addNumber;
				if(nPos - pos == 0){
					addNumber = std::nullopt;
				} else{
					addNumber = std::stoi(str.substr(pos, nPos - pos));
				}

				items.emplace_back(std::pair<Person, PhoneNumber>({surname, name, patronymic}, 
					{country, city, number, addNumber}));
			}
			file.close();
		}
		
	}


	friend std::ostream& operator<<(std::ostream& o, const PhoneBook& book);

	void SortByName() {
		std::sort(items.begin(), items.end(), 
			[](std::pair<Person, PhoneNumber> first, std::pair<Person, PhoneNumber> second){
				return first.first < second.first;
			});
	}

	void SortByPhone(){
		std::sort(items.begin(), items.end(), 
			[](std::pair<Person, PhoneNumber> first, std::pair<Person, PhoneNumber> second){
				return tie(first.second.country, first.second.city, first.second.number, first.second.addNumber)
				< tie(second.second.country, second.second.city, second.second.number, second.second.addNumber);
			});
	}

	std::tuple<std::string, PhoneNumber> GetPhoneNumber(const std::string& surname){
		size_t counter = 0;
		PhoneNumber temp;
		std::for_each(items.begin(), items.end(), 
			[&surname, &counter, &temp](std::pair<Person, PhoneNumber> item){

			if(item.first.surname == surname){
				counter++;
				temp = item.second;
			}
		});
		if (counter == 0){
			return std::make_tuple("not found", PhoneNumber{0, 0, "", std::nullopt});
		} else if(counter == 1) {
			return std::make_tuple("", temp);
		} else {
			return std::make_tuple("found more then 1", temp);
		}
	}

	void ChangePhoneNumber(Person person, PhoneNumber number){
		
		std::find_if(items.begin(), items.end(), [&person, &number](auto item){
			if(item.first == person){
				item.second = number;
				return true;
			} else{
				return false;
			}
		} );
		
	}

};


std::ostream& operator<<(std::ostream& o, const PhoneBook& book){
	for(auto item : book.items){
		o << item.first << ' ' << item.second << ' ' << std::endl;
	}
	return o;
}


int main() {

	// Person alex = { "Illarionov", "Alex","ALexandrovich"};
	// Person alex2 = { "Illarionov", "Alex"};
	// Person james = {"Joyes", "James"};
	// std::cout << alex;
	// std::cout << james;
	// std::cout << (alex < alex2);

	// std::ifstream file("database.txt");
	// PhoneBook book(file);
	// std::cout << book;


	std::ifstream file("database.txt"); // путь к файлу PhoneBook.txt
       PhoneBook book(file);
       std::cout << book;
 
       std::cout << "------SortByPhone-------" << std::endl;
       book.SortByPhone();
       std::cout << book;
 
       std::cout << "------SortByName--------" << std::endl;
       book.SortByName();
       std::cout << book;
 
       std::cout << "-----GetPhoneNumber-----" << std::endl;
       // лямбда функция, которая принимает фамилию и выводит номер телефона этого        человека, либо строку с ошибкой
       auto print_phone_number = [&book](const std::string& surname) { 
              std::cout << surname << "\t"; 
              auto answer = book.GetPhoneNumber(surname); 
              if (std::get<0>(answer).empty()) 
                     std::cout << std::get<1>(answer); 
              else 
                     std::cout << std::get<0>(answer); 
                     std::cout << std::endl; 
       };

       // вызовы лямбды
       print_phone_number("Ivanov");
       print_phone_number("Petrov");
 
       std::cout << "----ChangePhoneNumber----" << std::endl;
       book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },                    PhoneNumber{7, 123, "15344458", std::nullopt});
       book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
       std::cout << book;

	return 0;
}