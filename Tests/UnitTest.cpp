#include "HashTable/TestHashtable.cpp"
#include "Freelist/TestFreelist.cpp"
#include "String/TestString.cpp"
#include "Audio/TestAudio.cpp"
#include "Array/UnitTestArray.cpp"

int main() {

	Memory::Initialize(GIBIBYTES(1));
	
	TestArray();
	TestHashTable();
	TestFreelist();
	TestString();
	UnitTestAudio();

	return 0;
}