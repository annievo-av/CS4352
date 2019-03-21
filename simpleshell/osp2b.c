#include <stdio.h> // Standard Input/Output header
#include <stdlib.h> // Standard Library header
#include <stdint.h> // Standard Integer header
#include <inttypes.h> // Header for extended integer types
#include <unistd.h> // Header which exposes the POSIX.1 standard API (fork, getpid, etc)
#include <sys/wait.h> // Header which exposes the wait system call
#include <sys/types.h> // Header which exposes the process id type (pid_t)
#include <stddef.h> // Required for gnu99

int main()
{
	pid_t process_id; // Track single child PID

	if((process_id = fork()) < 0) 
	{ 
		perror("Unexpected Error");
		abort(); // Send SIGABRT
	} 
    // child processes because the PID of the child process is 0
	else if (process_id == 0) 
	{
	    printf("Waiting for single child's pid %d to finish\n", getpid());
	    
        // Variable to store the integer used for process creation
        int num_processes;
        printf("Please enter an integer: ");
        scanf("%d", &num_processes);
        
        // Array of processes ids used to track children
	    pid_t process_ids[num_processes];

        // Spawn num_processes number of children inside of child
        for(int i = 0; i < num_processes; i++) 
        {
            // Make child and catch error
            if((process_ids[i] = fork()) < 0) 
            {
                perror("Unexpected Error");
                abort(); // Send SIGABRT
            } 
        	else if (process_ids[i] == 0) 
        	{
                // Each child process prints out message together its PID
                pid_t curr_pid = getpid();
                pid_t parent_pid = getppid();
                printf("hello, my id is %d and my parent's id is %d\n", curr_pid, parent_pid);
    		    exit(0); // Return zero so wait does not error
        	}
        }

        // Wait for chain of processes to end
        while(num_processes > 0) 
        {
		    wait(NULL);
		    --num_processes;
	    }
	    
	    // Display message
	   	printf("All child processes are now done (chain)!\n");
	}
    
	// Wait for the single child to finish
    wait(NULL);
	return 0;
}

