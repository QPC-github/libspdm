/**
    Copyright Notice:
    Copyright 2021 DMTF. All rights reserved.
    License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
**/

/** @file
  AEAD (ChaCha20Poly1305) Wrapper Implementation.

  RFC 5116 - An Interface and Algorithms for Authenticated Encryption
  RFC 8439 - ChaCha20 and Poly1305
**/

#include "internal_crypt_lib.h"
#include <openssl/aes.h>
#include <openssl/evp.h>

/**
  Performs AEAD ChaCha20Poly1305 authenticated encryption on a data buffer and additional authenticated data (AAD).

  iv_size must be 12, otherwise FALSE is returned.
  key_size must be 32, otherwise FALSE is returned.
  tag_size must be 16, otherwise FALSE is returned.

  @param[in]   key         Pointer to the encryption key.
  @param[in]   key_size     size of the encryption key in bytes.
  @param[in]   iv          Pointer to the IV value.
  @param[in]   iv_size      size of the IV value in bytes.
  @param[in]   a_data       Pointer to the additional authenticated data (AAD).
  @param[in]   a_data_size   size of the additional authenticated data (AAD) in bytes.
  @param[in]   data_in      Pointer to the input data buffer to be encrypted.
  @param[in]   data_in_size  size of the input data buffer in bytes.
  @param[out]  tag_out      Pointer to a buffer that receives the authentication tag output.
  @param[in]   tag_size     size of the authentication tag in bytes.
  @param[out]  data_out     Pointer to a buffer that receives the encryption output.
  @param[out]  data_out_size size of the output data buffer in bytes.

  @retval TRUE   AEAD ChaCha20Poly1305 authenticated encryption succeeded.
  @retval FALSE  AEAD ChaCha20Poly1305 authenticated encryption failed.

**/
boolean aead_chacha20_poly1305_encrypt(
	IN const uint8 *key, IN uintn key_size, IN const uint8 *iv,
	IN uintn iv_size, IN const uint8 *a_data, IN uintn a_data_size,
	IN const uint8 *data_in, IN uintn data_in_size, OUT uint8 *tag_out,
	IN uintn tag_size, OUT uint8 *data_out, OUT uintn *data_out_size)
{
	EVP_CIPHER_CTX *ctx;
	uintn temp_out_size;
	boolean ret_value;

	if (data_in_size > INT_MAX) {
		return FALSE;
	}
	if (a_data_size > INT_MAX) {
		return FALSE;
	}
	if (iv_size != 12) {
		return FALSE;
	}
	if (key_size != 32) {
		return FALSE;
	}
	if (tag_size != 16) {
		return FALSE;
	}
	if (data_out_size != NULL) {
		if ((*data_out_size > INT_MAX) ||
		    (*data_out_size < data_in_size)) {
			return FALSE;
		}
	}

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		return FALSE;
	}

	ret_value = (boolean)EVP_EncryptInit_ex(ctx, EVP_chacha20_poly1305(),
						NULL, NULL, NULL);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN,
						 (int32)iv_size, NULL);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG,
						 (int32)tag_size, NULL);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_EncryptUpdate(
		ctx, NULL, (int32 *)&temp_out_size, a_data, (int32)a_data_size);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_EncryptUpdate(ctx, data_out,
					       (int32 *)&temp_out_size, data_in,
					       (int32)data_in_size);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_EncryptFinal_ex(ctx, data_out,
						 (int32 *)&temp_out_size);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_CIPHER_CTX_ctrl(
		ctx, EVP_CTRL_AEAD_GET_TAG, (int32)tag_size, (void *)tag_out);

done:
	EVP_CIPHER_CTX_free(ctx);
	if (!ret_value) {
		return ret_value;
	}

	if (data_out_size != NULL) {
		*data_out_size = data_in_size;
	}

	return ret_value;
}

/**
  Performs AEAD ChaCha20Poly1305 authenticated decryption on a data buffer and additional authenticated data (AAD).

  iv_size must be 12, otherwise FALSE is returned.
  key_size must be 32, otherwise FALSE is returned.
  tag_size must be 16, otherwise FALSE is returned.
  If additional authenticated data verification fails, FALSE is returned.

  @param[in]   key         Pointer to the encryption key.
  @param[in]   key_size     size of the encryption key in bytes.
  @param[in]   iv          Pointer to the IV value.
  @param[in]   iv_size      size of the IV value in bytes.
  @param[in]   a_data       Pointer to the additional authenticated data (AAD).
  @param[in]   a_data_size   size of the additional authenticated data (AAD) in bytes.
  @param[in]   data_in      Pointer to the input data buffer to be decrypted.
  @param[in]   data_in_size  size of the input data buffer in bytes.
  @param[in]   tag         Pointer to a buffer that contains the authentication tag.
  @param[in]   tag_size     size of the authentication tag in bytes.
  @param[out]  data_out     Pointer to a buffer that receives the decryption output.
  @param[out]  data_out_size size of the output data buffer in bytes.

  @retval TRUE   AEAD ChaCha20Poly1305 authenticated decryption succeeded.
  @retval FALSE  AEAD ChaCha20Poly1305 authenticated decryption failed.

**/
boolean aead_chacha20_poly1305_decrypt(
	IN const uint8 *key, IN uintn key_size, IN const uint8 *iv,
	IN uintn iv_size, IN const uint8 *a_data, IN uintn a_data_size,
	IN const uint8 *data_in, IN uintn data_in_size, IN const uint8 *tag,
	IN uintn tag_size, OUT uint8 *data_out, OUT uintn *data_out_size)
{
	EVP_CIPHER_CTX *ctx;
	uintn temp_out_size;
	boolean ret_value;

	if (data_in_size > INT_MAX) {
		return FALSE;
	}
	if (a_data_size > INT_MAX) {
		return FALSE;
	}
	if (iv_size != 12) {
		return FALSE;
	}
	if (key_size != 32) {
		return FALSE;
	}
	if (tag_size != 16) {
		return FALSE;
	}
	if (data_out_size != NULL) {
		if ((*data_out_size > INT_MAX) ||
		    (*data_out_size < data_in_size)) {
			return FALSE;
		}
	}

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		return FALSE;
	}

	ret_value = (boolean)EVP_DecryptInit_ex(ctx, EVP_chacha20_poly1305(),
						NULL, NULL, NULL);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_IVLEN,
						 (int32)iv_size, NULL);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG,
						 (int32)tag_size, (void *)tag);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_DecryptUpdate(
		ctx, NULL, (int32 *)&temp_out_size, a_data, (int32)a_data_size);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_DecryptUpdate(ctx, data_out,
					       (int32 *)&temp_out_size, data_in,
					       (int32)data_in_size);
	if (!ret_value) {
		goto done;
	}

	ret_value = (boolean)EVP_DecryptFinal_ex(ctx, data_out,
						 (int32 *)&temp_out_size);

done:
	EVP_CIPHER_CTX_free(ctx);
	if (!ret_value) {
		return ret_value;
	}

	if (data_out_size != NULL) {
		*data_out_size = data_in_size;
	}

	return ret_value;
}
