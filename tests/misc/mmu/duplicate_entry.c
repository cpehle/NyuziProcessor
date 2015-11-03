// 
// Copyright 2015 Jeff Bush
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

#include <stdio.h>
#include <unistd.h>

#define PAGE_SIZE 0x1000

volatile unsigned int *data_addr = (unsigned int*) 0x100000;

void tlb_miss_handler();

void add_itlb_mapping(unsigned int va, unsigned int pa)
{
	asm("itlbinsert %0, %1" : : "r" (va), "r" (pa));
}

void add_dtlb_mapping(unsigned int va, unsigned int pa)
{
	asm("dtlbinsert %0, %1" : : "r" (va), "r" (pa));
}

void tlb_miss_handler()
{
	printf("TLB miss %08x\n", __builtin_nyuzi_read_control_reg(5));
	exit(0);
}

int main(int argc, const char *argv[])
{
	int i;
	unsigned int stack_addr = (unsigned int) &i & ~(PAGE_SIZE - 1);

	// Map code & data
	for (i = 0; i < 9; i++)
	{
		add_itlb_mapping(i * PAGE_SIZE, i * PAGE_SIZE);
		add_dtlb_mapping(i * PAGE_SIZE, i * PAGE_SIZE);
	}

	// Stack
	add_dtlb_mapping(stack_addr, stack_addr);

	// A data region. Enter a few bogus mapping first, then the proper
	// one, which should replace them.
	add_dtlb_mapping(data_addr, 0x101000);
	add_dtlb_mapping(data_addr, 0x102000);
	add_dtlb_mapping(data_addr, 0x103000);
	add_dtlb_mapping(data_addr, 0x104000);
	add_dtlb_mapping(data_addr, data_addr);

	// I/O address
	add_dtlb_mapping(0xffff0000, 0xffff0000);

	// Enable MMU in flags register
	__builtin_nyuzi_write_control_reg(7, tlb_miss_handler);
	__builtin_nyuzi_write_control_reg(4, (1 << 2));

	*data_addr = 0x1f6818aa;
	printf("data value %08x\n", *data_addr); // CHECK: data value 1f6818aa

	return 0;
}
