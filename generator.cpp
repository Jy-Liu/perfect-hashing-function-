#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

const int MaxCharacterHash = 18;
const int HashShift = 6;

int searchMinimalPerfectHash(int i, vector<string>& fwords, vector<string>& hash, vector<bool>& visited, vector<int>& values );
vector<string> words = {
	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"int",
	"long",
	"register",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while"
};
bool freqComparator(pair<char,int> a, pair<char,int> b){
	return a.second > b.second;
}
bool priorityComparator(pair<string,int> a, pair<string,int> b){
	return a.second > b.second;
}
inline int toInt(char c){
	return c - 'a';
}
int getFrequency(vector<pair<char,int>> frequency, char c ){
	for(auto f:frequency){
		if( f.first == c )
			return f.second;
	}
}
void sortByAlphabet(vector<pair<char,int>>& frequency){
	for(int i=0;i<26;i++){
		frequency.push_back( make_pair((char)(i+'a'),0) );
	}
	for(auto word:words){
		frequency[ toInt(word.front()) ].second++;
		frequency[ toInt(word.back())].second++;
	}
	sort(frequency.begin(),frequency.end(),freqComparator);
}
void sortByPriority(vector<pair<char,int>>& frequency,vector<string>& fwords){
	vector<pair<string,int>> pwords;
	for(auto word:words){
		int priority = getFrequency(frequency,word.front()) + getFrequency(frequency, word.back());
		pwords.push_back(make_pair(word,priority));
	}
	sort(pwords.begin(),pwords.end(),priorityComparator);
	fwords.resize(pwords.size());
	transform(pwords.begin(),pwords.end(),fwords.begin(),[](pair<string,int> pw){ return pw.first; });
}
inline int hashValue(string word, vector<int>& values){
	return values[toInt(word.front())]+values[toInt(word.back())]+word.size() - HashShift;
}
int selectHash(int ch, int i, vector<string>& fwords, vector<string>& hash, vector<bool>& visited, vector<int>& values ){
	for(int v=0; v < MaxCharacterHash; ++v){
			values[ch] = v;
			visited[ch] = true;
			if(searchMinimalPerfectHash(i, fwords, hash, visited, values))
				return 1;
            else
                visited[ch] = false;
	}
	return 0;
}
int searchMinimalPerfectHash(int i, vector<string>& fwords, vector<string>& hash, vector<bool>& visited, vector<int>& values ){
	if( i == fwords.size() )
		return 1;

	int front = toInt(fwords[i].front());
	int back  = toInt(fwords[i].back());

	if( visited[ front ] && visited[ back ] ){
        int hashvalue = hashValue(fwords[i], values);

#ifdef DEBUG
        cout<<" Candidate Hash Value: "<<hashvalue<<endl;
#endif
		if( hashvalue >= 0 && hashvalue < hash.size() && hash[ hashvalue ].empty() ){ // If this is a suitable hash value
            hash[ hashvalue ] = fwords[i];
#ifdef DEBUG
            cout<<"Hash value for '"<<fwords[i]<<"' would be "<<hashvalue<<"\n";
#endif
			if( searchMinimalPerfectHash(i+1, fwords, hash, visited, values) )
                return 1;
            else{
                // If the hash value fails at next recursion, kick it off our current hash table and backtrack
                hash[ hashvalue ].clear();
                return 0;
            }
        }
		else
			return 0;
	}
	else if(!visited[ front ]){
		return selectHash(front, i, fwords, hash, visited, values);
	}
	else if(!visited[ back ]){
		return selectHash(back, i, fwords, hash, visited, values);
	}
}
int main() {
	vector<pair<char,int>> frequency;
	vector<string>         fwords;

	sortByAlphabet(frequency);
	sortByPriority(frequency,fwords);
	
	vector<bool>   visited(frequency.size(),false);
	vector<int>    values(frequency.size(),0);
	vector<string> hash(fwords.size(),string(""));
	if(searchMinimalPerfectHash(0, fwords, hash, visited,values)){
		for(int i =0;i<hash.size();++i){
			cout<<fixed<<setw(13)<<hash[i] << " --> "<<i<<" ";
            if( (i+1)%3 == 0 )
                cout<<endl;
		}
        for( int i = 0 ; i < values.size(); ++i ){
            cout<<fixed<<setw(2)<<"'"<<(char)(i+'a')<<"' -> "<<values[i]<<" , ";
            if( (1+i) % 6 == 0 )
                cout<<endl;
        }
	}
	else{
		cout<< "Fail" << endl;
	}
	
	return 0;
}