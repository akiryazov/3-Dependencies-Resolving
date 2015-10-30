#include <iostream>
#include <Jzon.h>
#include <map>
#include <fstream>
using namespace std;
map<string, vector<string>> fileReader(string file)
{
	Jzon::Node node = Jzon::object();
	Jzon::Parser parser; 
	map<string, vector<string>> parsedFile;
	vector<string> values;
	string name;
	node = parser.parseFile(file); 
	for (Jzon::Node::iterator it = node.begin(); it != node.end(); it++)
	{
		name = (*it).first;
		values.clear();
		Jzon::Node newNode = (*it).second;	
		for (Jzon::Node::iterator itSecond = newNode.begin(); itSecond != newNode.end(); itSecond++)
		{
			values.push_back((*itSecond).second.toString());
		}
		parsedFile[name] = values;
	}
	return parsedFile;
}
void recursion(map<string, vector<string>> files, vector<string> dependencies)
{
	for (int i = 0; i < dependencies.size(); i++)
	{
		string path = "installed_modules/" + dependencies[i];
		ifstream file(path);
		if (file.good()) {
			cout << dependencies[i] << "is already installed." << endl;
		}
		else {
			cout << "Installing" << dependencies[i] << endl;
			ofstream fileName;
			fileName.open(path);	
			if (!files[dependencies[i]].empty())
			{
				cout << "In order to install " << dependencies[i] << ", we need ";
				for (int j = 0; j < files[dependencies[i]].size(); j++)
				{
					cout << files[dependencies[i]][j];
					if (j == files[dependencies[i]].size() - 1) {
						cout << ".";
					}
					else {
						cout << " and ";
					}
				}
				cout << endl;
				recursion(files, files[dependencies[i]]);
			}
		}
	}
}

int main()
{
	map<string, vector<string>> all_packages = fileReader("all_packages.json");
	map<string, vector<string>> dependencies_file = fileReader("dependencies.json");
	vector <string> dependencies = dependencies_file["dependencies"];
	recursion(all_packages, dependencies);
	cout << "All done." << endl;

}
