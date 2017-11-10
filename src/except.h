#include "sim_support.h"

// Check for pending exceptions
// and start an exception for any pending exceptions
// also clears the pending exception
void check_except(void);

// Interface for starting a new exception
void except_enter(uint32_t exceptID);

// Interface for returning from exceptions
// Called from bx and pop instructions
void except_exit(uint32_t pType);
