#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
#include<fstream>
#include<iterator>
using namespace std;
class CFileType {
private:
	string fileName;
	char fileType;
public:
	CFileType () {
		this->fileName = "";
		this->fileType = ' ';
	}
	CFileType (const string fileName, const char fileType) {
		this->fileName = fileName;
		this->fileType = fileType;
	}
	string getFileName () const {
		return this->fileName;
	}
	char getFileType () const {
		return this->fileType;
	}
	void setFileType (const char fileType) {
		this->fileType = fileType;
	}
	bool operator < (const CFileType& someFile) const {
		return (this->fileName < someFile.fileName);
	}
	bool operator == (const CFileType& someFile) const {
		return (this->fileName == someFile.fileName);
	}
	CFileType& operator = (const CFileType& someFile) {
		this->fileName = someFile.fileName;
		this->fileType = someFile.fileType;
		return *this;
	}
	friend istream& operator >> (istream& fromStream, CFileType& someFile);
	friend ostream& operator << (ostream& toStream, const CFileType& someFile);
	static bool compare (const CFileType& file1, const CFileType& file2) {
		return (file1.fileType == file2.fileType);
	}
	friend class CFolder;
};
istream& operator >> (istream& fromStream, CFileType& someFile) {
	fromStream>>someFile.fileName>>someFile.fileType;
	return fromStream;
}
ostream& operator << (ostream& toStream, const CFileType& someFile) {
	toStream<<someFile.fileName<<" "<<someFile.fileType<<endl;
	return toStream;
}
class CFolder {
private:
	string folderName;
	vector<CFileType> v;
public:
	CFolder () {
		this->folderName = "";
	}
	CFolder (const string file) {
		ifstream from(file.c_str());
		if(from.is_open())
		{
			from>>folderName;
			copy(istream_iterator<CFileType>(from),istream_iterator<CFileType>(),back_inserter(v));
			from.close();
		}
		else throw "Can't open file";
		
	}
	CFolder (const CFolder& folder) {
		this->folderName = folder.folderName;
		this->v = folder.v;
	}
	string getFolderName () const {
		return this->folderName;
	}
	vector<CFileType> getVector () const {
		return this->v;
	}
	void setFolderName (const string folderName) {
		this->folderName = folderName;
	}
	void setVector (const vector<CFileType> v) {
		this->v = v;
	}
	int countFiles (const string fileName) {
		int counter = 0;
		vector<CFileType> :: iterator it = this->v.begin();
		while(it != this->v.end()) {
			if ((*it++).fileName == fileName) {
				counter++;
			}
		}
		return counter;
	}
	void addFile (const CFileType& someFile) {
		this->v.push_back(someFile);
	}
	void uniqueFiles () {
		vector<CFileType> :: iterator it;
		sort(this->v.begin(), this->v.end());
		it = unique(this->v.begin(),this->v.end());
		this->v.erase(it, this->v.end());
	}
	int countTypes (const char fileType) {
		int counter = 0;
		vector<CFileType> :: iterator it = this->v.begin();
		while(it != this->v.end()) {
			if ((*it++).fileType == fileType) {
				counter++;
			}
		}
		return counter;
	}
	map<char, int> doubleMap () {
		map<char, int> m;
		int counter = 0;
		CFolder uniqueTypes(*this);
		vector<CFileType> :: iterator it;
		it = unique (uniqueTypes.v.begin(), uniqueTypes.v.end(), CFileType :: compare);
		uniqueTypes.v.erase(it, uniqueTypes.v.end());
		it = uniqueTypes.v.begin();
		while (it != uniqueTypes.v.end()) {
			counter = countTypes ((*it).fileType);
			m.insert(make_pair((*it++).fileType, counter));
		}
		return m;
	}
	friend ostream& operator << (ostream& toStream, const CFolder& folder);
};
ostream& operator << (ostream& toStream, const CFolder& folder) {
	toStream<<folder.folderName<<endl;
	copy(folder.v.begin(),folder.v.end(),ostream_iterator<CFileType>(toStream,"\n"));
	return toStream;
}
int main()
{
	try{
	CFolder dir("data.txt");
	cout<<dir;
	cout<<" Count = "<<dir.countFiles("SomeFile")<<endl;
	map<char,int> mm;
	mm=dir.doubleMap();
	map<char,int>::iterator it=mm.begin();
	while(it!=mm.end())
	{
		cout<<it->first<<" -- "<<it->second<<endl;
		it++;
	}


	dir.uniqueFiles();
	cout<<dir;

	}
	catch(char *str)
	{
		cout<<str<<endl;
	}
	system("pause");
	return 0;
}