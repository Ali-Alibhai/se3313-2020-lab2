#include <iostream>
#include <vector>
#include <chrono>
#include "thread.h"          // Include the definition of the Thread base class
#include "SharedObject.h"    // Include the definition for managing shared memory

// Structure for data shared between Writer threads and Reader
struct SharedData {
    int threadID;                // Unique identifier for each thread
    int reportID;                // Incremental report number by each thread
    int timeSinceLastReport;     // Time elapsed since the last report by this thread, in whole seconds
};

// WriterThread class inherits from Thread to create separate writer threads
class WriterThread : public Thread {
private:
    int threadNum;                   // Thread's unique identifier
    int delaySeconds;                // Delay between reports in seconds
    Shared<SharedData>* sharedMemory;// Pointer to the shared memory object

public:
    // Constructor: Initializes thread properties and shared memory pointer
    WriterThread(int num, int delay, Shared<SharedData>* shared) 
        : threadNum(num), delaySeconds(delay), sharedMemory(shared) {}

    // Main function executed by the thread
    long ThreadMain() override {
        int reportID = 0;   // Initialize reportID for this thread
        auto lastReportTime = std::chrono::system_clock::now();  // Track the last report time

        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));  // Delay between reports
            
            auto now = std::chrono::system_clock::now();  // Current time
            std::chrono::duration<double> elapsed = now - lastReportTime;  // Time elapsed since the last report, in seconds
            lastReportTime = now;  // Update last report time

            // Access the shared memory
            SharedData* data = sharedMemory->get();
            // Write thread information to the shared memory, converting elapsed time to an int
            data->threadID = threadNum;
            data->reportID = ++reportID;
            data->timeSinceLastReport = static_cast<int>(elapsed.count());  // Convert to int to discard fractional part

            // Future implementation: Add a stopping condition to exit the loop
        }
        return 0;
    }
};

// Main function: Creates writer threads based on user input and manages them
int main(void) {
    std::vector<WriterThread*> threads;  // Vector to keep track of writer threads
    // Initialize shared memory as the owner
    Shared<SharedData> sharedMemory("MySharedMemory", true);

    int threadNum = 0;  // Counter for thread numbers
    int delay;          // User-specified delay for each thread
    char createThread;  // User input to continue creating threads

    // Loop to create threads based on user input
    do {
        std::cout << "Would you like to create a writer thread? (y/n): ";
        std::cin >> createThread;

        if (createThread == 'y' || createThread == 'Y') {
            std::cout << "What is the delay time for the new thread (in seconds)? ";
            std::cin >> delay;

            // Create and store a new writer thread with the specified delay
            threads.push_back(new WriterThread(++threadNum, delay, &sharedMemory));
        }
    } while (createThread == 'y' || createThread == 'Y');

    // Cleanup: Signal threads to stop and deallocate them
    for (WriterThread* thread : threads) {
        // Implementation needed to properly signal thread to stop
        delete thread;  // Assumes thread destructor properly handles thread joining
    }

    return 0;
}




// int main(void)
// {
// 	std::cout << "I am a Writer" << std::endl;
	
// 	////////////////////////////////////////////////////////////////////////
// 	// This is a possible starting point for using threads and shared memory. 
// 	// You do not have to start with this
// 	////////////////////////////////////////////////////////////////////////
// 	/*...
// 	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMamory
// 	...
// 	while(true){
// 		...
// 		//create thread using user input
// 		thread1 = new WriterThread(xyz); //add arguments as needed
// 		...
// 	}
// 	//example for one thread thread1
// 	thread1->flag= true;
// 	delete thread1;
// 	*/
// }


// ////////////////////////////////////////////////////////////////////////
// // This is a possible starting point for using threads and shared memory. 
// // You do not have to start with this
// ////////////////////////////////////////////////////////////////////////
// /*class WriterThread : public Thread{
// 	public:
// 		int 	threadNum;
// 		bool	flag;
		
// 		WriterThread(int in):Thread(8*1000){
// 			this->threadNum = in; //or whatever you want/need here
// 		}

// 		virtual long ThreadMain(void) override{
// 			...
// 			//declare shared memory var so this thread can access it
// 			Shared<MyShared> sharedMemory ("sharedMemory");
// 			while(true)
// 			{
// 				//write to shared memory
// 				...  
// 				if(flag){//Exit loop to end the thread
// 					break;
// 				}
// 			}
// 		}
// };
// */
