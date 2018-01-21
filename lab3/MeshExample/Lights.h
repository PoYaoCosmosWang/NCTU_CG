#pragma once
#include <string>
#include<sstream>
#include<fstream>
#include"vec3.h"
#include<vector>
#include<iostream>
using namespace std;

class Light
{
public:
	
	float position[4];
	float  ambient[3], diffuse[3], specular[3];
	Light()
	{
		position[3] = 1;
	}
};
/*light x y z ar ag ab dr dg db sr sg sb*/
class Lights
{
public:
	int lightNum;
	vector<Light> ls;//lights
	float ambient[3];
	void readFile(string file)
	{
		fstream fp(file, ios::in);
		string s;
		while (fp >> s)
		{
			if (s == "light")
			{
				++lightNum;
				Light l;
				fp >> l.position[0] >> l.position[1] >>  l.position[2];
				fp >> l.ambient[0] >> l.ambient[1] >> l.ambient[2];
				fp >> l.diffuse[0] >> l.diffuse[1] >> l.diffuse[2];
				fp >> l.specular[0] >> l.specular[1] >> l.specular[2];
				ls.push_back(l);
			}
			else if (s == "ambient")
			{
				fp >> ambient[0] >> ambient[1] >> ambient[2];
			}
			else
			{
				cout << "error" << endl << "do not have:" << s << endl;
				return;
			}
		}
		fp.close();
	}

	Lights(string s)
	{
		lightNum = 0;
		readFile(s);
		print();
	}
	void print()
	{
		
		for (int i = 0; i < lightNum; ++i)
		{
			cout << "light" << i << endl;
			cout << ls[i].position[0]<<" " << ls[i].position[1] << " " << ls[i].position[2] << " " << endl;
			cout << ls[i].ambient[0]<<" " << ls[i].ambient[1] << " " << ls[i].ambient[2] << " " << endl;
			cout << ls[i].diffuse[0] <<" " << ls[i].diffuse[1] << " " << ls[i].diffuse[2] << " " << endl;
			cout << ls[i].specular[0] <<" " << ls[i].specular[1] << " " << ls[i].specular[2] << " " << endl;
		}
		cout<< ambient[0] << ambient[1] << ambient[2];
	}

};


