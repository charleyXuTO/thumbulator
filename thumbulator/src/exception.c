#include "thumbulator/exception.h"

#include "thumbulator/cpu.h"
#include "thumbulator/sim_support.h"

#include "cpu_flags.h"

// Clear pending exception after handled
// Used for watchdog timer exception
void check_except(void)
{
  uint32_t i;

  for(i = 0; i < 32; i++) {
    if(((cpu_get_exception() >> i) & 1) == 1) {
      except_enter(i);
      cpu_clear_exception(i);
    }
  }
}

void except_enter(const uint32_t exceptID)
{
  // Do we need to align the stack frame
  uint32_t frame_align = 0; //(cpu_get_sp() & 0x4) >> 2;

  // Align the new SP to a frame
  cpu_set_sp((cpu_get_sp() - 0x20)); // & ~0x4);
  uint32_t *frame_ptr = (uint32_t *)cpu_get_sp();

  // Exception can be mapped as a normal function call
  // so we need to backup the callee-saved registers for
  // the interrupted function
  store((uint32_t)&frame_ptr[0], cpu_get_gpr(0));
  store((uint32_t)&frame_ptr[1], cpu_get_gpr(1));
  store((uint32_t)&frame_ptr[2], cpu_get_gpr(2));
  store((uint32_t)&frame_ptr[3], cpu_get_gpr(3));
  store((uint32_t)&frame_ptr[4], cpu_get_gpr(12));
  store((uint32_t)&frame_ptr[5], cpu_get_lr());
  store((uint32_t)&frame_ptr[6], cpu_get_pc() - 0x4);

  uint32_t psr = cpu_get_apsr();
  store((uint32_t)&frame_ptr[7], (psr & 0xFFFFFC00) | (frame_align << 9) | (psr & 0x1FF));

  // Encode the mode of the cpu at time of exception in LR value
  if(cpu_mode_is_handler())
    cpu_set_lr(0xFFFFFFF1);
  else if(cpu_stack_is_main())
    cpu_set_lr(0xFFFFFFF9);
  else
    cpu_set_lr(0xFFFFFFFD);

  // Put the cpu in exception handling mode
  cpu_mode_handler();
  cpu_set_ipsr(exceptID);
  cpu_stack_use_main();
  uint32_t handlerAddress = 0;
  load(exceptID << 2, &handlerAddress, 0);
  cpu_set_pc(handlerAddress);

  // This counts as a branch
  BRANCH_WAS_TAKEN = 1;
}

void except_exit(const uint32_t pType)
{
  // Return to the mode and stack that were active when the exception started
  // Error if handler mode and process stack, stops simulation
  if((pType & 0xF) == 1) {
    cpu_mode_handler();
    cpu_stack_use_main();
  } else if((pType & 0xF) == 0x9) {
    cpu_mode_thread();
    cpu_stack_use_main();
  } else if((pType & 0xF) == 0xD) {
    cpu_mode_thread();
    cpu_stack_use_process();
  } else {
    fprintf(stderr, "ERROR: Invalid exception return\n");
    terminate_simulation(1);
  }

  cpu_set_ipsr(0);

  // Restore registers
  uint32_t *frame_ptr = (uint32_t *)cpu_get_sp();
  uint32_t value;

  load((uint32_t)&frame_ptr[0], &value, 0);
  cpu_set_gpr(0, value);

  load((uint32_t)&frame_ptr[1], &value, 0);
  cpu_set_gpr(1, value);

  load((uint32_t)&frame_ptr[2], &value, 0);
  cpu_set_gpr(2, value);

  load((uint32_t)&frame_ptr[3], &value, 0);
  cpu_set_gpr(3, value);

  load((uint32_t)&frame_ptr[4], &value, 0);
  cpu_set_gpr(12, value);

  load((uint32_t)&frame_ptr[5], &value, 0);
  cpu_set_lr(value);

  load((uint32_t)&frame_ptr[6], &value, 0);
  cpu_set_pc(value);

  load((uint32_t)&frame_ptr[7], &value, 0);
  cpu_set_apsr(value);

  // Set special-purpose registers
  cpu_set_sp((cpu_get_sp() + 0x20)); // | ((cpu_get_apsr() > (9 - 2)) & 0x200));
  cpu_set_apsr(cpu_get_apsr() & 0xF0000000);
  cpu_set_ipsr(0);
  // Ignore epsr
  BRANCH_WAS_TAKEN = 1;
}
