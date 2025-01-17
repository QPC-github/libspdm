/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

#include "test_crypt.h"

/**
  Validate Crypto DH Interfaces.

  @retval  RETURN_SUCCESS  Validation succeeded.
  @retval  RETURN_ABORTED  Validation failed.

**/
return_status validate_crypt_dh(void)
{
	void *dh1;
	void *dh2;
	boolean status;
	uint8 ff_public_key1[256];
	uintn ff_public_key1_length;
	uint8 ff_public_key2[256];
	uintn ff_public_key2_length;
	uint8 ff_key1[256];
	uintn ff_key1_length;
	uint8 ff_key2[256];
	uintn ff_key2_length;

	my_print("\nCrypto DH Engine Testing:\n");

	//
	//
	//
	ff_public_key1_length = sizeof(ff_public_key1);
	ff_public_key2_length = sizeof(ff_public_key2);
	ff_key1_length = sizeof(ff_key1);
	ff_key2_length = sizeof(ff_key2);
	my_print("- Context1 ... ");
	dh1 = dh_new_by_nid(CRYPTO_NID_FFDHE2048);
	if (dh1 == NULL) {
		my_print("[Fail]");
		return RETURN_ABORTED;
	}

	my_print("Context2 ... ");
	dh2 = dh_new_by_nid(CRYPTO_NID_FFDHE2048);
	if (dh2 == NULL) {
		my_print("[Fail]");
		dh_free(dh1);
		return RETURN_ABORTED;
	}

	my_print("Generate key1 ... ");
	status = dh_generate_key(dh1, ff_public_key1, &ff_public_key1_length);
	if (!status || ff_public_key1_length != 256) {
		my_print("[Fail]");
		dh_free(dh1);
		dh_free(dh2);
		return RETURN_ABORTED;
	}

	my_print("Generate key2 ... ");
	status = dh_generate_key(dh2, ff_public_key2, &ff_public_key2_length);
	if (!status || ff_public_key2_length != 256) {
		my_print("[Fail]");
		dh_free(dh1);
		dh_free(dh2);
		return RETURN_ABORTED;
	}

	my_print("Compute key1 ... ");
	status = dh_compute_key(dh1, ff_public_key2, ff_public_key2_length,
				ff_key1, &ff_key1_length);
	if (!status || ff_key1_length != 256) {
		my_print("[Fail]");
		dh_free(dh1);
		dh_free(dh2);
		return RETURN_ABORTED;
	}

	my_print("Compute key2 ... ");
	status = dh_compute_key(dh2, ff_public_key1, ff_public_key1_length,
				ff_key2, &ff_key2_length);
	if (!status || ff_key2_length != 256) {
		my_print("[Fail]");
		dh_free(dh1);
		dh_free(dh2);
		return RETURN_ABORTED;
	}

	my_print("Compare Keys ... ");
	if (ff_key1_length != ff_key2_length) {
		my_print("[Fail]");
		dh_free(dh1);
		dh_free(dh2);
		return RETURN_ABORTED;
	}

	if (const_compare_mem(ff_key1, ff_key2, ff_key1_length) != 0) {
		my_print("[Fail]");
		dh_free(dh1);
		dh_free(dh2);
		return RETURN_ABORTED;
	}

	my_print("[Pass]\n");
	dh_free(dh1);
	dh_free(dh2);

	return RETURN_SUCCESS;
}