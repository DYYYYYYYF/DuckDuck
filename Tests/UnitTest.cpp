#include "HashTable/TestHashtable.cpp"
#include "Freelist/TestFreelist.cpp"
#include "String/TestString.cpp"
#include "Audio/TestAudio.cpp"
#include "Array/UnitTestArray.cpp"
#include "Matrix/TestMatrix.cpp"

int main() {

	Memory::Initialize(GIBIBYTES(1));
	
	TestArray();
	TestHashTable();
	TestFreelist();
	TestString();
	UnitTestAudio();

	TestMatrix();

	if (is_sse_supported()) {
		std::cout << "SSE is supported.\n";
	}
	if (is_sse2_supported()) {
		std::cout << "SSE2 is supported.\n";
	}
	if (is_avx_supported()) {
		std::cout << "AVX is supported.\n";
	}
	if (is_avx2_supported()) {
		std::cout << "AVX2 is supported.\n";
	}

	return 0;
}