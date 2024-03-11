#include <iostream>
#include <unistd.h> // For usleep
#include "SharedObject.h"
struct SharedData {
    int threadID;
    int reportID;
    int timeSinceLastReport; // Matches the structure in Writer
};

int main(void) {
    // Open the shared memory object without being the owner
    Shared<SharedData> sharedMemory("MySharedMemory", false);

    while(true) {
        // Read the shared data
        SharedData data = *sharedMemory;
        
        // Print the data
        std::cout << "Thread ID: " << data.threadID 
                  << ", Report ID: " << data.reportID 
                  << ", Time since last report: " << data.timeSinceLastReport << " seconds" << std::endl;
        
        // Polling interval
        usleep(500000); // 0.5 seconds.
    }



    return 0;
}