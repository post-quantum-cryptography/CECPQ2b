#ifndef cycles_h
#define cycles_h

#define TEST_LOOP 500

#define CYCLE_TEST_LOOP_START 					\
	uint64_t cycles__count, cycles__total = 0; 	\
	for(size_t i=0; i<TEST_LOOP; i++) { 		\
		cycles__count = cycles();

#define CYCLE_TEST_LOOP_STOP(DESC)				\
		cycles__total += cycles() - cycles__count; \
	} \
	printf(DESC " %60lu cycles/op\n", (cycles__total/TEST_LOOP));


static inline uint64_t cycles() {
// OZAPTF: that's wrong - I should have CPU for which it is compiled + optimizations
#if defined(ARCH_GENERIC) || defined(ARCH_X86_64)
	uint32_t lo, hi;
    asm volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
	return ((uint64_t)lo) | (((uint64_t)hi) << 32);
#endif
#if defined(ARCH_AARCH64)
#if (__ARM_ARCH >= 6)
	// OZAPTF: optimize
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;
  // Read the user mode perf monitor counter access permissions.
  asm volatile("msr p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    asm volatile("msr p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul) {  // Is it counting?
      asm volatile("msr p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      // The counter is set up to count every 64th cycle
      return (int64_t)(pmccntr) * 64;  // Should optimize to << 6
    }
  }
#endif // __ARM_ARCH >= 6
#endif
	return ~0;
}

#endif // cycles_h