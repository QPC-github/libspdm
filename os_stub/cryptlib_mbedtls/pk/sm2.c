/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

/** @file
  Shang-Mi2 Asymmetric Wrapper Implementation.
**/

#include "internal_crypt_lib.h"

/**
  Allocates and Initializes one Shang-Mi2 context for subsequent use.

  The key is generated before the function returns.

  @return  Pointer to the Shang-Mi2 context that has been initialized.
           If the allocations fails, sm2_new() returns NULL.

**/
void *sm2_new(void)
{
	return NULL;
}

/**
  Release the specified sm2 context.

  @param[in]  sm2_context  Pointer to the sm2 context to be released.

**/
void sm2_free(IN void *sm2_context)
{
}

/**
  Sets the public key component into the established sm2 context.

  The public_size is 64. first 32-byte is X, second 32-byte is Y.

  @param[in, out]  ec_context      Pointer to sm2 context being set.
  @param[in]       public         Pointer to the buffer to receive generated public X,Y.
  @param[in]       public_size     The size of public buffer in bytes.

  @retval  TRUE   sm2 public key component was set successfully.
  @retval  FALSE  Invalid sm2 public key component.

**/
boolean sm2_set_pub_key(IN OUT void *sm2_context, IN uint8 *public_key,
			IN uintn public_key_size)
{
	return FALSE;
}

/**
  Gets the public key component from the established sm2 context.

  The public_size is 64. first 32-byte is X, second 32-byte is Y.

  @param[in, out]  sm2_context     Pointer to sm2 context being set.
  @param[out]      public         Pointer to the buffer to receive generated public X,Y.
  @param[in, out]  public_size     On input, the size of public buffer in bytes.
                                  On output, the size of data returned in public buffer in bytes.

  @retval  TRUE   sm2 key component was retrieved successfully.
  @retval  FALSE  Invalid sm2 key component.

**/
boolean sm2_get_pub_key(IN OUT void *sm2_context, OUT uint8 *public_key,
			IN OUT uintn *public_key_size)
{
	return FALSE;
}

/**
  Validates key components of sm2 context.
  NOTE: This function performs integrity checks on all the sm2 key material, so
        the sm2 key structure must contain all the private key data.

  If sm2_context is NULL, then return FALSE.

  @param[in]  sm2_context  Pointer to sm2 context to check.

  @retval  TRUE   sm2 key components are valid.
  @retval  FALSE  sm2 key components are not valid.

**/
boolean sm2_check_key(IN void *sm2_context)
{
	return FALSE;
}

/**
  Generates sm2 key and returns sm2 public key (X, Y).

  This function generates random secret, and computes the public key (X, Y), which is
  returned via parameter public, public_size.
  X is the first half of public with size being public_size / 2,
  Y is the second half of public with size being public_size / 2.
  sm2 context is updated accordingly.
  If the public buffer is too small to hold the public X, Y, FALSE is returned and
  public_size is set to the required buffer size to obtain the public X, Y.

  The public_size is 64. first 32-byte is X, second 32-byte is Y.

  If sm2_context is NULL, then return FALSE.
  If public_size is NULL, then return FALSE.
  If public_size is large enough but public is NULL, then return FALSE.

  @param[in, out]  sm2_context     Pointer to the sm2 context.
  @param[out]      public         Pointer to the buffer to receive generated public X,Y.
  @param[in, out]  public_size     On input, the size of public buffer in bytes.
                                  On output, the size of data returned in public buffer in bytes.

  @retval TRUE   sm2 public X,Y generation succeeded.
  @retval FALSE  sm2 public X,Y generation failed.
  @retval FALSE  public_size is not large enough.

**/
boolean sm2_generate_key(IN OUT void *sm2_context, OUT uint8 *public,
			 IN OUT uintn *public_size)
{
	return FALSE;
}

/**
  Computes exchanged common key.

  Given peer's public key (X, Y), this function computes the exchanged common key,
  based on its own context including value of curve parameter and random secret.
  X is the first half of peer_public with size being peer_public_size / 2,
  Y is the second half of peer_public with size being peer_public_size / 2.

  If sm2_context is NULL, then return FALSE.
  If peer_public is NULL, then return FALSE.
  If peer_public_size is 0, then return FALSE.
  If key is NULL, then return FALSE.
  If key_size is not large enough, then return FALSE.

  The peer_public_size is 64. first 32-byte is X, second 32-byte is Y. The key_size is 32.

  @param[in, out]  sm2_context         Pointer to the sm2 context.
  @param[in]       peer_public         Pointer to the peer's public X,Y.
  @param[in]       peer_public_size     size of peer's public X,Y in bytes.
  @param[out]      key                Pointer to the buffer to receive generated key.
  @param[in, out]  key_size            On input, the size of key buffer in bytes.
                                      On output, the size of data returned in key buffer in bytes.

  @retval TRUE   sm2 exchanged key generation succeeded.
  @retval FALSE  sm2 exchanged key generation failed.
  @retval FALSE  key_size is not large enough.

**/
boolean sm2_compute_key(IN OUT void *sm2_context, IN const uint8 *peer_public,
			IN uintn peer_public_size, OUT uint8 *key,
			IN OUT uintn *key_size)
{
	return FALSE;
}

/**
  Carries out the SM2 signature.

  This function carries out the SM2 signature.
  If the signature buffer is too small to hold the contents of signature, FALSE
  is returned and sig_size is set to the required buffer size to obtain the signature.

  If sm2_context is NULL, then return FALSE.
  If message is NULL, then return FALSE.
  hash_nid must be SM3_256.
  If sig_size is large enough but signature is NULL, then return FALSE.

  The sig_size is 64. first 32-byte is R, second 32-byte is S.

  @param[in]       sm2_context   Pointer to sm2 context for signature generation.
  @param[in]       hash_nid      hash NID
  @param[in]       message      Pointer to octet message to be signed (before hash).
  @param[in]       size         size of the message in bytes.
  @param[out]      signature    Pointer to buffer to receive SM2 signature.
  @param[in, out]  sig_size      On input, the size of signature buffer in bytes.
                                On output, the size of data returned in signature buffer in bytes.

  @retval  TRUE   signature successfully generated in SM2.
  @retval  FALSE  signature generation failed.
  @retval  FALSE  sig_size is too small.

**/
boolean sm2_ecdsa_sign(IN void *sm2_context, IN uintn hash_nid,
		       IN const uint8 *message, IN uintn size,
		       OUT uint8 *signature, IN OUT uintn *sig_size)
{
	return FALSE;
}

/**
  Verifies the SM2 signature.

  If sm2_context is NULL, then return FALSE.
  If message is NULL, then return FALSE.
  If signature is NULL, then return FALSE.
  hash_nid must be SM3_256.

  The sig_size is 64. first 32-byte is R, second 32-byte is S.

  @param[in]  sm2_context   Pointer to SM2 context for signature verification.
  @param[in]  hash_nid      hash NID
  @param[in]  message      Pointer to octet message to be checked (before hash).
  @param[in]  size         size of the message in bytes.
  @param[in]  signature    Pointer to SM2 signature to be verified.
  @param[in]  sig_size      size of signature in bytes.

  @retval  TRUE   Valid signature encoded in SM2.
  @retval  FALSE  Invalid signature or invalid sm2 context.

**/
boolean sm2_ecdsa_verify(IN void *sm2_context, IN uintn hash_nid,
			 IN const uint8 *message, IN uintn size,
			 IN const uint8 *signature, IN uintn sig_size)
{
	return FALSE;
}
