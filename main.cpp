#include "binHeap.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
using namespace std;

class demo
{
	binHeap heap;
	map<string, string> commands;
	string outFileName;
public:
	demo(string outfile)
	{
		heap = binHeap();
		commands = map<string, string>({ 
				{ "add", "Insert new node" },
				{ "insert", "Insert new node" }, 
				{ "print", "Print the full heap" }, 
				{ "remove", "Remove the element by the key" },
				{ "clear", "Remove all the elements" },
				{ "delete", "Remove the element by the key" },
				{ "minimum", "Find the minimal element" },
				{ "search", "Search the element by the key" },
				{ "help", "Show help" },
				{ "exit", "Close the program" }
		});
		outFileName = outfile;
	}
	void getSubtree(node* root, std::queue<node*>* level, int depth)
	{
		if (!root)
			return;
		level[depth].push(root);
		getSubtree(root->child, level, depth + 1);
		getSubtree(root->brother, level, depth);
	}

	void printTree(node* root, ostream& stream)
	{
		int depth = root->degree;
		std::queue<node*>* level = new std::queue<node*>[depth + 1];
		level[0].push(root);
		getSubtree(root->child, level, 1);			//Записываем вершины в списки
		for (int i = 0; i <= root->degree; i++)
		{
			while (!level[i].empty())
			{
				stream << level[i].front()->key << ' ';
				level[i].pop();
			}
			stream << std::endl;
		}
		delete[] level;
	}

	void printTreeSelected(node* root, node* selected)
	{
		int depth = root->degree;
		std::queue<node*>* level = new std::queue<node*>[depth + 1];
		level[0].push(root);
		getSubtree(root->child, level, 1);			//Записываем вершины в списки
		for (int i = 0; i <= root->degree; i++)
		{
			while (!level[i].empty())
			{
				if (level[i].front() == selected)
					std::cout << "*" << level[i].front()->key << "*" << ' ';
				else
					std::cout << level[i].front()->key << ' ';
				level[i].pop();
			}
			std::cout << std::endl;
		}
		delete[] level;
	}

	void printHeap(ostream& stream = cout)
	{
		node* cur = heap.root;
		while (cur)					//По всем вершинам
		{
			printTree(cur, stream);
			stream << "-------------------------\n";
			cur = cur->brother;
		}
	}

	void printHeapSelected(node* selected)
	{
		node* cur = heap.root;
		while (cur)					//По всем вершинам
		{
			printTreeSelected(cur, selected);
			std::cout << "-------------------------\n";
			cur = cur->brother;
		}
	}

	void action(string query, bool output = true)
	{
		int firstSpace = query.find_first_of(' ');
		string command = query.substr(0, firstSpace);
		string param = (firstSpace != string::npos) ? query.substr(firstSpace + 1) : "";
		if (command == "print")
		{
			if (output)
			{
				system("cls");
				printHeap();
			}
		}
		else if (command == "insert" || command == "add")
		{
			int number;
			try
			{
				number = stoi(param);
			}
			catch (exception ex)
			{
				if (output)
				cout << "Error: Invalid parameter" << endl;
				return;
			}
			heap.insert(number);
			if (output)
			{
				system("cls");
				printHeap();
			}
		}
		else if (command == "minimum")
		{
			if (output)
			{
				system("cls");
				printHeap();
				cout << "Minimum: " << heap.getMinimum()->key << endl;
			}
		}
		else if (command == "search")
		{
			if (output)
			{
				int number;
				try
				{
					number = stoi(param);
				}
				catch (exception ex)
				{
					cout << "Error: Invalid parameter" << endl;
					return;
				}
				system("cls");
				node* found = heap.search(number);
				if (found)
					printHeapSelected(found);
				else
				{
					printHeap();
					cout << "Not found!" << endl;
				}
			}
		}
		else if (command == "remove" || command == "delete")
		{
			int number;
			try
			{
				number = stoi(param);
			}
			catch (exception ex)
			{
				if (output)
				cout << "Error: Invalid parameter" << endl;
				return;
			}
			heap.removeNode(heap.search(number));
			if (output)
			{
				system("cls");
				printHeap();
			}
		}
		else if (command == "help")
		{
			if (output)
			{
				system("cls");
				if (param.empty())
				{
					for each (pair<string, string> p in commands)
					{
						cout << p.first << " - " << p.second << endl;
					}
				}
				else
				{
					if (commands.count(param) > 0)
						cout << param << " - " << commands.at(param) << endl;
					else
						cout << "Error: help for command \"" << param << "\" doesn't exist" << endl;
				}
			}
		}
		else if (command == "clear")
		{
			if (output)
				system("cls");
			heap.clear();
		}
		else if (command == "save")
		{
			ofstream outfile(outFileName);
			printHeap(outfile);
			if (output)
			{
				system("cls");
				printHeap();
				cout << "Successfully saved" << endl;
			}
		}
		else if (command == "exit")
		{ }
		else if (command.empty())
		{
			if (output)
				cout << "Empty command" << endl;
		}
		else
		{
			if (output)
				cout << "Unknown command \"" << command << "\"" << endl;
		}
	}
};

void main(int argc, char* argv[])
{
	if (argc < 3)
		cout << "Error: not enough parameters";
	else if (argc > 3)
		cout << "Error: too many parameters";
	else
	{
		demo emp(argv[2]);
		string query = "";
		ifstream input(argv[1]);
		if (!input.fail())
		{
			while (!input.eof())
			{
				getline(input, query);
				emp.action(query, false);
			}
			input.close();
			emp.action("print");
			while (query != "exit")
			{
				cout << ">";
				getline(cin, query);
				emp.action(query);
			}
		}
		else
		{
			cout << "Error while opening input file";
		}
	}
}