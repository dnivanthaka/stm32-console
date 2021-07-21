// Create references to symbols defined in the linker script 
extern unsigned int _data_start;
extern unsigned int _data_end;
extern unsigned int _data_load;
extern unsigned int _bss_start;
extern unsigned int _bss_end;

void startup();			// Function prototype (forward declaration) for startup function
int main();			// Function prototype for main function


// The startup function, address was provided in the vector table
void startup()
{
    volatile unsigned int *src, *dest;

    // Copy data section values from load time memory address (LMA) to their address in SRAM
    for (src = &_data_load, dest = &_data_start; dest < &_data_end; src++, dest++) 
        *dest = *src;

    // Initialize all uninitialized variables (bss section) to 0
    for (dest = &_bss_start; dest < &_bss_end; dest++)
        *dest = 0;

    // Calling the main function
    main();
}
