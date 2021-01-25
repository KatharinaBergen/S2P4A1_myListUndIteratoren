#ifndef MYLIST_H
#define MYLIST_H

template<class Elem> struct MyNode // "fuer alle Typen Elem"
{
	MyNode<Elem>* pre; // Zeiger auf vorhergehenden Knoten (predecessor)
	MyNode<Elem>* suc; // Zeiger auf nachfolgenden Knoten (successor)
	Elem val; // der Wert vom Typ Elem
	explicit MyNode(const Elem& v = Elem{},
		MyNode<Elem>* p = nullptr,
		MyNode<Elem>* s = nullptr) // Standardkonstruktor
		: pre{ p }, suc{ s }, val{ v }
	{}
};

template<class Elem> class MyList
{
private:
	long int size; // Anzahl der Listenknoten
	MyNode<Elem> first; // Waechterknoten: Listenanfang (im Stack)
	MyNode<Elem> last; // Waechterknoten: Listenende (im Stack)
public:
	MyList();
	~MyList();
	void push_back(const Elem& v);
	void pop_back();

	class MyIterator; // Vorabdeklaration: Typ myIterator, als "nested class"
	MyIterator begin(); // Methode, die myIterator auf erstes Element liefert
	MyIterator end(); // liefert myIterator auf "eins hinter letztem Element"
	MyIterator find(const Elem&, MyIterator, MyIterator); // finden
	MyIterator insert(const Elem&, MyIterator); // einfuegen
	MyIterator remove(MyIterator); // entfernen

	void push_front(const Elem&); // neuen Knoten am Anfang einfuegen
	void pop_front(); // Knoten am Anfang loeschen
	void ins(const Elem&, MyNode<Elem>*); // vor Knoten p einfuegen
	void rem(MyNode<Elem>* p); // Knoten p loeschen
};

template<class Elem> class MyList<Elem>::MyIterator
{
	MyNode<Elem>* curr; // der aktuelle Knoten
public:
	MyIterator(const MyIterator&);
	explicit MyIterator(MyNode<Elem>*); // zum Debuggen
	MyNode<Elem>* get_curr() const { return curr; } // zum Debuggen
	// die Iterator-Operationen:
	MyIterator& operator++(); // ++it
	MyIterator& operator--(); // --it
	MyIterator operator++(int); // it++
	MyIterator operator--(int); // it--
	Elem& operator*();
	bool operator==(const MyIterator&) const;
	bool operator!=(const MyIterator&) const;
};


////////////////////////////// myList /////////////////////////////

template<class Elem> MyList<Elem>::MyList()
	: size{ 0L }, first{}, last{}
{
	first.suc = &last;
	last.pre = &first;
}

template<class Elem> MyList<Elem>::~MyList()
{
	MyNode<Elem>* current{ first.suc };
	MyNode<Elem>* tmp{ nullptr };
	while (current != &last)
	{
		tmp = current->suc;
		delete current;
		current = tmp;
	}
}

template<class Elem> void MyList<Elem>::push_back(const Elem& v)
{
	MyNode<Elem>* nn{ new MyNode<Elem>{v} }; // neues Element aus dem Heap
	nn->suc = &last;
	nn->pre = last.pre;
	last.pre->suc = nn;
	last.pre = nn;
	++size;
}
template<class Elem> void MyList<Elem>::pop_back()
{
	MyNode<Elem>* tmp = last.pre;
	if (tmp == &first) return; // Liste ist schon leer
	last.pre = last.pre->pre;
	last.pre->suc = &last;
	delete tmp; // nicht mehr gebrauchter Speicher dem Heap zurueckgeben
	tmp = nullptr;
	--size;
}


template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::begin()
{
	return MyIterator{ first.suc };
}

template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::end()
{
	return MyIterator{ &last };
}

template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::find(const Elem& v, MyIterator p, MyIterator q)
{
	while ((p != q) && (*p != v)) ++p;
	return p;
}

template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::insert(const Elem& v, MyIterator p)
{
	ins(v, p.get_curr());

	return --p;
}

template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::remove(MyIterator p)
{
	MyNode<Elem>* tmp = p.get_curr();
	p--;
	rem(tmp);

	return p;
}

template<class Elem>
void MyList<Elem>::push_front(const Elem& v)
{
	MyNode<Elem>* nn{ new MyNode<Elem> {v} };
	MyNode<Elem>* temp{ first.suc };

	first.suc = nn;
	nn->pre = &first;
	nn->suc = temp;
	temp->pre = nn;
}

template<class Elem>
void MyList<Elem>::pop_front()
{
	MyNode<Elem>* temp = first.suc;

	first.suc = first.suc->suc;
	first.suc->pre = &first;

	delete temp;
	temp = nullptr;
}

template<class Elem>
void MyList<Elem>::ins(const Elem& v, MyNode<Elem>* p)
{
	MyNode<Elem>* nn{ new MyNode<Elem>{v} };

	MyNode<Elem>* start{ begin().get_curr()};
	while(true)
	{
		if (start->suc == p) break;
		start = start->suc;
	}

	nn->suc = p;
	p->pre = nn;
	nn->pre = start;
	start->suc = nn;

	size++;
}

template<class Elem>
void MyList<Elem>::rem(MyNode<Elem>* p)
{

	p->pre->suc = p->suc;
	p->suc->pre = p->pre;

	delete p;
	p = nullptr;

	size--;
}




////////////////////////// myIterator ///////////////////////////////

// Kopierkonstruktor
template<class Elem>
MyList<Elem>::MyIterator::MyIterator(const MyIterator& other)
	: curr{ other.curr }
{}
// weiterer Konstruktor (v.a. zum Debuggen und Testen)
template<class Elem>
MyList<Elem>::MyIterator::MyIterator(MyNode<Elem>* p)
	: curr{ p }
{}

// ++it
template<class Elem>
typename MyList<Elem>::MyIterator& MyList<Elem>::MyIterator::operator++()
{
	curr = curr->suc;
	return *this;
}
// it++
template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::MyIterator::operator++(int)
{
	MyIterator tmp{ *this };
	curr = curr->suc;
	return tmp;
}

template<class Elem>
typename MyList<Elem>::MyIterator& MyList<Elem>::MyIterator::operator--()
{
	curr = curr->pre;
	return *this; 
}

template<class Elem>
typename MyList<Elem>::MyIterator MyList<Elem>::MyIterator::operator--(int)
{
	MyIterator tmp{ *this };
	curr = curr->pre;
	return tmp;
}

template<class Elem>
Elem& MyList<Elem>::MyIterator::operator*()
{
	return &this;
}

template<class Elem>
bool MyList<Elem>::MyIterator::operator==(const MyList<Elem>::MyIterator& other) const
{
	return curr == other.curr;
}

template<class Elem>
bool MyList<Elem>::MyIterator::operator!=(const MyList<Elem>::MyIterator& other) const
{
	return curr != other.curr;
}





#endif // !MYLIST_H
