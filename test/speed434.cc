#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <sike/sike.h>
#include <utils/cycles.h>

struct SpeedUtil {
    uint8_t sk[SIKE_PRV_BYTESZ] = {0};
    uint8_t pk[SIKE_PUB_BYTESZ] = {0};
    uint8_t ct[SIKE_CT_BYTESZ] = {0};
    uint8_t ss[SIKE_SS_BYTESZ] = {0};

	SpeedUtil() {
		SIKE_keypair(sk, pk);
		SIKE_encaps(ss, ct, pk);
	}
};

static void CyclesKeyGen() {
	SpeedUtil key;
    uint8_t sk[SIKE_PRV_BYTESZ] = {0};
    uint8_t pk[SIKE_PUB_BYTESZ] = {0};

	CYCLE_TEST_LOOP_START
	SIKE_keypair(sk, pk);
	CYCLE_TEST_LOOP_STOP("KEYGEN: ")
}

static void CyclesEncaps() {
	SpeedUtil key;
    uint8_t ct[SIKE_CT_BYTESZ] = {0};
    uint8_t ss[SIKE_SS_BYTESZ] = {0};

	CYCLE_TEST_LOOP_START
	SIKE_encaps(ss, ct, key.pk);
	CYCLE_TEST_LOOP_STOP("ENCAPS: ")
}

static void CyclesDecaps() {
	SpeedUtil key;
    uint8_t ct[SIKE_CT_BYTESZ] = {0};
    uint8_t ss[SIKE_SS_BYTESZ] = {0};

    CYCLE_TEST_LOOP_START
    SIKE_decaps(ss, key.ct, key.pk, key.sk);
    CYCLE_TEST_LOOP_STOP("DECAPS: ")
}

int main() {
	CyclesKeyGen();
	CyclesEncaps();
	CyclesDecaps();
}