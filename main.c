/*
 * Assignment 1: Best CPU
 * 		By Team 11: Jimmy Gajjar, Ankit Khani, Siddhata Patil, Jeet Shah
 *
 * Registers, Instructions, Memory of 16 bits
 * Instruction:
 * 	16 bits instruction (5 bits op-code, 3 bits register, 8 btis memory)
 * 	Load Reg, Mem (Memory to Register) (LOAD: 00001)
 * 		e.g. lw $c, $5: 00001 011 00000101: 2821 in decimal
 * 	Store Reg, Mem (Register to Memory) (STORE: 00010)
 * 		e.g. st $c, $14: 00010 011 00001110: 4878 in decimal
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>

//Last location of bootstrap code
#define BOOTSTRAP_END 10

//Register Declaration
//A to H in form of _Reg[0] to _Reg[7]
uint16_t _Reg[8] = {0};

//System registers
uint16_t _PC = 0, _MAR = 0, _MDR = 0, _IR = 0;

//Temp global registers
uint16_t opCode = 0, reg = 0, mem_addr = 0;

//Flag for Read/Write/No-op
bool FLAG = 0;

//Instruction Memory
uint16_t _IMem[256] = {0};

//Data Memory
uint16_t _DMem[256] = {0};

//Function to initialize instructions and data
void initialize();

//Instruction fetch
void fetch();

//Instruction decode
void decode();

//Instruction execute
void execute();

//Display the status of registers
void display(uint16_t);

//Function for finding the opcode, register and memory address
uint16_t find_code(uint16_t inst, uint16_t mask, uint16_t shift);

//Main
int main(int argc, const char * argv[])
{
	//Loop for just two instruction. 1. LOAD and 2. STORE
	int loop = 0;

	//Call the function to initialize data memory and instruction memory
	initialize();

	//For displaying the register status
    _PC = BOOTSTRAP_END - 1;
    display(2);

    _PC = BOOTSTRAP_END;

    //Loop for two instruction
    while (loop < 2)
    {
    	//Pipeline. 1. Fetch, 2. Decode and 3. Execute
        fetch();
        decode();
        execute();

        //Display status of registers after each instruction
        display(FLAG);

        //Increment the program counter
        _PC = BOOTSTRAP_END + 1;
        loop++;
    }

    //Return
	return 0;
}

void initialize()
{
	int i = 0;

	_IMem[BOOTSTRAP_END] = 2821; // Load instruction.
    _IMem[BOOTSTRAP_END + 1] = 4878; // Store instruction.

    //Populate the data in data memory
    for(i = 0; i < 256; i++)
    {
    	_DMem[i] = i * 5;
    }
}

void fetch()
{
	//Fetch instruction from instruction memory and store it in to
	//instruction memory register
	_IR = _IMem[_PC];
}

void decode()
{
	//Op-code of current instruction
	opCode = find_code(_IR, 14336, 11);
	//Register of current instruction
	reg = find_code(_IR, 1792, 8);
	//Memory address of current instruction
	mem_addr = find_code(_IR, 255, 0);
	_MAR = mem_addr;

	//Change the register values
	switch (opCode)
	{
		case 1:
			FLAG = 0;
			_MDR = _DMem[mem_addr];
			break;
		case 2:
			FLAG = 1;
			_MDR = _Reg[reg - 1];
			break;
		default:
			printf("Error..!!");
			break;
	}
}

void execute()
{
	//Data population to and from registers
	switch (FLAG)
	{
		case 0:
			_Reg[reg - 1] = _MDR;
			break;
		case 1:
			_DMem[_MAR] = _MDR;
			break;
	}
}

uint16_t find_code(uint16_t inst, uint16_t mask, uint16_t shift)
{
	uint16_t mask_val = 0, code = 0;
	//Mask the instruction
	mask_val = inst & mask;
	//Find the value after shifting
	code = mask_val >> shift;
	return code;
}

void display(uint16_t arg)
{
	int i = 0;
	char operation;

	printf("\n======================================\n  ");

	switch (arg) {
	case 0:
		printf("CPU Status after LOAD instruction");
		operation = 'R';
		break;
	case 1:
		printf("CPU Status after STORE instruction");
		operation = 'W';
		break;
	case 2:
		printf("     CPU Status at beginning");
		operation = 'N';
		break;
	}

	printf("\n======================================");

	printf("\nReg\t\t\t    Memory");
	printf("\n---\t\t\t  ----------");
	for(i = 0; i < 8; i++)
	{
		printf("\n %d\t\t\t",_Reg[i]);
		if(_MAR == i && _MAR != 0)
			printf("--> %d \t%d", _DMem[i], _DMem[i+8]);
		else if (_MAR == i + 8)
			printf("    %d \t%d <--", _DMem[i], _DMem[i+8]);
		else
			printf("    %d \t%d", _DMem[i], _DMem[i+8]);
	}

	printf("\n\n\nPC\tIR\tMAR\tMDR\tFLAGS");
	printf("\n--\t--\t---\t---\t-----");
	printf("\n%d\t%d\t%d\t%d\t  %c",_PC+1,_IR,_MAR,_MDR,operation);

	printf("\n\n");
}
