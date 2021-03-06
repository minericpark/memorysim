/*  Eric Minseo Park 
    1001018
    epark03@uoguelph.ca   */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ds_memory.h"
/*#define DEBUG*/
/*#define TEST2*/

struct ds_file_struct ds_file; /*Global variable that holds file pointer and block arrays*/
struct ds_counts_struct ds_counts; /*Global variable that reads and write counts*/

/*
int main () {

	#ifdef DEBUG
		int size = 1234;
		
		printf ("Debugging is turned on\n");
		printf ("sizeof(struct ds_counts_struct) = %ld\n", sizeof(struct ds_counts_struct));
		printf ("sizeof(struct ds_blocks_struct) = %ld\n", sizeof(struct ds_blocks_struct));
		printf ("sizeof(struct ds_file_struct) = %ld\n", sizeof(struct ds_file_struct));
	
		ds_create ("test", size); 
		
		printf ("%s\n", ds_file.fp); 
		ds_init ("test");
		
		printf ("Searching for block of size 1234\n");
		printf ("Return value is %ld\n", ds_malloc (10));
		printf ("Searching for block of large size\n");
		printf ("Return value is %ld\n", ds_malloc(100000));
		
		ds_free(0);
		
		ds_test_init();
		
		
		ds_finish();
		
		
	#endif
	
	#ifdef TEST
		unsigned char c1, c2;
		unsigned short s1, s2;
		unsigned int i1, i2;
		unsigned long l1, l2;
		float f1,f2;
		double d1, d2;

		long memory[12];     
 
		int i;

		c1 = 0;
		c2 = 1;
		s1 = 20;
		s2 = 30;
		i1 = 40;
		i2 = 50;
		l1 = 60;
		l2 = 70;
		f1 = 80.0;
		f2 = 90.0;
		d1 = 100.0;
		d2 = 120.0;


		ds_create( "test.bin", 1234);

		ds_init( "test.bin");

		memory[0] = ds_malloc( sizeof(c1) );
		ds_write( memory[0], &c1, sizeof(c1) );

		memory[1] = ds_malloc( sizeof(c2) );
		ds_write( memory[1], &c2, sizeof(c2) );

		ds_free( memory[1] ); 

		memory[2] = ds_malloc( sizeof(s1) );
		ds_write( memory[2], &s1, sizeof(s1) );

		memory[3] = ds_malloc( sizeof(s2) );
		ds_write( memory[3], &s2, sizeof(s2) );

		ds_free( memory[3] ); 

		memory[4] = ds_malloc( sizeof(i1) );
		ds_write( memory[4], &i1, sizeof(i1) );

		memory[5] = ds_malloc( sizeof(i2) );
		ds_write( memory[5], &i2, sizeof(i2) );

		ds_free( memory[5] );

		memory[6] = ds_malloc( sizeof(l1) );
		ds_write( memory[6], &l1, sizeof(l1) );

		memory[7] = ds_malloc( sizeof(l2) );
		ds_write( memory[7], &l2, sizeof(l2) );

		ds_free( memory[7] );

		memory[8] = ds_malloc( sizeof(f1) );
		ds_write( memory[8], &f1, sizeof(f1) );

		memory[9] = ds_malloc( sizeof(f2) );
		ds_write( memory[9], &f2, sizeof(f2) );
 
		ds_free( memory[9] );

		memory[10] = ds_malloc( sizeof(d1) );
		ds_write( memory[10], &d1, sizeof(d1) );

		memory[11] = ds_malloc( sizeof(d2) );
		ds_write( memory[11], &d2, sizeof(d2) );

		ds_free( memory[11] );

		printf( "Remaining memory allocations:\n");
		for(i=0;i<12;i+=2) 
		{
			printf( "%ld\n", memory[i] );
		}


		ds_test_init();
		ds_finish();
	#endif
	
	#ifdef TEST2
	
		unsigned char c1;
	    unsigned short s1;
		unsigned int i1;
		unsigned long l1;
		float f1;
		double d1;

		long memory[6] = { 0, 2, 6, 14, 10, 30};
		ds_init( "test.bin");

		ds_read( &c1, memory[0],sizeof( c1 ) );
		ds_read( &s1, memory[1],sizeof( s1 ) );
		ds_read( &i1, memory[2],sizeof( i1 ) );
		ds_read( &l1, memory[3], sizeof( l1 ) );
		ds_read( &f1, memory[4],sizeof( f1 ) );
		ds_read( &d1, memory[5],sizeof( d1 ) );

		printf( "%d %d %d %ld %f %f\n", c1, s1, i1, l1, f1, d1 );
		ds_finish();
	
	#endif
	
    return (0); 
}
*/

void ds_test_init() {
	
	int i;
	
	printf ("Block Num	Start	Length	Alloced\n");
	for (i = 0; i < MAX_BLOCKS; i++) { 
		printf ("%d		%ld	%ld 	%d\n", i, ds_file.block[i].start, ds_file.block[i].length, ds_file.block[i].alloced);
	}

}

/*
* int ds_create creates a giant block in the file that has
* no memory allocation in any size
*/
int ds_create (char *filename, long size) {

	int i;
	char *zero = 0;
	
	if ((ds_file.fp = fopen (filename, "wb+")) == NULL) { /*File checker*/
		#ifdef DEBUG
			printf ("Error: file could not be opened\n");
		#endif
		return -1;
	}	
	
	if (fseek (ds_file.fp, 0, SEEK_SET) != 0) { /*Sets file pointer*/
		#ifdef DEBUG
			printf ("Error: file pointer was not reset\n");
		#endif
		return -1;
	}

	for (i = 0; i < MAX_BLOCKS; i++) { /*For loop creates all structs up to MAX_BLOCKS*/
		ds_file.block[i].start = 0;
		ds_file.block[i].length = 0;
		ds_file.block[i].alloced = 0;
	}
	ds_file.block[0].length = size; /*Sets block 0's length to size of created file*/
	for (i = 0; i < MAX_BLOCKS; i++) { /*For loop writes into file all structs*/
		if (fwrite (&ds_file.block[i], sizeof (struct ds_blocks_struct), 1, ds_file.fp) != 1) {
			#ifdef DEBUG
				printf ("Error: file could not be written into or nothing to write!\n");
			#endif
			return -1;
		}
	}
	if (fwrite (&zero, sizeof (char) * size, 1, ds_file.fp) != 1) { /*fwrites 0 into file*/
		#ifdef DEBUG
			printf ("Error: file could not be written into or nothing to write!\n");
		#endif
		return -1;
	}
	if (fclose (ds_file.fp) == EOF) { /*Checks if file closes appropriately*/
		#ifdef DEBUG
			printf ("Error: file could not be closed\n");
		#endif
		return -1;
	}
	
	return (0);
	
}

/*
* int ds_init opens and reads block from filename
* and resets counts for both reads and writes to 0. 
* Returns 0 when complete, or -1 when failed
*/
int ds_init (char *filename) {
	
	if ((ds_file.fp = fopen (filename, "rb+")) == NULL) { /*File checker*/
		#ifdef DEBUG
			printf ("Error: file could not be opened\n");
		#endif
		return -1;
	}
	if (fread (&ds_file.block, sizeof(ds_file.block), 1, ds_file.fp) != 1) { /*Freads entire file of size ds_file.block into ds_file.block struct*/
		#ifdef DEBUG
			printf ("Error: file could not be read or is empty\n");
		#endif
		return -1;
	}
	/*Sets ds_count reads and writes both to 0 (resets)*/
	ds_counts.reads = 0;
	ds_counts.writes = 0;
	return (0);
	
}

/*
* long ds_malloc takes in long amount and finds the first block with size greater or equal to amount,
* to allocate and returns the address of the allocated block
*/
long ds_malloc (long amount) {
	
	int i;
	long tempStart;
	long tempLength;
	int secondBlockFound = 0;
	
	for (i = 0; i < MAX_BLOCKS; i++) { /*For loop covers all blocks and seeks first memory size (length) big enough for allocation, along with a block that is not allocated*/
		if (ds_file.block[i].length >= amount && ds_file.block[i].alloced == 0) {
			#ifdef DEBUG
				printf ("Discovered block %d, allocating...\n", i);
			#endif
			/*Temporarily hold previous values of unallocated block, then set block with new values*/
			tempStart = ds_file.block[i].start;
			tempLength = ds_file.block[i].length;
			ds_file.block[i].length = amount;
			ds_file.block[i].alloced = 1;
			for (i = 0; i < MAX_BLOCKS; i++) { /*For loop seeks 2nd block and writes structure information into it*/
				if (ds_file.block[i].length == 0 && secondBlockFound == 0) {
					#ifdef DEBUG
						printf ("Second block found at %d, adjusting...\n", i);
					#endif
					secondBlockFound = 1;
					ds_file.block[i].start = tempStart + amount;
					ds_file.block[i].length = tempLength - amount;
					ds_file.block[i].alloced = 0;
				}
			}
			return tempStart; /*Return address of allocated block*/
		}			
	}	
	return (-1);
	
}

/*
* void ds_free takes in long start and finds specified block
* then sets allocate to 0
*/
void ds_free (long start) {
	
	int i;
	int blockFreed = 0;
	
	for (i = 0; i < MAX_BLOCKS; i++) { /*For loop checks entire block and seeks the address given, then unallocates it if memory is not already unallocated*/
		if (ds_file.block[i].start == start && blockFreed == 0) {
			#ifdef DEBUG
				printf ("Discovered block %d allocated, freeing...\n", i);
			#endif
			ds_file.block[i].alloced = 0;
			blockFreed = 1;
		}
	}
}

/*
* void ds_read takes in void ptr, long start and long bytes
* to find location in file through start, read in the ptr of size bytes and increments ds_counts.reads up once.
* returns pointer read, or null if failed.
*/
void *ds_read (void *ptr, long start, long bytes) {

	if (fseek (ds_file.fp, sizeof(ds_file.block) + start, SEEK_SET) != 0) { /*File pointer checker*/
		#ifdef DEBUG
			printf ("Error: could not set file pointer during read\n");
		#endif
		return NULL;
	}
	if (fread (ptr, bytes, 1, ds_file.fp) != 1) { /*File read checker */
		#ifdef DEBUG
			printf ("Error: file could not be read or is empty\n");
		#endif
		return NULL;
	}
	ds_counts.reads++; /*Increments read by one*/
	return ptr;

}

/*
* int ds_write takes in long start, void ptr, long bytes
* to find location in file through start and write in the ptr of size bytes and increments ds_counts.writes up once. 
* Returns location of write if success, and -1 if failed.
*/
long ds_write (long start, void *ptr, long bytes) {
	
	if (fseek (ds_file.fp, sizeof(ds_file.block) + start, SEEK_SET) != 0) { /*File pointer checker*/
		#ifdef DEBUG
			printf ("Error: could not set file pointer during write\n");
		#endif
		return -1;
	}
	
	if (fwrite (ptr, bytes, 1, ds_file.fp) != 1) { /*File writer checker*/
		#ifdef DEBUG
			printf ("Error: file could not be written into or nothing to write!\n");
		#endif
		return -1;
	}		
	ds_counts.writes++;
	return (start);
	
}

/*
* int ds_finish resets file pointer, and writes in all blocks into file
* then closes file. Returns 1 if success or 0 if failed.
*/
int ds_finish () {
	
	int i;

	if (fseek (ds_file.fp, 0, SEEK_SET) != 0) { /*File pointer checker*/
		#ifdef DEBUG
			printf ("Error: could not set file pointer during finish\n");
		#endif
		return 0;
	}
	for (i = 0; i < MAX_BLOCKS; i++) { /*Checks entire structure and writes it all into file*/
		if (fwrite (&ds_file.block[i], sizeof (struct ds_blocks_struct), 1, ds_file.fp) != 1) {
			#ifdef DEBUG
				printf ("Error: file could not be written into or nothing to write!\n");
			#endif
			return 0;
		}
	}
	
		printf ("reads: %d\n", ds_counts.reads);
		printf ("writes: %d\n", ds_counts.writes);
	
	if (fclose (ds_file.fp) == EOF) {
		return 0;
	}
	return (1);
	
}





