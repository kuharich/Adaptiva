#include <iostream>
#include <fstream>
#include <pthread.h>

using namespace std;
// Global Definitions
struct thread_data {
  int threadData;
};

/**
 *  This is the computation that is running on the background thread
 **/
 void * perform_computation(void * ptr) {
   struct thread_data *data = (thread_data *)ptr;
   data->threadData += 4; 
   pthread_exit(0);
 }

void patch_bytes(std::fstream* fs, int offset, const char* bytes) {
  // Set position at beginning + offset.
  fs->seekg(offset, std::ios::beg);

  // Write 'strlen(bytes)' bytes into file.
  fs->write(bytes, strlen(bytes));
}

void read_bytes(std::fstream *fs, int offset, int length) {
  // Set position at beginning + offset.
  fs->seekg(offset, std::ios::beg);

  // Allocate memory for length + null terminator.
  char* buffer = new char[length + 1];
  buffer[length] = '\0';

  // Read string into buffer, print string.
  fs->read(buffer, length);
  std::cout << buffer << std::endl;

  delete[] buffer;
}

int main(int argc, const char * argv[]) {
  // Instantiate fstream object.
  std::fstream file_stream;

  // Load our file with some mode flags. These flags can be combined with the bitwise or operator.
  // in / out: Support read and write operations.
  // binary: Treat file as binary rather than text.
  file_stream.open("hello.exe", std::ios::in | std::ios::out | std::ios::binary);

  // Print original bytes.
  std::cout << "Original: ";
  read_bytes(&file_stream, 0x384B, 5);

  // Patch & print new bytes.
  const char* new_string = "\x50\x61\x74\x63\x68"; // "Patch"
  patch_bytes(&file_stream, 0x384B, new_string);
  std::cout << "Patched: ";
  read_bytes(&file_stream, 0x384B, 5);

  file_stream.close();
  struct thread_data thrd;
  pthread_t thread_id;
  int mainData = 3;
 
  thrd.threadData = 3;
 
  thrd.threadData += mainData;
 
  std::cout << "Main thread data: " << thrd.threadData << std::endl;  
 
  if (pthread_create(&thread_id, NULL, perform_computation, &thrd)) {
  	std::cout << "Creating thread failed " << std::endl;
  	return 0;
  }
 
  pthread_join(thread_id, NULL);
  std::cout << "Thread computation result: " << thrd.threadData << std::endl;
  return(0);
}