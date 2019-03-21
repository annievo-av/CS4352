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
	// Variable to store the integer used for process creation
	int num_processes;
	printf("Please enter an integer: ");
	scanf("%d", &num_processes);

	// Array of processes ids used to track children
	pid_t process_ids[num_processes];

	// Fork a new process and track the PID
	for(int i = 0; i < num_processes; i++) 
	{
		if((process_ids[i] = fork()) < 0) 
		{
			perror("Unexpected Error");
			abort(); // Send SIGABRT
		} 
		// child processes because the PID of the child process is 0
		else if (process_ids[i] == 0) 
		{
			// Each child process prints out message together its PID
            pid_t curr_pid = getpid();
            pid_t parent_pid = getppid();
            printf("hello, my id is %d and my parent's id is %d\n", curr_pid, parent_pid);
    		exit(0); // Return zero so wait does not error
		}
	}

	// Wait for all children processes to finish
	while(num_processes > 0) 
	{
        wait(NULL);
		--num_processes;
	}

	// Display message and exit
	printf("All child processes are now done!\n");
	return 0;
}

