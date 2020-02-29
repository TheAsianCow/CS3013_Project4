README
CS 3013 Project 4
Jeffrey Huang and Jyalu Wu

----------------------------------------------------------------
Overview
----------------------------------------------------------------
Main Functions:			Helper Functions:		Global Variables:
	main					exit_mem_man			mem[64]
	parse					isValidArgs				dsk[320]
	allocate				isNumber				free_list[4]
	store					isString				proc_reg[4]
	load					find_free				rnd_rbn_cnt
	swap					debugger
	evict
	VPN_TO_MEM			
	err_handler

For this project, we were tasked to implement a virtual memory
system that includes paging and swapping features. There were
two parts to this project: Part 1 focused on creating the paging
feature and Part 2 focused on creating the swapping feature.
Since there is a lot of overlap, the following summary includes
both parts instead of explaining each one separately.

The virtual memory system will keep asking the user for the next
instruction until it is told to stop. The memory manager will
either allocate, store, or load memory depending on the instruction
and swap and/or evict if necessary if memory is full.

Each page is 16 bytes big, meaning that there is enough space in
mem for 4 pages in total. In disk, there is enough space for 20
pages, which includes a spot for each process' page table and 4
pages. Each spot in the disk is reserved for a specific page table
or page for ease of access and storage.

The global variable free_list stores the page_ID of the page that
is stored in each page frame in mem. A page's page_ID is any
number [0,19] and corresponds to the page's designated index in
dsk. This means that each page table has a page_ID that is divisble
by 5, which is a tidbit that comes in handy in the swap function.

The global variable proc_reg stores the address of each process'
page table.

Jeffrey also included a debugger function that allows the user to
voew each global variable in a debugger mode. It's pretty cool.

----------------------------------------------------------------
Swap
----------------------------------------------------------------
The swap() function uses the round robin algorithm, which means
that the program will evict the page in frame 0 of mem the first
time it has to evict, then the one in frame 1 the second time,
then the one in frame 2, then frame 3, and then it will repeat
and evict the one in frame 0 the next time it has to evict. The
swap() function has a helper function evict() that, true to its
name, will evict a page from memory. This is where we implemented
the round robin algorithm.

Both functions are given the page_ID of the page it wants to put
in mem as an argument.

----------------------
Evict
----------------------
The function will first check whether or not it would be evicting
the page table corresponding to the page it wants to put in mem.
If it is, it evict the next frame instead. Then, it will check
if it will be evicting a page table or a page.

Evicting a page table: it will update the globals so that they
reflect the fact that the page table is now in dsk, then copy
the page table from mem to dsk.

Evicting a page: it will update the PTE corresponding to the page
so that the valid bit is set to 0. To do this, it must first get
the corresponding page table. If the page table is in mem, this
is easy; however, if it is in disk, it will have to copy it into
mem. For example, if it is trying to evict page A which is in 
spot X in mem, it will first evict A from spot X, copy A's page
table from dsk to X, change the PTE for A so that the valid bit
is 0, then copy A's page table back to dsk.
----------------------

After calling evict(), swap() will copy into mem the specified
page it was supposed to swap in.

----------------------------------------------------------------
Allocate
----------------------------------------------------------------
This instructs the memory manager to make space in mem, whether it
is by actually allocating an empty splot for the given page or by
swapping out an existing page so that the given page can fit. It
also will update the PTE corresponding to the page, meaning that
it must be able to access the corresponding page table in mem.

This means that if the page table is in dsk, it must first make
space for the page, swap in the page table, update the PTE, then
swap out the page table back to dsk.

----------------------------------------------------------------
Store
----------------------------------------------------------------
This instructs the memory manager to write the given value into
the physical memory location associated with a given virtual
address. If the page it's trying to store into is in dsk, it will
have to swap the page into mem first.

----------------------------------------------------------------
Load
----------------------------------------------------------------
This instructs the memory manager to return the value that is
stored by the physical memory location associated with a given
virtual address. If the page it's trying to load from is in dsk,
it will have to swap the page into mem first.
