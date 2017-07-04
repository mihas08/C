#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

enum Communications
{
	water, electricity, telephone, road, sewerage
};

enum EstateType
{
	flat, house, plot
};

class CEstate
{
public:
	CEstate(){
		price = area = 0;
	}
	CEstate(string add, string o, int p, int ar){
		address = add;
		owner = o;
		price = p;
		area = ar;
	}

	virtual ostream& print(ostream& stream) const = 0;
	virtual EstateType getType() const = 0;

	string get_address() const   { return address; }
	void set_address(string add) { address = add; }
	string get_owner() const     { return owner; }
	void set_owner(string o)     { owner = o; }
	int get_price() const        { return price; }
	void set_price(int p)        { price = p; }
	int get_area() const         { return area; }
	void set_area(int ar)        { area = ar; }

	friend ostream& operator << (ostream& to, const CEstate* obj)
	{
		if (&to == &cout)
		{
			to << "Address: " << obj->address << endl
				<< "Owner: " << obj->owner << endl
				<< "Price " << obj->price << endl
				<< "Area: " << obj->area << endl;
		}
		else
		{
			to << obj->address << endl
				<< obj->owner << " " << obj->price << " " << obj->area << endl;
		}
		return to;
	}

	friend istream& operator >> (istream& from, CEstate* obj)
	{
		from.ignore(256, '\n'); //izchistvame bufera ot predishni newline-ove za da ne bugnat getline-na
		getline(from, obj->address);
		from >> obj->owner >> obj->price >> obj->area;
		return from;
	}
protected:
	string address;
	string owner;
	int price;
	int area;
};

class CFlat : public CEstate{
public:
	CFlat() { number_rooms = floor = 0; }

	int get_number_rooms() const  { return number_rooms; }
	void set_number_rooms(int nr) { number_rooms = nr; }
	int get_floor() const         { return floor; }
	void set_floor(int f)         { floor = f; }
	EstateType getType() const    { return flat; }

	ostream& print(ostream& stream) const
	{
		if (&stream == &cout)
		{
			stream << "|FLAT|" << endl
				<< "Floor: " << floor << endl
				<< "Number of rooms: " << number_rooms;
		}
		else
		{
			stream << floor << " " << number_rooms;
		}
		stream << endl << static_cast<const CEstate*>(this) << endl;
		return stream;
	}

	friend istream& operator >> (istream& from, CFlat* obj)
	{
		from >> obj->floor >> obj->number_rooms >> static_cast<CEstate*>(obj);
		return from;
	}

private:
	int number_rooms;
	int floor;
};

class CHouse : public CEstate{
public:
	CHouse() { number_rooms = floors = space_yard = 0; }

	int get_number_rooms() const  { return number_rooms; }
	void set_number_rooms(int nr) { number_rooms = nr; }
	int get_floors() const        { return floors; }
	void set_floors(int f)        { floors = f; }
	int get_space_yard() const    { return space_yard; }
	void set_space_yard(int sy)   { space_yard = sy; }
	EstateType getType() const    { return house; }

	ostream& print(ostream& stream) const
	{
		if (&stream == &cout)
		{
			stream << "|HOUSE|" << endl
				<< "Floors: " << floors << endl
				<< "Number of rooms: " << number_rooms << endl
				<< "Yard space: " << space_yard;
		}
		else
		{
			stream << floors << " " << number_rooms << " " << space_yard;
		}
		stream << endl << static_cast<const CEstate*>(this) << endl;
		return stream;
	}

	friend istream& operator >> (istream& from, CHouse* obj)
	{
		from >> obj->floors >> obj->number_rooms >> obj->space_yard >> static_cast<CEstate*>(obj);
		return from;
	}
private:
	int number_rooms;
	int floors;
	int space_yard;
};

class CPlot : public CEstate {
public:
	list<Communications> get_comms() const  { return communications; }
	void set_comms(list<Communications> cm) { communications = cm; }
	EstateType getType() const              { return plot; }

	void commsToBools(bool* comms) const //ot comunications list-a vdigame flagovete v podadeniq masiv tam kudeto ima suotvetstvashta komunikaciq
	{
		list<Communications>::const_iterator it;
		for (it = communications.cbegin(); it != communications.cend(); it++)
			comms[*it] = true;
	}

	void boolsToComms(bool *comms) //ot podaden masiv ot bool stoinosti se obraziuva communications list-a
	{
		for (size_t i = 0; i < 5; i++)
		{
			if (comms[i])
				communications.push_back(static_cast<Communications>(i));
		}
	}

	string commsToString() const //vrushta stringova interpretaciq na elementite v communications
	{
		string str;
		for (Communications c : communications)
		{
			switch (c)
			{
			case water:
				str += "Water ";
				break;
			case electricity:
				str += "Electricity ";
				break;
			case telephone:
				str += "Telephone ";
				break;
			case road:
				str += "Road ";
				break;
			case sewerage:
				str += "Sewerage";
				break;
			default:
				break;
			}
		}
		return str;
	}

	ostream& print(ostream& stream) const
	{
		if (&stream == &cout) {
			stream << "|PLOT|" << endl
				<< "Communications: " << commsToString();
		}
		else{
			bool comms[5];
			commsToBools(comms);
			copy(comms, comms + 5, ostream_iterator<bool>(stream, " "));
		}
		stream << endl << static_cast<const CEstate*>(this) << endl;

		return stream;
	}

	friend istream& operator >> (istream& from, CPlot* obj)
	{
		bool vals[5];
		for (size_t i = 0; i < 5; i++)
			from >> vals[i];
		obj->boolsToComms(vals);
		from >> static_cast<CEstate*>(obj);
		return from;
	}
private:
	list<Communications> communications;
};

class CRealEstate{
public:
	CRealEstate(char* filename)
	{
		ifstream file(filename);
		if (file.is_open())
		{
			int type;
			while (!file.eof() && file >> type) //dokoato ne sme stignali kraq na faila i idva int koito da polzvame za identivikator na obekta
			{
				switch (static_cast<EstateType>(type)) //proverqvame kakvo chetem
				{
				case flat:
					properties.push_back(new CFlat());
					file >> dynamic_cast<CFlat*>(properties.back());
					break;
				case house:
					properties.push_back(new CHouse());
					file >> dynamic_cast<CHouse*>(properties.back());
					break;
				case plot:
					properties.push_back(new CPlot());
					file >> dynamic_cast<CPlot*>(properties.back());
					break;
				default:
					throw "Invalid property";
					break;
				}
			}
			file.close();
		}
		else
			throw  "Unable to open the file.";
	}

	~CRealEstate() //destruktor
	{
		//Trqbva da iztriem vsichko koeto sme zapisali v dinamichnata pamet (izpolzvali sme operatora new)
		for (size_t i = 0; i < properties.size(); i++)
		{
			delete properties[i]; //osvobojdavame pametta
			properties[i] = nullptr; // pravim obekta da sochi kum NULL pointer za da ne se opitva OS da osvobojdava povtorno
		}
	}

	vector<CEstate*> estateByType(const EstateType& type) const //tursene po vid na imota
	{
		vector<CEstate*> vec;
		for (size_t i = 0; i < properties.size(); i++)
		{
			if ((properties[i])->getType() == type)
				vec.push_back(properties[i]);
		}
		return vec;
	}

	vector<CEstate*> estateByPrice(const int& min, const int& max) const //tursene po cenova ramka
	{
		vector<CEstate*> vec;
		for (size_t i = 0; i < properties.size(); i++)
		{
			if ((properties[i])->get_price() >= min && (properties[i])->get_price() <= max)
				vec.push_back(properties[i]);
		}
		return vec;
	}

	//tursene po dadeni komunikacii
	vector<CEstate*> estateByComms(const bool& water, const bool& electricity, const bool& telephone, const bool& road, const bool& sewerage) const
	{
		vector<CEstate*> vec;
		bool arr[5] = { water, electricity, telephone, road, sewerage }; //slagame argumentite v masiv za po-lesno
		bool arr2[5] = { false }; //dopulnitelen masiv
		for (size_t i = 0; i < properties.size(); i++)
		{
			if ((properties[i])->getType() == plot)
			{
				dynamic_cast<CPlot*>(properties[i])->commsToBools(arr2); //pulnim vtoriqt masiv
				if (equal(arr, arr + 5, arr2)) //proverqvame dali dvata masiva sa ednakvi
					vec.push_back(properties[i]);
			}

		}
		return vec;
	}

	vector<CEstate*> estateByType_priceDesc(EstateType type) const //tursene po daden tip i narejdane po cena v nizhodqsht red
	{
		vector<CEstate*> vec = estateByType(type);
		if (vec.size() > 1) //ako ima poveche ot 1 imot trqbva da sortirame
		{
			sort(vec.begin(), vec.end(),
				[](CEstate* a, CEstate* b) { return a->get_price() > b->get_price(); } //sortirame po cena v nizhodqsht red s lambda expression
			);
		}
		return vec;
	}

	vector<CEstate*> estateByAddress(string address) const //tursene po adres
	{
		vector<CEstate*> vec;
		string addr;
		for (size_t i = 0; i < properties.size(); i++)
		{
			addr = properties[i]->get_address();
			if (addr.find(address) != -1)
				vec.push_back(properties[i]);
		}
		return vec;
	}

	friend ostream& operator << (ostream& to, const CRealEstate& obj)
	{
		vector<CEstate*>::const_iterator it = obj.properties.cbegin();
		for (it; it != obj.properties.cend(); it++)
			(*it)->print(to);
		return to;
	}
private:
	vector<CEstate*> properties;
};


int main()
{
	CRealEstate obj("props.txt");
		int choice,min1,max1,type1,type2;
		string address1;
		bool water1, electricity1, telephone1, road1, sewerage1;
		vector<CEstate*> v = obj.estateByComms(water1, electricity1, telephone1, road1, sewerage1);
		vector<CEstate*> v1 = obj.estateByPrice(min1, max1);
		vector<CEstate*> v2 = obj.estateByAddress(address1);
		vector<CEstate*> v3 = obj.estateByType(EstateType::house);
		vector<CEstate*> v4 = obj.estateByType(EstateType::flat);
		vector<CEstate*> v5 = obj.estateByType(EstateType::plot);
		vector<CEstate*> v6 = obj.estateByType_priceDesc(house);
		vector<CEstate*> v7 = obj.estateByType_priceDesc(flat);
		vector<CEstate*> v8 = obj.estateByType_priceDesc(plot);
	cout<<" Menu:"<<endl;
	while(true)
	{
		cout<<" 1. Read file."<<endl;
		cout<<" 2. Search estate by type."<<endl;
		cout<<" 3. Search estate by price."<<endl;
		cout<<" 4. Search estate by communication." << endl;
		cout<<" 5. Search estate by type_price." << endl; 
		cout<<" 6. Search estate by address. " << endl;
		cout<<" 7. Exit."<<endl;
		cout<<" Your Choice: ";
		cin>>choice;
		switch(choice)
		{
		case 1:	 
			cout << obj;
			break;
		case 2:	
			cout << "Enter the type you want (1=house, 2=flat, 3=plot) "; 
			cin >> type1;
			
			if (type1 == 1)
			{
				 v3 = obj.estateByType(house);
			      if (v3.size() > 0)
			        for (size_t i = 0; i < v3.size(); i++)
			          {
				        v3[i]->print(cout);
			          }
			           else
				         cout << "There is no house" << endl; 
				  break;
			}
			if (type1 == 2)
			{
					v4 = obj.estateByType(EstateType::flat);
			      if (v4.size() >0)
			       for (size_t i = 0; i < v4.size(); i++)
			         {
				       v4[i]->print(cout);
			         }
			      else
				    cout << "There is no flat" << endl;
				  break;
			} 
			if (type1 == 3)
			{
				v5 = obj.estateByType(EstateType::plot);
			     	 if (v5.size() >0)
				      for (size_t i = 0; i < v5.size(); i++)
				        {
					      v5[i]->print(cout);
				        }
				      else
					   cout << "There is no plot" << endl;
					 break;
			}
			break;
		case 3:
			cout << "Enter min price" << endl;
			cin >> min1;
			cout << "Enter max price" << endl;
			cin >> max1;
			v1 = obj.estateByPrice(min1,max1);
			if (v1.size() > 0)
			for (size_t i = 0; i < v1.size(); i++)
			{
				v1[i]->print(cout);
			}
			else
				cout << "There is no such property price." << endl;
			break;
		case 4: 
			cout << "Enter (1 if you want to have it or 0 if you don't) for water, electricity, telephone, road, sewerage ;"<<endl;  
			cout << "water "; cin >> water1; 
			cout << "electricity "; cin >> electricity1; 
			cout << "telephone "; cin >> telephone1; 
			cout << "road "; cin >> road1; 
			cout << "sewerage "; cin >> sewerage1; 
			v = obj.estateByComms(water1, electricity1, telephone1, road1, sewerage1); 
			if (v.size() > 0)
			for (size_t i = 0; i < v.size(); i++)
			{
				v[i]->print(cout);
			}
			else
				cout << "There is no such plot" << endl;
			break;
		case 5: 
			cout << "Enter the type you want (1=house, 2=flat, 3=plot) ";
			cin >> type2;
			if (type2 == 1)
			{
				v6 = obj.estateByType_priceDesc(house);
			if (v6.size() > 0)
			for (size_t i = 0; i < v6.size(); i++)
			{
				v6[i]->print(cout);
			}
			else
				cout << "There is no house" << endl;
			}
			break;
		    if (type2 == 2)
		     {
				v7 = obj.estateByType_priceDesc(flat);
			if (v7.size() >0)
			for (size_t i = 0; i < v7.size(); i++)
			{
				v7[i]->print(cout);
			}
			else
				cout << "There is no flat" << endl;
			} break;
				if (type2 == 3)
					{
					v8 = obj.estateByType_priceDesc(plot);
				if (v8.size() >0)
				for (size_t i = 0; i < v8.size(); i++)
				{
					v8[i]->print(cout);
				}
				else
					cout << "There is no plot" << endl;
			}
			break;
		case 6: 
			cout << "Enter address "; cin >> address1;
			v2 = obj.estateByAddress(address1);
			if (v2.size() > 0)
			for (size_t i = 0; i < v2.size(); i++)
			{
				v2[i]->print(cout);
			}
			else
				cout << "There is no such address" << endl;
			break;
		case 7: break;
		default: cout<<" Wrong input, (1~7)"<<endl;
		}
		if(choice==7)
			break;
	}
	system("pause");
	return 0;
} 
