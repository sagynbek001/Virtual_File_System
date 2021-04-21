//============================================================================
// Name         : Source.cpp
// Author       : Sagynbek Talgatuly st4121
// Version      : 4.0
// Date Created : 29/03/2021
// Date Modified: 10/04/2021
// Copyright    : All rights are reserved
// Description  : Assignment 2. Virtual File system
//============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <time.h>
#include <stdio.h>

using namespace std; 

//exception class
class MyExceptions {
private:
	string errorMessage;
public:
	MyExceptions(const string& error) : errorMessage(error) {}
	string getError() { return errorMessage; }
};

//Needed data sctructures such as a vector, a queue, a stack are implemented below
//vector is implemented using an array
//queue and stack are implemented using a linked list

template <typename E> class QNode
{
private:
	E elem;			//data element (Generic type E in this case)
	QNode* next;	//Link (pointer) to the next Node

public:
	QNode(E elem) : elem(elem), next(NULL) {}
	template <typename T> friend class MyLinkedList;
};

template <typename T> class MyLinkedList
{
private:
	QNode<T>* head; // pointer to the head of list
	QNode<T>* tail; // pointer to the tail of the list
public:
	MyLinkedList(); // empty list constructor
	~MyLinkedList(); // destructor to clean up all nodes
	bool empty() const; // is list empty?
	T& front(); // get front element
	void addBack(const T& elem); // add element to back of list
	void removeFront(); // remove front item list
};

template <typename T> class Queue
{
private:
	MyLinkedList<T> list;
	int s = 0;
public:
	int size();
	void push(T);
	T& front();
	void pop();
	bool empty();
};

template <typename T> MyLinkedList<T>::MyLinkedList() {
	tail = NULL;
	head = tail;
};
template <typename T> MyLinkedList<T>::~MyLinkedList() {
	while (!empty()) removeFront();
};
template <typename T> bool MyLinkedList<T>::empty() const {
	return head == NULL;
};
template <typename T> T& MyLinkedList<T>::front() {
	return head->elem;
};
template <typename T> void MyLinkedList<T>::addBack(const T& elem) {
	QNode<T>* v = new QNode<T>(elem);
	if (empty()) {
		tail = v;
		head = v;
	}
	else {
		tail->next = v;
		tail = v;
	}
};
template <typename T> void MyLinkedList<T>::removeFront() {
	if (!empty()) {
		QNode<T>* old = head;
		head = old->next;
		delete old;
	}
};
template<typename T> void Queue<T>::push(T element) {
	list.addBack(element);
	s++;
};
template<typename T> void Queue<T>::pop() {
	list.removeFront();
	s--;
};
template<typename T> T& Queue<T>::front() {
	return list.front();
};
template<typename T> bool Queue<T>::empty() {
	return list.empty();
};
template<typename T> int Queue<T>::size() {
	return s;
};

template <typename T> class MyVector
{
private:
	T* data;						//pointer to int(array) to store elements
	int v_size;						//current size of vector (number of elements in vector)
	int v_capacity;					//capacity of vector
public:
	MyVector();						//No argument constructor
	MyVector(int cap);				//One Argument Constructor
	~MyVector();					//Destructor
	void push_back(T element);		//Add an element at the end of vector
	void insert(int index, T element);
	void erase(int index);          //Erase an element with the given index
	T& operator[](int index);
	T& at(int index); 				//return reference to the element at index
	const T& front();				//Returns reference to the first element in the vector
	const T& back();				//Returns reference to the Last element in the vector
	int size() const;				//Return current size of vector
	int capacity() const;			//Return capacity of vector
	bool empty() const; 			//Rturn true if the vector is empty, False otherwise
};
template <typename T> MyVector<T>::MyVector()
{
	data = new T[0];
	v_size = 0;
	v_capacity = 0;
}
template <typename T> MyVector<T>::MyVector(int cap)
{
	data = new int[cap];
	v_capacity = cap;
	v_size = 0;
}
template <typename T> MyVector<T>::~MyVector()
{
	delete[] data;
}
template <typename T> void MyVector<T>::push_back(T element)
{
	//doing almost the same as in the insert fn
	//if it needs to be resized we resize it
	//otherwise we just set the last element of the array
	if (v_size >= v_capacity) {
		int newcapacity = 0;
		if (v_capacity == 0)
			newcapacity = 1;
		else newcapacity = 2 * v_capacity;
		T* newdata = new T[newcapacity];
		for (int j = 0; j < v_size; j++)
			newdata[j] = data[j];
		if (data != NULL) delete[] data;
		data = newdata;
		v_capacity = newcapacity;
	}
	data[v_size] = element;
	v_size++;
}
template <typename T> void MyVector<T>::insert(int index, T element)
{
	if (index > v_size - 1) throw MyExceptions("Index is out of range");
	//increasing the size of the array by copy-pasting
	if (v_size >= v_capacity) {
		int newcapacity = 0;
		if (v_capacity == 0)
			newcapacity = 1;
		else newcapacity = 2 * v_capacity;
		T* newdata = new T[newcapacity];
		for (int j = 0; j < v_size; j++)
			newdata[j] = data[j];
		//clearing the old array
		if (data != NULL) delete[] data;
		data = newdata;
		v_capacity = newcapacity;
	}
	//moving the elements one step to the right
	for (int j = v_size - 1; j >= index; j--)
		data[j + 1] = data[j];
	data[index] = element;
	v_size++;
}
template <typename T> void MyVector<T>::erase(int index)
{
	for (int j = index; j < v_size-1; j++)
		data[j] = data[j+1];
	v_size--;
}
template <typename T> T& MyVector<T>::operator[](int index)
{
	return data[index];
}
template <typename T> T& MyVector<T>::at(int index)
{
	if (!empty())
		if (index >= 0 and index < v_size)
			return data[index];
		else throw MyExceptions("Out of range");
	else throw MyExceptions("The vector is empty");
}
template <typename T> const T& MyVector<T>::front()
{
	if (!empty())
		return data[0];
	else throw MyExceptions("The vector is empty");
}
template <typename T> const T& MyVector<T>::back()
{
	if (!empty())
		return data[v_size - 1];
	else throw MyExceptions("The vector is empty");
}
template <typename T> int MyVector<T>::size() const
{
	return v_size;
}
template <typename T> int MyVector<T>::capacity() const
{
	return v_capacity;
}
template <typename T> bool MyVector<T>::empty() const
{
	return v_size == 0;
}

template <typename T> class LinkedList;

template <typename T> class Node {
private:
	T elem;
	Node<T>* next;
	friend class LinkedList<T>;
};

template <typename T> class LinkedList {
private:
	Node<T>* head;
public:
	LinkedList() { head = NULL; };
	~LinkedList() { while (!empty()) removeFront(); };
	void removeFront()
	{
		Node<T>* old = head;
		head = old->next;
		delete old;
	}
	void addFront(const T& e) {
		Node<T>* v = new Node<T>;
		v->elem = e;
		v->next = head;
		head = v;
	}
	bool empty() const { return head == NULL; }
	T& front() { return head->elem; }
};

template <typename T> class CStack {
public:
	CStack();
	~CStack() { while (!S->empty()) S->removeFront(); }
	int size() const;
	bool empty() const;
	T& top();
	void push(const T& e);
	void pop();
private:
	LinkedList<T>* S;
	int n;
};
template <typename T> CStack<T>::CStack() {
	S = new LinkedList<T>;
	n = 0;
}
template <typename T> int CStack<T>::size() const
{
	return n;
}
template <typename T> bool CStack<T>::empty() const {
	return n == 0;
}
template <typename T> T& CStack<T>::top() {
	if (empty()) 
		throw MyExceptions("Empty stack");
	else {
		return S->front();
	}
}
template <typename T> void CStack<T>::push(const T& e)
{
	++n;
	S->addFront(e);
}
template <typename T> void CStack<T>::pop()
{
	if (empty()) throw MyExceptions("The stack is empty");
	else {
		--n;
		S->removeFront();
	}
}
//a class for files/folders
class inode {
	public:
		bool isFolder;
		int size;
		string name;
		string date;

		inode* parent;
		MyVector <inode*> children;

		inode(string name, bool isFolder, int size, string date) 
		{
			parent = NULL;
			this->name = name;
			this->isFolder = isFolder;
			this->size = size;
			this->date = date;
		}
};
//a tree data structure 
class Tree {
	public:
		Tree(inode* root)
		{
			rootnode = root;
			allnodes.push_back(root);
		}
		bool empty() 
		{
			return allnodes.empty();
		}
		int size() 
		{
			return allnodes.size();
		}
		inode* root() 
		{
			return rootnode;
		}
		inode* rootnode;
		MyVector <inode*> allnodes;
};

string pwd(inode* &current) {
	if (current->parent == NULL)
		return "/";
	string output = "";
	//we use a stack to "reverse" the path from the current inode to the root
	CStack <inode*> pwdstack;
	inode* c = current;
	while (c->parent != NULL)
	{
		pwdstack.push(c);
		c = c->parent;
	}
	while (!pwdstack.empty())
	{
		output += "/" + pwdstack.top()->name;
		pwdstack.pop();
	}
	return output;
}

bool isOkFile(string nodename) {
	//files are checked whether they are alphanumeric 
	//and contains exactly one dot
	if (nodename == "")
		return false;
	if (nodename[0] == '.')
		return false;
	if (nodename[nodename.length() - 1] == '.')
		return false;
	for (char c : nodename)
		if (!isalnum(c) && c != '.')
			return false;
	int numofdots = 0;
	for (char c : nodename)
		if (c == '.')
			numofdots++;
	return numofdots == 1;
}

bool isOkFolder(string nodename) {
	//folders must be alphanumeric only
	if (nodename == "")
		return false;
	for (char c : nodename)
		if (!isalnum(c))
			return false;
	return true;
}

bool isPath(string path) {
	if (path == "")
		return false;
	if (path[0] != '/')
		return false;
	//path can have slashes, one dot at most, and alphanumeric letters
	if (path[0] != '/')
		return false;
	for (char c : path)
		if (!isalnum(c) && c != '.' && c!='/')
			return false;
	int numofdots = 0;
	for (char c : path)
		if (c == '.')
			numofdots++;
	return numofdots <= 1;
}

void splitter(string const &str, const char delimeter, MyVector<string>& out)
{
	//this function splits the string into parts using
	//a specific delimeter variable
	stringstream ss(str);
	string s;
	while (getline(ss, s, delimeter)) {
		out.push_back(s);
	}
}

string date() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	char date[10];
	strftime(date, 10, "%d-%m-%y", ltm);
	return date;
}

inode* stringtoinode(bool create, Tree* &T, string path, int size, string date) {
	if (!isPath(path))
		return NULL;
	if (path == "/")
		return T->root();
	inode* current = T->root();
	stringstream s(path);
	string step;
	int numberofslashes = 0;
	for (char c : path)
		if (c == '/')
			numberofslashes++;
	getline(s, step, '/');
	for (int i = 0; i < numberofslashes - 1; i++) {
		getline(s, step, '/');
		for (int i=0; i<current->children.size(); i++)
			if (current->children[i]->name == step)
				current = current->children[i];
	}
	getline(s, step);
	inode* found = NULL;
	for (int i = 0; i < current->children.size(); i++)
		if (current->children[i]->name == step)
			found = current->children[i];
	if (found == NULL) {
		if (create) {
			inode* newnode = new inode(step, isOkFolder(step), size, date);
			current->children.push_back(newnode);
			newnode->parent = current;
			current = newnode;
		}
		else
			current = NULL;
	}
	else {
		current = found;
	}
	return current;
}

void load(Tree* &T, string path) {
	ifstream fin(path);
	if (fin.is_open())
	{
		string line;
		string name;
		string size;
		string date1;
		getline(fin, line);
		const char delim = ',';
		MyVector<string> out;
		splitter(line, delim, out);
		name = out[0];
		size = out[1];
		date1 = out[2];
		inode* root = new inode("root", true, 0, date1);
		T = new Tree(root);
		while (!fin.eof()) {
			getline(fin, line, '\n');
			const char delim = ',';
			MyVector<string> out;
			splitter(line, delim, out);
			name = out[0];
			size = out[1];
			date1 = out[2];
			stringtoinode(true, T, name, stoi(size), date1);
		}
	}
	else
	{
		throw MyExceptions("Invalid path to the database");
	}
	fin.close();
}

void save(inode* &current, string path) {
	ofstream fout(path, ios::app);
	if (fout.is_open())
	{
		fout << pwd(current) << "," << current->size << "," << current->date;
		for (int i = 0; i < current->children.size(); i++) {
			fout << endl;
			save(current->children[i], path);
		}
	}
	else
	{
		throw MyExceptions("Invalid path to the database");
	}
	fout.close();
}

void ShowCommands()
{
	cout << "List of available Commands:" << endl
		<< "0  -> help                        \t: Display the list of available commands" << endl
		<< "1  -> pwd                         \t: Prints the path of the current inode" << endl
		<< "2  -> realpath <file/fodlername>  \t: Prints the full path of a given file in the current node" << endl
		<< "3  -> ls                          \t: Prints the children of the current inode" << endl
		<< "4  -> ls sort                     \t: Prints the children of the current inode sorted in a descending order" << endl
		<< "5  -> mkdir <foldername>          \t: Creates a folder under the current folder" << endl
		<< "6  -> touch <filename> <size>     \t: Creates a file under the current inode" << endl
		<< "7  -> cd                          \t: Changes current inode to the specified folder or folderpath" << endl
		<< "8  -> find <file/foldername>      \t: Returns the path of the files (or the folders) if they exist" << endl
		<< "9  -> mv filename foldername      \t: Moves a file/folder to some other folder" << endl
		<< "10 -> rm file/foldername          \t: Removes the specified folder or file and puts it in a Queue of size 10" << endl
		<< "11 -> size file/foldername        \t: Returns the total size of the folder or a file" << endl
		<< "12 -> emptybin                    \t: Empties the bin" << endl
		<< "13 -> showbin                     \t: Shows the oldest inode of the bin (including its path)" << endl
		<< "14 -> recover                     \t: Reinstates the oldest inode back from the bin" << endl
		<< "15 -> clear                       \t: Clear the screen" << endl
		<< "!  -> exit                        \t: Exit the Program" << endl;
}

void recover(Tree* &T, Queue <inode*>* &bin) {
	//necessary validations are made
	if (bin->size() == 0)
		throw MyExceptions("The bin is empty");
	inode* torecover = bin->front();

	//function stringtonode is user to find out if the path still exists
	if (stringtoinode(false, T, pwd(torecover->parent), 0, "") == NULL)
		throw MyExceptions("The path doesn't exist anymore");
	if (stringtoinode(false, T, pwd(torecover), 0, "") != NULL)
		throw MyExceptions("There is a folder/file with the same name");

	else
	{
		stringtoinode(false, T, pwd(torecover->parent), 0, "")->children.push_back(torecover);
		torecover->parent = stringtoinode(false, T, pwd(torecover->parent), 0, "");
		bin->pop();
	}	
}
//calculatesize function works recursively (preorder traversal)
int calculatesize(inode* c) {
	if (c->children.empty())
		return c->size;
	else {
		int sum = c->size;
		for (int i=0; i<c->children.size(); i++)
			sum += calculatesize(c->children[i]);
		return sum;
	}
}

void size(inode* current, string nodename, Tree* &T) {
	//first we assume that the nodename parameter is a foldername
	//if it is we calculate its size and return
	if (nodename == "")
		throw MyExceptions("Invalid parameter(s)");
	for (int i = 0; i < current->children.size(); i++)
	{
		if ((current->children[i])->name == nodename) {
			cout << calculatesize(current->children[i]) << " bytes" << endl;
			return;
		}
	}
	//else we go here
	//we check if this path exists
	if (stringtoinode(false, T, nodename, 0, "") == NULL)
		throw MyExceptions("This path doesn't exist");
	else
	{
		//if it does then we calculate it
		cout << calculatesize(stringtoinode(false, T, nodename, 0, "")) << " bytes" << endl;
		return;
	}
}

void showbin(Queue <inode*>* &bin) {
	if (bin->empty()) {
		cout << "The bin is empty" << endl;
		return;
	}
	cout << "Next element to remove: ";
	cout << pwd(bin->front()) << " ";
	cout << "(" << bin->front()->size << " bytes, " << bin->front()->date << ")" << endl;
}

void emptybin(Queue <inode*>* &bin) {
	//popping unitl it's empty
	if (bin->empty()) {
		cout << "The bin is already empty" << endl;
		return;
	}
	while (!bin->empty())
		bin->pop();
	cout << "The bin was emptied succesfully!" << endl;
}

void rm(Tree* &T, string parameter, inode* &current, Queue <inode*>* &bin) {
	//validations
	if (parameter == "")
		throw MyExceptions("Invalid parameter(s)");
	if (bin->size() >= 10)
		throw MyExceptions("The bin is full");
	if (parameter == "/")
		throw MyExceptions("Deleting the root is impossible");
	//looking for the file/folder
	for (int i = 0; i < current->children.size(); i++)
	{
		if (current->children[i]->name == parameter) {
			//deleting it
			bin->push(current->children[i]);
			current->children.erase(i);
			return;
		}
	}
	//if it wasn't a folder/file name but a path
	//then we must go here
	inode* todelete = stringtoinode(false, T, parameter, 0, "");
	//if this path is ok and it is not a root then we delete it
	if (todelete != NULL && todelete != T->rootnode) {
		for (int i = 0; i < todelete->parent->children.size(); i++) {
			if (todelete->parent->children[i]->name == todelete->name) {
				bin->push(todelete->parent->children[i]);
				todelete->parent->children.erase(i);
				return;
			}
		}
	}
	//otherwise we throw an exception
	throw MyExceptions("the file/folder wasn't found or the input is invalid");
}

void mv(Tree* &T, string filename, string foldername, inode* &current) {
	if (foldername == "" || filename == "")
		throw MyExceptions("Invalid parameter(s)");
	inode* folder = NULL;
	inode* file = NULL;
	//first we find the folder
	//we validate it
	//then we find the file/folder
	//we validate it
	if (isPath(foldername)) {
		if (stringtoinode(false, T, foldername, 0, "") != NULL) {
			inode* q = stringtoinode(false, T, foldername, 0, "");
			if (q->isFolder)
				folder = q;
			else
				throw MyExceptions("Moving into a file is impossible");
		}
		else
			throw MyExceptions("The folder wasn't found");
	}
	else {
		if (isOkFile(foldername))
			throw MyExceptions("Moving into a file is impossible");
		if (isOkFolder(foldername))
			for (int i = 0; i < current->children.size(); i++) {
				if ((current->children[i])->name == foldername) {
					folder = current->children[i];
					break;
				}
			}
		else
			throw MyExceptions("Invalid parameter(s)");
		if (folder == NULL)
			throw MyExceptions("The folder wasn't found");
	}
	if (folder != NULL) {
		for (int i = 0; i < folder->children.size(); i++)
			if (folder->children[i]->name == filename)
				throw MyExceptions("The folder already contains a file with this name");
	}
	if (isPath(filename)) {
		if (stringtoinode(false, T, filename, 0, "") != NULL)
			if (stringtoinode(false, T, filename, 0, "") != T->rootnode) {
				file = stringtoinode(false, T, filename, 0, "");
				for (int i = 0; i < file->parent->children.size(); i++) {
					if ((file->parent->children[i]) == file) {
						file->parent->children.erase(i);
						break;
					}
				}
			}
			else
				throw MyExceptions("The root cannot be moved");
		else
			throw MyExceptions("The file/folder to move wasn't found");
	}
	else {
		for (int i = 0; i < current->children.size(); i++) {
			if ((current->children[i])->name == filename) {
				file = current->children[i];
				current->children.erase(i);
				break;
			}
		}
		if (file == NULL)
			throw MyExceptions("The file/folder wasn't found");
	}
	if (folder != NULL && file != NULL)
	{
		folder->children.push_back(file);
		file->parent = folder;
	}
}
//find function works recursively (preorder traversal)
void find(inode* &c, string name) {
	if (name == "")
		throw MyExceptions("Invalid parameter(s)");
	for (int i = 0; i < c->children.size(); i++) {
		if (c->children[i]->name == name) {
			cout << pwd(c->children[i]) << " ";
			cout << "(" << c->children[i]->size << " bytes, " << c->children[i]->date << ")" << endl;
		}
		find(c->children[i], name);
	}
}

void cd(CStack <inode*> &history, inode* &current, string parameter, Tree* T) {
	//different parameters are done by if-else structure
	//we have a stack called history that stores all the previous folders we visited
	if (parameter == "..") {
		if (current->parent != NULL) 
		{
			history.push(current);
			current = current->parent;
		}
		cout << pwd(current);
		cout << endl;
		return;
	}
	//we pop the history stack only when we go back
	else if (parameter == "-") {
		if (history.empty()) {
			cout << pwd(current) << endl;
			return;
		}
		if (stringtoinode(false, T, pwd(history.top()), 0, "") == NULL) 
		{
			throw MyExceptions("The path doesn't exist anymore");
		}
		current = history.top();
		history.pop();
		cout << pwd(current);
		cout << endl;
		return;
	}
	else if (parameter == "") {
		history.push(current);
		current = T->root();
		cout << pwd(current);
		cout << endl;
		return;
	}
	else {
		//moving into a file and other things are validated
		if (!isPath(parameter)) 
		{
			for (int i = 0; i < current->children.size(); i++) {
				if (current->children[i]->name == parameter) {
					if (!current->children[i]->isFolder)
						throw MyExceptions("Moving into a file is impossible");
					history.push(current);
					current = current->children[i];
					cout << pwd(current);
					cout << endl;
					return;
				}
			}
			throw MyExceptions("Folder wasn't found or the input is invalid");
		}
		//if the parameter is a path then we validate it
		if (stringtoinode(false, T, parameter, 0, "") == NULL)
			throw MyExceptions("This path doesn't exist");
		if (!stringtoinode(false, T, parameter, 0, "")->isFolder)
			throw MyExceptions("Moving into a file is impossible");
		history.push(current);
		current = stringtoinode(false, T, parameter, 0, "");
		cout << pwd(current);
		cout << endl;
		return;
	}
	throw MyExceptions("Folder wasn't found or the input is invalid");
}

void touch(Tree* &T, inode* &current, string parameter1, string parameter2str) {
	//validation of the file's name
	if (!isOkFile(parameter1))
		throw MyExceptions("The name of the file must a have an extension and be alphanumeric");
	if (parameter2str == "")
		throw MyExceptions("Invalid parameter(s)");
	for (char c : parameter2str)
		if (!isdigit(c))
			throw MyExceptions("Invalid parameter(s)");
	int parameter2 = stoi(parameter2str);
	//validating if the file already exists
	for (int i = 0; i < current->children.size(); i++) {
		if (current->children[i]->name == parameter1) {
			throw MyExceptions("A file with this name already exists");
			return;
		}
	}
	//creating the file with the relevant parameters
	//initializing its members
	//adding it to the folder's children vector
	inode* newfile = new inode(parameter1, false, parameter2, date());
	current->children.push_back(newfile);
	newfile->parent = current;
}

void mkdir(Tree* &T, inode* &current, string parameter) {
	if (parameter == "")
		throw MyExceptions("Invalid parameter(s)");
	//validation of the folder's name
	if (!isOkFolder(parameter))
		throw MyExceptions("The name of the folder must be alphanumeric only");
	//validating if the folder already exists
	for (int i = 0; i < current->children.size(); i++) {
		if (current->children[i]->name == parameter) {
			throw MyExceptions("A folder with this name already exists");
			return;
		}
	}
	//creating the folder with the relevant parameters
	//initializing its members
	//adding it to the folder's children vector
	inode* newfolder = new inode(parameter, true, 10, date());
	current->children.push_back(newfolder);
	newfolder->parent = current;
}
//bubble sort algorithm
void bubblesorting(MyVector<inode*> &children) { 
	for (int i = children.size()-1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (children[j]->size < children[j + 1]->size) {
				inode* tmp = children[j];
				children[j] = children[j + 1];
				children[j + 1] = tmp;
			}
		}
	}
}

void ls(inode* &current, string sorting) {
	if (sorting != "sort" && sorting != "")
		throw MyExceptions("Invalid parameter(s)");
	//parameter sorting points to whether we need to sort the files
	if (!current->isFolder)
		throw MyExceptions("this inode must be a folder");
	if (current->children.empty())
		cout << endl;
	else
	{
		if (sorting == "sort")
			bubblesorting(current->children);
		for (int i = 0; i < current->children.size(); i++) {
			cout << ((current->children[i]->isFolder) ? "dir" : "file");
			cout << " " << current->children[i]->name << " ";
			cout << current->children[i]->date << " ";
			cout << current->children[i]->size << "bytes" << endl;
		}
	}
}

void realpath(inode* &current, string name) {
	if (name == "")
		throw MyExceptions("Invalid parameter(s)");
	//this funnction uses the pwd function but only for the children of 
	//the current inode
	for (int i = 0; i < current->children.size(); i++) {
		if (current->children[i]->name == name) {
			cout << pwd(current->children[i]);
			cout << endl;
			return;
		}
	}
	throw MyExceptions("the file/folder wasn't found");
}

void welcome() {
	//welcome message
	system("clear");
	cout << "Welcome to the virtual filesystem!" << endl;
	cout << "Please, make sure to read the next notes and instructions:" << endl;
	cout << endl;
	cout << "1. You can type the command itself or it's code instead" << endl;
	cout << "2. Foldernames are alphanumeric only and cannot be an empty string" << endl;
	cout << "3. Filenames are alphanumeric and must have an extension" << endl;
	cout << "4. Paths must start with / character" << endl;
	cout << "5. Whenever you input a path to file/folder for any of the commands (cd, mv, rm, size), " << endl;
	cout << "   it must start from a root. Example: if current folder is /123, path /124/125" << endl;
	cout << "   doesn't indicate a folder /123/124/125 but a folder/124/125 starting from the root" <<endl;
	cout << "6. Moving a folder to another folder is accepted, but moving a file/folder to a file is not" << endl;
	cout << "7. Folders cannot contain files/folders with the same name" << endl;
	cout << "8. Bin size is limited to 10. If it is full then exception is thrown" << endl;
	cout << "9. Going to previous folder many times using the cd - command doesn't loop. It goes back to the root." << endl;
	cout << endl;
	cout << "Press any key + enter to start" << endl;
	string input;
	cin >> input;
	system("clear");
	return;
}

int main() {
	//we create default root incase the database file is empty
	inode* defroot;
	Tree* filesystem;
	ifstream fin("vfs.dat");
	if (fin.peek() == ifstream::traits_type::eof()) 
	{
		string today = date();
		defroot = new inode("root", true, 0, today);
		filesystem = new Tree(defroot);
	}
	else
	{
		try
		{
			load(filesystem, "vfs.dat");
		}
		catch (MyExceptions& e)
		{
			cout << "Exception: " << e.getError() << endl << endl;
		}
	}

	//we create a history stack
	//we create a bin queue
	//also we make track of the current inode we are visiting
	Queue <inode*>* bin = new Queue <inode*>;
	inode* current = filesystem->rootnode;
	CStack <inode*> history;
	history.push(filesystem->root());

	welcome();
	ShowCommands();

	string user_input;
	string comm;
	string parameter1;
	string parameter2;

	do
	{
		string comm = "";
		string parameter1 = "";
		string parameter2 = "";
		cout << ">";
		getline(cin, user_input);
		stringstream s(user_input);
		if (user_input == "") { cout << '\b'; continue; }

		if (!s.eof()) {
			s >> comm;
		}
		if (!s.eof()) {
			s >> parameter1;
		}
		if (!s.eof()) {
			s >> parameter2;
		}

		try {
			if (comm == "help" || comm == "0") { system("clear"); ShowCommands(); }
			else if (comm == "pwd" || comm == "1") { cout << pwd(current); cout << endl; }
			else if (comm == "realpath" || comm == "2")	realpath(current, parameter1);
			else if (comm == "ls" || comm == "3")	ls(current, parameter1);
			else if (comm == "ls sort" || comm == "4")	ls(current, parameter1);
			else if (comm == "mkdir" || comm == "5") { mkdir(filesystem, current, parameter1); ls(current, ""); }
			else if (comm == "touch" || comm == "6") { touch(filesystem, current, parameter1, parameter2); ls(current, ""); }
			else if (comm == "cd" || comm == "7") { cd(history, current, parameter1, filesystem); }
			else if (comm == "find" || comm == "8")	find(filesystem->rootnode, parameter1);
			else if (comm == "mv" || comm == "9") { mv(filesystem, parameter1, parameter2, current); ls(current, ""); }
			else if (comm == "rm" || comm == "10") { rm(filesystem, parameter1, current, bin); ls(current, ""); }
			else if (comm == "size" || comm == "11") size(current, parameter1, filesystem);
			else if (comm == "emptybin" || comm == "12")	emptybin(bin);
			else if (comm == "showbin" || comm == "13")	showbin(bin);
			else if (comm == "recover" || comm == "14")	recover(filesystem, bin);
			else if (comm == "quitProgram" || comm == "exit" || comm == "!") break;
			else if (comm == "clear" || comm == "15") { system("clear"); }
			else cout << "Invalid Command !!!" << endl;
		}
		catch (MyExceptions& e)
		{
			cout << "Exception: " << e.getError() << endl << endl;
		}

	} while (comm != "quitProgram" && comm != "exit" || comm != "!");

	//we clear the datafile first
	ofstream fout("vfs.dat");
	fout.close();

	//then we save the current data to it
	save(filesystem->rootnode, "vfs.dat"); 

	return 0;
}