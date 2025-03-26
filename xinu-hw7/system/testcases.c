/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/* This fake page table will allow you to test your printPageTable function
 * without having paging completely working.
 */
pgtbl createFakeTable(void){
	pgtbl root = pgalloc();
	pgtbl lvl1 = pgalloc();
	pgtbl lvl0 = pgalloc();

	volatile ulong *pte = &(root[5]);
	*pte = PA2PTE(lvl1) | PTE_V;

	ulong *lvl1pte = &(lvl1[145]);
	*lvl1pte = PA2PTE(lvl0) | PTE_V;

	ulong *lvl0pte = &(lvl0[343]);
	*lvl0pte = PA2PTE(0x1000) | PTE_W | PTE_R | PTE_V;

	ulong *lvl0pte1 = &(lvl0[120]);
	*lvl0pte1 = PA2PTE(0x4000) | PTE_X | PTE_R | PTE_V;

	ulong *lvl0pte2 = &(lvl0[45]);
	*lvl0pte2 = PA2PTE(0x8000) | PTE_X | PTE_R | PTE_V;

	return root;
}

void printPageTable(pgtbl pagetable, int lvl)
{
	/*
	* TODO: Write a function that prints out the page table.
	* Your function should print out all *valid* page table entries in
	* the page table.  If any of the entries are a link (if the
	* Read/Write/Execute bits aren't set), recursively print that page
	* table.  If it is a leaf, print the page table entry and the
	* physical address is maps to. 
	*/

	//for loop for each possible entry in page table
	int i;
	for (i=0; i<512; i++){
		ulong pte  = pagetable[i];
		ulong physaddr = PTE2PA(pte);

		int j;
		for(j=0; j>lvl; j++){
			kprintf("\t");
		}

		if(pte & PTE_V){
			kprintf("Valid Bit: %d \r\n", (pte << 63) >>63); //proves valid PTE
			kprintf("Entry: # %d\tPhysical Address: %d\tPTE: %d\r\n", i, physaddr, pte);

			if ((pte & PTE_R) || (pte & PTE_W) || (pte & PTE_X)){
				kprintf("leaf\r\n");

			}
			else{
				printPageTable((pgtbl)physaddr, lvl-1);
			}
		}
	//	else {
	//		kprintf("no entry #%d \r\n", i);
	//	}
	}
	kprintf("done\r\n");
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");

	// TODO: Test your operating system!

	c = kgetc();
	switch (c)
	{
		case '0':
			// TODO: Write a testcase that creates a user process
			// and prints out it's page table
			printPageTable(createFakeTable(), 2);
			break;
		case '1':
			// TODO: Write a testcase that demonstrates a user
			// process cannot access certain areas of memory
			break;
		case '2':
			// TODO: Write a testcase that demonstrates a user
			// process can read kernel variables but cannot write
			// to them
			break;
		case '3':
			// TODO: Extra credit! Add handling in xtrap to detect
			// and print out a Null Pointer Exception.  Write a
			// testcase that demonstrates your OS can detect a
			// Null Pointer Exception.
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
