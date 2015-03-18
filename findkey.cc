#include <stdint.h>
#include <iostream>
#include <ctime>
#include <openssl/des.h>
#include <openssl/ssl.h>

#include <fcntl.h>
#include <unistd.h>

# define S_IREAD        S_IRUSR
# define S_IWRITE       S_IWUSR

void decrypt(uint32_t seed, const char* fileName, char* plaintext){
    DES_cblock iv, key;
    DES_key_schedule schedule;
    srand(seed);
    for (int i=0; i<8; i++)
      key[i] = (unsigned char)rand();
    for (int i=0; i<8; i++)
      iv[i] = (unsigned char)rand();
    DES_set_odd_parity(&key);
    DES_set_key_checked(&key, &schedule);
    const char* FILE = strdup(fileName);
    int fd = open(fileName, O_RDONLY);
    if(!fd) {
      std::cout << "Could not open file" << std::endl;
      fflush(stdout);
      delete(FILE);
      exit(EXIT_FAILURE);
    }
    	DES_enc_read(fd, plaintext, 36, &schedule, &iv);
		close(fd);

}

void saveToFile(const char* fileName, const char* messageToSave){
    int fd = open(fileName, O_CREAT|O_TRUNC|O_WRONLY, S_IREAD|S_IWRITE);

    if(!fd) {
      std::cout << "Could not open output file" << std::endl;
      fflush(stdout);
      exit(EXIT_FAILURE);
    }

    write(fd, messageToSave, strlen(messageToSave));
    close(fd);
}

int main(int argc, char* argv[]){
	if(argc != 3){
		std::cout << "Usage is findkey inputFile outputFile" << std::endl;
		exit(EXIT_FAILURE);
	}

	const char* fileName = argv[1];
	// ASCII text indicating a correct seed was found
	char* correctMatch = strdup("Correct");
	char* plaintext = new char[255]();
	void* pointer = 0;
	char* outFile = argv[2];
	std::string hex;

	// HIGH should be greater than 1424221609
	uint32_t const HIGH = std::time(NULL);
	std::cout<<HIGH;
	uint32_t const LOW  = 0;

	uint32_t seed = HIGH;
	for (; seed>LOW; --seed){
		decrypt(seed,fileName,plaintext);

		pointer = strstr(plaintext,correctMatch);
		if(pointer != NULL) {


			std::cout<<std::hex<<seed<<std::endl;
			saveToFile(outFile,plaintext);
			break;
		}
	}

	free(correctMatch);
	delete[] plaintext;
	return (EXIT_SUCCESS);
}
