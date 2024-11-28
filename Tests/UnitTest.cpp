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

#if defined(SIMD_SUPPORTED_NEON)
		std::cout << "arm NEON is supported.\n";
#endif
#if defined(SIMD_SUPPORTED_AVX2)
    std::cout << "AVX2 is supported.\n";
#endif
#if defined(SIMD_SUPPORTED_SSE)
    std::cout << "SSE2 is supported.\n";
#endif

	return 0;
}
