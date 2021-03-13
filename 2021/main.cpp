#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

void getSize(string &s, int *x, int *y);
void getSize(string &s, int *numB, int *numA, int *reward);
void getSize(string &s, int *bX, int *bY, int *bL, int *bS);

class Building{
	private:
		int x;
		int y;
		int latency;
		int connectionSpeedWieght;
	public:
		int getX() const {return this->x;}
		int getY() const {return this->y;}
		int getLatency() const {return this->latency;}
		int getConnectionSpeedWeight() {return this->connectionSpeedWieght;}
		Building(int &x, int &y, int &latency, int &connectionSpeedWieght){
			this->x = x;
			this->y = y;
			this->latency = latency;
			this->connectionSpeedWieght = connectionSpeedWieght;
		}
};

class Antenna{
	private:
		int pos;
		int range;
		int connectionSpeed;
	public:
		int getPos() {return this->pos;}
		int getRange() const {return this->range;}
		int getConnectionSpeed() const {return this->connectionSpeed;}
		Antenna(int &pos, int &range, int &connectionSpeed){
			this->pos = pos;
			this->range = range;
			this->connectionSpeed = connectionSpeed;
		}
};

bool antennaSort(const Antenna &one, const Antenna &two){
	return one.getRange() > two.getRange();
}

typedef vector<Building> B;
typedef vector<Building>::const_iterator BI;
typedef vector<Antenna> A;
typedef vector<Antenna>::const_iterator AI;
typedef vector<int> vI;
typedef vector<int>::const_iterator vII;

int main(){
	srand(time(NULL));
	ifstream f("ex1.txt");
	
	//get map size
	int mapX, mapY;
	string s;
	getline(f, s);
	getSize(s, &mapX, &mapY);
	
	//get n buildings - antennas - reward
	int numA, numB, reward;
	getline(f, s);
	getSize(s, &numB, &numA, &reward);

	//get building
	B buildingVector;
	int buildingX, buildingY, buildingLatency, buildingConnectionSpeed;
	for (int i=1; i<=numB; i++){
		getline(f, s);
		getSize(s, &buildingX, &buildingY, &buildingLatency, &buildingConnectionSpeed);
		Building newBuilding(buildingX, buildingY, buildingLatency, buildingConnectionSpeed);
		buildingVector.push_back(newBuilding);
	}			
	
	//get antenna
	A antennaVector;
	int antennaRange, antennaSpeed;
       	for (int i=0; i<numA; i++){
		getline(f, s);
		getSize(s, &antennaRange, &antennaSpeed);
		Antenna newAntenna(i, antennaRange, antennaSpeed);		
		antennaVector.push_back(newAntenna);	
	}
	f.close();

	ofstream fOut("one.txt");
	int spaceRate = numB/numA;
	BI tB = buildingVector.begin();
	fOut << numA << "\n";
	for (Antenna a : antennaVector){
		fOut << a.getPos() << " " << tB->getX() << " " << tB->getY() << "\n";
		tB += spaceRate;
	}
	fOut.close();
	
	return 0;
}

void getSize(string &s, int *bX, int *bY, int *bL, int *bS){
	int i = 0;
	string a;
	while (s[i] != ' '){
		a += s[i];
		i++;
	}
	*bX = stol(a, nullptr, 10);
	i++;
	string b;
	while (s[i] != ' '){
		b += s[i];
		i++;
	}
	*bY = stol(b, nullptr, 10);
	i++;
	string c;
	while (s[i] != ' '){
		c += s[i];
		i++;
	}
	*bL = stol(c, nullptr, 10);
	i++;
	string d;
	while (i < s.length()){
		d += s[i];
		i++;
	}
	*bS = stol(d, nullptr, 10);
	cout << *bX << " " << *bY << " " << *bL << " " << *bS << endl;
}

void getSize(string &s, int *numB, int *numA, int *reward){
	int i = 0;
	string valNumB;
	while (s[i] != ' '){
		valNumB += s[i];
		i++;
	}
	*numB = stol(valNumB, nullptr, 10);
	i++;
	string valNumA;
	while (s[i] != ' '){
		valNumA += s[i];
		i++;
	}
	*numA = stol(valNumA, nullptr, 10);
	i++;
	string valReward;
	while (i < s.length()){
		valReward += s[i];
		i++;
	}
	*reward = stol(valReward, nullptr, 10);
	cout << *numB << " " << *numA << " " << *reward << endl;
}

void getSize(string &s, int *x, int *y){
	int i = 0;
	string valX;
	while (s[i] != ' '){
		valX += s[i];
		i++;
	}
	*x = stol(valX, nullptr, 10);
	i++;
	string valY;
	while (i < s.length()){
		valY += s[i];
		i++;
	}
	*y = stol(valY, nullptr, 10);
	cout << *x << " " << *y << endl;
}
