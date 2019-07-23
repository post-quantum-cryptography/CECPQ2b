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
  uint64_t virtual_timer_value;
  asm volatile("mrs %0, cntpct_el0" : "=r"(virtual_timer_value)); /// CNTPCT_EL0
  return virtual_timer_value;
#endif // endif ARCH_AARCH64
	return ~0;
}

#endif // cycles_h